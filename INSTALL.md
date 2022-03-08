# Building and Installing Cgreen

Here are your alternatives for installing `Cgreen`.

## Install using your distro package manager

Cgreen is available for [some Linux distros]
(https://repology.org/project/cgreen/versions). If you are on one of
those you can do

    $ sudo apt install cgreen1

or equivalent. Not all distros have an up-to-date version.


## Install pre-built binary

There are occassionally pre-built binaries available from [the GitHub
repo](https://github.com/cgreen-devs/cgreen/releases). You can
download and install these using an appropriate package manager.


## Build from source

### Get source and build

Clone the [`Cgreen` repo](https://github.com/cgreen-devs/cgreen)

    $ git clone https://github.com/cgreen-devs/cgreen

or get the source from the same place.

Then build it

    $ cd cgreen
    $ make

The Makefile is mainly for convenience as `Cgreen` is actually built
using `CMake`. So you can tweak the build using normal `CMake`
settings.

You can run some tests using

    $ make unit
    $ make test

NOTE: to also build the dynamically auto-discovering runner
`cgreen-runner`, which `make unit` uses, you need `binutils` as per
the description in the README.md.

### Build options

You also have some extra options available

- build with static libraries
- build HTML or PDF documentation

To enable any of these use the `CMake` graphical user interface (CMakeSetup
on Windows or ccmake on UNIX) to turn these options on or off.

Note on CYGWIN: Cygwin is not a WIN32 platform, but it is a
DLL-platform where libraries normally goes in the bin directory. Running
the self-tests handles this automatically but if you want to use the binaries
in the 'build'-tree you can either:

1) install before running the tests and also set your PATH to include
"/usr/local/lib"

2) setting the PATH to include the build directory where the libraries are
   there is a sh-compatible command script to do that for you. From the top
   of the Cgreen source directory do:

     . cygwin-setup.sh

### Use directly

You can use `Cgreen` from the source tree without actually
installing. Just set your compilation includes to include
`<path_to_cgreen>/include` and link with the built library by pointing
the linker to `<path_to_cgreen>/build/src/` and use `-lcgreen`. E.g.

    $ cc ... -I/home/me/cgreen/include ...
    $ cc ... -L/home/me/cgreen/build/src -lcgreen ...

### Install

You can also install `Cgreen` with

    $ make install

which will install `Cgreen` in what `CMake` considers standard
locations for your environment. Assuming that is `/usr/local` you
should now be able to compile and link using

    $ cc ... -I/usr/local/include ...
    $ cc ... -L/usr/local/lib -lcgreen ...

