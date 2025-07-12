cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseBenchmarks LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED)
endif()

enable_testing()
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks
)
set(benchmark_DIR "D:/Libraries/win64/benchmark/build")
find_package(benchmark REQUIRED)

macro(baseBenchmarks_add_benchmark filename)
    add_executable(${filename} ${filename}.cpp)

    set(CMAKE_CXX_FLAGS "-march=native")
    target_link_libraries(
      ${filename} PRIVATE
      lib_base::lib_base
      benchmark::benchmark
    )
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endmacro(baseBenchmarks_add_benchmark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB baseBenchmarks *.cpp )

FOREACH(benchsource ${baseBenchmarks})
   GET_FILENAME_COMPONENT(filename ${benchsource} NAME_WE)
   baseBenchmarks_add_benchmark(${filename})
ENDFOREACH()
