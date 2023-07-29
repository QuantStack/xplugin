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

# Options
# =======

OPTION(XPLUGIN_BUILD_TESTS "Build the test suite" ON)
OPTION(XPLUGIN_BUILD_EXAMPLES "Build the examples" ON)


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
${XPLUGIN_INCLUDE_DIR}/xplugin/xshared_library.hpp
${XPLUGIN_INCLUDE_DIR}/xplugin/xfactory.hpp
${XPLUGIN_INCLUDE_DIR}/xplugin/xplugin_registry.hpp
)

if(EMSCRIPTEN)
    set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
    set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-s SIDE_MODULE=1")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "-s SIDE_MODULE=1")
    set(CMAKE_STRIP FALSE) 
endif()


# Output
# ======

# interface library plugins link against
add_library(xplugin INTERFACE)
add_library(xplugin_host INTERFACE)
add_library(xplugin_plugin INTERFACE)

target_link_libraries(xplugin_host INTERFACE xplugin)
target_link_libraries(xplugin_plugin INTERFACE xplugin)

#define XPLUGIN_EXPORTS
target_compile_definitions(xplugin_plugin INTERFACE XPLUGIN_EXPORTS)

if((APPLE OR UNIX OR LINUX ) AND NOT EMSCRIPTEN)
    target_link_libraries(xplugin_host INTERFACE ${CMAKE_DL_LIBS})
endif()

if(EMSCRIPTEN)
    # compiler flags
    set_target_properties(xplugin_host PROPERTIES COMPILE_FLAGS "-s MAIN_MODULE=1")
    set_target_properties(xplugin_plugin PROPERTIES COMPILE_FLAGS "-s SIDE_MODULE=1")
    set_target_properties(xplugin_host PROPERTIES LINK_FLAGS "-s MAIN_MODULE=1")
    set_target_properties(xplugin_plugin PROPERTIES LINK_FLAGS "-s SIDE_MODULE=1")
endif()


set_property(TARGET xplugin PROPERTY CXX_STANDARD 17)
target_compile_features(xplugin INTERFACE cxx_std_17)

# compile with fpic
set_target_properties(xplugin PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if (APPLE)
    set_target_properties(xplugin PROPERTIES
        MACOSX_RPATH ON
    )
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib; ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
elseif(NOT EMSCRIPTEN)
    set_target_properties(xplugin PROPERTIES
        BUILD_WITH_INSTALL_RPATH 1
    )
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib; ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

target_include_directories(xplugin INTERFACE $<BUILD_INTERFACE:${XPLUGIN_INCLUDE_DIR}>
                                                         $<INSTALL_INTERFACE:include>)

target_include_directories(xplugin_plugin INTERFACE $<BUILD_INTERFACE:${XPLUGIN_INCLUDE_DIR}>
                                                         $<INSTALL_INTERFACE:include>)

# Tests
# =====
if(XPLUGIN_BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif()

# Examples
# ========
if(XPLUGIN_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()