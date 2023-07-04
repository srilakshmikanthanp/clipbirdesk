cmake_minimum_required(VERSION 3.0.0)
project(clipbirdesk VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (UNIX AND NOT APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/Qt/6.5.1/gcc_64/lib/cmake")
ELSEIF(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:\\Qt\\6.5.1\\mingw_64\\lib\\cmake")
ELSEIF(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/Qt/6.5.1/clang_64/lib/cmake")
ENDIF()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup()
qt_add_executable(clipbirdesk
    main.cpp
)

target_link_libraries(clipbirdesk PRIVATE Qt6::Widgets)
target_link_libraries(clipbirdesk PRIVATE Qt6::Network)
