
# add a min accumulator plugin
add_xplugin(plugin_accumulator_min ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_min.cpp)
target_include_directories(plugin_accumulator_min PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_xplugin(plugin_accumulator_max ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_min.cpp)
target_include_directories(plugin_accumulator_max PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# add  main executable using the plugins
add_executable(main_accumulator ${CMAKE_CURRENT_SOURCE_DIR}/main_accumulator.cpp)
target_include_directories(main_accumulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_accumulator PUBLIC xplugin_host)

# compile with fpic
# set_target_properties(main_accumulator PROPERTIES
#     POSITION_INDEPENDENT_CODE ON
# )

if(EMSCRIPTEN)
    target_link_options(main_accumulator
        PUBLIC "SHELL: -sMAIN_MODULE=1"
        PUBLIC "SHELL: -sMODULARIZE=1"
    )
endif()
