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
    src/core/utility/Math.cpp
    src/core/utility/TFunctions.cpp

    src/core/system/AutoRunUtils.cpp
    src/core/system/Time.cpp

    src/core/async/CommonAsync.cpp
    src/core/async/CommonQueue.cpp

    src/core/async/WinapiAsync.cpp
    src/core/async/WinapiList.cpp

    src/core/async/WinapiSemaphore.cpp

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

    src/core/system/SystemInfo.cpp
    src/core/io/File.cpp

    src/core/io/WindowsFileEngine.cpp
    src/core/io/UnixFileEngine.cpp

    src/core/system/SystemTools.cpp
    src/core/io/Compressor.cpp

    src/core/io/WindowsSmartHandle.cpp

    src/core/system/WindowsSystemInfo.cpp
    src/core/system/UnixSystemInfo.cpp

    src/core/thread/Thread.cpp
    src/core/thread/AtomicInteger.cpp

    src/core/thread/WindowsAtomic.cpp
    src/core/thread/UnixAtomic.cpp

    src/core/thread/WindowsThread.cpp
    src/core/thread/UnixThread.cpp

    src/core/thread/WindowsMutex.cpp
    src/core/thread/UnixMutex.cpp

    src/core/thread/Mutex.cpp

    src/core/thread/ThreadsData.h
    src/core/thread/ThreadsData.cpp

    src/core/thread/MutexLocker.cpp
    src/core/io/FileInfo.cpp

    src/core/io/WindowsFileInfo.cpp
    src/core/io/UnixFileInfo.cpp

    src/core/io/Directory.cpp
    src/core/io/WindowsDirectory.cpp

    src/core/io/UnixDirectory.cpp

    src/core/system/WindowsStandardPaths.cpp
    src/core/system/StandardPaths.cpp

    src/core/system/UnixStandardPaths.cpp
    src/core/string/String.cpp

    src/core/string/Char.cpp

    src/core/string/UnicodeTables.h
    src/core/string/UnicodeTables.cpp
)

set(PUBLIC_HEADERS 
    include/base/core/utility/Assert.h
    include/base/core/BaseNamespace.h

    include/base/core/utility/Algorithm.h
    include/base/core/async/Concurrent.h

    include/base/core/system/SystemConfig.h

    include/base/core/utility/Flags.h
    include/base/core/system/Time.h

    include/base/core/arch/Platform.h
    include/base/core/utility/OverflowCheck.h

    include/base/core/system/Windows.h
    include/base/core/arch/ProcessorDetection.h

    include/base/core/async/CommonConfig.h
    include/base/core/async/CommonGuards.h

    include/base/core/async/CommonAsync.h

    include/base/core/async/CommonAsyncGuarded.h

    include/base/core/async/CommonQueue.h

    include/base/core/async/CommonSync.h
    include/base/core/async/CommonUtils.h

    include/base/core/async/WinapiAsync.h

    include/base/core/async/WinapiDll.h

    include/base/core/async/WinapiList.h
    include/base/core/async/WinapiSemaphore.h

    include/base/core/BaseExport.h
    include/base/core/system/SystemTools.h

    include/base/core/utility/TypeTraits.h
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
    include/base/core/arch/SimdHelpers.h

    include/base/images/ImagesImage.h

    include/base/core/arch/SystemDetection.h
    include/base/core/arch/CompilerDetection.h

    include/base/core/io/WindowsFileEngine.h
    include/base/core/io/UnixFileEngine.h

    include/base/core/io/File.h
    include/base/core/system/SystemInfo.h

    include/base/core/io/WindowsSmartHandle.h
    include/base/core/io/Compressor.h

    include/base/core/io/IOConfig.h
    include/base/core/arch/StaticCodeAnalysis.h

    include/base/core/system/WindowsSystemInfo.h
    include/base/core/system/UnixSystemInfo.h

    include/base/core/thread/WindowsThread.h
    include/base/core/thread/UnixThread.h

    include/base/core/utility/ClassHelperMacros.h
    include/base/core/utility/Swap.h

    include/base/core/utility/TypeInfo.h

    include/base/core/utility/BitOps.h
    include/base/core/utility/Random.h

    include/base/core/utility/MathConstants.h
    include/base/core/arch/Endian.h

    include/base/core/thread/Thread.h
    include/base/core/thread/AtomicInteger.h

    include/base/core/thread/AbstractAtomic.h
    include/base/core/thread/WindowsAtomic.h

    include/base/core/thread/UnixAtomic.h
    include/base/core/thread/CommonAtomicOperations.h

    include/base/core/thread/AtomicConfig.h

    include/base/core/thread/WindowsMutex.h
    include/base/core/thread/UnixMutex.h

    include/base/core/thread/Mutex.h
    include/base/core/thread/AbstractMutex.h

    include/base/core/arch/YieldCpu.h
    include/base/core/thread/ThreadsConfig.h

    include/base/core/arch/KeywordSupport.h
    include/base/core/thread/MutexLocker.h

    include/base/core/thread/WindowsThreadPrivate.h
    include/base/core/utility/MacrosOverload.h

    include/base/core/io/FileInfo.h

    include/base/core/io/WindowsFileInfo.h
    include/base/core/io/UnixFileInfo.h

    include/base/core/io/Directory.h
    include/base/core/io/WindowsDirectory.h

    include/base/core/io/UnixDirectory.h

    include/base/core/system/WindowsStandardPaths.h
    include/base/core/system/StandardPaths.h

    include/base/core/system/UnixStandardPaths.h
    include/base/core/io/AbstractFileInfo.h

    include/base/core/system/AbstractStandardPaths.h
    include/base/core/io/AbstractDirectory.h

    include/base/core/string/StringConfig.h
    include/base/core/string/String.h

    include/base/core/string/StringIterator.h
    include/base/core/string/Char.h

    include/base/core/string/CharacterHelper.h
    include/base/core/string/StringConstIterator.h

    include/base/core/memory/MemoryAllocator.h
    include/base/core/memory/MemoryRange.h

    include/base/core/memory/MemoryAllocation.h

    include/base/core/container/Map.h
    include/base/core/container/Set.h

    include/base/core/container/FlatMap.h
    include/base/core/container/List.h

    include/base/core/container/Vector.h

    include/base/core/memory/Memory.h

    include/base/core/container/VectorIterator.h
    include/base/core/container/VectorConstIterator.h

    include/base/core/memory/MemoryUtility.h

    include/base/core/container/Pair.h
    include/base/core/container/CompressedPair.h

    include/base/core/math/VectorizedAlgorithm.h

    include/base/core/math/MinMax.h
    include/base/core/math/MinMaxTraits.h

    include/base/core/arch/ProcessorFeatures.h
    
    include/base/core/arch/CpuId.h

    include/base/core/math/MinMaxTraitsAVX.h
    include/base/core/math/MinMaxTraitsAVX512.h

    include/base/core/math/MinMaxTraitsSSE2.h
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
            
            /guard:cf

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
