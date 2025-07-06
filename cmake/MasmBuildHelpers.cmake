set(CMAKE_ASM_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/ml64.exe")
set(CMAKE_MASM_COMPILER ${CMAKE_ASM_COMPILER})

set(CMAKE_ASM_MASM_FLAGS "-x" "/c" "assembler-with-cpp")
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_MASM_FLAGS})

if(NOT CMAKE_CL_64)
    set(CMAKE_ASM_MASM_FLAGS ${CMAKE_ASM_MASM_FLAGS} /safeseh)
endif()

set(BASE_ASM_EXTENSION "S")

set(ASM_DIALECT "_MASM")
set(CMAKE_ASM${ASM_DIALECT}_COMPILER_INIT ${_CMAKE_TOOLCHAIN_PREFIX}-masm)

include(CMakeDetermineASMCompiler)
set(ASM_DIALECT)

set(BASE_MASM_FILES "")

if (WIN32) 
    if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(BASE_MASM_FILES
            # Xmm

            src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnaligned.S

            src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo256BitUnaligned.S
            src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo512BitUnaligned.S

            src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo1024BitUnaligned.S
            src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo2048BitUnaligned.S

            src/core/memory/crt/x86_64/xmm/aligned/BaseMemcpy128Bit.S

            src/core/memory/crt/x86_64/xmm/aligned/BaseMemcpy128BitUnrolledTo256Bit.S
            src/core/memory/crt/x86_64/xmm/aligned/BaseMemcpy128BitUnrolledTo512Bit.S

            src/core/memory/crt/x86_64/xmm/aligned/BaseMemcpy128BitUnrolledTo1024Bit.S
            src/core/memory/crt/x86_64/xmm/aligned/BaseMemcpy128BitUnrolledTo2048Bit.S

            # Xmm General

            src/core/memory/crt/x86_64/xmm/BaseMemcpySse.S

            # Ymm

            src/core/memory/crt/x86_64/ymm/unaligned/BaseMemcpy256BitUnaligned.S

            src/core/memory/crt/x86_64/ymm/unaligned/BaseMemcpy256BitUnrolledTo512BitUnaligned.S
            src/core/memory/crt/x86_64/ymm/unaligned/BaseMemcpy256BitUnrolledTo1024BitUnaligned.S

            src/core/memory/crt/x86_64/ymm/unaligned/BaseMemcpy256BitUnrolledTo2048BitUnaligned.S
            src/core/memory/crt/x86_64/ymm/unaligned/BaseMemcpy256BitUnrolledTo4096BitUnaligned.S

            src/core/memory/crt/x86_64/ymm/aligned/BaseMemcpy256Bit.S

            src/core/memory/crt/x86_64/ymm/aligned/BaseMemcpy256BitUnrolledTo512Bit.S
            src/core/memory/crt/x86_64/ymm/aligned/BaseMemcpy256BitUnrolledTo1024Bit.S

            src/core/memory/crt/x86_64/ymm/aligned/BaseMemcpy256BitUnrolledTo2048Bit.S
            src/core/memory/crt/x86_64/ymm/aligned/BaseMemcpy256BitUnrolledTo4096Bit.S

            # Ymm General 
            src/core/memory/crt/x86_64/ymm/BaseMemcpyAvx.S

            # Zmm

            src/core/memory/crt/x86_64/zmm/unaligned/BaseMemcpy512BitUnaligned.S

            src/core/memory/crt/x86_64/zmm/unaligned/BaseMemcpy512BitUnrolledTo1024BitUnaligned.S
            src/core/memory/crt/x86_64/zmm/unaligned/BaseMemcpy512BitUnrolledTo2048BitUnaligned.S

            src/core/memory/crt/x86_64/zmm/unaligned/BaseMemcpy512BitUnrolledTo4096BitUnaligned.S
            src/core/memory/crt/x86_64/zmm/unaligned/BaseMemcpy512BitUnrolledTo8192BitUnaligned.S

            src/core/memory/crt/x86_64/zmm/aligned/BaseMemcpy512Bit.S

            src/core/memory/crt/x86_64/zmm/aligned/BaseMemcpy512BitUnrolledTo1024Bit.S
            src/core/memory/crt/x86_64/zmm/aligned/BaseMemcpy512BitUnrolledTo2048Bit.S

            src/core/memory/crt/x86_64/zmm/aligned/BaseMemcpy512BitUnrolledTo4096Bit.S
            src/core/memory/crt/x86_64/zmm/aligned/BaseMemcpy512BitUnrolledTo8192Bit.S

            # Zmm General 
            src/core/memory/crt/x86_64/zmm/BaseMemcpyAvx512.S

            # Scalar

            src/core/memory/crt/x86_64/scalar/BaseMemcpy8Bit.S

            src/core/memory/crt/x86_64/scalar/BaseMemcpy16Bit.S
            src/core/memory/crt/x86_64/scalar/BaseMemcpy32Bit.S

            src/core/memory/crt/x86_64/scalar/BaseMemcpy64Bit.S

            # Scalar General
            src/core/memory/crt/x86_64/scalar/BaseMemcpyScalar.S
        )
    elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(BASE_MASM_FILES
            src/core/memory/crt/x86/BaseMemmoveX86.S
            src/core/memory/crt/x86/BaseMemcpyX86.S
        )
    endif()
else() 
    set(BASE_MASM_FILES "")      
endif() 

set(BASE_MASM_FILES "")
function(BaseBuildMasmFiles)
if (WIN32)
    foreach(masm_file IN LISTS BASE_MASM_FILES)
        get_filename_component(fname ${masm_file} NAME_WE)
        get_filename_component(directory_path ${masm_file} DIRECTORY)

        set(obj_file "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/${directory_path}/${fname}.${BASE_ASM_EXTENSION}.obj")

        set(ASM_FILE_PROPERTIES "-x -c assembler-with-cpp /Fo${obj_file}")
    
        message("Compiling: ${masm_file} to ${obj_file}")
        set(FULL_PATH_TO_MASM_FILE "${CMAKE_CURRENT_LIST_DIR}/${masm_file}")

        execute_process(COMMAND ${CMAKE_MASM_COMPILER} -c -Fo${obj_file} ${FULL_PATH_TO_MASM_FILE})
    
        message("Command: ${CMAKE_MASM_COMPILER} -c -Fo${obj_file} ${FULL_PATH_TO_MASM_FILE}")
        set_source_files_properties(${masm_file} PROPERTIES
            LANGUAGE ASM_MASM
            COMPILE_FLAGS ${ASM_FILE_PROPERTIES}
        )

        list(APPEND OBJECT_FILES ${obj_file})
    endforeach()
endif()
endfunction()

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> /c /Fo<OBJECT> <COMPILE_FLAGS> <SOURCE>" CACHE STRING "")
