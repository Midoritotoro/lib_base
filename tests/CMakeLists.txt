cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTests LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
find_package(lib_base REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lib_base_test test.cpp)
target_link_libraries(
    lib_base_test PRIVATE
    lib_base::lib_base
)
