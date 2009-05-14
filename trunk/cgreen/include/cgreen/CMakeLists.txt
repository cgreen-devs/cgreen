project(cgreen-headers C)

set(cgreen_HDRS
  cgreen.h
  unit.h
  text_reporter.h
  cute_reporter.h
  assertions.h
)

install(
  FILES
    ${cgreen_HDRS}
  DESTINATION
    ${INCLUDE_INSTALL_DIR}/${APPLICATION_NAME}
  COMPONENT
    headers
)

