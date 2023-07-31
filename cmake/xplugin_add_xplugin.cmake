function(add_xplugin target_name)

    add_library(${target_name} SHARED ${ARGN})
    target_link_libraries(${target_name} xplugin_plugin)

    if (WIN32)
        # add compile definitions
        target_compile_definitions(${target_name} PUBLIC  XPLUGIN_EXPORTS)

        set_property(TARGET ${target_name} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS true)
    endif()

    set_target_properties(${target_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

endfunction()
