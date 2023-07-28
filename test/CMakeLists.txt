# Prepare doctest for other targets to use
find_package(doctest REQUIRED)



# add custom empty target for all
add_custom_target(all_plugins)

# make a function to simplify adding plugins
function(add_plugin name source)
    add_library(${name} SHARED ${source})
    target_link_libraries(${name} xplugin)
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    add_dependencies(all_plugins ${name})

    set_target_properties(${name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

endfunction()

# build all the test plugins
add_subdirectory(testplugin_a)

# Make test executable
add_executable(cpp_tests main_test.cpp
    test_xplugin.cpp
)
target_link_libraries(cpp_tests PRIVATE xplugin )
target_include_directories(cpp_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# apple linux unix 
if (APPLE OR UNIX OR LINUX)
    target_link_libraries(cpp_tests  ${CMAKE_DL_LIBS})
endif()

# add dependencies st. test is built after all plugins
add_dependencies(cpp_tests all_plugins)


# compile with fpic
set_target_properties(cpp_tests PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

#add_test(NAME tests COMMAND cpp_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME tests COMMAND cpp_tests )

