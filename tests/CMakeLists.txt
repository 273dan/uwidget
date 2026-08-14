function(uw_add_positive_compilation_test UW_TEST_NAME)
  add_library(${UW_TEST_NAME} ${UW_TEST_NAME}.cpp)
  target_link_libraries(${UW_TEST_NAME} PRIVATE ${UW_LIB})
  set_target_properties(${UW_TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
  add_test(
    NAME compilation_test.${UW_TEST_NAME}
    COMMAND ${CMAKE_COMMAND} --build . --target ${UW_TEST_NAME} --config $<CONFIGURATION>)
endfunction()

uw_add_positive_compilation_test(pos_copy_and_move_constraints)
uw_add_positive_compilation_test(pos_default_widget)
