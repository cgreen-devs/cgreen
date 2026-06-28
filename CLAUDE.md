# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Cgreen is a unit test and mocking framework for C and C++. It is a library (`libcgreen`) plus a dynamic test runner (`cgreen-runner`). The same fluent API (`assert_that`, `is_equal_to`, mocks, BDD-style `Describe`/`Ensure`) works from both C and C++. A key design point: each test runs in its **own forked process** by default, so a crashing or hanging test cannot take down the rest of the suite and tests cannot leak state into each other.

## Build & test commands

The top-level `Makefile` is a thin convenience wrapper; the real build is CMake, building out-of-source into `build/`.

```sh
make              # configure + build library and cgreen-runner into build/
make debug        # configure with CMAKE_BUILD_TYPE=Debug, then build
make test         # build, then run the full suite via ctest (cd build; ctest)
make install      # install via CMake (/usr/local by default)
make doc          # build HTML guide (needs asciidoctor); make pdf for PDF
make valgrind     # run every test library under valgrind, results in valgrind.log
```

Running a focused subset while developing:

```sh
make unit         # fast/clear feedback: runs the C, C++ and runner test libraries
                  # via cgreen-runner, plus the output-diff (golden) tests
```

CMake options of note (pass as `-DCGREEN_WITH_*` when configuring `build/` directly):
- `CGREEN_WITH_STATIC_LIBRARY` — also build/link the static lib
- `CGREEN_WITH_UNIT_TESTS` — build the test suite (required for `make test`/`ctest`)
- `CGREEN_WITH_XML` / `CGREEN_WITH_LIBXML2` — XML reporters
- `CGREEN_WITH_HTML_DOCS` / `CGREEN_WITH_PDF_DOCS` — documentation

> Building `cgreen-runner` requires `binutils` (BFD) for symbol discovery. Running Cgreen's own self-tests also needs `perl`, `diff`, `find`, and `sed`.

### Running a single test

Tests compile into shared libraries (e.g. `build/tests/libmocks_tests.so`). Run one library, or one named test, with the runner. The test name syntax is `Context:testname` (or just `testname` for TDD style):

```sh
# from build/, with the lib path set so the runner finds libcgreen
LD_LIBRARY_PATH=build/src build/tools/cgreen-runner build/tests/libmocks_tests.so
LD_LIBRARY_PATH=build/src build/tools/cgreen-runner build/tests/libmocks_tests.so Mocks:expect_a_sequence
```

(`cgreen-runner --help` lists options, e.g. `-c` for coloured output.) To debug a single test in a debugger, use the `cgreen-debug` script (same args as the runner), which breaks at the start of the named test. The `.cgreen-debug-commands` file shows the gdb pattern. On macOS use `DYLD_LIBRARY_PATH`; on Cygwin/Msys put `build/src` on `PATH` (the Makefile's `LDPATH` logic handles this per-platform).

## Architecture

The library is plain C (the C++ surface is a thin layer over it — see `src/cpp_*.cpp`). Major subsystems in `src/`:

- **Test model & execution** — `suite.c` (compose `TestSuite`s, setup/teardown), `runner.c` (fork per test, timeouts via `die_in`, collect results). Platform-specific process/pipe/timer code is split into `posix_*` and `win32_*` files selected at configure time (`runner_platform`, `cgreen_pipe`, `cgreen_time`).
- **Assertions & constraints** — `assertions.c` plus `constraint.c` (the big one: `is_equal_to`, `contains_string`, struct/double/pointer constraints, etc.). User-defined custom constraints live here too. `boxed_double.c`/`string_comparison.c` are helpers.
- **Mocks** — `mocks.c`. Implements strict/loose/learning mocks, `expect`/`will_return`/`when`, parameter capture and side effects. `parameters.c` parses the parameter lists.
- **Messaging** — `messaging.c` carries results from the forked child process back to the parent over a pipe; `message_formatting.c` builds the human-readable failure text.
- **Reporters** — pluggable output. `reporter.c` is the base; concrete reporters are `text_reporter.c` (default), `cute_reporter.c`, `xml_reporter.c`, `libxml_reporter.c`, `cdash_reporter.c`. Each reporter is overridable by users — results are *streamed* to a reporter, never stored.

Public headers are in `include/cgreen/`; `cgreen.h` is the umbrella include. Implementation-detail headers (macro machinery for `Ensure`/`Describe`, mock tables, platform shims) are under `include/cgreen/internal/` — the macro expansions there are what make test auto-discovery and the fluent syntax work.

### The runner & test discovery (`tools/`)

`cgreen-runner` (`tools/cgreen-runner.c`) loads a test shared library at runtime and **auto-discovers** test functions by reading the binary's symbol table via `binutils`/BFD — there are no registration macros or static initializers. `discoverer.c` walks symbols, `runner.c`/`test_item.c` build the run list, `gopt.c` parses options, `io.c` handles output. This is why tests just need to be `Ensure(...)`-defined and compiled into a `.so`/`.dylib`/`.dll`. A bash completion script (`cgreen_completion.bash`) can TAB-complete test names inside libraries.

## Test conventions

- Tests live in `tests/` (framework/user-facing functionality) and `tools/` (runner/discoverer unit tests). Each `*_tests.c`/`.cpp` file compiles into a `lib*_tests` shared library listed in the relevant `CMakeLists.txt`. **When adding a new test file you must add it to `CMakeLists.txt`** (e.g. `tests/CMakeLists.txt`'s `c_tests_library_SRCS`) and, for `make unit`, to the `Makefile` lists.
- Many C tests are also compiled as C++ to verify the API works identically in both languages.
- **Golden / output-diff tests:** files like `tests/assertion_messages_tests.c` have a paired `.expected` file. The `tools/cgreen_runner_output_diff` and `cgreen_xml_output_diff` scripts run the library and diff its *normalized* output against the `.expected` file. If you intentionally change reporter/failure-message output, regenerate the expected output (see `tests/accept_output_for`) rather than hand-editing.

## Code style

C with 4-space indentation, no tabs (`.dir-locals.el`). Match the existing K&R-ish brace style in the file you're editing. The codebase targets broad portability (Linux, macOS, Windows/MSVC, Cygwin/Msys, multiple arches incl. s390x) — keep platform-specific code behind the existing `posix_`/`win32_` split and configure-time checks rather than inlining `#ifdef`s.

## Commit messages

Follow the existing log convention: `[topic]: Brief summary`, e.g. `[fix]`, `[feat]`, `[refactor]`, `[test]`, `[docs]`/`[doc]`, `[build]`, `[ci]`, `[bug]`. Optional body for detail. Development is trunk-based — work lands directly on `master`, not via PRs/feature branches.
