# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(clipbird VERSION 1.0.0 LANGUAGES CXX)

# include cmake modules
include(FetchContent)

# Set the logo.png path variable
set(APPLICATION_LOGO_PATH "${PROJECT_SOURCE_DIR}/assets/logo.png")

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# Append ECM cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{ECM_DIR})

# Append KDNSSD_DIR cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{KDNSSD_DIR})

# Set Qt version
set(QT_MAJOR_VERSION 6)

# --------------------------------- Main Project ---------------------------------#
# glob pattern for main cpp files exclude
file(GLOB_RECURSE main_cpp *.cpp)

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  Network
)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find KDE DNS-SD Framework
find_package(KF5DNSSD REQUIRED)

find_package(DNSSD)

# Configure config.hpp
configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp
)

# set up project using Qt
qt_standard_project_setup()

# Add executable
qt_add_executable(clipbird
  ${main_cpp}
)

# Link libraries
target_link_libraries(clipbird
  PRIVATE Qt6::Widgets
  PRIVATE KF5::DNSSD
  PRIVATE Qt6::Network
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto)

# Include directories
target_include_directories(clipbird
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

# --------------------------------- Unit Tests ---------------------------------#
# Download and unpack googletest for unit testing
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Make Available googletest
FetchContent_MakeAvailable(googletest)

# glob pattern for test cpp files
file(GLOB_RECURSE test_cpp
  utility/functions/packet/*.cpp
  tests/*.cpp
  types/*.cpp
  network/packets/*.cpp
)

# Add Executable to test
qt_add_executable(check
  ${test_cpp}
)

# Enable testing
enable_testing()

# Include gtest
include(GoogleTest)

# Discover tests
gtest_discover_tests(check)

# Include directories
target_include_directories(check
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

#link test executable to gtest & gtest_main
target_link_libraries(check
  PRIVATE GTest::gtest_main
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network)
