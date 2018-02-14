# Mini-HOWTO:
#
# 1. Update gitcommit, gitncommits and gitversion in this file with
#    data from "git describe --tags". (See below).
#
# 2. Fetch the latest source archive from github:
#
#    curl -Lo ~/rpmbuild/SOURCES/cgreen-master.zip https://github.com/cgreen-devs/cgreen/archive/master.zip
#
# 3. Build the source and binary RPMs:
#
#    mock $(rpmbuild -bs cgreen.spec | cut -d" " -f2)
#
# You could also provide your own source archive if that's what you're
# looking for. Make sure to adjust Source0: and the %prep phase
# accordingly.


# $ git describe --tags
# 1.0.0-387-g8130aa7
#   |    |     +--v
%define gitcommit g8130aa7
#   |    +----------v
%define gitncommits 387
#   +--------------v
%define gitversion 1.0.0

Summary: Cgreen is a modern unit test and mocking framework for C and C++.
Name: cgreen
Version: %{gitversion}
Release: %{gitncommits}.%{gitcommit}
License: ISC
URL: https://github.com/cgreen-devs/cgreen
Source0: %{name}-master.zip
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: cmake

%description
A modern, portable, cross-language unit testing and mocking framework for C and C++

%prep
%setup -q -n cgreen-master

%build
%cmake .

%install
rm -rf $RPM_BUILD_ROOT

# Don't put things into /usr/lib/cmake on 64-bit systems
%if "%{?_lib}" == "lib64"
    sed -i -e "s@/lib/cmake/cgreen@/lib64/cmake/cgreen@g" cmake_install.cmake
%endif

%make_install


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/cgreen-runner
%dir %{_includedir}/cgreen
%{_includedir}/cgreen/assertions.h
%{_includedir}/cgreen/boxed_double.h
%{_includedir}/cgreen/breadcrumb.h
%{_includedir}/cgreen/cdash_reporter.h
%{_includedir}/cgreen/cgreen.h
%{_includedir}/cgreen/cgreen_value.h
%{_includedir}/cgreen/constraint.h
%{_includedir}/cgreen/constraint_syntax_helpers.h
%{_includedir}/cgreen/cpp_assertions.h
%{_includedir}/cgreen/cpp_constraint.h
%{_includedir}/cgreen/cute_reporter.h
%{_includedir}/cgreen/internal/assertions_internal.h
%{_includedir}/cgreen/internal/c_assertions.h
%{_includedir}/cgreen/internal/cgreen_pipe.h
%{_includedir}/cgreen/internal/cgreen_time.h
%{_includedir}/cgreen/internal/cpp_assertions.h
%{_includedir}/cgreen/internal/function_macro.h
%{_includedir}/cgreen/internal/mock_table.h
%{_includedir}/cgreen/internal/mocks_internal.h
%{_includedir}/cgreen/internal/runner_platform.h
%{_includedir}/cgreen/internal/stringify_token.h
%{_includedir}/cgreen/internal/suite_internal.h
%{_includedir}/cgreen/internal/unit_implementation.h
%{_includedir}/cgreen/legacy.h
%{_includedir}/cgreen/mocks.h
%{_includedir}/cgreen/reporter.h
%{_includedir}/cgreen/runner.h
%{_includedir}/cgreen/string_comparison.h
%{_includedir}/cgreen/suite.h
%{_includedir}/cgreen/text_reporter.h
%{_includedir}/cgreen/unit.h
%{_includedir}/cgreen/vector.h

%dir %{_libdir}/cmake/cgreen
%{_libdir}/cmake/cgreen/cgreen-config-version.cmake
%{_libdir}/cmake/cgreen/cgreen-config.cmake

%{_libdir}/libcgreen.so
%{_libdir}/libcgreen.so.1
%{_libdir}/libcgreen.so.1.1.0
%{_mandir}/man1/cgreen-runner.1.gz
%{_mandir}/man5/cgreen.5.gz


%changelog
* Tue Feb  6 2018 Karl Mikaelsson <derfian@cendio.se> - 1.0.0-387.g8130aa7
- Initial build of git master.

