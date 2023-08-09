function(add_xplugin target_name)

    add_library(${target_name} SHARED ${ARGN})
    target_link_libraries(${target_name} ${XPLUGIN_XPLUGIN_LIBRARY})

    if (WIN32)
        # add compile definitions
        target_compile_definitions(${target_name} PRIVATE  XPLUGIN_EXPORTS)

    endif()

    set_target_properties(${target_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

    if(EMSCRIPTEN)
        message(STATUS "Emscripten detected, adding -sSIDE_MODULE=1 to target ${target_name}")
        target_link_options(${target_name}
            PUBLIC "SHELL: -sSIDE_MODULE=1"
        )
    endif()

endfunction()
