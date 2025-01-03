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

option(LIB_BASE_BUILD_TESTS "Сборка тестов библиотеки" ON)
option(LIB_BASE_BUILD_EXAMPLES "Сборка примеров использования библиотеки" OFF)
option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${PROJECT_IS_TOP_LEVEL})
option(LIB_BASE_LINK_OPTIONS "Опции линковки библиотеки для MSVC" ${linkOptions})
option(LIB_BASE_ENABLE_QT "Включить вспомогательный интерфейс для Qt" OFF)
option(BUILD_SHARED_LIBS "Сборка динамической библиотеки" OFF)

set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")

   
set(PRIVATE_SOURCES 
    src/concurrent/common/CommonConfig.h
    src/concurrent/common/CommonGuards.h

    src/concurrent/common/CommonAsync.cpp
    src/concurrent/common/CommonAsync.h

    src/concurrent/common/CommonAsyncGuarded.h

    src/concurrent/common/CommonQueue.cpp
    src/concurrent/common/CommonQueue.h

    src/concurrent/common/CommonSync.h
    src/concurrent/common/CommonUtils.h

    src/concurrent/winapi/WinapiAsync.cpp
    src/concurrent/winapi/WinapiAsync.h

    src/concurrent/winapi/WinapiDll.h

    src/concurrent/winapi/WinapiList.cpp
    src/concurrent/winapi/WinapiList.h

    src/concurrent/winapi/WinapiSemaphore.cpp
    src/concurrent/winapi/WinapiSemaphore.h

   src/utility/Math.cpp
    src/utility/Math.h

   src/utility/TFunctions.cpp
   src/utility/TFunctions.h

   src/AutoRunUtils.cpp
   src/AutoRunUtils.h

   src/Time.h
   src/Time.cpp
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
)


include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base ${PUBLIC_HEADERS})
add_library(lib_base::lib_base ALIAS lib_base)

include(cmake/Util_ExportConfig.cmake)

target_sources(lib_base PRIVATE ${PRIVATE_SOURCES})

generate_export_header(
    lib_base
    BASE_NAME lib_base
    EXPORT_FILE_NAME export/lib_base/lib_base.h
)

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

# message("LIBTYPE: " ${LIBTYPE})

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
  # set_target_properties(${PROJECT_NAME} PROPERTIES
  #     MSVC_RUNTIME_LIBRARY ${CMAKE_DEBUG_TARGET_PROPERTIES})

  #  target_link_options(${PROJECT_NAME}
  #  INTERFACE
  #     ${linkOptions}
  # )
endif()

if(PROJECT_IS_TOP_LEVEL)# AND LIB_BASE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PROJECT_IS_TOP_LEVEL AND LIB_BASE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


if(LIB_BASE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/Util_InstallRules.cmake)
endif()
