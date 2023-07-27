# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(clipbird VERSION 1.0.0 LANGUAGES CXX)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 17)

# set c++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include cmake modules
include(FetchContent)

# For MSVX
set(protobuf_MSVC_STATIC_RUNTIME OFF)

# set the APPLICATION_ISSUEPAGE
set(APPLICATION_ISSUEPAGE "https://github.com/srilakshmikanthanp/clipbirdesk/issues")

# set the main.qss path variable
set(APPLICATION_MAIN_QSS_PATH "${PROJECT_SOURCE_DIR}/assets/styles/main.qss")

# Set the logo.png path variable
set(APPLICATION_LOGO_PATH "${PROJECT_SOURCE_DIR}/assets/images/logo.png")

# Set the APPLICATION_HOMEPAGE
set(APPLICATION_HOMEPAGE "https://github.com/srilakshmikanthanp/clipbirdesk")

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# Append ECM cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{ECM_DIR})

# Append KDNSSD_DIR cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{KDNSSD_DIR})

# Set Qt version
set(QT_MAJOR_VERSION 6)

# --------------------------------- Main Project ---------------------------------#
# Fetch SingleApplication from github
FetchContent_Declare(SingleApplication
  GIT_REPOSITORY https://github.com/itay-grudev/SingleApplication.git
  GIT_TAG        v3.4.0
)

# set QT_DEFAULT_MAJOR_VERSION to QT_MAJOR_VERSION
set(QT_DEFAULT_MAJOR_VERSION ${QT_MAJOR_VERSION})

# Set the QApplication class
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

# Make Available SingleApplication
FetchContent_MakeAvailable(SingleApplication)

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
  PRIVATE KF5::DNSSD
  PRIVATE Qt6::Network
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto)

# Include directories
target_include_directories(clipbird
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

# set target properties
# set_target_properties(clipbird PROPERTIES
#   WIN32_EXECUTABLE TRUE
#   MACOSX_BUNDLE TRUE
# )

# --------------------------------- Unit Tests ---------------------------------#
# Download and unpack googletest for unit testing
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Prevent overriding the parent project's compiler/linker settings on Windows
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

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
