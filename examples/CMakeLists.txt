
add_library(plugin_accumulator_min SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_min.cpp)
target_include_directories(plugin_accumulator_min PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_accumulator_min PUBLIC xplugin)
target_compile_features(plugin_accumulator_min PRIVATE cxx_std_17)

add_library(plugin_accumulator_max SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin_accumulator_max.cpp)
target_include_directories(plugin_accumulator_max PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugin_accumulator_max PUBLIC xplugin)
target_compile_features(plugin_accumulator_max PRIVATE cxx_std_17)

# add  main executable
add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main PUBLIC xplugin)
target_compile_features(main PRIVATE cxx_std_17)
