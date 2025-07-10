cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTestsCore LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED COMPONENTS
        lib_base 
   )
endif()

enable_testing()
macro(baseTestsCore_add_test filename)
    add_executable(${filename} ${filename}.cpp)

    set(CMAKE_CXX_FLAGS "-march=native")
    target_link_libraries(
      ${filename} PRIVATE
      lib_base::lib_base
    )
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/"core"/${filename})
  
    target_compile_features(${filename} PUBLIC cxx_std_23)
endmacro(baseTestsCore_add_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB baseTestsCore *.cpp )

FOREACH(testsource ${baseTestsCore})
   GET_FILENAME_COMPONENT(filename ${testsource} NAME_WE)
   baseTestsCore_add_test(${filename})
ENDFOREACH()
