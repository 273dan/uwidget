set(UW_UNIT_TESTS uw_tests)

add_executable(${UW_UNIT_TESTS})

target_sources(${UW_UNIT_TESTS} PRIVATE
  doctest.cpp
  test_tracking.cpp
  test_runtime_faults.cpp
  test_runtime_features.cpp
  test_traits.cpp
)

target_include_directories(${UW_UNIT_TESTS} PRIVATE
  ${CMAKE_SOURCE_DIR}/third_party
)

target_link_libraries(${UW_UNIT_TESTS} ${UW_LIB})

add_test(NAME ${UW_UNIT_TESTS} COMMAND ${UW_UNIT_TESTS})


