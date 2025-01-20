function(find_zlib TARGET)
    # ...
    target_compile_definitions(${TARGET}
        PUBLIC
            LIB_BASE_ENABLE_ZLIB
    )
endfunction()