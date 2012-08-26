project(cgreen-headers C)

set(cgreen_HDRS
  cgreen.h
  unit.h
  reporter.h
  text_reporter.h
  cute_reporter.h
  cdash_reporter.h
  assertions.h
  constraint.h
  memory.h
  mocks.h
  mock_table.h
  constraint_syntax_helpers.h
  string_comparison.h
  assertions.h
  runner.h
  suite.h
  c_assertions.h
  vector.h
  breadcrumb.h
)

install(
  FILES
    ${cgreen_HDRS}
  DESTINATION
    ${INCLUDE_INSTALL_DIR}/${APPLICATION_NAME}
  COMPONENT
    headers
)
