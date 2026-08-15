cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uwidget VERSION 0.1.0 LANGUAGES CXX)

set(UW_LIB uwidget)
set(UW_LIB_TARGETS uwidgetTargets)
set(UW_LIB_CONFIG uwidgetConfig)

add_library(${UW_LIB} INTERFACE)
add_library(${UW_LIB}::${UW_LIB} ALIAS ${UW_LIB})

target_compile_features(${UW_LIB} INTERFACE cxx_std_23)
target_include_directories(${UW_LIB} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install(TARGETS ${UW_LIB} EXPORT ${UW_LIB_TARGETS})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${UW_LIB_TARGETS}
  FILE ${UW_LIB_CONFIG}.cmake
  NAMESPACE ${UW_LIB}::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${UW_LIB}
)

if(${PROJECT_IS_TOP_LEVEL})
  enable_testing()
  add_subdirectory(tests)
endif()


