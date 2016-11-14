
[![Build Status](https://travis-ci.org/cgreen-devs/cgreen.svg?branch=master)](https://travis-ci.org/cgreen-devs/cgreen)
[![Coverage Status](https://coveralls.io/repos/cgreen-devs/cgreen/badge.svg?branch=master&service=github)](https://coveralls.io/github/cgreen-devs/cgreen?branch=master)
[![Chat on Gitter](https://img.shields.io/gitter/room/badges/shields.svg)](https://gitter.im/cgreen-devs/chat)

Cgreen - The Modern Unit Test and Mocking Framework for C and C++
=================================================================

Do you TDD? In C? Maybe you want to have your tests read out in a
fluent fashion? Like this

    Ensure(Converter, converts_XIV_to_14) {
        assert_that(convert_roman_to_decimal("XIV"), is_equal_to(14));
    }

And you want output like this

    roman_test.c:12: Failure: Converter -> converts_XIV_to_14
            Expected [convert_roman_to_decimal("XIV")] to [equal] [14]
                    actual value:                   [0]
                    expected value:                 [14]

Then *Cgreen* is the thing for you!


## What It Is

Cgreen is a modern unit test and mocking framework for C and C++. Here
are some of Cgreens unique selling points:

  - fast build, clean code, highly portable
  - auto-discovery of tests without the abuse of static initializers or globals
  - extensible without recompiling
  - fluent, expressive and readable API with the same modern syntax across C and C++
  - process isolation for each test preventing intermittent failures
    and cross-test dependencies
  - built-in mocking for C, compatible with mockitopp and other C++ mocking libraries

## Getting It

Cgreen is hosted on GitHub. As of now there are no pre-built packages
to download, so you have to download the source zip from
[GitHub](http://www.github.org/cgreen-devs/cgreen) and build it
yourself.

## Building It

You need the [CMake](http://www.cmake.org) build system. Most standard
C/C++ compilers should work. GCC definitely does.

In the root directory run ``make``. That will configure and build the
library and the `cgreen-runner`, both supporting both C and C++. See
also the documentation.

## Using Cgreen in other CMake projects
Once Cgreen is installed you can use ``find_package(cgreen)`` in your CMake
projects to get access to useful variables like ``${CGREEN_LIBRARIES}``,
``${CGREEN_EXECUTABLE}`` and ``${CGREEN_INCLUDE_DIRS}``. Version can be
specified in ``find_package`` as well. For example, in order to enforce a minimum
version of Cgreen in your project use ``find_package(cgreen 1.1.0)``

## Reading Up!

You can read the extensive tutorial directly on
[GitHub](https://cgreen-devs.github.io).

You can also build the documentation yourself in HTML and PDF format.
Generate it using Asciidoctor, which can be done using the CMake
configuration. Of course you need
[Asciidoctor](http://www.asciidoctor.org).

Navigate to the ``build`` directory. You need to add the
``WITH_HTML`` and/or ``WITH_PDF`` option:

    cmake -DWITH_HTML:bool=ON ..

Run ``make``. For example

    cd build
    cmake -DWITH_HTML:bool=ON -DWITH_PDF:bool=ON ..
    make

## License

Cgreen is licensed under the ISC License
(http://spdx.org/licenses/ISC), sometimes known as the OpenBSD
license. If there is no licence agreement with this package please
download a version from the location above. You must read and accept
that licence to use this software. The file is titled simply LICENSE.

## The Original, Longer Version

What is it? It's a framework for unit testing, written in C. A tool
for C developers writing tests of their own code.

If you have used JUnit, or any of the xUnit clones, you will find
the concept familiar. In particular the tool supports a range of
assertions, composable test suites and setup/teardown facilities.
Because of the peculiarities of C programming, each test function
is normally run in it's own process.

This project is very close in scope to the "Check" unit tester and
was influenced by it...
http://check.sourceforge.net/projects/check/

The main difference from this tool and other xUnit tools, such as
"Check",  is that test results are not stored. Instead they are
streamed to the reporter psuedo-class, one that is easily
overridden by the end user.

The other main extra feature is the support for writing mock
callbacks. This includes generating sequences for return values
or parameter expectations.

Feedback, queries and request should be put to the cgreen developers through
https://github.com/cgreen-devs/cgreen.

This tool is basically a spin off from a research project at
Wordtracker and would not have happened without the generous
financial support of the Wordtracker keyword tool...
http://www.wordtracker.com/

Substantial inital work by Marcus Baker <marcus@lastcraft.com>. Recent
additions by Matt Hargett <matt@use.net>, Thomas Nilsson
<thomas@junovagen.se>, João Freitas <joaohf@gmail.com> and others.
