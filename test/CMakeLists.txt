 # add custom empty target for all libraries / plugins
 # st. we hand make the main test executable depend on all of them
add_custom_target(all_plugins)

add_subdirectory(testplugin_a)

if(NOT EMSCRIPTEN)
    add_subdirectory(testlib)

    find_package(doctest REQUIRED)

    SET(TEST_SOURCES main_test.cpp
        test_xplugin.cpp
        test_xshared_library.cpp
        test_xlazy_shared_library.cpp
    )

    # Make test executable (one with threading support, one without)
    add_executable(cpp_tests            ${TEST_SOURCES})
    add_executable(cpp_tests_no_threads ${TEST_SOURCES})

    # compile definition to disable threading
    target_compile_definitions(cpp_tests_no_threads PRIVATE XPLUGIN_NO_THREADS)

    #  function to set properties for both test executables
    function(set_tests_properties  test_target)
        target_link_libraries(${test_target} PRIVATE xplugin_host doctest::doctest)
        target_include_directories(${test_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        add_dependencies(${test_target} all_plugins)
        set_target_properties(${test_target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
        add_test(NAME ${test_target} COMMAND ${test_target} )
    endfunction(set_tests_properties )

    set_tests_properties(cpp_tests)
    set_tests_properties(cpp_tests_no_threads)

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

    set_target_properties(xplugin_embind PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
