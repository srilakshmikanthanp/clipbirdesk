# set the main.qss path variable (Light)
set(APPLICATION_LIGHT_QSS_PATH ":/styles/light.qss")

# set the main.qss path variable (Dark)
set(APPLICATION_DARK_QSS_PATH ":/styles/dark.qss")

# Set the logo.ico path variable
set(APPLICATION_LOGO_PATH ":/images/logo.ico")

# Clipbird Domain
set(CLIPBIRD_API_DOMAIN "clipbird-api.srilakshmikanthanp.com" CACHE STRING "Clipbird Domain")

# websocket url
set(CLIPBIRD_WEBSOCKET_URL "wss://${CLIPBIRD_API_DOMAIN}" CACHE STRING "Clipbird WebSocket URL")

# Base url
set(CLIPBIRD_API_URL "https://${CLIPBIRD_API_DOMAIN}" CACHE STRING "Clipbird Base URL")

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

# Fetch https://github.com/nlohmann/json.git
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

# Make Available nlohmann_json
FetchContent_MakeAvailable(json)

# Fetch https://github.com/libcpr/cpr.git
FetchContent_Declare(cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG        1.12.0
)

# Make Available cpr
FetchContent_MakeAvailable(cpr)

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
  clipboard/applicationclipboard.cpp
  clipboard/clipboard_controller.cpp
  clipboard/platformclipboard.cpp
  clipboard/qtclipboard.cpp
  clipboard/waylandclipboard.cpp
  constants/constants.cpp
  controller/controller.cpp
  history/history_controller.cpp
  mdns/browser.cpp
  mdns/dnssd_browser/dnssd_browser.cpp
  mdns/dnssd_register/dnssd_register.cpp
  mdns/register.cpp
  packets/authentication/authentication.cpp
  packets/invalidrequest/invalidrequest.cpp
  packets/pingpongpacket/pingpongpacket.cpp
  packets/syncingpacket/syncingpacket.cpp
  store/read_auth_token_dto_job.cpp
  store/read_hub_host_device_job.cpp
  store/secure_storage.cpp
  store/storage.cpp
  syncing/lan/client/client.cpp
  syncing/lan/lan_controller.cpp
  syncing/lan/server/server.cpp
  syncing/synchronizer.cpp
  syncing/wan/auth/auth_api_client.cpp
  syncing/wan/auth/auth_api_repository.cpp
  syncing/wan/auth/auth_controller.cpp
  syncing/wan/auth/auth_repository.cpp
  syncing/wan/auth/auth_token_holder.cpp
  syncing/wan/device/device_api_client.cpp
  syncing/wan/device/device_api_repository.cpp
  syncing/wan/device/device_repository.cpp
  syncing/wan/device/device_response_dto.cpp
  syncing/wan/hub/hub_host_device.cpp
  syncing/wan/hub/hub_message_clipboard_dispatch_payload_handler.cpp
  syncing/wan/hub/hub_message_device_added_payload_handler.cpp
  syncing/wan/hub/hub_message_device_removed_payload_handler.cpp
  syncing/wan/hub/hub_message_device_updated_payload_handler.cpp
  syncing/wan/hub/hub_message_devices_payload_handler.cpp
  syncing/wan/hub/hub_message_handler.cpp
  syncing/wan/hub/hub_message_nonce_challenge_completed_payload_handler.cpp
  syncing/wan/hub/hub_message_nonce_challenge_request_payload_handler.cpp
  syncing/wan/hub/hub_websocket.cpp
  syncing/wan/hub/hub.cpp
  syncing/wan/wan_controller.cpp
  syncing/wan/wan_service.cpp
  types/device.cpp
  types/except/except.cpp
  ui/gui/components/cliphistory/cliphistory.cpp
  ui/gui/components/clipsend/clipsend.cpp
  ui/gui/components/cliptile/cliptile.cpp
  ui/gui/components/hostlist/hostlist.cpp
  ui/gui/components/hosttile/hosttile.cpp
  ui/gui/modals/aboutus/aboutus.cpp
  ui/gui/modals/connect/connect.cpp
  ui/gui/modals/group/group.cpp
  ui/gui/modals/signin/signin.cpp
  ui/gui/notification/joinrequest/linux/joinrequest/joinrequest.cpp
  ui/gui/notification/joinrequest/win/joinrequest/joinrequest.cpp
  ui/gui/traymenu/traymenu.cpp
  ui/gui/utilities/functions/functions.cpp
  ui/gui/widgets/clipbird/clipbird.cpp
  ui/gui/widgets/history/history.cpp
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
  WebSockets)

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
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE Qt6Keychain::Qt6Keychain
  PRIVATE cpr::cpr
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE Qt6::Concurrent
  PRIVATE OpenSSL::SSL
  PRIVATE OpenSSL::Crypto
  PRIVATE qrencode
  PRIVATE QHotkey::QHotkey
  PRIVATE Qt6::WebSockets)

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
