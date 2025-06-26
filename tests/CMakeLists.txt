cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lib_baseTests LANGUAGES CXX)
if (PROJECT_IS_TOP_LEVEL)
    find_package(lib_base REQUIRED COMPONENTS
        lib_base 
       #lib_baseQt 
        #lib_baseFFmpeg 
   )
endif()

enable_testing()

include("D:/lib_base/lib_base/cmake/Util_FindFFmpeg.cmake")
set(Qt6_DIR "D:/Packages/Qt/6.7.0/msvc2019_64/lib/cmake/Qt6")

macro(baseTests_add_test filename)
    add_executable(${filename} ${filename}.cpp)
   
    if (LIB_BASE_ENABLE_FFMPEG)
        find_ffmpeg(${filename})
    endif()

    set(CMAKE_CXX_FLAGS "-march=native")
    target_link_libraries(
      ${filename} PRIVATE
      lib_base::lib_base
     # lib_base::lib_baseQt
     # lib_base::lib_baseFFmpeg
    )
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endmacro(baseTests_add_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB baseTests *.cpp )

FOREACH(testsource ${baseTests})
   GET_FILENAME_COMPONENT(filename ${testsource} NAME_WE)
   baseTests_add_test(${filename})
ENDFOREACH()
