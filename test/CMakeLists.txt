 # add custom empty target for all libraries / plugins
 # st. we hand make the main test executable depend on all of them
add_custom_target(all_plugins)

add_subdirectory(testplugin_a)

if(NOT EMSCRIPTEN)

    add_subdirectory(testlib)

    find_package(doctest REQUIRED)
    # Make test executable
    add_executable(cpp_tests main_test.cpp
        test_xplugin.cpp
        test_xshared_library.cpp
    )

    target_link_libraries(cpp_tests PRIVATE xplugin_host doctest::doctest)
    target_include_directories(cpp_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # add dependencies st. test is built after all plugins
    add_dependencies(cpp_tests all_plugins)

    set_target_properties(cpp_tests PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

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
