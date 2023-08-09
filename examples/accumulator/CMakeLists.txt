
# add a min accumulator plugin
add_xplugin(plugin_accumulator_min ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_min.cpp)
target_include_directories(plugin_accumulator_min PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add a max accumulator plugin
add_xplugin(plugin_accumulator_max ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_max.cpp)
target_include_directories(plugin_accumulator_max PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add  main executable using the plugins
add_executable(main_accumulator ${CMAKE_CURRENT_SOURCE_DIR}/main_accumulator.cpp)
target_include_directories(main_accumulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_accumulator PUBLIC xplugin_host)
target_compile_features(main_accumulator PRIVATE cxx_std_17)
set_target_properties(main_accumulator PROPERTIES POSITION_INDEPENDENT_CODE ON)

xplugin_add_emscripten_main_module_flags(main_accumulator)


# if tests are enabled, add a the example to the tests
if (XPLUGIN_BUILD_TESTS)
    add_test(NAME example_accumulator COMMAND main_accumulator ${CMAKE_CURRENT_BINARY_DIR})
endif()
