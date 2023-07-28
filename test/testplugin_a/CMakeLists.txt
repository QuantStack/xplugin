add_plugin(plugin_01 plugin_01.cpp)
#add_plugin(plugin_02 plugin_02.cpp)
# add_plugin(plugin_03 plugin_03.cpp)

target_link_libraries(plugin_01 PRIVATE xplugin)