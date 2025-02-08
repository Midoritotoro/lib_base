add_library(external_ffmpeg INTERFACE IMPORTED GLOBAL)
add_library(app::external_ffmpeg ALIAS external_ffmpeg)

enable_language(ASM)

set(ffmpeg_lib_list)
set(ffmpeg_lib_loc ${libs_loc}/ffmpeg)

list(APPEND ffmpeg_lib_list
    app::external_opus
    app::external_openh264
)

if (WIN32)
    list(APPEND ffmpeg_lib_list
        ${libs_loc}/dav1d/builddir-$<IF:$<CONFIG:Debug>,debug,release>/src/libdav1d.a
    )
endif()


target_include_directories(external_ffmpeg SYSTEM
INTERFACE
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

target_link_libraries(external_ffmpeg
INTERFACE
    ${ffmpeg_lib_list}
)
