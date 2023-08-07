# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# set project name and version
project(clipbird VERSION 1.0.0 LANGUAGES CXX)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 17)

# set c++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include FindBonjour
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# include cmake modules
include(FetchContent)

# set the APPLICATION_ISSUEPAGE
set(APPLICATION_ISSUEPAGE "https://github.com/srilakshmikanthanp/clipbirdesk/issues")

# Set the APPLICATION_HOMEPAGE
set(APPLICATION_HOMEPAGE "https://github.com/srilakshmikanthanp/clipbirdesk")

# set the main.qss path variable (Light)
set(APPLICATION_LIGHT_QSS_PATH ":/styles/light.qss")

# set the main.qss path variable (Dark)
set(APPLICATION_DARK_QSS_PATH ":/styles/dark.qss")

# Set the logo.png path variable
set(APPLICATION_LOGO_PATH ":/images/logo.png")

# Set the BBONJOUR_DIR from env
set(BONJOUR_SDK_HOME $ENV{BONJOUR_SDK_HOME})

# Set Qt version
set(QT_MAJOR_VERSION 6)

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

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

# Add network/service/avahi if the os is linux
if(NOT WIN32 AND NOT APPLE)
  list(FILTER main_cpp EXCLUDE REGEX "network/service/dnsd")
endif()

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Widgets
  Network)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find Bonjour on windows and mac
if(WIN32 OR APPLE)
  find_package(Bonjour REQUIRED)
endif()

# Configure config.hpp
configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp)

# set up project using Qt
qt_standard_project_setup()

# add resources to project
qt_add_resources(RESOURCES
  ${PROJECT_SOURCE_DIR}/assets/resources.qrc)

# Add executable
qt_add_executable(clipbird
  ${main_cpp} ${RESOURCES})

# Link libraries
target_link_libraries(clipbird
  PRIVATE SingleApplication::SingleApplication
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto)

# link bonjour if windows or Mac
if(WIN32 OR APPLE)
  target_link_libraries(clipbird
    PRIVATE ${BONJOUR_LIBRARIES})
endif()

# Include directories
target_include_directories(clipbird
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

# include in win or mac
if (WIN32 OR APPLE)
  target_include_directories(clipbird
    PUBLIC ${BONJOUR_INCLUDE_DIR})
endif()

# set target properties
set_target_properties(clipbird PROPERTIES
  WIN32_EXECUTABLE TRUE
  MACOSX_BUNDLE TRUE
)

# add target compiler options GCC -std=c++17
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(clipbird PRIVATE -std=c++17)
endif()

# add target linker options MSVC /NODEFAULTLIB:library
# add target compiler options MSVC /std:c++17
if(MSVC)
  target_link_options(clipbird PRIVATE /NODEFAULTLIB:library)
  target_compile_options(clipbird PRIVATE /std:c++17)
endif()

# Add Platform specific definitions for apple
if(APPLE)
  add_definitions(-D__APPLE__)
endif()

# Add Platform specific definitions for win
if(WIN32)
  add_definitions(-D_WIN32)
endif()

# Add Platform specific definitions for linux
if(UNIX AND NOT APPLE)
  add_definitions(-D__linux__)
endif()

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
  network/packets/*.cpp)

# Add Executable to test
qt_add_executable(check
  ${test_cpp})

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
