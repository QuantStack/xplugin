add_xplugin(plugin_01 plugin_01.cpp)
add_xplugin(plugin_02 plugin_02.cpp)
add_xplugin(plugin_03 plugin_03.cpp)

target_include_directories(plugin_01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(plugin_02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(plugin_03 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
