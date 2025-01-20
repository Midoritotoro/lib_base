function(find_qt TARGET)
    if(TARGET Qt6::Core AND TARGET Qt6::Widgets AND TARGET Qt6::WidgetsPrivate)
        message("Qt was found in parent project")

        set(targets            
             Qt6::Core 
             Qt6::Widgets 
             Qt6::WidgetsPrivate)

       get_target_property( 
           targets
           ${TARGET}
           INTERFACE_INCLUDE_DIRECTORIES)

        target_include_directories(${TARGET}
           PUBLIC
               ${QT6_INCLUDE}
        )
    else()
        message(STATUS "Can't find Qt in the parent project, trying to find it using find_package")
        find_package(Qt6 REQUIRED COMPONENTS
            Core
            Widgets
            OpenGL)

        if (NOT Qt6_FOUND)
            message(FATAL_ERROR "Can't find Qt using find_package, 
                check the integrity of the linkage of Qt in the project,
                or remove the -DLIB_BASE_ENABLE_QT option.")
        endif()

        target_link_libraries(${TARGET}
            PUBLIC
                Qt6::Core 
                Qt6::Widgets 
                Qt6::WidgetsPrivate
            )
    endif()

    target_compile_definitions(${TARGET}
        PUBLIC
            LIB_BASE_ENABLE_QT
    )

    if (TARGET Qt6::OpenGL)
        find_package(Qt6 REQUIRED COMPONENTS OpenGL)
        target_link_libraries(${TARGET}
            PUBLIC
            Qt6::OpenGL
        )

        target_compile_definitions(${TARGET}
        PUBLIC
            LIB_BASE_ENABLE_QT_OPENGL
    )

    endif()
endfunction()