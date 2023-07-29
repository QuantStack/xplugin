 # add custom empty target for all
 add_custom_target(all_plugins)

 # make a function to simplify adding plugins
 function(add_plugin name source)
    add_library(${name} SHARED ${source})
    target_link_libraries(${name} PRIVATE xplugin_plugin)
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    add_dependencies(all_plugins ${name})
    # add compile definitions
    target_compile_definitions(${name} PRIVATE
        XPLUGIN_EXPORTS
    )

    set_target_properties(${name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
endfunction()

# build all the test plugins
add_subdirectory(testplugin_a)

if(NOT EMSCRIPTEN)

    find_package(doctest REQUIRED)
    # Make test executable
    add_executable(cpp_tests main_test.cpp
        test_xplugin.cpp
    )
    target_link_libraries(cpp_tests PRIVATE xplugin_host doctest::doctest)
    target_include_directories(cpp_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # add dependencies st. test is built after all plugins
    add_dependencies(cpp_tests all_plugins)

    # compile with fpic
    set_target_properties(cpp_tests PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

    #add_test(NAME tests COMMAND cpp_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME tests COMMAND cpp_tests )

else()

    add_executable(xplugin_embind main_xplugin_embind.cpp)
    target_link_libraries(xplugin_embind PRIVATE xplugin_host)


    # flag to include embind
    target_link_options(xplugin_embind
        PUBLIC "SHELL: -s MAIN_MODULE=1"
        PUBLIC "SHELL: -l embind"
        PUBLIC "SHELL: -s ENVIRONMENT=web"
        PUBLIC "SHELL: -s MODULARIZE=1"
        PUBLIC "SHELL: -s EXPORT_NAME=\"createModule\""
    )

    set_target_properties(xplugin_embind PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

endif()