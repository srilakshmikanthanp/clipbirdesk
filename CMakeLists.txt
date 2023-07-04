cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(clipbirdesk VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  Network
)

configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp
)

qt_standard_project_setup()
qt_add_executable(clipbirdesk
  main.cpp
)

target_link_libraries(clipbirdesk
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network)

target_include_directories(clipbirdesk
  PUBLIC ${PROJECT_BINARY_DIR})
