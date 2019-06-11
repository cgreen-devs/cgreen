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
progress is being made. Here's the closest we have come so far:

- Use Msys2/Mingw32 (this is the Mingw32 cross-compilation variant of
  Msys2, NOT the Posix-compliant variant...)

- Ensure that you have the Mingw32 toolchain installed, including
  mingw32-cmake

- Create a 'build' directory and 'cd' into it

- Do

    cmake -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE:string=Debug ..
    make

This will (at this moment) build cleanly for both MSYS Native and
MSYS/Mingw32 (haven't tried with Mingw64 yet).

The current status is that this creates `libcgreen.dll` and
`libcgreen.dll.a` which allows you to use the "main program runner"
strategy for creating Cgreen tests.

Unfortunately, at this point tests will crash when forking so you need
to prevent this and have all tests run in the same process:

    CGREEN_NO_FORK=1 all_tests.exe

It is also unknown at this time if mocks works at all.

But this is a work in progress, and we are happy to get this far.

=== cgreen-runner ===

The `cgreen-runner` can be compiled and linked without errors, but,
again, at this time, with the libcgreen.dll in the path, it starts ok,
but cannot load any libraries.

==== Prerequisite: dlfcn =====

The reflective runner `cgreen-runner` relies on Posix functionality to
dynamically load and inspect dynamically loadable libraries (DDLs,
.so's, .dylib's, ...)

Fortunately there is an adapter available for the Windows API,
dlfcn-win32. It is even available for Msys2/Mingw32. Install it with

    pacman -Syu mingw32/mingw-w64-i686-dlfcn
