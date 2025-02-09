set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(libs_loc "D:/Libraries/win64")

set(FFMPEG_PRIVATE_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Chroma.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/ColorSpace.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Filter.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Fourcc.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/FrameGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/HardwareAcceleration.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Object.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Picture.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Scale.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/SubPicture.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/ThumbnailGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/Variables.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/video/VideoFormat.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/audio/AbstractAudioFFmpegReader.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/audio/AbstractAudioLoader.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/audio/AbstractAudioReader.cpp
    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/audio/AudioFormat.cpp

    ${PROJECT_SOURCE_DIR}/src/media/ffmpeg/audio/AudioReader.cpp
)

set(FFMPEG_PUBLIC_HEADERS 
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Chroma.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/ColorSpace.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Config.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Filter.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Fourcc.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/FourccCodecs.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/FrameGenerator.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/HardwareAcceleration.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Object.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Picture.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Scale.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/SubPicture.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/ThumbnailGenerator.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/Variables.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/VideoFormat.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/video/VideoHelper.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/audio/AbstractAudioFFmpegReader.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/audio/AbstractAudioLoader.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/audio/AbstractAudioReader.h
    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/audio/AudioFormat.h

    ${PROJECT_SOURCE_DIR}/include/base/media/ffmpeg/audio/AudioReader.h
)


include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base_ffmpeg ${FFMPEG_PUBLIC_HEADERS})

if(LIB_BASE_ENABLE_QT)
    include(${PROJECT_SOURCE_DIR}/cmake/Util_FindQt.cmake)
    find_qt(lib_base_ffmpeg)
endif()

set_target_properties(
    lib_base_ffmpeg PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    EXPORT_NAME lib_base_ffmpeg
    OUTPUT_NAME lib_base_ffmpeg
)

target_compile_features(lib_base_ffmpeg PUBLIC cxx_std_23)

include(${PROJECT_SOURCE_DIR}/cmake/Util_FindOpenH264.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Util_FindOpus.cmake)

include(${PROJECT_SOURCE_DIR}/cmake/Util_FindFFmpeg.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Util_FindGSL.cmake)

find_ms_gsl(lib_base_ffmpeg)

target_sources(lib_base_ffmpeg PRIVATE ${FFMPEG_PRIVATE_SOURCES})

target_link_libraries(lib_base_ffmpeg
    PRIVATE
    app::external_ffmpeg)

target_include_directories(lib_base_ffmpeg
    PUBLIC
         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_include_directories(lib_base_ffmpeg
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
)

target_include_directories(lib_base_ffmpeg
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(lib_base_ffmpeg
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)