from typing import List

from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation
# from packagesInstaller.SystemDetection import mac

import os
from packagesInstaller.SetupPaths import removeDir

from packagesInstaller.InstallOptionsCheck import options

supportedLibraries: List[str] = [
    "ffmpeg", "qt", "jom", "msys64", "zlib", "gyp", "yasm", "lzma", "xz", "mozjpeg", "openssl3",
    "gas-preprocessor", "dav1d", "openh264", "libavif", "libde265", "libwebp", "openal-soft",
    "stackwalk", "protobuf", "opus"
]

installCommands: List[LibraryInstallationInformation] =  []

def isLibrarySupported(libraryName: str) -> bool:
    return (libraryName in supportedLibraries)

installCommands.append(
    LibraryInstallationInformation("ffmpeg", 
"""
    FFmpeg is the leading multimedia framework, able to decode, encode, transcode, mux,
    demux, stream, filter and play pretty much anything that humans and machines have created""",
"""
    git clone -b n6.1.1 https://github.com/FFmpeg/FFmpeg.git ffmpeg
    cd ffmpeg
win:
depends:patches/ffmpeg.patch
    git apply ../patches/ffmpeg.patch

    SET PATH_BACKUP_=%PATH%
    SET PATH=%ROOT_DIR%\\ThirdParty\\msys64\\usr\\bin;%PATH%

    SET CHERE_INVOKING=enabled_from_arguments
    SET MSYS2_PATH_TYPE=inherit

    SET "ARCH_PARAM="
winarm:
    SET "ARCH_PARAM=--arch=aarch64"
win:
depends:patches/build_ffmpeg_win.sh
    bash --login ../patches/build_ffmpeg_win.sh

    SET PATH=%PATH_BACKUP_%
mac:
    export PKG_CONFIG_PATH=$USED_PREFIX/lib/pkgconfig
depends:yasm/yasm

    configureFFmpeg() {
        arch=$1

        ./configure --prefix=$USED_PREFIX \
        --enable-cross-compile \
        --target-os=darwin \
        --arch="$arch" \
        --extra-cflags="$MIN_VER -arch $arch $UNGUARDED -DCONFIG_SAFE_BITSTREAM_READER=1 -I$USED_PREFIX/include" \
        --extra-cxxflags="$MIN_VER -arch $arch $UNGUARDED -DCONFIG_SAFE_BITSTREAM_READER=1 -I$USED_PREFIX/include" \
        --extra-ldflags="$MIN_VER -arch $arch $USED_PREFIX/lib/libopus.a -lc++" \
        --disable-programs \
        --disable-doc \
        --disable-network \
        --disable-everything \
        --enable-protocol=file \
        --enable-libdav1d \
        --enable-libopenh264 \
        --enable-libopus \
        --enable-libvpx \
        --enable-hwaccel=h264_videotoolbox \
        --enable-hwaccel=hevc_videotoolbox \
        --enable-hwaccel=mpeg1_videotoolbox \
        --enable-hwaccel=mpeg2_videotoolbox \
        --enable-hwaccel=mpeg4_videotoolbox \
        --enable-decoder=aac \
        --enable-decoder=aac_at \
        --enable-decoder=aac_fixed \
        --enable-decoder=aac_latm \
        --enable-decoder=aasc \
        --enable-decoder=ac3 \
        --enable-decoder=alac \
        --enable-decoder=alac_at \
        --enable-decoder=av1 \
        --enable-decoder=eac3 \
        --enable-decoder=flac \
        --enable-decoder=gif \
        --enable-decoder=h264 \
        --enable-decoder=hevc \
        --enable-decoder=libdav1d \
        --enable-decoder=libvpx_vp8 \
        --enable-decoder=libvpx_vp9 \
        --enable-decoder=mp1 \
        --enable-decoder=mp1float \
        --enable-decoder=mp2 \
        --enable-decoder=mp2float \
        --enable-decoder=mp3 \
        --enable-decoder=mp3adu \
        --enable-decoder=mp3adufloat \
        --enable-decoder=mp3float \
        --enable-decoder=mp3on4 \
        --enable-decoder=mp3on4float \
        --enable-decoder=mpeg4 \
        --enable-decoder=msmpeg4v2 \
        --enable-decoder=msmpeg4v3 \
        --enable-decoder=opus \
        --enable-decoder=pcm_alaw \
        --enable-decoder=pcm_alaw_at \
        --enable-decoder=pcm_f32be \
        --enable-decoder=pcm_f32le \
        --enable-decoder=pcm_f64be \
        --enable-decoder=pcm_f64le \
        --enable-decoder=pcm_lxf \
        --enable-decoder=pcm_mulaw \
        --enable-decoder=pcm_mulaw_at \
        --enable-decoder=pcm_s16be \
        --enable-decoder=pcm_s16be_planar \
        --enable-decoder=pcm_s16le \
        --enable-decoder=pcm_s16le_planar \
        --enable-decoder=pcm_s24be \
        --enable-decoder=pcm_s24daud \
        --enable-decoder=pcm_s24le \
        --enable-decoder=pcm_s24le_planar \
        --enable-decoder=pcm_s32be \
        --enable-decoder=pcm_s32le \
        --enable-decoder=pcm_s32le_planar \
        --enable-decoder=pcm_s64be \
        --enable-decoder=pcm_s64le \
        --enable-decoder=pcm_s8 \
        --enable-decoder=pcm_s8_planar \
        --enable-decoder=pcm_u16be \
        --enable-decoder=pcm_u16le \
        --enable-decoder=pcm_u24be \
        --enable-decoder=pcm_u24le \
        --enable-decoder=pcm_u32be \
        --enable-decoder=pcm_u32le \
        --enable-decoder=pcm_u8 \
        --enable-decoder=vorbis \
        --enable-decoder=vp8 \
        --enable-decoder=wavpack \
        --enable-decoder=wmalossless \
        --enable-decoder=wmapro \
        --enable-decoder=wmav1 \
        --enable-decoder=wmav2 \
        --enable-decoder=wmavoice \
        --enable-encoder=aac \
        --enable-encoder=libopus \
        --enable-encoder=libopenh264 \
        --enable-encoder=pcm_s16le \
        --enable-filter=atempo \
        --enable-parser=aac \
        --enable-parser=aac_latm \
        --enable-parser=flac \
        --enable-parser=gif \
        --enable-parser=h264 \
        --enable-parser=hevc \
        --enable-parser=mpeg4video \
        --enable-parser=mpegaudio \
        --enable-parser=opus \
        --enable-parser=vorbis \
        --enable-demuxer=aac \
        --enable-demuxer=flac \
        --enable-demuxer=gif \
        --enable-demuxer=h264 \
        --enable-demuxer=hevc \
        --enable-demuxer=matroska \
        --enable-demuxer=m4v \
        --enable-demuxer=mov \
        --enable-demuxer=mp3 \
        --enable-demuxer=ogg \
        --enable-demuxer=wav \
        --enable-muxer=mp4 \
        --enable-muxer=ogg \
        --enable-muxer=opus \
        --enable-muxer=wav
    }

    configureFFmpeg arm64
    make $MAKE_THREADS_CNT

    mkdir out.arm64
    mv libavfilter/libavfilter.a out.arm64
    mv libavformat/libavformat.a out.arm64
    mv libavcodec/libavcodec.a out.arm64
    mv libswresample/libswresample.a out.arm64
    mv libswscale/libswscale.a out.arm64
    mv libavutil/libavutil.a out.arm64

    make clean

    configureFFmpeg x86_64
    make $MAKE_THREADS_CNT

    mkdir out.x86_64
    mv libavfilter/libavfilter.a out.x86_64
    mv libavformat/libavformat.a out.x86_64
    mv libavcodec/libavcodec.a out.x86_64
    mv libswresample/libswresample.a out.x86_64
    mv libswscale/libswscale.a out.x86_64
    mv libavutil/libavutil.a out.x86_64

    lipo -create out.arm64/libavfilter.a out.x86_64/libavfilter.a -output libavfilter/libavfilter.a
    lipo -create out.arm64/libavformat.a out.x86_64/libavformat.a -output libavformat/libavformat.a
    lipo -create out.arm64/libavcodec.a out.x86_64/libavcodec.a -output libavcodec/libavcodec.a
    lipo -create out.arm64/libswresample.a out.x86_64/libswresample.a -output libswresample/libswresample.a
    lipo -create out.arm64/libswscale.a out.x86_64/libswscale.a -output libswscale/libswscale.a
    lipo -create out.arm64/libavutil.a out.x86_64/libavutil.a -output libavutil/libavutil.a

    make install
"""
)
)

installCommands.append(
    LibraryInstallationInformation("jom", 
"jom is a clone of nmake to support the execution of multiple independent commands in parallel.", 
"""
win:
    powershell -Command "iwr -OutFile ./jom.zip https://master.qt.io/official_releases/jom/jom_1_1_3.zip"
    powershell -Command "Expand-Archive ./jom.zip"
    del jom.zip
"""
)
)

installCommands.append(
    LibraryInstallationInformation("msys64", 
"MSYS2 is a collection of tools and libraries providing you with an easy-to-use environment for building, installing and running native Windows software.",
"""
win:
    SET PATH_BACKUP_=%PATH%
    SET PATH=%ROOT_DIR%\\ThirdParty\\msys64\\usr\\bin;%PATH%

    SET CHERE_INVOKING=enabled_from_arguments
    SET MSYS2_PATH_TYPE=inherit

    powershell -Command "iwr -OutFile ./msys64.exe https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-base-x86_64-20250221.sfx.exe"
    msys64.exe
    del msys64.exe

    bash -c "pacman-key --init; pacman-key --populate; pacman -Syu --noconfirm"
    pacman -Syu --noconfirm ^
        make ^
        mingw-w64-x86_64-diffutils ^
        mingw-w64-x86_64-gperf ^
        mingw-w64-x86_64-nasm ^
        mingw-w64-x86_64-perl ^
        mingw-w64-x86_64-pkgconf ^
        mingw-w64-x86_64-yasm

    SET PATH=%PATH_BACKUP_%
"""
)
)


installCommands.append(
    LibraryInstallationInformation("yasm", 
"Yasm is an assembler that is an attempt to completely rewrite the NASM assembler. It is licensed under the BSD license.",
"""
mac:
    git clone https://github.com/yasm/yasm.git
    cd yasm
    git checkout 41762bea
    ./autogen.sh
    make $MAKE_THREADS_CNT
"""
)
)

installCommands.append(
    LibraryInstallationInformation("lzma", 
"lzma is a general-purpose data compression library with a zlib-like API.", 
"""
win:
    git clone https://github.com/desktop-app/lzma.git
    cd lzma\\C\\Util\\LzmaLib
    msbuild -m LzmaLib.sln /property:Configuration=Debug /property:Platform="$X8664"
release:
    msbuild -m LzmaLib.sln /property:Configuration=Release /property:Platform="$X8664"
"""
)
)

installCommands.append(
    LibraryInstallationInformation("xz", 
"XZ Utils provide a general-purpose data-compression library plus command-line tools.",
"""
!win:
    git clone -b v5.4.5 https://github.com/tukaani-project/xz.git
    cd xz
    sed -i '' '\\@check_symbol_exists(futimens "sys/types.h;sys/stat.h" HAVE_FUTIMENS)@d' CMakeLists.txt
    CFLAGS="$UNGUARDED" CPPFLAGS="$UNGUARDED" cmake -B build . \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" \\
        -D CMAKE_INSTALL_PREFIX:STRING=$USED_PREFIX
    cmake --build build $MAKE_THREADS_CNT
    cmake --install build
"""
)
)

# if not mac or 'build-stackwalk' in options:
installCommands.append(
    LibraryInstallationInformation("gyp", 
"GYP (Generate Your Projects) is a build automation system created by Google to generate projects for various IDEs",
"""
win:
    git clone https://github.com/desktop-app/gyp.git
    cd gyp
    git checkout 618958fdbe
mac:
    python3 -m pip install \\
        --ignore-installed \\
        --target=$THIRDPARTY_DIR/gyp \\
        git+https://chromium.googlesource.com/external/gyp@master
"""
)
)


installCommands.append(
    LibraryInstallationInformation("mozjpeg", 
"""
MozJPEG improves JPEG compression efficiency achieving higher visual quality and smaller file sizes at the same time. 
It is compatible with the JPEG standard, and the vast majority of the world's deployed JPEG decoders.
""",
"""
    git clone -b v4.1.5 https://github.com/mozilla/mozjpeg.git
    cd mozjpeg
win:
    cmake . ^
        -A %WIN32X64% ^
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
        -DWITH_JPEG8=ON ^
        -DPNG_SUPPORTED=OFF
    cmake --build . --config Debug --parallel
release:
    cmake --build . --config Release --parallel
mac:
    CFLAGS="-arch arm64" cmake -B build.arm64 . \\
        -D CMAKE_POLICY_VERSION_MINIMUM=3.5 \\
        -D CMAKE_SYSTEM_NAME=Darwin \\
        -D CMAKE_SYSTEM_PROCESSOR=arm64 \\
        -D CMAKE_BUILD_TYPE=Release \\
        -D CMAKE_INSTALL_PREFIX=$USED_PREFIX \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D WITH_JPEG8=ON \\
        -D ENABLE_SHARED=OFF \\
        -D PNG_SUPPORTED=OFF
    cmake --build build.arm64 $MAKE_THREADS_CNT
    CFLAGS="-arch x86_64" cmake -B build . \\
        -D CMAKE_POLICY_VERSION_MINIMUM=3.5 \\
        -D CMAKE_SYSTEM_NAME=Darwin \\
        -D CMAKE_SYSTEM_PROCESSOR=x86_64 \\
        -D CMAKE_BUILD_TYPE=Release \\
        -D CMAKE_INSTALL_PREFIX=$USED_PREFIX \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D WITH_JPEG8=ON \\
        -D ENABLE_SHARED=OFF \\
        -D PNG_SUPPORTED=OFF
    cmake --build build $MAKE_THREADS_CNT
    lipo -create build.arm64/libjpeg.a build/libjpeg.a -output build/libjpeg.a
    lipo -create build.arm64/libturbojpeg.a build/libturbojpeg.a -output build/libturbojpeg.a
    cmake --install build
"""                           
)
)

installCommands.append(
    LibraryInstallationInformation("openssl3", 
"OpenSSL is a software library for applications that provide secure communications over computer networks against eavesdropping, and identify the party at the other end.",
"""
    git clone -b openssl-3.2.1 https://github.com/openssl/openssl openssl3
    cd openssl3
win32:
    perl Configure no-shared no-tests debug-VC-WIN32 /FS
win64:
    perl Configure no-shared no-tests debug-VC-WIN64A /FS
winarm:
    perl Configure no-shared no-tests debug-VC-WIN64-ARM /FS
win:
    jom -j%NUMBER_OF_PROCESSORS% build_libs
    mkdir out.dbg
    move libcrypto.lib out.dbg
    move libssl.lib out.dbg
    move ossl_static.pdb out.dbg
release:
    move out.dbg\\ossl_static.pdb out.dbg\\ossl_static
    jom clean
    move out.dbg\\ossl_static out.dbg\\ossl_static.pdb
win32_release:
    perl Configure no-shared no-tests VC-WIN32 /FS
win64_release:
    perl Configure no-shared no-tests VC-WIN64A /FS
winarm_release:
    perl Configure no-shared no-tests VC-WIN64-ARM /FS
win_release:
    jom -j%NUMBER_OF_PROCESSORS% build_libs
    mkdir out
    move libcrypto.lib out
    move libssl.lib out
    move ossl_static.pdb out
mac:
    ./Configure --prefix=$USED_PREFIX no-shared no-tests darwin64-arm64-cc $MIN_VER
    make build_libs $MAKE_THREADS_CNT
    mkdir out.arm64
    mv libssl.a out.arm64
    mv libcrypto.a out.arm64
    make clean
    ./Configure --prefix=$USED_PREFIX no-shared no-tests darwin64-x86_64-cc $MIN_VER
    make build_libs $MAKE_THREADS_CNT
    mkdir out.x86_64
    mv libssl.a out.x86_64
    mv libcrypto.a out.x86_64
    lipo -create out.arm64/libcrypto.a out.x86_64/libcrypto.a -output libcrypto.a
    lipo -create out.arm64/libssl.a out.x86_64/libssl.a -output libssl.a
"""
)
)

installCommands.append(
    LibraryInstallationInformation("gas-preprocessor",
"Perl script that implements a subset of the GNU as preprocessor that Apple's as doesn't ",
"""
win:
    git clone https://github.com/FFmpeg/gas-preprocessor
    cd gas-preprocessor
    echo @echo off > cpp.bat
    echo cl %%%%%%** >> cpp.bat
"""
)
)



installCommands.append(
    LibraryInstallationInformation("dav1d",
"dav1d is an AV1 cross-platform decoder, open-source, and focused on speed and correctness.",
"""
    git clone -b 1.5.1 https://code.videolan.org/videolan/dav1d.git
    cd dav1d
win32:
    SET "TARGET=x86"
    SET "DAV1D_ASM_DISABLE=-Denable_asm=false"
win64:
    SET "TARGET=x86_64"
    SET "DAV1D_ASM_DISABLE="
winarm:
    SET "TARGET=aarch64"
    SET "DAV1D_ASM_DISABLE="
    SET "PATH_BACKUP_=%PATH%"
    SET "PATH=%LIBS_DIR%\\gas-preprocessor;%PATH%"
    echo armasm64 fails with 'syntax error in expression: tbnz x14, #4, 8f' as if this instruction is unknown/unsupported.
    git revert --no-edit d503bb0ccaf104b2f13da0f092e09cc9411b3297
win:
    set FILE=cross-file.txt
    echo [binaries] > %FILE%
    echo c = 'cl' >> %FILE%
    echo cpp = 'cl' >> %FILE%
    echo ar = 'lib' >> %FILE%
    echo windres = 'rc' >> %FILE%
    echo [host_machine] >> %FILE%
    echo system = 'windows' >> %FILE%
    echo cpu_family = '%TARGET%' >> %FILE%
    echo cpu = '%TARGET%' >> %FILE%
    echo endian = 'little' >> %FILE%

depends:python/Scripts/activate.bat
    %THIRDPARTY_DIR%\\python\\Scripts\\activate.bat
    meson setup --cross-file %FILE% --prefix %LIBS_DIR%/local --default-library=static --buildtype=debug -Denable_tools=false -Denable_tests=false %DAV1D_ASM_DISABLE% -Db_vscrt=mtd builddir-debug
    meson compile -C builddir-debug
    meson install -C builddir-debug
release:
    meson setup --cross-file %FILE% --prefix %LIBS_DIR%/local --default-library=static --buildtype=release -Denable_tools=false -Denable_tests=false -Db_vscrt=mt builddir-release
    meson compile -C builddir-release
    meson install -C builddir-release
win:
    copy %LIBS_DIR%\\local\\lib\\libdav1d.a %LIBS_DIR%\\local\\lib\\dav1d.lib
    deactivate
winarm:
    SET "PATH=%PATH_BACKUP_%"
mac:
    buildOneArch() {
        arch=$1
        folder=`pwd`/$2

        meson setup \\
            --cross-file ../patches/macos_meson_${arch}.txt \\
            --prefix ${USED_PREFIX} \\
            --default-library=static \\
            --buildtype=minsize \\
            -Denable_tools=false \\
            -Denable_tests=false \\
            ${folder}
        meson compile -C ${folder}
        meson install -C ${folder}

        mv ${USED_PREFIX}/lib/libdav1d.a ${folder}/libdav1d.a
    }

    buildOneArch arm64 build.arm64
    buildOneArch x86_64 build

    lipo -create build.arm64/libdav1d.a build/libdav1d.a -output ${USED_PREFIX}/lib/libdav1d.a
"""
)
)

installCommands.append(
    LibraryInstallationInformation("openh264",
"OpenH264 is a codec library which supports H.264 encoding and decoding.",
"""
    git clone -b v2.6.0 https://github.com/cisco/openh264.git
    cd openh264
win32:
    SET "TARGET=x86"
win64:
    SET "TARGET=x86_64"
winarm:
    SET "TARGET=aarch64"
    SET "PATH_BACKUP_=%PATH%"
    SET "PATH=%LIBS_DIR%\\gas-preprocessor;%PATH%"
win:
    set FILE=cross-file.txt
    echo [binaries] > %FILE%
    echo c = 'cl' >> %FILE%
    echo cpp = 'cl' >> %FILE%
    echo ar = 'lib' >> %FILE%
    echo windres = 'rc' >> %FILE%
    echo [host_machine] >> %FILE%
    echo system = 'windows' >> %FILE%
    echo cpu_family = '%TARGET%' >> %FILE%
    echo cpu = '%TARGET%' >> %FILE%
    echo endian = 'little' >> %FILE%

depends:python/Scripts/activate.bat
    %THIRDPARTY_DIR%\\python\\Scripts\\activate.bat
    meson setup --cross-file %FILE% --prefix %LIBS_DIR%/local --default-library=static --buildtype=debug -Db_vscrt=mtd builddir-debug
    meson compile -C builddir-debug
    meson install -C builddir-debug
release:
    meson setup --cross-file %FILE% --prefix %LIBS_DIR%/local --default-library=static --buildtype=release -Db_vscrt=mt builddir-release
    meson compile -C builddir-release
    meson install -C builddir-release
win:
    copy %LIBS_DIR%\\local\\lib\\libopenh264.a %LIBS_DIR%\\local\\lib\\openh264.lib
    deactivate
winarm:
    SET "PATH=%PATH_BACKUP_%"
mac:
    buildOneArch() {
        arch=$1
        folder=`pwd`/$2

        meson setup \
            --cross-file ../patches/macos_meson_${arch}.txt \
            --prefix ${USED_PREFIX} \
            --default-library=static \
            --buildtype=minsize \
            ${folder}
        meson compile -C ${folder}
        meson install -C ${folder}

        mv ${USED_PREFIX}/lib/libopenh264.a ${folder}/libopenh264.a
    }

    buildOneArch aarch64 build.aarch64
    buildOneArch x86_64 build.x86_64

    lipo -create build.aarch64/libopenh264.a build.x86_64/libopenh264.a -output ${USED_PREFIX}/lib/libopenh264.a
"""
)
)

installCommands.append(
    LibraryInstallationInformation("libavif",
"This library aims to be a friendly, portable C implementation of the AV1 Image File Format",
"""
    git clone -b v1.3.0 https://github.com/AOMediaCodec/libavif.git
    cd libavif
win:
    cmake . ^
        -A %WIN32X64% ^
        -DCMAKE_INSTALL_PREFIX=%LIBS_DIR%/local ^
        -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" ^
        -DCMAKE_POLICY_DEFAULT_CMP0091=NEW ^
        -DBUILD_SHARED_LIBS=OFF ^
        -DAVIF_ENABLE_WERROR=OFF ^
        -DAVIF_CODEC_DAV1D=SYSTEM ^
        -DAVIF_LIBYUV=OFF
    cmake --build . --config Debug --parallel
    cmake --install . --config Debug
release:
    cmake --build . --config Release --parallel
    cmake --install . --config Release
mac:
    cmake . \\
        -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D CMAKE_INSTALL_PREFIX:STRING=$USED_PREFIX \\
        -D BUILD_SHARED_LIBS=OFF \\
        -D AVIF_ENABLE_WERROR=OFF \\
        -D AVIF_CODEC_DAV1D=SYSTEM \\
        -D AVIF_LIBYUV=OFF
    cmake --build . --config MinSizeRel $MAKE_THREADS_CNT
    cmake --install . --config MinSizeRel
"""
)
)

installCommands.append(
    LibraryInstallationInformation("libde265",
"libde265 is an open source implementation of the h.265 video codec. ",
"""
    git clone -b v1.0.16 https://github.com/strukturag/libde265.git
    cd libde265
win:
    cmake . ^
        -A %WIN32X64% ^
        -DCMAKE_INSTALL_PREFIX=%LIBS_DIR%/local ^
        -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" ^
        -DCMAKE_POLICY_DEFAULT_CMP0091=NEW ^
        -DCMAKE_C_FLAGS="/DLIBDE265_STATIC_BUILD" ^
        -DCMAKE_CXX_FLAGS="/DLIBDE265_STATIC_BUILD" ^
        -DENABLE_SDL=OFF ^
        -DBUILD_SHARED_LIBS=OFF ^
        -DENABLE_DECODER=OFF ^
        -DENABLE_ENCODER=OFF
    cmake --build . --config Debug --parallel
    cmake --install . --config Debug
release:
    cmake --build . --config Release --parallel
    cmake --install . --config Release
mac:
    cmake . \\
        -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D CMAKE_INSTALL_PREFIX:STRING=$USED_PREFIX \\
        -D DISABLE_SSE=ON \\
        -D ENABLE_SDL=OFF \\
        -D BUILD_SHARED_LIBS=OFF \\
        -D ENABLE_DECODER=ON \\
        -D ENABLE_ENCODER=OFF
    cmake --build . --config MinSizeRel $MAKE_THREADS_CNT
    cmake --install . --config MinSizeRel
"""
)
)

installCommands.append(
    LibraryInstallationInformation("libwebp",
"WebP codec is a library to encode and decode images in WebP format. ",
"""
    git clone -b v1.5.0 https://github.com/webmproject/libwebp.git
    cd libwebp
win:
    nmake /f Makefile.vc CFG=debug-static OBJDIR=out RTLIBCFG=static all
    nmake /f Makefile.vc CFG=release-static OBJDIR=out RTLIBCFG=static all
    copy out\\release-static\\$X8664\\lib\\libwebp.lib out\\release-static\\$X8664\\lib\\webp.lib
    copy out\\release-static\\$X8664\\lib\\libwebpdemux.lib out\\release-static\\$X8664\\lib\\webpdemux.lib
    copy out\\release-static\\$X8664\\lib\\libwebpmux.lib out\\release-static\\$X8664\\lib\\webpmux.lib
mac:
    buildOneArch() {
        arch=$1
        folder=$2

        CFLAGS=$UNGUARDED cmake -B $folder -G Ninja . \\
            -D CMAKE_BUILD_TYPE=Release \\
            -D CMAKE_INSTALL_PREFIX=$USED_PREFIX \\
            -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
            -D CMAKE_OSX_ARCHITECTURES=$arch \\
            -D WEBP_BUILD_ANIM_UTILS=OFF \\
            -D WEBP_BUILD_CWEBP=OFF \\
            -D WEBP_BUILD_DWEBP=OFF \\
            -D WEBP_BUILD_GIF2WEBP=OFF \\
            -D WEBP_BUILD_IMG2WEBP=OFF \\
            -D WEBP_BUILD_VWEBP=OFF \\
            -D WEBP_BUILD_WEBPMUX=OFF \\
            -D WEBP_BUILD_WEBPINFO=OFF \\
            -D WEBP_BUILD_EXTRAS=OFF
        cmake --build $folder $MAKE_THREADS_CNT
    }
    buildOneArch arm64 build.arm64
    buildOneArch x86_64 build

    lipo -create build.arm64/libsharpyuv.a build/libsharpyuv.a -output build/libsharpyuv.a
    lipo -create build.arm64/libwebp.a build/libwebp.a -output build/libwebp.a
    lipo -create build.arm64/libwebpdemux.a build/libwebpdemux.a -output build/libwebpdemux.a
    lipo -create build.arm64/libwebpmux.a build/libwebpmux.a -output build/libwebpmux.a
    cmake --install build
"""
)
)

installCommands.append(
    LibraryInstallationInformation("openal-soft",
"OpenAL Soft is an LGPL-licensed, cross-platform, software implementation of the OpenAL 3D audio API",
"""
    git clone https://github.com/telegramdesktop/openal-soft.git
    cd openal-soft
win:
    git checkout 291c0fdbbd
    cmake -B build . ^
        -A %WIN32X64% ^
        -D LIBTYPE:STRING=STATIC ^
        -D FORCE_STATIC_VCRT=ON ^
        -D ALSOFT_UTILS=OFF ^
        -D ALSOFT_EXAMPLES=OFF ^
        -D ALSOFT_TESTS=OFF
    cmake --build build --config Debug --parallel
release:
    cmake --build build --config RelWithDebInfo --parallel
mac:
    git checkout coreaudio_device_uid
    CFLAGS=$UNGUARDED CPPFLAGS=$UNGUARDED cmake -B build . \\
        -D CMAKE_BUILD_TYPE=RelWithDebInfo \\
        -D CMAKE_INSTALL_PREFIX:PATH=$USED_PREFIX \\
        -D ALSOFT_EXAMPLES=OFF \\
        -D ALSOFT_UTILS=OFF \\
        -D ALSOFT_TESTS=OFF \\
        -D LIBTYPE:STRING=STATIC \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64"
    cmake --build build $MAKE_THREADS_CNT
    cmake --install build
"""
)
)

if 'build-stackwalk' in options:
    installCommands.append(
        LibraryInstallationInformation("stackwalk",
    "",
    """
    mac:
        git clone https://chromium.googlesource.com/breakpad/breakpad stackwalk
        cd stackwalk
        git checkout dfcb7b6799
    depends:patches/breakpad.diff
        git apply ../patches/breakpad.diff
        git clone -b release-1.11.0 https://github.com/google/googletest src/testing
        git clone https://chromium.googlesource.com/linux-syscall-support src/third_party/lss
        cd src/third_party/lss
        git checkout e1e7b0ad8e
        cd ../../build
        PYTHONPATH=$THIRDPARTY_DIR/gyp python3 gyp_breakpad
        cd ../processor
        xcodebuild -project processor.xcodeproj -target minidump_stackwalk -configuration Release build
    """
    )
    )

installCommands.append(
    LibraryInstallationInformation("protobuf",
"The C++ Protocol Buffers (Protobuf) library is a core component of Google's data serialization mechanism",
"""
win:
    git clone --recursive -b v21.9 https://github.com/protocolbuffers/protobuf
    cd protobuf
    git clone https://github.com/abseil/abseil-cpp third_party/abseil-cpp
    cd third_party/abseil-cpp
    git checkout 273292d1cf
    cd ../..
    mkdir build
    cd build
    cmake .. ^
        -A %WIN32X64% ^
        -Dprotobuf_BUILD_TESTS=OFF ^
        -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON ^
        -Dprotobuf_BUILD_LIBPROTOC=ON ^
        -Dprotobuf_WITH_ZLIB_DEFAULT=OFF ^
        -Dprotobuf_DEBUG_POSTFIX=""
    cmake --build . --config Release --parallel
    cmake --build . --config Debug --parallel
"""
)
)

installCommands.append(
    LibraryInstallationInformation("opus",
"Opus is a totally open, royalty-free, highly versatile audio codec. ",
"""
    git clone -b v1.5.2 https://github.com/xiph/opus.git
    cd opus
win:
    cmake -B out . ^
        -A %WIN32X64% ^
        -DCMAKE_INSTALL_PREFIX=%LIBS_DIR%/local ^
        -DOPUS_STATIC_RUNTIME=ON
    cmake --build out --config Debug --parallel
    cmake --build out --config Release --parallel
    cmake --install out --config Release
mac:
    CFLAGS="$UNGUARDED" CPPFLAGS="$UNGUARDED" cmake -B build . \\
        -D CMAKE_OSX_DEPLOYMENT_TARGET:STRING=$MACOSX_DEPLOYMENT_TARGET \\
        -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" \\
        -D CMAKE_INSTALL_PREFIX:STRING=$USED_PREFIX
    cmake --build build $MAKE_THREADS_CNT
    cmake --install build
"""              
)
)

qt = os.environ.get('QT')

if qt < '6':
    installCommands.append(LibraryInstallationInformation('qt_' + qt, 
"""
Qt is a cross-platform application development framework for creating graphical user interfaces
as well as cross-platform applications that run on various software and hardware platforms such as Linux,
Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still
being a native application with native capabilities and speed.
""",
"""
    git clone -b v$QT-lts-lgpl https://github.com/qt/qt5.git qt_$QT
    cd qt_$QT
    git submodule update --init --recursive --progress qtbase qtimageformats qtsvg
depends:patches/qtbase_""" + qt + """/*.patch
    cd qtbase
win:
    setlocal enabledelayedexpansion
    for /r %%i in (..\\..\\patches\\qtbase_%QT%\\*) do (
        git apply %%i -v
        if errorlevel 1 (
            echo ERROR: Applying patch %%~nxi failed!
            exit /b 1
        )
    )

    cd ..

    SET CONFIGURATIONS=-debug
release:
    SET CONFIGURATIONS=-debug-and-release
win:
    """ + removeDir('"%LIBS_DIR%\\Qt-' + qt + '"') + """
    SET ANGLE_DIR=%LIBS_DIR%\\tg_angle
    SET ANGLE_LIBS_DIR=%ANGLE_DIR%\\out
    SET MOZJPEG_DIR=%LIBS_DIR%\\mozjpeg
    SET OPENSSL_DIR=%LIBS_DIR%\\openssl3
    SET OPENSSL_LIBS_DIR=%OPENSSL_DIR%\\out
    SET ZLIB_LIBS_DIR=%LIBS_DIR%\\zlib
    SET WEBP_DIR=%LIBS_DIR%\\libwebp
    configure -prefix "%LIBS_DIR%\\Qt-%QT%" ^
        %CONFIGURATIONS% ^
        -force-debug-info ^
        -opensource ^
        -confirm-license ^
        -static ^
        -static-runtime ^
        -opengl es2 -no-angle ^
        -I "%ANGLE_DIR%\\include" ^
        -D "KHRONOS_STATIC=" ^
        -D "DESKTOP_APP_QT_STATIC_ANGLE=" ^
        QMAKE_LIBS_OPENGL_ES2_DEBUG="%ANGLE_LIBS_DIR%\\Debug\\tg_angle.lib %ZLIB_LIBS_DIR%\\Debug\\zlibstaticd.lib d3d9.lib dxgi.lib dxguid.lib" ^
        QMAKE_LIBS_OPENGL_ES2_RELEASE="%ANGLE_LIBS_DIR%\\Release\\tg_angle.lib %ZLIB_LIBS_DIR%\\Release\\zlibstatic.lib d3d9.lib dxgi.lib dxguid.lib" ^
        -egl ^
        QMAKE_LIBS_EGL_DEBUG="%ANGLE_LIBS_DIR%\\Debug\\tg_angle.lib %ZLIB_LIBS_DIR%\\Debug\\zlibstaticd.lib d3d9.lib dxgi.lib dxguid.lib Gdi32.lib User32.lib" ^
        QMAKE_LIBS_EGL_RELEASE="%ANGLE_LIBS_DIR%\\Release\\tg_angle.lib %ZLIB_LIBS_DIR%\\Release\\zlibstatic.lib d3d9.lib dxgi.lib dxguid.lib Gdi32.lib User32.lib" ^
        -openssl-linked ^
        -I "%OPENSSL_DIR%\\include" ^
        OPENSSL_LIBS_DEBUG="%OPENSSL_LIBS_DIR%.dbg\\libssl.lib %OPENSSL_LIBS_DIR%.dbg\\libcrypto.lib Ws2_32.lib Gdi32.lib Advapi32.lib Crypt32.lib User32.lib" ^
        OPENSSL_LIBS_RELEASE="%OPENSSL_LIBS_DIR%\\libssl.lib %OPENSSL_LIBS_DIR%\\libcrypto.lib Ws2_32.lib Gdi32.lib Advapi32.lib Crypt32.lib User32.lib" ^
        -I "%MOZJPEG_DIR%" ^
        LIBJPEG_LIBS_DEBUG="%MOZJPEG_DIR%\\Debug\\jpeg-static.lib" ^
        LIBJPEG_LIBS_RELEASE="%MOZJPEG_DIR%\\Release\\jpeg-static.lib" ^
        -system-webp ^
        -I "%WEBP_DIR%\\src" ^
        -L "%WEBP_DIR%\\out\\release-static\\$X8664\\lib" ^
        -mp ^
        -no-feature-netlistmgr ^
        -nomake examples ^
        -nomake tests ^
        -platform win32-msvc

    jom -j%NUMBER_OF_PROCESSORS%
    jom -j%NUMBER_OF_PROCESSORS% install
mac:
    find ../../patches/qtbase_$QT -type f -print0 | sort -z | xargs -0 git apply
    cd ..

    CONFIGURATIONS=-debug
release:
    CONFIGURATIONS=-debug-and-release
mac:
    ./configure -prefix "$USED_PREFIX/Qt-$QT" \
        $CONFIGURATIONS \
        -force-debug-info \
        -opensource \
        -confirm-license \
        -static \
        -opengl desktop \
        -no-openssl \
        -securetransport \
        -I "$USED_PREFIX/include" \
        LIBJPEG_LIBS="$USED_PREFIX/lib/libjpeg.a" \
        ZLIB_LIBS="$USED_PREFIX/lib/libz.a" \
        -nomake examples \
        -nomake tests \
        -platform macx-clang

    make $MAKE_THREADS_CNT
    make install
"""
    )
    )
else: # qt > '6'
    branch = 'v$QT' + ('-lts-lgpl' if qt < '6.3' else '')
    installCommands.append(
        LibraryInstallationInformation('qt_' + qt,
"""
    Qt is a cross-platform application development framework for creating graphical user interfaces
    as well as cross-platform applications that run on various software and hardware platforms such as Linux,
    Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still
    being a native application with native capabilities and speed.
""",
"""
    git clone -b """ + branch + """ https://github.com/qt/qt5.git qt_$QT
    cd qt_$QT
    git submodule update --init --recursive --progress qtbase qtimageformats qtsvg
depends:patches/qtbase_""" + qt + """/*.patch
    cd qtbase
mac:
    find ../../patches/qtbase_$QT -type f -print0 | sort -z | xargs -0 git apply -v
    cd ..
    sed -i.bak 's/tqtc-//' {qtimageformats,qtsvg}/dependencies.yaml

    CONFIGURATIONS=-debug
release:
    CONFIGURATIONS=-debug-and-release
mac:
    ./configure -prefix "$USED_PREFIX/Qt-$QT" \
        $CONFIGURATIONS \
        -force-debug-info \
        -opensource \
        -confirm-license \
        -static \
        -opengl desktop \
        -no-openssl \
        -securetransport \
        -system-webp \
        -I "$USED_PREFIX/include" \
        -no-feature-futimens \
        -no-feature-brotli \
        -nomake examples \
        -nomake tests \
        -platform macx-clang -- \
        -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
        -DCMAKE_PREFIX_PATH="$USED_PREFIX"

    ninja
    ninja install
win:
    for /r %%i in (..\\..\\patches\\qtbase_%QT%\\*) do git apply %%i -v
    cd ..

    SET CONFIGURATIONS=-debug
release:
    SET CONFIGURATIONS=-debug-and-release
win:
    """ + removeDir('"%LIBS_DIR%\\Qt' + qt + '"') + """
    SET MOZJPEG_DIR=%LIBS_DIR%\\mozjpeg
    SET OPENSSL_DIR=%LIBS_DIR%\\openssl3
    SET OPENSSL_LIBS_DIR=%OPENSSL_DIR%\\out
    SET ZLIB_LIBS_DIR=%LIBS_DIR%\\zlib
    SET WEBP_DIR=%LIBS_DIR%\\libwebp
    SET LCMS2_DIR=%LIBS_DIR%\\liblcms2
    configure -prefix "%LIBS_DIR%\\Qt-%QT%" ^
        %CONFIGURATIONS% ^
        -force-debug-info ^
        -opensource ^
        -confirm-license ^
        -static ^
        -static-runtime ^
        -feature-c++20 ^
        -no-sbom ^
        -openssl linked ^
        -system-webp ^
        -system-zlib ^
        -system-libjpeg ^
        -nomake examples ^
        -nomake tests ^
        -platform win32-msvc ^
        -D ZLIB_WINAPI ^
        -- ^
        -D OPENSSL_FOUND=1 ^
        -D OPENSSL_INCLUDE_DIR="%OPENSSL_DIR%\\include" ^
        -D LIB_EAY_DEBUG="%OPENSSL_LIBS_DIR%.dbg\\libcrypto.lib" ^
        -D SSL_EAY_DEBUG="%OPENSSL_LIBS_DIR%.dbg\\libssl.lib" ^
        -D LIB_EAY_RELEASE="%OPENSSL_LIBS_DIR%\\libcrypto.lib" ^
        -D SSL_EAY_RELEASE="%OPENSSL_LIBS_DIR%\\libssl.lib" ^
        -D JPEG_FOUND=1 ^
        -D JPEG_INCLUDE_DIR="%MOZJPEG_DIR%" ^
        -D JPEG_LIBRARY_DEBUG="%MOZJPEG_DIR%\\Debug\\jpeg-static.lib" ^
        -D JPEG_LIBRARY_RELEASE="%MOZJPEG_DIR%\\Release\\jpeg-static.lib" ^
        -D ZLIB_FOUND=1 ^
        -D ZLIB_INCLUDE_DIR="%ZLIB_LIBS_DIR%" ^
        -D ZLIB_LIBRARY_DEBUG="%ZLIB_LIBS_DIR%\\Debug\\zlibstaticd.lib" ^
        -D ZLIB_LIBRARY_RELEASE="%ZLIB_LIBS_DIR%\\Release\\zlibstatic.lib" ^
        -D WebP_INCLUDE_DIR="%WEBP_DIR%\\src" ^
        -D WebP_demux_INCLUDE_DIR="%WEBP_DIR%\\src" ^
        -D WebP_mux_INCLUDE_DIR="%WEBP_DIR%\\src" ^
        -D WebP_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webp.lib" ^
        -D WebP_demux_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webpdemux.lib" ^
        -D WebP_mux_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webpmux.lib" ^
        -D LCMS2_FOUND=1 ^
        -D LCMS2_INCLUDE_DIR="%LCMS2_DIR%\\include" ^
        -D LCMS2_LIBRARIES="%LCMS2_DIR%\\out\\Release\\src\\liblcms2.a"

    cmake --build . --config Debug --parallel
    cmake --install . --config Debug
    cmake --build . --parallel
    cmake --install .
"""
)
)
    
