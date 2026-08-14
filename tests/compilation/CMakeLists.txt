function(uw_add_compilation_test UW_TEST_NAME UW_FAIL_OR_PASS)
  add_library(${UW_TEST_NAME} ${UW_TEST_NAME}.cpp)
  target_link_libraries(${UW_TEST_NAME} PRIVATE ${UW_LIB})
  set_target_properties(${UW_TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
  string(COMPARE EQUAL ${UW_FAIL_OR_PASS} FAIL UW_TEST_FAIL)
  add_test(
    NAME compilation_tests.${UW_TEST_NAME}
    COMMAND ${CMAKE_COMMAND} --build . --target ${UW_TEST_NAME} --config $<CONFIGURATION>)
  if(${UW_TEST_FAIL})
    set_tests_properties(runtime_tests.${UW_TEST_NAME} PROPERTIES WILL_FAIL TRUE)
  endif()
endfunction()

uw_add_compilation_test(pos_copy_and_move_constraints PASS)
uw_add_compilation_test(pos_default_widget PASS)
uw_add_compilation_test(pos_value_types PASS)
uw_add_compilation_test(pos_non_default_constructible_value_type PASS)
