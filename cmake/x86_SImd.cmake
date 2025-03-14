if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
	set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -m32")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -m64")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_source_files_properties(${PUBLIC_HEADERS} PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} -arch:SSE4.2")
	set_source_files_properties(${PRIVATE_SOURCES} PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} -arch:SSE4.2")
endif()