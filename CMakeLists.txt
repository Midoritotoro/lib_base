cmake_minimum_required (VERSION 3.29 FATAL_ERROR)

project ("lib_base"
    VERSION 1.0.0
    DESCRIPTION "..."
    LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)


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

option(LIB_BASE_BUILD_BENCHMARKS "" ON)

option(LIB_BASE_BUILD_TESTS "" ON)
option(LIB_BASE_BUILD_EXAMPLES "" OFF)

option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${PROJECT_IS_TOP_LEVEL})
option(LIB_BASE_ENABLE_QT "" OFF)

option(LIB_BASE_ENABLE_OPENGL "" OFF)
option(LIB_BASE_ENABLE_FFMPEG "" OFF)

option(LIB_BASE_ENABLE_ZLIB "" OFF)
option(LIB_BASE_ENABLE_IMAGES "" OFF)

if (UNIX)
    option(LIB_BASE_ENABLE_PHTHREAD "" OFF)
endif()


set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")

set(LIB_BASE_BUILD_TESTS ON)
set(LIB_BASE_BUILD_BENCHMARKS ON)

set(BUILD_SHARED_LIBS OFF)
set(LIB_BASE_ENABLE_QT OFF)

set(LIB_BASE_ENABLE_OPENGL OFF) 
set(LIB_BASE_ENABLE_FFMPEG OFF)

set(LIB_BASE_ENABLE_IMAGES OFF)

set(LIB_BASE_ENABLE_ZLIB OFF)  
set(LIB_BASE_TOP_LEVEL_DIR CACHE STRING ${CMAKE_CURRENT_LIST_DIR})

set(PRIVATE_SOURCES
    src/core/math/Math.cpp
    src/core/utility/TFunctions.cpp

    src/core/system/AutoRunUtils.cpp
    src/core/system/Time.cpp

    src/core/async/CommonAsync.cpp
    src/core/async/CommonQueue.cpp

    src/core/async/WinapiAsync.cpp
    src/core/async/WinapiList.cpp
    
    src/core/async/WinapiSemaphore.cpp

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
    src/core/string/Char.cpp

    src/core/utility/simd/MinMaxImplementation.cpp
    src/core/utility/simd/traits/SimdCountTraits.cpp

    src/core/utility/simd/traits/SimdFindTraits.cpp
    src/core/string/UnicodeTables.cpp

    src/core/utility/simd/CountImplementation.cpp
    src/core/utility/simd/FindImplementation.cpp

    src/core/utility/simd/ReplaceImplementation.cpp

    src/core/utility/simd/SwapRangesVectorized.cpp
    src/core/arch/ProcessorFeatures.cpp

    src/core/utility/simd/traits/MinMaxTraitsAVX.cpp
    src/core/utility/simd/traits/MinMaxTraitsAVX512.cpp
    
    src/core/utility/simd/traits/MinMaxTraitsSSE2.cpp

    src/core/utility/simd/ContainsSubRangeVectorized.cpp
    src/core/utility/simd/FindLastSubRangeVectorized.cpp

    src/core/utility/simd/FindSubRangeVectorized.cpp
    src/core/utility/simd/CountSubRangeVectorized.cpp

    src/core/utility/simd/ContainsVectorized.cpp

    src/core/utility/simd/MismatchVectorized.cpp
    src/core/utility/simd/FindLastVectorized.cpp

    src/core/utility/simd/ReverseTriviallySwappableVectorized.cpp
    src/core/utility/simd/ReverseTriviallyCopyableVectorized.cpp

    src/core/memory/crt/BaseMemmove.cpp

    src/core/memory/crt/BaseMemcpy.cpp

    src/core/memory/crt/BaseMemcmp.h
    src/core/memory/crt/BaseMemcmp.cpp

    src/core/memory/crt/internal/BaseMemcpyInternal.h
    src/core/memory/crt/internal/BaseMemcpyInternal.cpp

    src/core/memory/crt/internal/BaseMemcpyImplementationInternal.h

    src/core/memory/MemoryAllocation.cpp

    src/core/network/Socket.cpp

    src/core/network/NetworkInterfaceMemorySafety.cpp

    src/core/network/WindowsNetworkInformation.cpp
    src/core/network/UnixNetworkInformation.cpp

        src/core/string/traits/StringConverterSseTraits.cpp
    src/core/string/traits/StringConverterScalarTraits.cpp
        
    src/core/string/traits/StringConverterAvx512Traits.cpp
     src/core/string/traits/StringConverterAvxTraits.cpp

     src/core/network/Url.cpp
     src/core/network/IPAddress.cpp

     src/core/network/WlanApiErrorChecker.cpp

     src/core/logging/MessageLogger.cpp
      src/core/logging/LogMessageFormatter.cpp

      src/core/logging/LoggingCategory.cpp
      src/core/logging/LoggingRegistry.cpp
)

set(PRIVATE_HEADERS
    src/core/arch/CpuFeatureSimdTraits.h
   
    src/core/thread/ThreadsData.h
  
    src/core/string/CharTraits.h
    src/core/string/UnicodeTables.h

    src/core/utility/simd/traits/SimdFindTraits.h
    src/core/utility/simd/traits/SimdCountTraits.h

    src/core/utility/simd/MinMax.h
    src/core/utility/simd/traits/MinMaxTraits.h

    src/core/utility/simd/traits/MinMaxTraitsAVX.h
    src/core/utility/simd/traits/MinMaxTraitsAVX512.h
    
    src/core/utility/simd/traits/MinMaxTraitsSSE2.h

    src/core/utility/simd/SimdAlgorithmSafety.h
    src/core/utility/simd/SimdTailMask.h

    src/core/utility/simd/SimdAlgorithm.h

    src/core/algorithm/AlgorithmDebug.h
    src/core/algorithm/IteratorWrap.h

    src/core/memory/crt/BaseMemcpySimd.h

    src/core/memory/crt/internal/BaseMemcpyOptimized.h
    src/core/memory/crt/internal/BaseMemcpyOptimized.cpp

    src/core/memory/UninitializedBackout.h

    src/core/memory/IteratorsDifference.h

    src/core/system/WindowsVersion.h

    src/core/string/BasicString.h
    
    src/core/string/StringDebug.h
    src/core/string/CharTypeCompatibility.h
    
    src/core/string/BasicStringStorage.h
    src/core/string/StringOptimization.h

    src/core/string/BasicStringUtility.h
    src/core/string/StringStorageRefCount.h

    src/core/memory/crt/BaseMemmove.h

    src/core/memory/crt/BaseMemset.h
    src/core/memory/crt/BaseMemset.cpp

    src/core/memory/crt/BaseMemcpy.h

    src/core/string/StringConversionOptions.h
    src/core/string/StringConversionOptions.h

    src/core/utility/simd/SimdHelpers.h

    src/core/string/traits/StringConverterAvxTraits.h

    src/core/string/traits/StringConverterAvx512Traits.h

    src/core/string/traits/StringConverterSseTraits.h

    src/core/string/StringConversionResult.h
    src/core/string/traits/StringConverterScalarTraits.h

    src/core/network/WindowsNetworkInformation.h
    src/core/network/UnixNetworkInformation.h

    src/core/network/WirelessConstants.h
    src/core/network/NetworkParameters.h

    src/core/network/NetworkInterfaceMemorySafety.h

    src/core/network/IPAddress.h
    src/core/network/Url.h

    src/core/network/WlanApiErrorChecker.h
    src/core/logging/MessageContext.h

    src/core/logging/LogMessageFormatter.h
      src/core/logging/LoggingCategory.h

      src/core/logging/LoggingRegistry.h

      src/core/network/NetworkTypes.h

      src/core/utility/IsComparableToNullptr.h

      src/core/time/MachineClock.h
)

set(PUBLIC_HEADERS 
    include/base/core/compatibility/BranchPrediction.h
    include/base/core/compatibility/CxxVersionDetection.h

    include/base/core/compatibility/Warnings.h
    include/base/core/compatibility/CallingConventions.h

    include/base/core/compatibility/CompilerVisibility.h
    include/base/core/compatibility/MsvcSpecificMacros.h

    include/base/core/compatibility/FunctionAttributes.h
    include/base/core/compatibility/AlignmentMacros.h

    include/base/core/compatibility/UnreachableCode.h
    include/base/core/compatibility/MemoryMacros.h
    
    include/base/core/compatibility/Inline.h
    include/base/core/compatibility/Nodiscard.h

    include/base/core/compatibility/CompilerDetection.h
    include/base/core/compatibility/LanguageFeatures.h

    include/base/core/compatibility/Compatibility.h
    
    include/base/core/utility/Assert.h
    include/base/core/utility/NotNull.h

    include/base/core/BaseNamespace.h

    include/base/core/arch/CpuFeature.h

    include/base/core/utility/Algorithm.h
    include/base/core/async/Concurrent.h

    include/base/core/system/SystemConfig.h

    include/base/core/utility/Flags.h
    include/base/core/system/Time.h

    include/base/core/arch/Platform.h
    include/base/core/math/OverflowCheck.h

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
 

    include/base/core/Types.h
    include/base/core/arch/SimdHelpers.h
    include/base/core/arch/SystemDetection.h

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

    include/base/core/math/BitMath.h
    include/base/core/math/Random.h

    include/base/core/math/MathConstants.h
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

    include/base/core/thread/MutexLocker.h

    include/base/core/thread/WindowsThreadPrivate.h

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

    include/base/core/string/StringConverter.h

    include/base/core/memory/MemoryAllocation.h

    include/base/core/container/Map.h
    include/base/core/container/Set.h

    include/base/core/container/FlatMap.h
    include/base/core/container/List.h

    include/base/core/container/Vector.h

    include/base/core/memory/Memory.h
    include/base/core/memory/FixedMemcmp.h

    include/base/core/container/VectorIterator.h
    include/base/core/container/VectorConstIterator.h

    include/base/core/container/Pair.h
    include/base/core/container/CompressedPair.h

    include/base/core/container/VectorDebug.h
    include/base/core/container/VectorValue.h

    include/base/core/arch/ProcessorFeatures.h
    
    include/base/core/arch/CpuId.h
     
    include/base/core/utility/Concepts.h
    include/base/core/utility/Execution.h

    include/base/core/network/Socket.h
    include/base/core/network/NetworkInformation.h

    include/base/core/logging/MessageLogger.h
    include/base/core/string/StringView.h

    include/base/core/algorithm/AllOf.h
    include/base/core/algorithm/AnyOf.h

    include/base/core/algorithm/NoneOf.h
    include/base/core/algorithm/Copy.h

    include/base/core/algorithm/Count.h

    include/base/core/algorithm/find/Find.h
    include/base/core/algorithm/find/FindIf.h

    include/base/core/algorithm/find/FindIfNot.h
    include/base/core/algorithm/find/FindFirstOf.h

    include/base/core/algorithm/find/FindLastOf.h


    include/base/core/algorithm/batch/ForEach.h
    include/base/core/algorithm/batch/ForEachN.h
    
    include/base/core/algorithm/comparison/LexicographicalCompare.h
    include/base/core/algorithm/comparison/LexicographicalCompareThreeWay.h

    include/base/core/algorithm/PartialSort.h

    include/base/core/algorithm/Remove.h
    include/base/core/algorithm/Replace.h

    include/base/core/algorithm/Reverse.h
    include/base/core/algorithm/Sort.h

    include/base/core/algorithm/SwapRanges.h
    include/base/core/algorithm/Transform.h
    
    include/base/core/algorithm/ReverseCopy.h
    include/base/core/algorithm/Rotate.h

    include/base/core/algorithm/Fill.h
    include/base/core/algorithm/Mismatch.h

    include/base/core/algorithm/Search.h
    include/base/core/algorithm/Move.h

    include/base/core/memory/Copy.h
    include/base/core/memory/Construct.h

    include/base/core/memory/Deallocate.h
    include/base/core/type_traits/IteratorCategory.h

    include/base/core/type_traits/FillMemsetSafety.h
    include/base/core/type_traits/IsConstantEvaluated.h

    include/base/core/type_traits/IsDetected.h
    include/base/core/type_traits/IsVirtualBaseOf.h

    include/base/core/type_traits/IteratorCheck.h
    include/base/core/type_traits/ContainerComparison.h

    include/base/core/type_traits/IntegralProperties.h
    include/base/core/type_traits/SwapSafety.h

    include/base/core/type_traits/TypeCheck.h

    include/base/core/memory/ToAddress.h
    include/base/core/memory/Destroy.h

    include/base/core/memory/Alignment.h
    include/base/core/memory/AlignmentOf.h

    include/base/core/memory/AddressOf.h
    include/base/core/memory/PointerConversion.h

    include/base/core/memory/AllocatorUtility.h
    include/base/core/memory/MemoryUtility.h
)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base STATIC ${PRIVATE_SOURCES} ${PUBLIC_HEADERS})
add_library(lib_base::lib_base ALIAS lib_base)

include(cmake/Util_ExportConfig.cmake)

include(cmake/Util_FindQt.cmake)
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

target_sources(lib_base PUBLIC 
    $<BUILD_INTERFACE:${PRIVATE_HEADERS}> 
    PRIVATE ${PRIVATE_SOURCES})

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

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

set_target_properties(
    lib_base PROPERTIES

    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON

    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"

    EXPORT_NAME lib_base
    OUTPUT_NAME lib_base
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_features(lib_base PUBLIC cxx_std_23)
endif()


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_DEBUG_TARGET_PROPERTIES "MultiThreaded$<$<CONFIG:Debug>:Debug>") # "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
endif()

target_link_libraries(lib_base PUBLIC OpenMP::OpenMP_CXX)

if(LIB_BASE_ENABLE_QT)
    find_qt(lib_base)
    add_subdirectory(src/qt) 

    add_library(lib_baseQt INTERFACE)
    target_link_libraries(lib_baseQt INTERFACE lib_base_qt)

    add_library(lib_base::lib_baseQt ALIAS lib_baseQt)
endif()

# if(LIB_BASE_ENABLE_OPENGL)
    # find_gl(lib_base)
    # add_subdirectory(src/gl)
# endif()

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

if(LIB_BASE_ENABLE_IMAGES)
    include(cmake/Util_FindLibPngJpeg.cmake)
    find_libpng_and_libjpeg(lib_base)

    add_subdirectory(src/images)

    add_library(lib_baseImages INTERFACE)
    target_link_libraries(lib_baseImages INTERFACE lib_base_images)

    add_library(lib_base::lib_baseImages ALIAS lib_baseImages)
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


if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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

if (WIN32)
    target_link_libraries(lib_base
    PUBLIC
        user32.dll
        kernel32.dll
        shlwapi.dll
        wlanapi.dll
        ole32.dll
        OpenMP::OpenMP_CXX
    )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # set_target_properties(lib_base
     #   PROPERTIES
      #  MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

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

if(PROJECT_IS_TOP_LEVEL AND LIB_BASE_BUILD_BENCHMARKS)
    enable_testing()
    add_subdirectory(benchmarks)
endif()


if(PROJECT_IS_TOP_LEVEL AND LIB_BASE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIB_BASE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/Util_InstallRules.cmake)
endif()
