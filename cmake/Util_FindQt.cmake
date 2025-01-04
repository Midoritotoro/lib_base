function(find_qt)
    if(TARGET Qt6::Core AND TARGET Qt6::Widgets AND TARGETS Qt6::WidgetsPrivate)
        get_target_property(QT6_INCLUDE 
            Qt6::Core 
            Qt6::Widgets 
            INTERFACE_INCLUDE_DIRECTORIES)

        target_include_directories(lib_base
            PUBLIC
                ${QT6_INCLUDE}
        )
    else()
        message(STATUS "Can't find Qt in the parent project, trying to find it using find_package")
        find_package(Qt6 REQUIRED COMPONENTS
            Core
            Widgets)

        if (NOT Qt6_FOUND)
            message(FATAL_ERROR "Can't find Qt using find_package, 
                check the integrity of the linkage of Qt in the project,
                or remove the -DLIB_BASE_ENABLE_QT option.")
        endif()
    endif()

    target_compile_definitions(lib_base
        PUBLIC
            LIB_BASE_ENABLE_QT
    )
endfunction()