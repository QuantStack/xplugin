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

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# helper function to add plugins
set(XPLUGIN_XPLUGIN_LIBRARY xplugin_plugin)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xplugin_add_xplugin.cmake)
# helper functions(s) for emscripten
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/xplugin_emscripten_utils.cmake)

################################################################
# Options
################################################################

OPTION(XPLUGIN_BUILD_TESTS "Build the test suite" ON)
OPTION(XPLUGIN_BUILD_EXAMPLES "Build the examples" ON)
OPTION(XPLUGIN_BUILD_DOCS "Build the documentation" OFF)

################################################################
# Versionning
################################################################

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

################################################################
# Headers
################################################################
set(XPLUGIN_HEADERS
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xfactory.hpp
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xlazy_shared_library_plugin_factory.hpp
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xplugin_config.hpp
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xplugin_registry.hpp
    ${XPLUGIN_INCLUDE_DIR}/xplugin/xshared_library.hpp
)

################################################################
#  Emscripten related
################################################################
xplugin_add_emscripten_flags()

################################################################
# Interface libraries
################################################################
add_library(xplugin INTERFACE)
add_library(xplugin_host INTERFACE)
add_library(xplugin_plugin INTERFACE)
target_link_libraries(xplugin_host INTERFACE xplugin)
target_link_libraries(xplugin_plugin INTERFACE xplugin)

################################################################
#  Dependencies
################################################################
if((APPLE OR UNIX OR LINUX ) AND NOT EMSCRIPTEN)
    target_link_libraries(xplugin_host INTERFACE ${CMAKE_DL_LIBS})
endif()

################################################################
# Flags
################################################################
if(EMSCRIPTEN)





    # compiler flags
    set_target_properties(xplugin_host PROPERTIES COMPILE_FLAGS "-s MAIN_MODULE=1")
    set_target_properties(xplugin_plugin PROPERTIES COMPILE_FLAGS "-s SIDE_MODULE=1")
    set_target_properties(xplugin_host PROPERTIES LINK_FLAGS "-s MAIN_MODULE=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['FS']")
    set_target_properties(xplugin_plugin PROPERTIES LINK_FLAGS "-s SIDE_MODULE=1")
endif()

set_property(TARGET xplugin PROPERTY CXX_STANDARD 17)
target_compile_features(xplugin INTERFACE cxx_std_17)

target_include_directories(xplugin INTERFACE $<BUILD_INTERFACE:${XPLUGIN_INCLUDE_DIR}>
                                                         $<INSTALL_INTERFACE:include>)

target_include_directories(xplugin_plugin INTERFACE $<BUILD_INTERFACE:${XPLUGIN_INCLUDE_DIR}>
                                                         $<INSTALL_INTERFACE:include>)


################################################################
# Tests
################################################################
if(XPLUGIN_BUILD_TESTS)
enable_testing()
    add_subdirectory(test)
endif()

################################################################
# Examples
################################################################
if(XPLUGIN_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


################################################################
# Documentation
################################################################
if(XPLUGIN_BUILD_DOCS)
    add_subdirectory(docs)
endif()


################################################################
# Installation
################################################################
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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(XPLUGIN_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for xpluginConfig.cmake")

# install the targets
install(
    TARGETS xplugin xplugin_host xplugin_plugin
    EXPORT xpluginTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# install the targets file
INSTALL(
    EXPORT xpluginTargets
    FILE xpluginTargets.cmake
    DESTINATION "${XPLUGIN_CMAKECONFIG_INSTALL_DIR}"
    NAMESPACE xplugin::
)

# configure the config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/xpluginConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xpluginConfig.cmake
    INSTALL_DESTINATION ${XPLUGIN_CMAKECONFIG_INSTALL_DIR}
)

# the config
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/xpluginConfig.cmake"
    DESTINATION "${XPLUGIN_CMAKECONFIG_INSTALL_DIR}"
)

# the headers
install(DIRECTORY ${XPLUGIN_INCLUDE_DIR}/xplugin
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

# install cmake file
install(FILES cmake/xplugin_add_xplugin.cmake
        DESTINATION ${XPLUGIN_CMAKECONFIG_INSTALL_DIR})
