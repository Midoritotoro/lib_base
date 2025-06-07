set(CMAKE_ASM_COMPILER "D:/VS/2022/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/ml64.exe")
set(CMAKE_MASM_COMPILER ${CMAKE_ASM_COMPILER})

set(CMAKE_ASM_MASM_FLAGS "-x" "/c" "assembler-with-cpp")
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_MASM_FLAGS})
if(NOT CMAKE_CL_64)
    set(CMAKE_ASM_MASM_FLAGS ${CMAKE_ASM_MASM_FLAGS} /safeseh)
endif()

set(ASM_DIALECT "_MASM")
set(CMAKE_ASM${ASM_DIALECT}_COMPILER_INIT ${_CMAKE_TOOLCHAIN_PREFIX}-masm)

include(CMakeDetermineASMCompiler)
set(ASM_DIALECT)

set(BASE_MASM_FILES "")

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(BASE_MASM_FILES
        src/core/memory/crt/x86_64/BaseMemmove.S
        src/core/memory/crt/x86_64/BaseMemcpy.S

        src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnaligned.S

        src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo256BitUnaligned.S
        src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo512BitUnaligned.S

        src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo1024BitUnaligned.S
        src/core/memory/crt/x86_64/xmm/unaligned/BaseMemcpy128BitUnrolledTo2048BitUnaligned.S
    )
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(BASE_MASM_FILES
        src/core/memory/crt/x86/BaseMemmoveX86.S
        src/core/memory/crt/x86/BaseMemcpyX86.S
    )
endif()

function(BaseBuildMasmFiles)
    foreach(masm_file IN LISTS BASE_MASM_FILES)
        get_filename_component(fname ${masm_file} NAME_WE)
        get_filename_component(directory_path ${masm_file} DIRECTORY)

        set(obj_file "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/${directory_path}/${fname}.S.obj")

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
endfunction()

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> /c /Fo<OBJECT> <COMPILE_FLAGS> <SOURCE>" CACHE STRING "")
