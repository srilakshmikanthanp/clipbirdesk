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
file(GLOB_RECURSE main_cpp
  clipboard/application_clipboard_factory.cpp
  clipboard/applicationclipboard.cpp
  clipboard/platformclipboard.cpp
  clipboard/qtclipboard.cpp
  clipboard/waylandclipboard.cpp
  common/trust/trusted_clients.cpp
  common/trust/trusted_clients_factory.cpp
  common/trust/trusted_clients_qsettings.cpp
  common/trust/trusted_servers.cpp
  common/trust/trusted_servers_factory.cpp
  common/trust/trusted_servers_qsettings.cpp
  common/types/exceptions/exceptions.cpp
  common/types/ssl_config/ssl_config_factory.cpp
  constants/constants.cpp
  history/clipboard_history_factory.cpp
  history/clipboard_history.cpp
  packets/authentication/authentication.cpp
  packets/certificate_exchange_packet/certificate_exchange_packet.cpp
  packets/invalidrequest/invalidrequest.cpp
  packets/pingpongpacket/pingpongpacket.cpp
  packets/syncingpacket/syncingpacket.cpp
  service/clipbird_service_factory.cpp
  service/clipbird_service.cpp
  syncing/bluetooth/bt_browser.cpp
  syncing/bluetooth/bt_client_server_browser.cpp
  syncing/bluetooth/bt_client_server.cpp
  syncing/bluetooth/bt_client_server_session.cpp
  syncing/bluetooth/bt_device_connection_browser.cpp
  syncing/bluetooth/bt_server_client_session.cpp
  syncing/bluetooth/bt_server.cpp
  syncing/client_server_browser.cpp
  syncing/client_server.cpp
  syncing/manager/client_manager.cpp
  syncing/manager/host_manager.cpp
  syncing/manager/server_manager.cpp
  syncing/manager/syncing_manager_factory.cpp
  syncing/manager/syncing_manager.cpp
  syncing/network/dnssd_browser/dnssd_browser.cpp
  syncing/network/dnssd_register/dnssd_register.cpp
  syncing/network/net_browser.cpp
  syncing/network/net_client_server_browser.cpp
  syncing/network/net_client_server.cpp
  syncing/network/net_client_server_session.cpp
  syncing/network/net_register.cpp
  syncing/network/net_server_client_session.cpp
  syncing/network/net_server.cpp
  syncing/server.cpp
  syncing/session.cpp
  syncing/synchronizer.cpp
  ui/gui/notification/joinrequest/linux/joinrequest/joinrequest.cpp
  ui/gui/notification/joinrequest/win/joinrequest/joinrequest.cpp
  ui/gui/traymenu/traymenu.cpp
  ui/gui/utilities/functions/functions.cpp
  utility/appeventfilter/appeventfilter.cpp
  utility/functions/crypto/crypto.cpp
  utility/functions/ipconv/ipconv.cpp
  utility/functions/packet/packet.cpp
  utility/functions/qrcode/qrcode.cpp
  utility/functions/socket/socket.cpp
  utility/functions/ssl/ssl.cpp
  utility/logging/logging.cpp
  utility/powerhandler/powerhandler.cpp
  application.cpp
  application_factory.cpp
  application_state.cpp
  application_state_qsettings.cpp
  main.cpp
)

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
  Network
  Bluetooth
  WebSockets
  Qml
  Quick)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find qtkeychain
find_package(Qt6Keychain CONFIG REQUIRED)

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

qt_add_qml_module(clipbird
  URI Clipbird.Qml
  QML_FILES
    ui/gui/qml/AboutUs/AboutUs.qml
    ui/gui/qml/Components/NavigationMenu.qml
    ui/gui/qml/Components/Card.qml
    ui/gui/qml/Devices/ClientGroup.qml
    ui/gui/qml/Devices/ClientServer.qml
    ui/gui/qml/Devices/ClientServerSession.qml
    ui/gui/qml/Devices/Devices.qml
    ui/gui/qml/Devices/ServerClientSession.qml
    ui/gui/qml/Devices/ServerGroup.qml
    ui/gui/qml/History/SendClipboardCard.qml
    ui/gui/qml/History/HistoryItem.qml
    ui/gui/qml/History/HistoryItemAction.qml
    ui/gui/qml/History/HistoryItemContent.qml
    ui/gui/qml/History/History.qml
    ui/gui/qml/Settings/Settings.qml
    ui/gui/qml/Trust/Trust.qml
    ui/gui/qml/Trust/TrustedDevice.qml
    ui/gui/qml/Trust/TrustedDevices.qml
    ui/gui/qml/Main.qml
  SOURCES
    ui/gui/binding/clipboard/clipbird_qml_application_clipboard.cpp
    ui/gui/binding/common/trust/clipbird_qml_trusted_clients.cpp
    ui/gui/binding/common/trust/clipbird_qml_trusted_servers.cpp
    ui/gui/binding/constants/clipbird_qml_constants.cpp
    ui/gui/binding/history/clipbird_qml_history.cpp
    ui/gui/binding/image/clipbird_qml_image_provider.cpp
    ui/gui/binding/syncing/manager/clipbird_qml_syncing_manager.cpp
    ui/gui/binding/syncing/clipbird_qml_client_server.cpp
    ui/gui/binding/syncing/clipbird_qml_session.cpp
    ui/gui/binding/utility/clipbird_qml_hash_helper.cpp
    ui/gui/binding/utility/clipbird_qml_image_helper.cpp
    ui/gui/binding/clipbird_qml_application_state.cpp
)

# Include Directories for libqrencode
target_include_directories(clipbird
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE ${libqrencode_SOURCE_DIR}
)

file(GLOB_RECURSE BINDING_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/ui/gui/binding/*)

target_include_directories(clipbird
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ui/gui/binding
)

foreach(dir ${BINDING_DIRS})
  if(IS_DIRECTORY ${dir})
    target_include_directories(clipbird PRIVATE ${dir})
  endif()
endforeach()

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
  PRIVATE Qt6::Bluetooth
  PRIVATE Qt6::Concurrent
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto
  PRIVATE qrencode
  PRIVATE QHotkey::QHotkey
  PRIVATE Qt6::WebSockets
  PRIVATE Qt6::Qml
  PRIVATE Qt6::Quick)

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
  target_compile_definitions(clipbird PRIVATE WITH_WAYLAND)
  target_include_directories(clipbird PUBLIC ${WAYLAND_CLIENT_INCLUDE_DIR})
  target_include_directories(clipbird PUBLIC ${Avahi_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${Avahi_LIBRARIES})
  target_include_directories(clipbird PUBLIC ${DBUS_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${DBUS_LIBRARIES})
  target_include_directories(clipbird PUBLIC ${LIBNOTIFY_INCLUDE_DIRS})
  target_link_libraries(clipbird PUBLIC ${LIBNOTIFY_LIBRARIES})
  target_link_libraries(clipbird PUBLIC Qt6::DBus)
  add_definitions(-D__linux__)
endif()

# add target compiler options GCC -std=c++20
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(clipbird PRIVATE -std=c++20)
endif()

# add target compiler options MSVC /std:c++20
if(MSVC)
  target_compile_options(clipbird PRIVATE /std:c++20)
endif()
