function(find_gl TARGET)
    # ...
    target_compile_definitions(${TARGET}
        PUBLIC
            LIB_BASE_ENABLE_OPENGL
    )
endfunction()