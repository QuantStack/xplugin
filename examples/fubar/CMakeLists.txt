
# add a min accumulator plugin
add_library(plugin_bar SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_bar.cpp)
target_include_directories(plugin_bar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_bar PUBLIC xplugin_plugin)
target_compile_features(plugin_bar PRIVATE cxx_std_17)
set_property(TARGET plugin_bar PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS true)

# add a max accumulator plugin
add_library(plugin_foo SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_foo.cpp)
target_include_directories(plugin_foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_foo PUBLIC xplugin_plugin)
target_compile_features(plugin_foo PRIVATE cxx_std_17)
set_property(TARGET plugin_foo PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS true)

# add  main executable using the plugins
add_executable(main_fubar ${CMAKE_CURRENT_SOURCE_DIR}/main_fubar.cpp)
target_include_directories(main_fubar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_fubar PUBLIC xplugin_host)
target_compile_features(main_fubar PRIVATE cxx_std_17)

# compile with fpic
set_target_properties(main_fubar PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if(EMSCRIPTEN)
    target_link_options(main_fubar
        PUBLIC "SHELL: -sMAIN_MODULE=1"
        PUBLIC "SHELL: -sMODULARIZE=1"
    )
endif()
