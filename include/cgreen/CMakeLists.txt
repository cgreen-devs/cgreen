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
)

install(
  FILES
    ${cgreen_HDRS}
  DESTINATION
    ${INCLUDE_INSTALL_DIR}/${APPLICATION_NAME}
  COMPONENT
    headers
)
