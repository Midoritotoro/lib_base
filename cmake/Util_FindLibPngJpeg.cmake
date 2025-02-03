function(find_libpng_and_libjpeg TARGET)
    set(PNG_ROOT "D:/Libraries/win64/libpng/")
    set(libjpeg-turbo_DIR "D:/Libraries/win64/libjpeg-turbo/share/libjpeg-turbo/")

    find_package(PNG REQUIRED)
    target_link_libraries(${TARGET} PRIVATE PNG::PNG)

    find_package(libjpeg-turbo REQUIRED)
    target_link_libraries(${TARGET} PRIVATE libjpeg-turbo::turbojpeg-static)
endfunction()