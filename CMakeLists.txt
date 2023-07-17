# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(clipbird VERSION 1.0.0 LANGUAGES CXX)

# include cmake modules
include(FetchContent)

# --------------------------------- Main Project ---------------------------------#
# glob pattern for main cpp files exclude
file(GLOB_RECURSE main_cpp *.cpp)

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  Network
)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# set qt version for singleapplication
set(QT_DEFAULT_MAJOR_VERSION 6)

# SingleApplication Librray Fetch from github
FetchContent_Declare(SingleApplication
  URL https://github.com/itay-grudev/SingleApplication/archive/refs/tags/v3.4.0.zip
)

# Fetch content
FetchContent_MakeAvailable(SingleApplication)

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
  PRIVATE SingleApplication::SingleApplication
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto)

# Include directories
target_include_directories(clipbird
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

# --------------------------------- Unit Tests ---------------------------------#
# glob pattern for test cpp files
file(GLOB_RECURSE test_cpp tests/*.cpp network/packets/*.cpp types/*.cpp)

# Download and unpack googletest for unit testing
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

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
