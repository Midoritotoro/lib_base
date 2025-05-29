cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTests LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED COMPONENTS
        lib_base 
        lib_baseQt 
        lib_baseFFmpeg 
   )
endif()

enable_testing()
include("D:/lib_base/lib_base/cmake/Util_FindFFmpeg.cmake")
set(Qt6_DIR "D:/Packages/Qt/6.7.0/msvc2019_64/lib/cmake/Qt6")
set(benchmark_DIR "D:/benchmark")

macro(baseBenchmarks_add_benchmark filename)
    add_executable(${filename} ${filename}.cpp)
   
    find_ffmpeg(${filename})

    set(CMAKE_CXX_FLAGS "-march=native")
    target_link_libraries(
      ${filename} PRIVATE
      lib_base::lib_base
      lib_base::lib_baseQt
      lib_base::lib_baseFFmpeg
      benchmark::benchmark
    )
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endmacro(baseBenchmarks_add_benchmark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark REQUIRED)

FILE(GLOB baseBenchmarks *.cpp )

FOREACH(benchsource ${baseBenchmarks})
   GET_FILENAME_COMPONENT(filename ${testsource} NAME_WE)
   baseBenchmarks_add_benchmark(${filename})
ENDFOREACH()
