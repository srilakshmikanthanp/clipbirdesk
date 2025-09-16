# Copyright (c) 2024 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# CMakeLists.txt for clipbird
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# read version from ini file
file(READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" VERSION)

# Read UUID from file
file(READ "${CMAKE_CURRENT_LIST_DIR}/APPUUID" UUID)

# set project name and version
project(clipbird VERSION ${VERSION} LANGUAGES C CXX)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 17)

# set c++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include FindBonjour
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# include cmake modules
include(FetchContent)

# include ExternalProject
include(ExternalProject)

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

# Set Qt version
set(QT_MAJOR_VERSION 6)

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# Qt Keychain dir
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_KEYCHAIN_CMAKE_DIR})

# set CMP0071 to NEW
cmake_policy(SET CMP0071 NEW)

# add subdirectory for src
add_subdirectory(src)

# add subdirectory for tests
add_subdirectory(tests)
