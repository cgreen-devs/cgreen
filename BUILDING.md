= Building Cgreen =

Here's some quick notes on what we know about various problems
building Cgreen on multiple platforms.

Cgreen is CMake-based but we have tried to make building as convenient
as possible.

== General instructions ==

On platforms where Cgreen can be built in full here are the simple instructions:

- In the root directory just do

    make

  or

    make unit

This will configure an out-of-source CMake build directory in 'build',
compile everything and run some unit tests. If you want the CTest
tests, try

    make test

To install, do

    sudo make install

== Linux/Posix/Darwin/Cygwin/Msys2 ==

Everything just works as expected ;-) except that for Cygwin where you
don't need 'sudo' to install.

For all these platforms you get a complete native/Posix Cgreen
installation.

NOTE that for Cygwin and Msys2 the build cannot be used for Windows
applications. Both platforms are Posix-layers incompatible with
Windows. However, for Msys2, read on.

== Windows ==

As Cgreen uses Posix fork() and dynamic library loading, creating a
fully working Windows build is not straight forward. However, some
progress is being made.

We are, for now, primarily targeting Msys2 build environment.

=== Prerequisites ===

==== Prerequisites: Msys2 ====

Use Msys2/Mingw32 or Msys2/Mingw64. This is the Mingw
cross-compilation variants of Msys2, which is different from the
Posix-compliant variant which falls in the "just works" category as
described above.

==== Prerequisites: Mingw toolchain ====

Ensure that you have the Mingw32 or the Mingw64 toolchain installed,
including mingw32/64-cmake

    pacman -Syu mingw32/mingw-w64-i686-gcc mingw32/mingw-w64-i686-cmake

==== Prerequisite: dlfcn =====

The reflective runner `cgreen-runner` relies on Posix functionality to
dynamically load and inspect dynamically loadable libraries (DDLs,
.so's, .dylib's, ...)

Fortunately there is an adapter available for the Windows API,
dlfcn-win32. It is even available for Msys2/Mingw32/Mingw64. Install it with

    pacman -Syu mingw32/mingw-w64-i686-dlfcn

=== Current status ===

The top level (and basically only) Makefile is fixed to cater for a
one command configuration of Msys2 builds. So you can just do

    make

Take care to do that in the correct Msys2 environment, for potentially
Windows cross-compilation that means Msys2/Mingw32 or Msys/Mingw64
terminal. For plain Posix in Msys2/Posix see above.

Currently everything builds cleanly and creates `libcgreen.dll`,
`libcgreen.dll.a` and `cgreen-runner`.

Unfortunately, at this time tests will crash when forking so you need
to prevent this and have all tests run in the same process:

    CGREEN_NO_FORK=1 all_tests.exe

or

    CGREEN_NO_FORK=1 cgreen-runner <your_dll>

It is also unknown at this time if mocks works at all.

But this is a work in progress, and we are happy to get this far.

=== cgreen-runner ===

The `cgreen-runner` will also be compiled and linked without errors,
but, again, at this time, with the libcgreen.dll in the path (remember
to ". setup.sh"), it starts ok and can load libraries. But since the
underlying runner fails, so does `cgreen-runner`.
