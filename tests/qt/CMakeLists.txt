cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTestsQt LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED COMPONENTS
        lib_base 
        lib_base::lib_baseQt
   )
endif()

enable_testing()
macro(baseTestsQt_add_test filename)
    add_executable(${filename} ${filename}.cpp)
  
    set(CMAKE_CXX_FLAGS "-march=native")
    target_link_libraries(
      ${filename} PRIVATE
      lib_base::lib_base
      lib_base::lib_baseQt
    )
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/"qt"/${filename})
endmacro(baseTestsQt_add_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB baseTestsQt *.cpp)

FOREACH(testsource ${baseTestsQt})
   GET_FILENAME_COMPONENT(filename ${testsource} NAME_WE)
   baseTestsQt_add_test(${filename})
ENDFOREACH()
