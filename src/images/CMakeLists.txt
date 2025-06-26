set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMAGES_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/images/ImagesBuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/images/ImagesPrepare.cpp

    ${PROJECT_SOURCE_DIR}/src/images/ImagesBoxBlur.cpp
    ${PROJECT_SOURCE_DIR}/src/images/ImagesStackBlur.cpp

    ${PROJECT_SOURCE_DIR}/src/images/ImagesGaussianBlur.cpp
    ${PROJECT_SOURCE_DIR}/src/images/gl/ImagesGLImage.cpp
    
    ${PROJECT_SOURCE_DIR}/src/images/ImagesIntegral.cpp
    ${PROJECT_SOURCE_DIR}/src/images/ImagesRgbBlur.cpp

    ${PROJECT_SOURCE_DIR}/src/images/ImagesImage.cpp
    ${PROJECT_SOURCE_DIR}/src/images/ImagesUtility.cpp

    ${PROJECT_SOURCE_DIR}/src/images/formats/PngHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/images/formats/JpegHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/images/formats/BmpHandler.cpp

    ${PROJECT_SOURCE_DIR}/src/images/filters/ImageFilter.cpp
    ${PROJECT_SOURCE_DIR}/src/images/ImageReader.cpp

    ${PROJECT_SOURCE_DIR}/src/images/StackBlurTask.h
)

set(IMAGES_HEADERS
    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesImage.h
    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesBlur.h

	${PROJECT_SOURCE_DIR}/include/base/images/ImagesBuffer.h
    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesPrepare.h

    ${PROJECT_SOURCE_DIR}/include/base/images/Images.h
    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesIntegral.h

    ${PROJECT_SOURCE_DIR}/include/base/images/gl/ImagesGLImage.h

    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesCommon.h
    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesGeometry.h

    ${PROJECT_SOURCE_DIR}/include/base/images/formats/AbstractFormatHandler.h

    ${PROJECT_SOURCE_DIR}/include/base/images/formats/PngHandler.h
    ${PROJECT_SOURCE_DIR}/include/base/images/formats/JpegHandler.h
    ${PROJECT_SOURCE_DIR}/include/base/images/formats/BmpHandler.h

    ${PROJECT_SOURCE_DIR}/include/base/images/filters/ImageFilter.h
    ${PROJECT_SOURCE_DIR}/include/base/images/ImageReader.h

    ${PROJECT_SOURCE_DIR}/include/base/images/ImagesUtility.h
)

add_library(lib_base_images ${IMAGES_HEADERS})
target_sources(lib_base_images ${IMAGES_SOURCES})

set_target_properties(
    lib_base_images PROPERTIES

    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON

    EXPORT_NAME lib_base_images
    OUTPUT_NAME lib_base_images
)

include(${PROJECT_SOURCE_DIR}/cmake/Util_FindLibPngJpeg.cmake)

find_libpng_and_libjpeg(lib_base_images)
target_compile_features(lib_base_images PUBLIC cxx_std_23)

target_include_directories(lib_base_images
    PUBLIC
         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_include_directories(lib_base_images
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
)

target_include_directories(lib_base_images
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(lib_base_images
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)
