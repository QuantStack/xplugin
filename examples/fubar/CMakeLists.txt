
# add a min accumulator plugin
add_library(plugin_bar SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_bar.cpp)
target_include_directories(plugin_bar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_bar PUBLIC xplugin)
target_compile_features(plugin_bar PRIVATE cxx_std_17)

# add a max accumulator plugin
add_library(plugin_foo SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_foo.cpp)
target_include_directories(plugin_foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_foo PUBLIC xplugin)
target_compile_features(plugin_foo PRIVATE cxx_std_17)

# add  main executable using the plugins
add_executable(main_fubar ${CMAKE_CURRENT_SOURCE_DIR}/main_fubar.cpp)
target_include_directories(main_fubar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_fubar PUBLIC xplugin)
target_compile_features(main_fubar PRIVATE cxx_std_17)


# apple linux unix 
if (APPLE OR UNIX OR LINUX)
    target_link_libraries(main_fubar PRIVATE ${CMAKE_DL_LIBS})
endif()