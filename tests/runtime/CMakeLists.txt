function(uw_add_runtime_test UW_TEST_NAME UW_FAIL_OR_PASS)
  add_executable(${UW_TEST_NAME} ${UW_TEST_NAME}.cpp)
  target_link_libraries(${UW_TEST_NAME} PRIVATE ${UW_LIB})
  string(COMPARE EQUAL ${UW_FAIL_OR_PASS} FAIL UW_TEST_FAIL)
  add_test(
    NAME runtime_tests.${UW_TEST_NAME}
    COMMAND ${UW_TEST_NAME})
  if(${UW_TEST_FAIL})
    set_tests_properties(runtime_tests.${UW_TEST_NAME} PROPERTIES WILL_FAIL TRUE)
  endif()
endfunction()

uw_add_runtime_test(track_default_construct PASS)

uw_add_runtime_test(track_copy_assign PASS)
uw_add_runtime_test(track_copy_construct PASS)

uw_add_runtime_test(track_move_assign PASS)
uw_add_runtime_test(track_move_construct PASS)

uw_add_runtime_test(track_lifetimes PASS)
uw_add_runtime_test(value_primitive_preservation PASS)
