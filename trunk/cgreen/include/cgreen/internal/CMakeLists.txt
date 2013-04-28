project(cgreen-internal-headers C)

set(cgreen_internal_HDRS
  unit_implementation.h
  mock_table.h
  mocks_internal.h
  suite_internal.h
  assertions_internal.h
  c_assertions.h
  cpp_assertions.h
  cgreen_pipe.h
  runner_platform.h
  function_macro.h
)

install(
  FILES
    ${cgreen_internal_HDRS}
  DESTINATION
    ${INCLUDE_INSTALL_DIR}/${APPLICATION_NAME}/internal
  COMPONENT
    headers
)
