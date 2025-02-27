cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTests LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED COMPONENTS
        lib_base 
        lib_baseQt 
        lib_baseFFmpeg 
    )
endif()

set(Qt6_DIR "D:/Packages/Qt/6.7.0/msvc2019_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS
    Core 
    Widgets)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lib_base_test test.cpp)
target_link_libraries(
    lib_base_test PRIVATE
    lib_base::lib_base
    lib_base::lib_baseQt
    lib_base::lib_baseFFmpeg
)