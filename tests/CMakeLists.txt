# Copyright (c) 2024 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# Download and unpack googletest for unit testing
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Prevent overriding the parent project's compiler/linker settings on Windows
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Available googletest
FetchContent_MakeAvailable(googletest)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Network)

# glob pattern for test cpp files
file(GLOB_RECURSE test_cpp
  ${PROJECT_SOURCE_DIR}/src/utility/functions/packet/*.cpp
  ${PROJECT_SOURCE_DIR}/src/types/*.cpp
  *.cpp
  ${PROJECT_SOURCE_DIR}/src/packets/*.cpp)

# Add Executable to test
qt_add_executable(check
  ${test_cpp} ${PROTO_SRCS} ${PROTO_HDRS})

# Enable testing
enable_testing()

# Include gtest
include(GoogleTest)

# Discover tests
gtest_discover_tests(check)

# Include directories
target_include_directories(check
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_BINARY_DIR}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# link test executable to gtest & gtest_main
target_link_libraries(check
  PRIVATE GTest::gtest_main
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network)
