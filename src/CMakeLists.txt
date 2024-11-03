# set the main.qss path variable (Light)
set(APPLICATION_LIGHT_QSS_PATH ":/styles/light.qss")

# set the main.qss path variable (Dark)
set(APPLICATION_DARK_QSS_PATH ":/styles/dark.qss")

# Set the logo.ico path variable
set(APPLICATION_LOGO_PATH ":/images/logo.ico")

# Set the BBONJOUR_DIR from env
set(BONJOUR_SDK_HOME $ENV{BONJOUR_SDK_HOME})

# set OPENSSL_ROOT_DIR
set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})

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

# Fetch https://github.com/Skycoder42/QHotkey.git
FetchContent_Declare(QHotkey
  GIT_REPOSITORY https://github.com/Skycoder42/QHotkey.git
  GIT_TAG bb630252684d3556b79ac7a521616692f348fcf7
)

# Make Available QHotkey
FetchContent_MakeAvailable(QHotkey)

# Fetch qzxing from libqrencode
FetchContent_Declare(libqrencode
  GIT_REPOSITORY https://github.com/fukuchi/libqrencode.git
  GIT_TAG        v4.1.1
)

# WITH_TOOLS
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)

# Make Available qzxing
FetchContent_MakeAvailable(libqrencode)

# set auto rcc
set(CMAKE_AUTORCC ON)

# glob pattern for main cpp files exclude
file(GLOB_RECURSE main_cpp *.cpp)

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Configure config.hpp
configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Concurrent
  Widgets
  Network)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find DNS-SD libraries for different platforms
if(WIN32 OR APPLE)
  find_package(Bonjour REQUIRED)
elseif(UNIX AND NOT APPLE)
  find_package(Avahi REQUIRED)
else()
  message(FATAL_ERROR "Platform not supported")
endif()

# Find LibNotify on linux
if(UNIX AND NOT APPLE)
  find_package(LibNotify REQUIRED)
endif()

# find wayland
if(UNIX AND NOT APPLE)
  find_package(Wayland REQUIRED COMPONENTS Client)
  find_package(Qt6 REQUIRED COMPONENTS WaylandCompositor WaylandClient)
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
  ${CMAKE_CURRENT_LIST_DIR}/assets/resources.qrc)

# copy assets to build directory
file(
  COPY
  ${CMAKE_CURRENT_LIST_DIR}/assets
  DESTINATION
  ${PROJECT_BINARY_DIR}
)

# if windows
if(WIN32)
  set (RESOURCES ${RESOURCES} ${CMAKE_CURRENT_LIST_DIR}/assets/windows.rc)
endif()

# Add executable
qt_add_executable(clipbird
  ${main_cpp} ${RESOURCES})

# Include Directories for libqrencode
target_include_directories(clipbird
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE ${libqrencode_SOURCE_DIR}
)

# Include Directories
include_directories(${PROJECT_BINARY_DIR})

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
  PRIVATE qrencode
  PRIVATE QHotkey::QHotkey)

# set target properties
set_target_properties(clipbird PROPERTIES
  WIN32_EXECUTABLE TRUE
  MACOSX_BUNDLE TRUE
)

# generate wayland protocol sources
if (UNIX AND NOT APPLE)
  qt_generate_wayland_protocol_client_sources(
    clipbird
    FILES
      ${CMAKE_CURRENT_LIST_DIR}/protocols/wlr-data-control-unstable-v1.xml
  )
endif()

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
  target_compile_definitions(clipbird PRIVATE WITH_WAYLAND)
  target_link_libraries(clipbird PRIVATE Qt6::GuiPrivate Qt6::WaylandCompositor Qt6::WaylandClient Qt6::WaylandClientPrivate Wayland::Client)
  target_include_directories(clipbird PUBLIC ${WAYLAND_CLIENT_INCLUDE_DIR})
  target_include_directories(clipbird PUBLIC ${Avahi_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${Avahi_LIBRARIES})
  target_include_directories(clipbird PUBLIC ${DBUS_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${DBUS_LIBRARIES})
  target_include_directories(clipbird PUBLIC ${LIBNOTIFY_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${LIBNOTIFY_LIBRARIES})
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
