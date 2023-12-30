# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# read version from ini file
file(READ "${CMAKE_CURRENT_LIST_DIR}/conf/VERSION" VERSION)

# Read UUID from file
file(READ "${CMAKE_CURRENT_LIST_DIR}/conf/APPUUID" UUID)

# set project name and version
project(clipbird VERSION ${VERSION} LANGUAGES CXX)

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

# Set the DONATE_URL
set(DONATE_URL "https://srilakshmikanthanp.github.io/donate")

# Set the APPLICATION_HOMEPAGE
set(APPLICATION_HOMEPAGE "https://github.com/srilakshmikanthanp/clipbirdesk")

# Set the Version
set(VERSION ${PROJECT_VERSION})

# Set the UUID
set(UUID ${UUID})

# set organization name
set(APPLICATION_ORGANIZATION_NAME "srilakshmikanthanp")

# Set the logo.png path variable
set(APPLICATION_LOGO_PATH ":/images/logo.ico")

# set the main.qss path variable (Light)
set(APPLICATION_LIGHT_QSS_PATH ":/styles/light.qss")

# set the main.qss path variable (Dark)
set(APPLICATION_DARK_QSS_PATH ":/styles/dark.qss")

# Set the BBONJOUR_DIR from env
set(BONJOUR_SDK_HOME $ENV{BONJOUR_SDK_HOME})

# set OPENSSL_ROOT_DIR
set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})

# Set Qt version
set(QT_MAJOR_VERSION 6)

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# set CMP0071 to NEW
cmake_policy(SET CMP0071 NEW)

# set CMP0135 to NEW
cmake_policy(SET CMP0135 NEW)

# --------------------------------- Main Project ---------------------------------#
# Fetch https://github.com/mohabouje/WinToast.git only if windows
if (WIN32)
  FetchContent_Declare(WinToast
    GIT_REPOSITORY https://github.com/mohabouje/WinToast.git
    GIT_TAG        v1.3.0
  )

  # Make Available WinToast
  FetchContent_MakeAvailable(WinToast)
endif()

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

# Fetch qzxing from libqrencode
FetchContent_Declare(libqrencode
  GIT_REPOSITORY https://github.com/fukuchi/libqrencode.git
  GIT_TAG        v4.1.1
)

# WITH_TOOLS
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)

# Make Available qzxing
FetchContent_MakeAvailable(libqrencode)

# glob pattern for main cpp files exclude
file(GLOB_RECURSE main_cpp *.cpp)

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Configure config.hpp
configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Concurrent
  Widgets
  Network
  Sql)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find DNS-SD libraries for different platforms
if(WIN32 OR APPLE)
  find_package(Bonjour REQUIRED)
elseif(LINUX)
  find_package(Avahi REQUIRED)
else()
  message(FATAL_ERROR "Platform not supported")
endif()

# Finc D-Bus
if (UNIX AND NOT APPLE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(DBUS dbus-1 REQUIRED)
endif()

# set up project using Qt
qt_standard_project_setup()

# add resources to project
qt_add_resources(RESOURCES
  ${PROJECT_SOURCE_DIR}/assets/resources.qrc)

# if windows
if(WIN32)
  set (RESOURCES ${RESOURCES} ${PROJECT_SOURCE_DIR}/assets/windows.rc)
endif()

# Add executable
qt_add_executable(clipbird
  ${main_cpp} ${RESOURCES})

# Include Directories for libqrencode
target_include_directories(clipbird
  PRIVATE ${libqrencode_SOURCE_DIR}
)

# Include Directories for Protobuf
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})

# Include directories
target_include_directories(clipbird
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR})

# Link libraries
target_link_libraries(clipbird
  PRIVATE SingleApplication::SingleApplication
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE Qt6::Concurrent
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto
  PRIVATE qrencode)

# set target properties
set_target_properties(clipbird PROPERTIES
  WIN32_EXECUTABLE TRUE
  MACOSX_BUNDLE TRUE
)

# Add Platform specific definitions for apple
if(APPLE)
  target_include_directories(clipbird PUBLIC ${BONJOUR_INCLUDE_DIR})
  target_link_libraries(clipbird PRIVATE ${BONJOUR_LIBRARIES})
  add_definitions(-D__APPLE__)
endif()

# Add Platform specific definitions for win
if(WIN32)
  target_include_directories(clipbird PUBLIC ${BONJOUR_INCLUDE_DIR})
  target_link_libraries(clipbird PRIVATE WinToast)
  target_link_libraries(clipbird PRIVATE Dwmapi.lib)
  target_link_libraries(clipbird PRIVATE runtimeobject.lib)
  target_link_libraries(clipbird PRIVATE ${BONJOUR_LIBRARIES})
  add_definitions(-D_WIN32)
endif()

# Add Platform specific definitions for linux
if(UNIX AND NOT APPLE)
  target_include_directories(clipbird PUBLIC ${Avahi_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${Avahi_LIBRARIES})
  target_include_directories(clipbird PUBLIC ${DBUS_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${DBUS_LIBRARIES})
  add_definitions(-D__linux__)
endif()

# add target compiler options GCC -std=c++17
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(clipbird PRIVATE -std=c++17)
endif()

# add target compiler options MSVC /std:c++17
if(MSVC)
  target_compile_options(clipbird PRIVATE /std:c++17)
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
  ${test_cpp} ${PROTO_SRCS} ${PROTO_HDRS})

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

# link test executable to gtest & gtest_main
target_link_libraries(check
  PRIVATE GTest::gtest_main
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network)
