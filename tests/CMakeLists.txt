cmake_minimum_required(VERSION 3.21 FATAL_ERROR)


project(lib_baseTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(lib_base)
add_subdirectory(core)


if (LIB_BASE_ENABLE_QT)
    add_subdirectory(qt)
endif()

