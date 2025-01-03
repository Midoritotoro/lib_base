include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(package lib_base)

install(
    DIRECTORY 
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT lib_baseDevelopment
)

install(
    TARGETS lib_base 
    EXPORT lib_baseTargets
      RUNTIME #
    COMPONENT lib_baseRuntime
    LIBRARY #
    COMPONENT lib_baseRuntime
    NAMELINK_COMPONENT 
    ARCHIVE #
    COMPONENT lib_baseDevelopment
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

configure_file(
    "cmake/${package}-config.cmake.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/lib_base/lib_baseConfig.cmake"
    COPYONLY)

write_basic_package_version_file("${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

set(
    "LIB_BASE_INSTALL_CMAKEDIR" "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)

set_property(CACHE "LIB_BASE_INSTALL_CMAKEDIR" PROPERTY TYPE PATH)
mark_as_advanced("LIB_BASE_INSTALL_CMAKEDIR")
message(CMAKE_INSTALL_INCLUDEDIR:${CMAKE_INSTALL_INCLUDEDIR})

#install(
#    FILES "${CMAKE_CURRENT_BINARY_DIR}/include/base/${export_file_name}"
 #   COMPONENT ${CMAKE_INSTALL_LIBDIR}
 #   DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/base")

set(targets_file "lib_base-shared-targets.cmake")

if(NOT BUILD_SHARED_LIBS)
    set(targets_file "lib_base-static-targets.cmake")
endif()

install(
FILES "cmake/Util_InstallConfig.cmake"
DESTINATION "${LIB_BASE_INSTALL_CMAKEDIR}"
RENAME "${package}Config.cmake"
COMPONENT lib_baseDevelopment
)


install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${LIB_BASE_INSTALL_CMAKEDIR}"
    COMPONENT lib_baseDevelopment
)


install(
    EXPORT lib_baseTargets
    NAMESPACE base::
    DESTINATION "${LIB_BASE_INSTALL_CMAKEDIR}"
    COMPONENT lib_baseDevelopment
)

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
        COMPONENT ${CMAKE_INSTALL_LIBDIR}
        CONFIGURATIONS Debug RelWithDebInfo
        DESTINATION "${pdb_file_destination}"
        OPTIONAL)
endif()

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()