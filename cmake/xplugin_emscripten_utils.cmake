function(xplugin_add_emscripten_main_module_flags target_name)
    if(EMSCRIPTEN)
        target_link_options(${target_name}
            PUBLIC "SHELL: -sMAIN_MODULE=1"
            PUBLIC "SHELL: -sMODULARIZE=1"
        )
    endif()
endfunction()

function(xplugin_add_emscripten_flags)
    if(EMSCRIPTEN)
        set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
        #  for emscripten we do not want any threads or thread primitives
        add_compile_definitions(XPLUGIN_NO_THREADS)

        set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-s SIDE_MODULE=1")
        set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "-s SIDE_MODULE=1")
        set(CMAKE_STRIP FALSE)

    endif()
endfunction()
