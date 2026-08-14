cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(UW_LIB uwidget)
project(uwidget)

add_library(${UW_LIB} INTERFACE)

target_include_directories(${UW_LIB} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(${PROJECT_IS_TOP_LEVEL})
  enable_testing()
  add_subdirectory(tests)
endif()


