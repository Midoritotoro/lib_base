add_library(external_openal INTERFACE IMPORTED GLOBAL)
add_library(app::external_openal ALIAS external_openal)

target_include_directories(external_openal SYSTEM
INTERFACE
    ${libs_loc}/openal-soft/include
    ${libs_loc}/openal-soft/include/AL
)

target_link_libraries(external_openal
INTERFACE
    ${libs_loc}/openal-soft/build/$<IF:$<CONFIG:Debug>,Debug,Release>/OpenAL32.lib
)

target_compile_definitions(external_openal INTERFACE
    AL_LIBTYPE_STATIC
)
