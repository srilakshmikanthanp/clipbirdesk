# CMakeLists.txt for clipbirdesk
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(clipbirdesk VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# include cmake modules
include(FetchContent)

# include project root
include_directories(${PROJECT_SOURCE_DIR})

# --------------------------------- Main Project ---------------------------------#

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  Network
)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Configure config.hpp
configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp
)

# set up project using Qt
qt_standard_project_setup()

# Add executable
qt_add_executable(clipbirdesk
  main.cpp
)

# Link libraries
target_link_libraries(clipbirdesk
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto)

# Include config.hpp
target_include_directories(clipbirdesk
  PUBLIC ${PROJECT_BINARY_DIR})

# --------------------------------- Unit Tests ---------------------------------#

# Download and unpack googletest for unit testing
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Add Executable to test
qt_add_executable(check
  tests/test.cpp
)

# Enable testing
enable_testing()

# Include gtest
include(GoogleTest)

# Discover tests
gtest_discover_tests(check)

#link test executable to gtest & gtest_main
target_link_libraries(check
  PRIVATE GTest::gtest_main
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network)
