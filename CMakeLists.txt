cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project ("lib_base"
    VERSION 1.0.0
    DESCRIPTION "..."
    LANGUAGES CXX)

set(CMAKE_LINK_ $<IF:$<CONFIG:Debug>, /NODEFAULTLIB:LIBCMT, /DEBUG;/OPT:REF>
       /INCREMENTAL:NO)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin")
endif()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

set(libs_loc "D:/Libraries/win64")

option(LIB_BASE_BUILD_TESTS "Сборка тестов библиотеки" ON)
option(LIB_BASE_BUILD_EXAMPLES "Сборка примеров использования библиотеки" OFF)
option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${PROJECT_IS_TOP_LEVEL})
option(LIB_BASE_LINK_OPTIONS "Опции линковки библиотеки для MSVC" ${linkOptions})
option(LIB_BASE_ENABLE_QT "Включить вспомогательный интерфейс для Qt" OFF)
option(LIB_BASE_ENABLE_OPENGL "Поддержка OpenGL ES 2" OFF)
option(LIB_BASE_ENABLE_FFMPEG "Утилиты ffmpeg" OFF)
option(LIB_BASE_ENABLE_ZLIB "Поддержка zlib" OFF)
option(LIB_BASE_ENABLE_STRING_PARSER "" ON)


set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")
set(BUILD_SHARED_LIBS OFF)
set(LIB_BASE_ENABLE_QT OFF)
set(LIB_BASE_ENABLE_OPENGL OFF)
set(LIB_BASE_ENABLE_FFMPEG OFF)
set(LIB_BASE_ENABLE_ZLIB OFF)
set(LIB_BASE_ENABLE_STRING_PARSER OFF)
set(LIB_BASE_TOP_LEVEL_DIR CACHE STRING ${CMAKE_CURRENT_LIST_DIR})


   
set(PRIVATE_SOURCES 
    src/utility/Math.cpp
    src/utility/TFunctions.cpp

    src/AutoRunUtils.cpp
    src/Time.cpp

    src/concurrent/common/CommonAsync.cpp
    src/concurrent/common/CommonQueue.cpp

    src/concurrent/win/WinapiAsync.cpp
    src/concurrent/win/WinapiList.cpp

    src/concurrent/win/WinapiSemaphore.cpp

 #   src/concurrent/common/CommonAncillary.cpp
    
 #  src/concurrent/win/WinapiAtomic.cpp
 #   src/concurrent/win/WinapiThreads.cpp

	src/images/ImagesBuffer.cpp
    src/images/ImagesPrepare.cpp

    src/images/ImagesBoxBlur.cpp
    src/images/ImagesStackBlur.cpp

    src/images/ImagesGaussianBlur.cpp
    src/images/gl/ImagesGLImage.cpp

    src/images/StackBlurTask.h

    src/images/ImagesIntegral.cpp
    src/images/ImagesRgbBlur.cpp

    src/images/ImagesImage.cpp
    src/images/ImagesUtility.cpp

    src/images/formats/PngHandler.cpp
    src/images/formats/JpegHandler.cpp
    src/images/formats/BmpHandler.cpp

    src/images/filters/ImageFilter.cpp
    src/images/ImageReader.cpp

    src/system/SystemInfo.cpp
    src/io/File.cpp

    src/io/WindowsFileEngine.cpp
    src/io/UnixFileEngine.cpp

    src/system/SystemTools.cpp
    src/io/Compressor.cpp

    src/io/WindowsSmartHandle.cpp

    src/system/WindowsSystemInfo.cpp
    src/system/UnixSystemInfo.cpp

    src/io/UnixFileSystemPath.cpp
    src/io/WindowsFileSystemPath.cpp

  #  src/io/ByteArray.cpp
  #  src/io/ArrayDataPointer.cpp

  #  src/io/ArrayData.cpp
  #  src/io/ByteArrayMatcher.cpp
    src/core/ConcurrentAncillary.cpp

    src/core/Thread.cpp
    src/core/Atomic.cpp

    src/core/WindowsAtomic.cpp
    src/core/UnixAtomic.cpp
)

set(PUBLIC_HEADERS 
    include/base/utility/Assert.h

    include/base/utility/Algorithm.h
    include/base/concurrent/Concurrent.h

    include/base/system/SystemConfig.h

    include/base/utility/Flags.h
    include/base/system/Time.h

    include/base/system/Platform.h
    include/base/utility/OverflowCheck.h

    include/base/system/Windows.h
    include/base/system/ProcessorDetection.h

    include/base/concurrent/common/CommonConfig.h
    include/base/concurrent/common/CommonGuards.h

    include/base/concurrent/common/CommonAsync.h

    include/base/concurrent/common/CommonAsyncGuarded.h

    include/base/concurrent/common/CommonQueue.h

    include/base/concurrent/common/CommonSync.h
    include/base/concurrent/common/CommonUtils.h

 #  include/base/concurrent/common/CommonAncillary.h

    include/base/concurrent/win/WinapiAsync.h

    include/base/concurrent/win/WinapiDll.h

    include/base/concurrent/win/WinapiList.h
    include/base/concurrent/win/WinapiSemaphore.h

   # include/base/concurrent/win/WinapiAtomic.h
  # include/base/concurrent/win/WinapiThreads.h

    include/base/core/BaseExport.h
    include/base/utility/TypeWrapper.h

    include/base/system/SystemTools.h

    include/base/utility/TypeTraits.h
    include/base/images/ImagesBlur.h

	include/base/images/ImagesBuffer.h
    include/base/images/ImagesPrepare.h


    include/base/images/Images.h
    include/base/images/ImagesIntegral.h

    include/base/images/gl/ImagesGLImage.h

    include/base/images/ImagesCommon.h
    include/base/images/ImagesGeometry.h

    include/base/images/formats/AbstractFormatHandler.h

    include/base/images/formats/PngHandler.h
    include/base/images/formats/JpegHandler.h
    include/base/images/formats/BmpHandler.h

    include/base/images/filters/ImageFilter.h
    include/base/images/ImageReader.h

    include/base/images/ImagesUtility.h

    include/base/core/Types.h
    include/base/system/SimdHelpers.h

    include/base/images/ImagesImage.h

    include/base/system/SystemDetection.h
    include/base/system/CompilerDetection.h

    include/base/io/WindowsFileEngine.h
    include/base/io/UnixFileEngine.h

    include/base/io/File.h
    include/base/system/SystemInfo.h

    include/base/io/WindowsSmartHandle.h
    include/base/io/Compressor.h

    include/base/io/FileSystemPath.h
    include/base/io/IOConfig.h

    include/base/system/SystemString.h
    include/base/system/SystemStaticCodeAnalysis.h

    include/base/system/WindowsSystemInfo.h
    include/base/system/UnixSystemInfo.h

  #  include/base/io/ByteArray.h
  #  include/base/io/ArrayDataPointer.h

    include/base/utility/ClassHelperMacros.h
    include/base/utility/Swap.h

    include/base/utility/TypeInfo.h
    include/base/io/ArrayData.h

    include/base/utility/BitOps.h
    include/base/utility/Random.h

    include/base/utility/MathConstants.h
  #  include/base/io/ByteArrayView.h

  #  include/base/io/ByteArrayAlgorithms.h
  #  include/base/io/ByteArrayPrivate.h

    include/base/system/Endian.h
    include/base/string/StringCommonConfig.h

    include/base/core/ConcurrentAncillary.h

    include/base/core/Thread.h
    include/base/core/Atomic.h

    include/base/core/AbstractAtomic.h
    include/base/core/WindowsAtomic.h

    include/base/core/UnixAtomic.h
    include/base/core/CommonAtomicOperations.h

    include/base/core/AtomicConfig.h
)


include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

set(STRING_HEADERS)

if(LIB_BASE_ENABLE_STRING_PARSER AND NOT LIB_BASE_ENABLE_QT)
    set(STRING_HEADERS
        include/base/string/parser/TextUtility.h
        include/base/string/parser/String.h

        include/base/string/String.h
        include/base/string/Char.h

        include/base/string/StringView.h
        include/base/string/UnicodeTable.h

        include/base/string/AnyStringView.h
        include/base/string/Latin1StringView.h

        include/base/string/StringFwd.h
        include/base/io/ByteArrayMatcher.h

        include/base/string/StringMatcher.h
        include/base/string/Latin1StringMatcher.h

        include/base/io/ByteArrayView.h
        include/base/string/UnicodeTools.h

        include/base/string/StringTokenizer.h
        include/base/string/Utf8StringView.h

        include/base/string/StringLiteral.h
        include/base/string/StringConverter.h

        include/base/string/StringConverterBase.h
        include/base/string/StringConverterPrivate.h

        include/base/string/StringAlgorithms.h
        include/base/string/StringAlgorithmsPrivate.h

        include/base/string/StringBuilder.h
        )
endif()


add_library(lib_base ${PUBLIC_HEADERS} ${STRING_HEADERS})
add_library(lib_base::lib_base ALIAS lib_base)

include(cmake/Util_ExportConfig.cmake)
include(cmake/Util_FindGSL.cmake)
include(cmake/Util_FindQt.cmake)
include(cmake/Util_FindLibPngJpeg.cmake)

find_libpng_and_libjpeg(lib_base)
find_ms_gsl(lib_base)
find_package(OpenMP REQUIRED)


target_sources(lib_base PRIVATE ${PRIVATE_SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

set_target_properties(
    lib_base PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME lib_base
    OUTPUT_NAME lib_base
)

target_compile_features(lib_base PUBLIC cxx_std_23)

if(NOT DEFINED CMAKE_DEBUG_TARGET_PROPERTIES OR NOT CMAKE_DEBUG_TARGET_PROPERTIES)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_DEBUG_TARGET_PROPERTIES "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

target_link_libraries(lib_base PUBLIC OpenMP::OpenMP_CXX)

if(LIB_BASE_ENABLE_QT)
    find_qt(lib_base)
    add_subdirectory(src/qt) 

    add_library(lib_baseQt INTERFACE)
    target_link_libraries(lib_baseQt INTERFACE lib_base_qt)

    add_library(lib_base::lib_baseQt ALIAS lib_baseQt)
endif()

if(LIB_BASE_ENABLE_OPENGL)
    find_gl(lib_base)
    # add_subdirectory(src/gl)
endif()

#if(LIB_BASE_ENABLE_ZLIB)
    #find_zlib(lib_base)
    # add_subdirectory(src/zlib)
#endif()

if(LIB_BASE_ENABLE_FFMPEG)
    add_subdirectory(src/media/ffmpeg)

    add_library(lib_baseFFmpeg INTERFACE)
    target_link_libraries(lib_baseFFmpeg INTERFACE lib_base_ffmpeg)

    add_library(lib_base::lib_baseFFmpeg ALIAS lib_baseFFmpeg)
endif()

if(LIB_BASE_ENABLE_STRING_PARSER)
    if (LIB_BASE_ENABLE_QT)
        target_compile_definitions(lib_base
            PUBLIC
            LIB_BASE_USE_QSTRING_CLASS_AS_MAIN
        )
    else()
        target_sources(lib_base
            PRIVATE
            src/string/Char.cpp
            src/string/String.cpp

            src/string/StringView.cpp
            src/string/UnicodeTable.cpp

            src/string/AnyStringView.cpp
            src/string/Latin1StringView.cpp
        
            src/string/StringMatcher.cpp
            src/string/Latin1StringMatcher.cpp

            src/string/UnicodeTools.cpp
            src/string/StringTokenizer.cpp

            src/string/StringConverter.cpp
            src/string/StringBuilder.cpp

            src/qt/text/TextEntities.cpp
            src/string/parser/TextEntities.h

            src/string/parser/TextWord.cpp
            src/string/parser/TextWord.h
    
            src/string/parser/TextDrawUtility.cpp
            src/string/parser/TextDrawUtility.h

            src/string/parser/TextClickHandlers.cpp

            src/string/parser/String.cpp
            src/string/parser/TextBidiAlgorithm.h

            src/string/parser/TextStackEngine.cpp
            src/string/parser/TextStackEngine.h

            src/string/parser/WordParser.cpp
            src/string/parser/WordParser.h

            src/string/parser/BlockParser.cpp
            src/string/parser/BlockParser.h

            src/string/parser/TextRenderer.cpp
            src/string/parser/TextRenderer.h

            src/string/parser/TextUtility.cpp

            src/string/parser/TextBlock.cpp
            src/string/parser/TextBlock.h
        )

        target_compile_definitions(lib_base
            PUBLIC
            LIB_BASE_USE_STRING_CLASS_AS_MAIN
        )
    endif()

    target_compile_definitions(lib_base
        PUBLIC
        LIB_BASE_ENABLE_STRING_PARSER
    )
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g) 
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2 -Ot)
    endif()

    target_compile_options(
        lib_base 
        PRIVATE
            /permissive-
            /W4
            /utf-8
            /MP
            /EHsc   
            /w15038 
            /w14265 
            /wd4018
            /wd4100
            /wd4242
            /wd4244
            /wd4245
            /wd4267
            /wd4305
            /wd4324
            /wd4389
            /wd4456
            /wd4457
            /wd4458
            /wd4459
            /wd4611
            /wd4702
            /wd4828
            /Zi
            /Zc:__cplusplus
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(
        lib_base 
        PRIVATE
            /utf-8
            /O3
            /W4
            /EHsc   
            /Zi
            /Zc:__cplusplus
    )
endif()

target_include_directories(lib_base
    PUBLIC
         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_include_directories(lib_base
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
)

target_include_directories(lib_base
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(lib_base
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

if (WIN32)
    target_link_libraries(lib_base
    PUBLIC
        user32.dll
        kernel32.dll
        shlwapi.dll
        OpenMP::OpenMP_CXX
    )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   set_target_properties(${PROJECT_NAME} PROPERTIES
       MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

    target_link_options(${PROJECT_NAME}
    INTERFACE
       ${linkOptions}
   )
endif()

if(PROJECT_IS_TOP_LEVEL AND LIB_BASE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PROJECT_IS_TOP_LEVEL AND LIB_BASE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIB_BASE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/Util_InstallRules.cmake)
endif()
