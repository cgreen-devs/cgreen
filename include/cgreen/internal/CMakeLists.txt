project(cgreen-internal-headers C)

set(cgreen_internal_HDRS
  unit_implementation.h
  mock_table.h
)

install(
  FILES
    ${cgreen_internal_HDRS}
  DESTINATION
    ${INCLUDE_INSTALL_DIR}/${APPLICATION_NAME}/internal
  COMPONENT
    headers
)
