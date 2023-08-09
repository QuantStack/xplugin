add_xplugin(plugin_foo ${CMAKE_CURRENT_SOURCE_DIR}/plugin_foo.cpp)
target_include_directories(plugin_foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_xplugin(plugin_bar ${CMAKE_CURRENT_SOURCE_DIR}/plugin_bar.cpp)
target_include_directories(plugin_bar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add  main executable using the plugins
add_executable(main_fubar ${CMAKE_CURRENT_SOURCE_DIR}/main_fubar.cpp)
target_include_directories(main_fubar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_fubar PUBLIC xplugin_host)
set_target_properties(main_fubar PROPERTIES POSITION_INDEPENDENT_CODE ON)

xplugin_add_emscripten_main_module_flags(main_fubar)

# if tests are enabled, add a the example to the tests
if (XPLUGIN_BUILD_TESTS)
    add_test(NAME example_fubar COMMAND main_fubar ${CMAKE_CURRENT_BINARY_DIR})
endif()
