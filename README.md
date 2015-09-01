[![Build Status](https://travis-ci.org/cgreen-devs/cgreen.svg?branch=master)](https://travis-ci.org/cgreen-devs/cgreen)

Cgreen
======

You probably got this package from...
http://cgreen.sourceforge.net/projects/cgreen/

If there is no licence agreement with this package please download
a version from the location above. You must read and accept that
licence to use this software. The file is titled simply LICENSE.

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

If you want to install CGreen and have a play, just run "make" from
the top level directory. CGreen uses CMake and I'm afraid I am no expert
with this tool. There may well be teething problems. Please read the
INSTALL file for more information and alternate ways to build.

Feedback and queries should be sent to the mail list at Sourceforge:
cgreen-users@lists.sourceforge.net

If that doesn't work for some reason (bounce policies or whatever)
then feel free to contact me directly:
marcus@lastcraft.com

This project would not have happened without the generous
financial support of the Wordtracker keyword tool...
http://www.wordtracker.com/

This tool is basically a spin off from a research project at Wordtracker.

yours Marcus Baker
-- 
marcus@lastcraft.com

