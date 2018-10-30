############################################################################
# Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   #
# Martin Renou                                                             #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.1)
project(xplugin)

set(XPLUGIN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(XPLUGIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Versionning
# ===========

file(STRINGS "${XPLUGIN_INCLUDE_DIR}/xplugin/xplugin_config.hpp" xplugin_version_defines
    REGEX "#define XPLUGIN_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${xplugin_version_defines})
    if(ver MATCHES "#define XPLUGIN_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XPLUGIN_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(${PROJECT_NAME}_VERSION
    ${XPLUGIN_VERSION_MAJOR}.${XPLUGIN_VERSION_MINOR}.${XPLUGIN_VERSION_PATCH})
message(STATUS "Building xplugin v${${PROJECT_NAME}_VERSION}")

# Dependencies
# ============

# Source files
# ============

set(XPLUGIN_HEADERS
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xplugin_config.hpp
    )

set(XPLUGIN_SOURCES
    )

# Output
# ======
