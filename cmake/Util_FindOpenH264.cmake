add_library(external_openh264 STATIC IMPORTED GLOBAL)
add_library(app::external_openh264 ALIAS external_openh264)

target_include_directories(external_openh264 SYSTEM
INTERFACE
    ${libs_loc}/local/include
)

set_target_properties(external_openh264 PROPERTIES
    IMPORTED_LOCATION "${libs_loc}/openh264/builddir-release/libopenh264.a"
    IMPORTED_LOCATION_DEBUG "${libs_loc}/openh264/builddir-debug/libopenh264.a"
)