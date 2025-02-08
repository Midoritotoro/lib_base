add_library(external_opus STATIC IMPORTED GLOBAL)
add_library(app::external_opus ALIAS external_opus)

target_include_directories(external_opus SYSTEM
INTERFACE
    ${libs_loc}/opus/include
)

set(opus_lib_loc ${libs_loc}/opus/out)

set_target_properties(external_opus PROPERTIES
    IMPORTED_LOCATION "${opus_lib_loc}/Release/opus.lib"
    IMPORTED_LOCATION_DEBUG "${opus_lib_loc}/Debug/opus.lib"
)
