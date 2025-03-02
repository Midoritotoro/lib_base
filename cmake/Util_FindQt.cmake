function(find_qt TARGET)
    if(TARGET Qt6::Core AND TARGET Qt6::Widgets AND TARGET Qt6::OpenGL AND TARGET Qt6::OpenGLWidgets)
        message("Qt was found in parent project")

        set(targets            
             Qt6::Core 
             Qt6::Widgets 
             Qt6::WidgetsPrivate
             Qt6::OpenGL
             Qt6::OpenGLWidgets)

       get_target_property( 
           targets
           ${TARGET}
           INTERFACE_INCLUDE_DIRECTORIES)

        target_include_directories(${TARGET}
           PUBLIC
               ${QT6_INCLUDE}
        )

        target_link_libraries(${TARGET} # Потом удалю
           PUBLIC
                Qt6::Core 
                Qt6::Widgets 
                Qt6::WidgetsPrivate
                Qt6::OpenGL
                Qt6::OpenGLWidgets
        )

    else()
        message(STATUS "Can't find Qt in the parent project, trying to find it using find_package")
        find_package(Qt6 REQUIRED COMPONENTS
            Core
            Widgets
            OpenGL
            OpenGLWidgets)

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
                Qt6::OpenGL
                Qt6::OpenGLWidgets
            )
    endif()

    target_compile_definitions(${TARGET}
        PUBLIC
            LIB_BASE_ENABLE_QT
    )
endfunction()