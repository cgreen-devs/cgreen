[![Build Status](https://app.travis-ci.com/cgreen-devs/cgreen.svg?branch=master)](https://app.travis-ci.com/github/cgreen-devs/cgreen)
[![Coverage Status](https://coveralls.io/repos/cgreen-devs/cgreen/badge.svg?branch=master&service=github)](https://coveralls.io/github/cgreen-devs/cgreen?branch=master)

![](https://github.com/cgreen-devs/cgreen/blob/master/doc/logo.png?s=300)


Cgreen - The Modern Unit Test and Mocking Framework for C and C++
=================================================================

Do you TDD? In C or C++? Maybe you want to have your tests read out in
a fluent fashion? Like this

    Ensure(Converter, converts_XIV_to_14) {
        assert_that(convert_roman_to_decimal("XIV"), is_equal_to(14));
    }

And you want output like this

    roman_test.c:12: Failure: Converter -> converts_XIV_to_14
            Expected [convert_roman_to_decimal("XIV")] to [equal] [14]
                    actual value:                   [0]
                    expected value:                 [14]

Then *Cgreen* is the thing for you!

**TLDR;** The full tutorial is on
[github.io](https://cgreen-devs.github.io).
Or have a look at the [cheat sheet](doc/cheat-sheet.md).

## What It Is

Cgreen is a modern unit test and mocking framework for C and C++.
Here are some of Cgreens unique selling points:

  - fast build, clean code, highly portable
  - auto-discovery of tests without the abuse of static initializers or globals
  - extensible without recompiling
  - fluent, expressive and readable API with the same modern syntax across C and C++
  - process isolation for each test preventing intermittent failures
    and cross-test dependencies
  - built-in mocking for C, compatible with mockitopp and other C++ mocking libraries
  - expressive and clear output using the default reporter
  - fully functional mocks, both strict, loose and learning
  - mocks with side effects
  - extensive and expressive constraints for many datatypes
  - custom constraints can be constructed by user
  - bdd-flavoured test declarations with Before and After declarations
  - extensible reporting mechanism
  - fully composable test suites
  - a single test can be run in a single process for easier debugging

## Getting It

Cgreen is hosted on [GitHub](https://github.com/cgreen-devs/cgreen).
As of now there are no pre-built packages to download, but Cgreen is available in [Debian, Fedora and some other package repositories](https://repology.org/project/cgreen/versions), although some are lagging.

There are also some other packaging scripts available, not all official:

  - [PACMAN script](https://github.com/voins/cgreen-pkg)
  - [MacOS packagesbuild](https://github.com/cgreen-devs/cgreen-macosx-packaging)
  - [Cygwin package script](https://github.com/cgreen-devs/cgreen-cygport)

You can also clone the repository or download the source zip from [GitHub](http://www.github.com/cgreen-devs/cgreen) and build it yourself.

## Building It

You need the [CMake](http://www.cmake.org) build system.
Most standard C/C++ compilers should work. GCC definitely does.

Perl, diff, find and sed are required to run unit-tests. Most distro will have
those already installed.

To build the dynamically loading `cgreen-runner` you need the `libbfd`
libraries which are available with the binutils development
package. At runtime, `cgreen-runner` only requires the run-time
version of the libraries. Some distros do not distinguish between the
two.

|Distro | Build | Run | Comments |
--- | --- | ---
|Arch|binutils|binutils||
|Cygwin|binutils|binutils||
|Debian|binutils-dev|libbinutils||
|Fedora|binutils-devel|binutils-devel||
|OpenSUSE|binutils-devel|binutils-devel||
|Darwin| - | - | 
|Darwin/Homebrew|binutils|binutils| There is no equivalent to `libbfd` for Apple Clang but you can compile with Apple Clang and link with the brew versions. NOTE: `binutils` need to be re-installed with `--enable-install-libiberty` as per https://stackoverflow.com/a/65978315/204658. |

In the root directory run ``make``. That will configure and build,
actually by setting up and handing over to `CMake`, the library and
the `cgreen-runner`, both supporting both C and C++. See also the
documentation which will only be built if build requirements are
met.

Here are some example of how to build using docker for various distro:

```
# debian build (will not clean)
$ docker run --rm -v $PWD:/cgreen debian bash -c \
    "apt update && apt -y install git cmake gcc g++ binutils-dev && cd /cgreen && make unit test"
# debian run (we cannot use make unit test since cmake will want to recompile)
$ docker run --rm -v $PWD:/cgreen debian bash -c \
    'apt update && apt -y install libbinutils && cd /cgreen && ./build/tools/cgreen-runner $(find . -name *.so) rm -rf build'

# Fedora
$ docker run -t --rm -v $PWD:/cgreen fedora bash -c \
    "/usr/bin/yum -y install cmake gcc g++ git diffutils findutils && cd /cgreen && make unit test && make clean"

# OpenSUSE
$ docker run --rm -v $PWD:/cgreen opensuse/tumbleweed bash -c \
    '/usr/bin/zypper refresh && /usr/bin/zypper --non-interactive install git cmake gcc gcc-c++ git binutils-devel diffutils findutils  && cd /cgreen && make unit test && make clean'
```

## Using It

Tests are fairly easy write, as shown by the examples in the beginning
of this readme. You should probably read the
[tutorial](https://cgreen-devs.github.io) once before writing your
first test, though.

Basically you can run your tests in two ways

1. Compile and link all your tests with a test driver (as shown in the
   fist chapters of the tutorial)
2. Link your tests into separate shared libraries (`.so`, `.dylib` or
   similar) and run them with the `cgreen-runner` (described in chapter
   6 of the tutorial)

Option 2 is very handy, you can run multiple libraries in the same
run, but also specify single tests that you want to run. And with the
completion script available for bash you can get TAB-completion not
only for files and options but also for tests inside the libraries.

`cgreen-debug` is a small script that you invoke in the same way as
the runner but runs a single, specified, test and puts you in the
debugger at the start of that test. Awesome!

## Using Cgreen in other CMake projects
Once Cgreen is installed you can use ``find_package(cgreen)`` in your CMake
projects to get access to useful variables like ``${CGREEN_LIBRARIES}``,
``${CGREEN_EXECUTABLE}`` and ``${CGREEN_INCLUDE_DIRS}``. Version can be
specified in ``find_package`` as well. For example, in order to enforce a minimum
version of Cgreen in your project use ``find_package(cgreen 1.1.0)``

## Reading Up!

You can read the extensive tutorial directly on
[GitHub](https://cgreen-devs.github.io).

There is a [cheat sheet](https://github.com/cgreen-devs/cgreen/blob/master/doc/cheat-sheet.md)
available.

You can also build the documentation yourself in HTML and PDF format.
Generate it using Asciidoctor, which can be done using the CMake
configuration. Of course you need
[Asciidoctor](http://www.asciidoctor.org).

    make doc
    make pdf

(Generating PDF also requires [asciidoctor-pdf](https://asciidoctor.org/docs/asciidoctor-pdf/).)

## License

Cgreen is licensed under the ISC License
(http://spdx.org/licenses/ISC), sometimes known as the OpenBSD
license. If there is no licence agreement with this package please
download a version from the location above. You must read and accept
that licence to use this software. The file is titled simply LICENSE.

## The Original Version

What is it? It's a framework for unit testing, written in C. A tool
for C developers writing tests of their own code.

If you have used JUnit, or any of the xUnit clones, you will find
the concept familiar. In particular the tool supports a range of
assertions, composable test suites and setup/teardown facilities.
Because of the peculiarities of C programming, each test function
is normally run in it's own process.

This project is very close in scope to the "Check" unit tester and
was initially influenced by it.

The main difference from this tool and other xUnit tools, such as
"Check",  is that test results are not stored. Instead they are
streamed to the reporter psuedo-class, one that is easily
overridden by the end user.

The other main extra feature is the support for writing mock
callbacks. This includes generating sequences for return values
or parameter expectations.

Feedback, queries and request should be put to the cgreen developers
through https://github.com/cgreen-devs/cgreen.

This tool is basically a spin off from a research project at
Wordtracker and would not have happened without the generous
financial support of the Wordtracker keyword tool...
http://www.wordtracker.com/

Substantial inital work by Marcus Baker <marcus@lastcraft.com>. Recent
additions by Matt Hargett <matt@use.net>, Thomas Nilefalk
<thomas@junovagen.se>, João Freitas <joaohf@gmail.com> and others.
