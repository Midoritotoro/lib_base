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

option(LIB_BASE_BUILD_TESTS "" ON)
option(LIB_BASE_BUILD_EXAMPLES "" OFF)

option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${PROJECT_IS_TOP_LEVEL})
option(LIB_BASE_ENABLE_QT "" ON)

option(LIB_BASE_ENABLE_OPENGL "" OFF)
option(LIB_BASE_ENABLE_FFMPEG "" ON)

option(LIB_BASE_ENABLE_ZLIB "" ON)

if (UNIX)
    option(LIB_BASE_ENABLE_PHTHREAD "" OFF)
endif()


set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")

set(LIB_BASE_BUILD_TESTS ON)

set(BUILD_SHARED_LIBS OFF)
set(LIB_BASE_ENABLE_QT ON)

set(LIB_BASE_ENABLE_OPENGL OFF)
set(LIB_BASE_ENABLE_FFMPEG ON)

set(LIB_BASE_ENABLE_ZLIB ON)
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

    src/core/ConcurrentAncillary.cpp

    src/core/Thread.cpp
    src/core/AtomicInteger.cpp

    src/core/WindowsAtomic.cpp
    src/core/UnixAtomic.cpp

    src/core/WindowsThread.cpp
    src/core/UnixThread.cpp

    src/core/WindowsMutex.cpp
    src/core/UnixMutex.cpp

    src/core/Mutex.cpp

    src/core/ThreadsData.h
    src/core/ThreadsData.cpp

    src/core/MutexLocker.cpp
    src/io/FileInfo.cpp

    src/io/WindowsFileInfo.cpp
    src/io/UnixFileInfo.cpp

    src/io/Directory.cpp
    src/io/WindowsDirectory.cpp

    src/io/UnixDirectory.cpp

    src/core/WindowsStandardPaths.cpp
    src/core/StandardPaths.cpp

    src/core/UnixStandardPaths.cpp
)

set(PUBLIC_HEADERS 
    include/base/utility/Assert.h
    include/base/core/BaseNamespace.h

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

    include/base/concurrent/win/WinapiAsync.h

    include/base/concurrent/win/WinapiDll.h

    include/base/concurrent/win/WinapiList.h
    include/base/concurrent/win/WinapiSemaphore.h

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

    include/base/core/WindowsThread.h
    include/base/core/UnixThread.h


    include/base/utility/ClassHelperMacros.h
    include/base/utility/Swap.h

    include/base/utility/TypeInfo.h

    include/base/utility/BitOps.h
    include/base/utility/Random.h

    include/base/utility/MathConstants.h
    include/base/system/Endian.h

    include/base/core/ConcurrentAncillary.h

    include/base/core/Thread.h
    include/base/core/AtomicInteger.h

    include/base/core/AbstractAtomic.h
    include/base/core/WindowsAtomic.h

    include/base/core/UnixAtomic.h
    include/base/core/CommonAtomicOperations.h

    include/base/core/AtomicConfig.h
    include/base/utility/Foreach.h

    include/base/core/WindowsMutex.h
    include/base/core/UnixMutex.h

    include/base/core/Mutex.h
    include/base/core/AbstractMutex.h

    include/base/core/YieldCpu.h
    include/base/core/ThreadsConfig.h

    include/base/system/KeywordSupport.h
    include/base/core/MutexLocker.h

    include/base/core/WindowsThreadPrivate.h
    include/base/utility/MacrosOverload.h

    include/base/io/FileInfo.h

    include/base/io/WindowsFileInfo.h
    include/base/io/UnixFileInfo.h

    include/base/io/Directory.h
    include/base/io/WindowsDirectory.h

    include/base/io/UnixDirectory.h

    include/base/core/WindowsStandardPaths.h
    include/base/core/StandardPaths.h

    include/base/core/UnixStandardPaths.h
    include/base/core/AbstractFileInfo.h

    include/base/core/AbstractStandardPaths.h
)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)


add_library(lib_base ${PUBLIC_HEADERS})

add_library(lib_base::lib_base ALIAS lib_base)

include(cmake/Util_ExportConfig.cmake)
include(cmake/Util_FindGSL.cmake)

include(cmake/Util_FindQt.cmake)
include(cmake/Util_FindLibPngJpeg.cmake)

find_libpng_and_libjpeg(lib_base)
find_ms_gsl(lib_base)

find_package(OpenMP REQUIRED)

if(LIB_BASE_ENABLE_PHTHREAD)
    find_package(Threads REQUIRED)

    if (Threads_FOUND)
        target_compile_definitions(lib_base
            PUBLIC
            LIB_BASE_USE_PHTHREAD)

        target_link_libraries(lib_base 
            PUBLIC
            Threads::Threads)
    endif()
endif()

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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_DEBUG_TARGET_PROPERTIES "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
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
    include(cmake/Util_FindFFmpeg.cmake)
    find_ffmpeg(lib_base)

    if (LIB_BASE_ENABLE_QT)
        find_ffmpeg(lib_base_qt)
    endif()

    add_subdirectory(src/media/ffmpeg)

    add_library(lib_baseFFmpeg INTERFACE)
    target_link_libraries(lib_baseFFmpeg INTERFACE lib_base_ffmpeg)

    add_library(lib_base::lib_baseFFmpeg ALIAS lib_baseFFmpeg)
endif()

#string(REGEX REPLACE "(-march=[a-zA-Z0-9-]*)|(-mtune=[a-zA-Z0-9-]*)|(-msse[0-9,\\.]*)|(-mavx[a-z0-9]*)|(-mfma)|(-mamx-[a-z0-9]*)" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(CMAKE_GENERATOR MATCHES "Visual Studio")
	if((CMAKE_VS_PLATFORM_NAME STREQUAL "x64") OR (CMAKE_VS_PLATFORM_NAME STREQUAL "x86"))
		include(cmake/x86_Simd.cmake)
    endif()
endif()


if((CMAKE_SYSTEM_PROCESSOR STREQUAL "i686") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64"))
		include(cmake/x86_Simd.cmake)
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g) 
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -Ot)
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

message(${CMAKE_CURRENT_SOURCE_DIR})
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
   set_target_properties(lib_base
        PROPERTIES
        MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

        target_link_options(lib_base
    INTERFACE
       ${CMAKE_LINK_}
   )

    if (LIB_BASE_ENABLE_FFMPEG)
       set_target_properties(lib_baseFFmpeg
        PROPERTIES
        MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

        target_link_options(lib_baseFFmpeg
            INTERFACE
        ${CMAKE_LINK_}
     )
    endif() 

    if (LIB_BASE_ENABLE_QT)
       set_target_properties(lib_baseQt
        PROPERTIES
        MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

        target_link_options(lib_baseQt
            INTERFACE
        ${CMAKE_LINK_}
     )
    endif() 
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
