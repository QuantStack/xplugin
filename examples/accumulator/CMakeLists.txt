
# add a min accumulator plugin
add_library(plugin_accumulator_min SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_min.cpp)
target_include_directories(plugin_accumulator_min PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_accumulator_min PUBLIC xplugin)
target_compile_features(plugin_accumulator_min PRIVATE cxx_std_17)

# add a max accumulator plugin
add_library(plugin_accumulator_max SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_max.cpp)
target_include_directories(plugin_accumulator_max PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_accumulator_max PUBLIC xplugin)
target_compile_features(plugin_accumulator_max PRIVATE cxx_std_17)

# add  main executable using the plugins
add_executable(main_accumulator ${CMAKE_CURRENT_SOURCE_DIR}/main_accumulator.cpp)
target_include_directories(main_accumulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_accumulator PUBLIC xplugin)
target_compile_features(main_accumulator PRIVATE cxx_std_17)

# apple linux unix 
if (APPLE OR UNIX OR LINUX)
    target_link_libraries(main_accumulator PRIVATE ${CMAKE_DL_LIBS})
endif()
