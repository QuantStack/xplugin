cmake_minimum_required(VERSION 3.1)
project(serialize_example)

find_package(xplugin REQUIRED)

######################################################################
# the main executable
######################################################################
add_executable(main_serialize main_serialize.cpp)
target_link_libraries(main_serialize xplugin::xplugin_host)
target_include_directories(main_serialize PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

######################################################################
# the plugins:
######################################################################

# simple (plain string)
find_package(nlohmann_json REQUIRED)
add_xplugin(plugin_simple serialize_simple.cpp)
target_include_directories(plugin_simple PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# json
find_package(nlohmann_json REQUIRED)
add_xplugin(plugin_json serialize_json.cpp)
target_link_libraries(plugin_json nlohmann_json::nlohmann_json)
target_include_directories(plugin_json PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
