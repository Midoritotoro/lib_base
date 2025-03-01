function(find_ffmpeg TARGET) 
    enable_language(ASM)

    set(ffmpeg_lib_list)
    set(ffmpeg_lib_loc ${libs_loc}/ffmpeg)

    message(ffmpeglibloc ${ffmpeg_lib_loc})

    list(APPEND ffmpeg_lib_list
        app::external_opus
        app::external_openh264
    )

    if (WIN32)
        list(APPEND ffmpeg_lib_list
            ${libs_loc}/dav1d/builddir-$<IF:$<CONFIG:Debug>,debug,release>/src/libdav1d.a
        )
    endif()


    target_include_directories(${TARGET}
    PUBLIC
        ${ffmpeg_lib_loc}
    )

    list(APPEND ffmpeg_lib_list
        ${ffmpeg_lib_loc}/libavfilter/libavfilter.a
        ${ffmpeg_lib_loc}/libavformat/libavformat.a

        ${ffmpeg_lib_loc}/libavcodec/libavcodec.a
        ${ffmpeg_lib_loc}/libswresample/libswresample.a

        ${ffmpeg_lib_loc}/libswscale/libswscale.a
        ${ffmpeg_lib_loc}/libavutil/libavutil.a
    )

    target_link_libraries(${TARGET}
    PUBLIC
        ${ffmpeg_lib_list}
    )

    if (WIN32)
        target_link_libraries(${TARGET} 
        PUBLIC
            Synchronization.dll
            winmm.dll
            bcrypt.dll)
    endif()
endfunction()