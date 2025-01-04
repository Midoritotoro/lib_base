include(CMakeFindDependencyMacro)

macro(import_targets type)
    if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/lib_baseTargets-${type}.cmake")
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
            "lib_base ${type} libs requested, but no found")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND OFF)
        return()
    endif()

    include("${CMAKE_CURRENT_LIST_DIR}/lib_baseTargets-${type}.cmake")
endmacro()

if(NOT TARGET lib_base::lib_base)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(type "debug")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(type "release")
    endif()

    import_targets(${type})
    check_required_components(lib_base)
    message("-- Found ${type} lib_base (version ${${CMAKE_FIND_PACKAGE_NAME}_VERSION})")
endif()