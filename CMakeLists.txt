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

option(BUILD_TESTS "Сборка тестов библиотеки" ON)
option(BUILD_EXAMPLES "Сборка примеров использования библиотеки" OFF)
option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${PROJECT_IS_TOP_LEVEL})
option(LINK_OPTIONS "Опции линковки библиотеки для MSVC" ${linkOptions})
option(ENABLE_QT "Включить вспомогательный интерфейс для Qt" ON)


set(LINK_OPTIONS ${linkOptions})
set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")
set(BUILD_SHARED_LIBS OFF)
set(ENABLE_QT ON)

   
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
)


set(PUBLIC_HEADERS 
    include/base/Base.h
    include/base/Assert.h

    include/base/Utility.h
    include/base/Concurrent.h

    include/base/Flags.h
    include/base/Time.h

    include/base/Platform.h
    include/base/OverflowCheck.h

    include/base/Windows.h
    include/base/ProcessorDetection.h

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

    include/base/BaseExport.h
)


include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base ${PUBLIC_HEADERS})
add_library(lib_base::lib_base ALIAS lib_base)

include(cmake/Util_ExportConfig.cmake)
include(cmake/Util_FindGSL.cmake)

find_ms_gsl(lib_base)

target_sources(lib_base PRIVATE ${PRIVATE_SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

set_target_properties(
    lib_base PROPERTIES
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
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

if(ENABLE_QT)
    add_subdirectory(src/qt) 

    add_library(lib_baseQt INTERFACE)
    target_link_libraries(lib_baseQt INTERFACE lib_base_qt)

    add_library(lib_base::lib_baseQt ALIAS lib_baseQt)
endif()


# message("LIBTYPE: " ${LIBTYPE})

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
    )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   set_target_properties(${PROJECT_NAME} PROPERTIES
       MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

    target_link_options(${PROJECT_NAME}
    INTERFACE
       ${LINK_OPTIONS}
   )
endif()

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PROJECT_IS_TOP_LEVEL AND BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIB_BASE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/Util_InstallRules.cmake)
endif()
