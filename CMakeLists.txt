cmake_minimum_required (VERSION 3.16...3.21)

project ("lib_base"
    VERSION 1.0.0
    DESCRIPTION "..."
    LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    isTopLevel)

option(LIB_BASE_INSTALL "Генерация цели для установки библиотеки" ${isTopLevel})

set(LIB_BASE_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/lib_base")
message(${LIB_BASE_INSTALL_CMAKE_DIR})

set(EXPORT_DECL "")
set(LIBTYPE "Shared")
   
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

	src/AutoRunUtils.cpp
    src/AutoRunUtils.h

    src/utility/Math.cpp
    src/utility/Math.h

    src/utility/TFunctions.cpp
    src/utility/TFunctions.h

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


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(CheckCSourceCompiles)


add_library(lib_base ${PRIVATE_SOURCES} ${PUBLIC_HEADERS})
add_library(lib_base::lib_base ALIAS lib_base)

set_target_properties(
    lib_base PROPERTIES
    CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME lib_base
    OUTPUT_NAME lib_base
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(lib_base PUBLIC LIB_BASE_STATIC)
    set(LIBTYPE "STATIC")
endif()

message(${LIBTYPE})

if(WIN32)
    if(NOT LIBTYPE STREQUAL "STATIC")
        set(EXPORT_DECL "__declspec(dllexport)")
    endif()
else()
    check_c_source_compiles("int foo() __attribute__((visibility(\"protected\")));
                             int main() {return 0;}" HAVE_GCC_PROTECTED_VISIBILITY)
    if(HAVE_GCC_PROTECTED_VISIBILITY)
        if(NOT LIBTYPE STREQUAL "STATIC")
            set(EXPORT_DECL "__attribute__((visibility(\"protected\")))")
        endif()
    else()
        check_c_source_compiles("int foo() __attribute__((visibility(\"default\")));
                                 int main() {return 0;}" HAVE_GCC_DEFAULT_VISIBILITY)
        if(HAVE_GCC_DEFAULT_VISIBILITY)
            if(NOT LIBTYPE STREQUAL "STATIC")
                set(EXPORT_DECL "__attribute__((visibility(\"default\")))")
            endif()
        endif()
    endif()

    if(HAVE_GCC_PROTECTED_VISIBILITY OR HAVE_GCC_DEFAULT_VISIBILITY)
        check_c_compiler_flag(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN_SWITCH)
        if(HAVE_VISIBILITY_HIDDEN_SWITCH)
            set(CXX_FLAGS ${CXX_FLAGS} -fvisibility=hidden)
        endif()
    endif()
endif()

target_compile_definitions(lib_base PRIVATE "LIB_BASE=${EXPORT_DECL}")

target_include_directories(lib_base
    PUBLIC
        "\$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "\$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "\$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src")

if (WIN32)
    target_link_libraries(lib_base
    PUBLIC
        user32.dll
        kernel32.dll
        shlwapi.dll
    )
endif()

set(export_file_name "lib_base_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "lib_base_static.h")
endif()

generate_export_header(
    lib_base
    BASE_NAME lib_base
    EXPORT_FILE_NAME export/${export_file_name}
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(LIB_BASE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/lib_base-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/lib_base-config.cmake
        INSTALL_DESTINATION "${LIB_BASE_INSTALL_CMAKE_DIR}")

    write_basic_package_version_file(lib_base-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(
        TARGETS lib_base 
        EXPORT lib_base_targets
        LIBRARY COMPONENT ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE COMPONENT ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT ${CMAKE_INSTALL_LIBDIR})

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/base/${export_file_name}"
        COMPONENT ${CMAKE_INSTALL_LIBDIR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/base")

    set(targets_file "lib_base-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "lib_base-static-targets.cmake")
    endif()

    install(
        EXPORT lib_base_targets
        COMPONENT ${CMAKE_INSTALL_LIBDIR}
        FILE "${targets_file}"
        DESTINATION "${LIB_BASE_INSTALL_CMAKE_DIR}"
        NAMESPACE base::)

     install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/lib_base-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/lib_base-config-version.cmake"
        COMPONENT ${CMAKE_INSTALL_LIBDIR}
        DESTINATION "${LIB_BASE_INSTALL_CMAKE_DIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:lib_base>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            set(pdb_file "$<TARGET_FILE_DIR:lib_base>/$<TARGET_FILE_PREFIX:lib_base>$<TARGET_FILE_BASE_NAME:lib_base>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT lib
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()
