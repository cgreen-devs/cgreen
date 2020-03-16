#!/usr/bin/env python
# -----------------------------------------------------------------
# cgreen-mocker.py
#
# Create Cgreen mocks from extern declarations of functions,
# typically in a header file.
#
# Usage:
#   cgreen-mocker.py { <cpp_directive> } <headerfile>
#
# <cpp_directive>: any 'cpp' directive but most useful is e.g.
#                  "-I <directory>" to ensure cpp finds files.
#
# <headerfile>: file with function declarations that you want
#               to mock
#
# Simplistically adapted from pycparser example: func_defs.py
#
# Since it uses pycparser it will only handle C functions and you will
# probably need the pycparsers "fake_libc_include" to avoid parsing
# the whole world of libc headers. To use it, make a soft link with
# the name 'pycparser' in the directory you are running this from, or
# in the directory of 'cgreen-mocker' itself, to the top directory of
# the pycparser source, and cgreen-mocker will pick it up
# automatically. Or you can point to it using a command line
# 'cpp_directive' arg.
#
# Thanks to @gardenia for the pointer to pycparser!
#
#    https://github.com/eliben/pycparser
#
# (C) 2016, Thomas Nilefalk
#
# Using pycparser for printing out all the functions defined in a
# C file.
#
# PyCParser - Copyright (C) 2008-2015, Eli Bendersky
# License: BSD
# -----------------------------------------------------------------
from __future__ import print_function
from pycparser.plyparser import ParseError
from pycparser import c_parser, c_ast, parse_file, c_generator
from functools import reduce
import sys
import os

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend(['.', '..'])


# A visitor for FuncDef nodes that prints the
# Cgreen mock equivalent of the function
class FuncDefVisitor(c_ast.NodeVisitor):
    def __init__(self, filename):
        self._types = {}
        self.filename = filename

    def visit_FuncDecl(self, node):
        if node.coord.file == self.filename:
            # Only consider definitions that are in the processed file
            generator = c_generator.CGenerator()
            try:
                print(generator.visit(node), end="")
                print(" { ")
                self.should_return(node)
                print("mock(%s);" % ", ".join(arg_list(node.args)))
                print("}")
                print()
            except Exception as e:
                print("ERROR: {} - Unexpected AST @ {}:{}:{}:".format(e, node.coord.file,
                                                                      node.coord.line, node.coord.column))
                node.show()
                return

    def visit_Typedef(self, node):
        self._types[node.name] = {
            'is_pointer': isinstance(node.type, c_ast.PtrDecl),
        }
        if self._types[node.name]['is_pointer']:
            self._types[node.name]['to_class'] = node.type.type.type.names
        else:
            self._types[node.name]['to_class'] = None

    def should_return(self, node):
        generator = c_generator.CGenerator()
        type = node.type
        if is_double_decl(node):
            print("  return unbox_double(", end="")
        elif not is_void_decl(node):
            print("  return %s(" %
                  ("*" if self.is_return_struct_by_value(node) else ""), end="")
            print(generator.visit(node.type), end="")
            if isinstance(node.type, c_ast.PtrDecl) or self.is_return_struct_by_value(node):
                print(" *", end="")
            print(") ", end="")
        else:
            print("  ", end="")

    def is_return_struct_by_value(self, node):
        type = node.type
        return not isinstance(type, c_ast.PtrDecl) and type.type.names[0] in self._types and not self._types[type.type.names[0]]['is_pointer']

    def is_return_by_value_pointer(self, node):
        type = node.type
        return not isinstance(type, c_ast.PtrDecl) and self._types[type.type.names[0]]['is_pointer']


def arg_list(args):
    if args != None and len(args.params) > 0:
        return [el for el in map(parameter_name_or_box_double,
                                 filter(lambda x: not is_ellipsis_param(x),
                                        args.params))
                if el is not None]
    else:
        return []


def parameter_name_or_box_double(node):
    if is_double_decl(node):
        return "box_double({})".format(node.name)
    else:
        return node.name


def is_void_decl(node):
    type = node.type
    return isinstance(type, c_ast.TypeDecl) and type.type.names == ['void']


def is_double_decl(node):
    type = node.type
    return isinstance(type, c_ast.TypeDecl) and type.type.names == ['double']


def is_ellipsis_param(node):
    return isinstance(node, c_ast.EllipsisParam)


def show_func_defs(args):
    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.

    pycparser_path = None
    # Try to find a fake_libc
    if os.path.isdir('pycparser'):
        # In current directory
        pycparser_path = r'./pycparser'
    elif os.path.isdir(os.path.dirname(os.path.join(os.path.abspath(__file__),
                                                    'pycparser'))):
        # In the directory of this script
        pycparser_path = os.path.dirname(os.path.join(os.path.abspath(__file__),
                                                      'pycparser'))
    if pycparser_path:
        pycparser_lib = reduce(
            os.path.join, [pycparser_path, 'utils', 'fake_libc_include'])
        if verbose:
            print("/* Generated with cgreen-mocker and pycparser's fake_libc from %s */" % (pycparser_path))
    try:
        cpp_args=list(filter(None, [
            '-I'+pycparser_lib if pycparser_path else '',
            # And add some common GNUisms
            r'-D__gnuc_va_list(c)=',
            r'-D__attribute__(x)=',
            r'-D__extension__=',
            r'-D__restrict=',
            r'-D__inline='
        ]))
        ast = parse_file(args[-1], use_cpp=True,
                         cpp_args=cpp_args + args[0:-1])
    except ParseError as e:
        print("ERROR: {} - C99 parse error".format(e))
        return

    print('/* -*- c -*-*/')    # Suggest c-mode for Emacs
    print('#include "%s"' % args[len(args)-1])
    print('#include <cgreen/mocks.h>')
    print()
    v = FuncDefVisitor(args[-1])
    v.visit(ast)


def usage():
    print("""
Usage:
    cgreen-mocker.py { <cpp_directive> } <headerfile>

    <cpp_directive>: any 'cpp' directive but most useful are e.g.
                     "-I <directory>" to ensure cpp finds files and
                     "-D <define>" to create an inline define

    <headerfile>:    file with function declarations that you want
                     to mock

    Cgreen-mocker takes a header file and generates cgreen mocks for
    all functions in it. It will print the generated mocks to standard
    output so you can inspect it, or pipe it to a file that can be
    compiled and linked with your tests.

    The mocker will only handle functions that are declared in the
    header file you provide. This is based on the presumtion that the
    header file represents functions in a unit. Aggregating functions
    from multiple units into a single header for convenience is not
    supported. Also the mocker cannot handle data declarations (yet?).

    If your header does not name some arguments you will not be able
    to use those arguments in 'expect when' statements, of course.

    A tip is to ensure that the header file does not include other
    header files that have external function definitions since they
    will also be mocked, which is probably not what you want.

    If cgreen-mocker encounters parse errors and they look like
    gnu-isms you should get a copy of the source for pycparser (on
    which cgreen-mocker is built). In it you will find a
    'fake_libc_include' which help. Create a symbolic link named
    'pycparser' that links to the root of pycparser source and
    cgreen-mocker will find it itself.

    You can find pycparser at https://github.com/eliben/pycparser

""")


if __name__ == "__main__":
    if len(sys.argv) <= 1:
        usage()
        exit(-1)
    if '-v' in sys.argv:
        verbose = True
        sys.argv.remove('-v')
    else:
        verbose = False
    show_func_defs(sys.argv[1:])
