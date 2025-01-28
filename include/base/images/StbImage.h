#pragma once

#ifndef STBI_NO_STDIO
#include <stdio.h>
#endif // STBI_NO_STDIO

#define STBI_VERSION 1

enum
{
    STBI_default = 0, // only used for desired_channels

    STBI_grey = 1,
    STBI_grey_alpha = 2,
    STBI_rgb = 3,
    STBI_rgb_alpha = 4
};

#include <stdlib.h>
typedef unsigned char stbi_uc;
typedef unsigned short stbi_us;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STBIDEF
#ifdef STB_IMAGE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif
#endif

    //////////////////////////////////////////////////////////////////////////////
    //
    // PRIMARY API - works on images of any type
    //

    //
    // load image by filename, open file, or memory buffer
    //

    typedef struct
    {
        int      (*read)  (void* user, char* data, int size);   // fill 'data' with 'size' bytes.  return number of bytes actually read
        void     (*skip)  (void* user, int n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
        int      (*eof)   (void* user);                       // returns nonzero if we are at end of file/data
    } stbi_io_callbacks;

    ////////////////////////////////////
    //
    // 8-bits-per-channel interface
    //

    STBIDEF stbi_uc* stbi_load_from_memory(stbi_uc           const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF stbi_uc* stbi_load_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
    STBIDEF stbi_uc* stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF stbi_uc* stbi_load_from_file(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
    // for stbi_load_from_file, file pointer is left pointing immediately after image
#endif

#ifndef STBI_NO_GIF
    STBIDEF stbi_uc* stbi_load_gif_from_memory(stbi_uc const* buffer, int len, int** delays, int* x, int* y, int* z, int* comp, int req_comp);
#endif

#ifdef STBI_WINDOWS_UTF8
    STBIDEF int stbi_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input);
#endif

    ////////////////////////////////////
    //
    // 16-bits-per-channel interface
    //

    STBIDEF stbi_us* stbi_load_16_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF stbi_us* stbi_load_16_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
    STBIDEF stbi_us* stbi_load_16(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF stbi_us* stbi_load_from_file_16(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
#endif

    ////////////////////////////////////
    //
    // float-per-channel interface
    //
#ifndef STBI_NO_LINEAR
    STBIDEF float* stbi_loadf_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF float* stbi_loadf_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
    STBIDEF float* stbi_loadf(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
    STBIDEF float* stbi_loadf_from_file(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
#endif
#endif

#ifndef STBI_NO_HDR
    STBIDEF void   stbi_hdr_to_ldr_gamma(float gamma);
    STBIDEF void   stbi_hdr_to_ldr_scale(float scale);
#endif // STBI_NO_HDR

#ifndef STBI_NO_LINEAR
    STBIDEF void   stbi_ldr_to_hdr_gamma(float gamma);
    STBIDEF void   stbi_ldr_to_hdr_scale(float scale);
#endif // STBI_NO_LINEAR

    // stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
    STBIDEF int    stbi_is_hdr_from_callbacks(stbi_io_callbacks const* clbk, void* user);
    STBIDEF int    stbi_is_hdr_from_memory(stbi_uc const* buffer, int len);
#ifndef STBI_NO_STDIO
    STBIDEF int      stbi_is_hdr(char const* filename);
    STBIDEF int      stbi_is_hdr_from_file(FILE* f);
#endif // STBI_NO_STDIO


    // get a VERY brief reason for failure
    // on most compilers (and ALL modern mainstream compilers) this is threadsafe
    STBIDEF const char* stbi_failure_reason(void);

    // free the loaded image -- this is just free()
    STBIDEF void     stbi_image_free(void* retval_from_stbi_load);

    // get image dimensions & components without fully decoding
    STBIDEF int      stbi_info_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp);
    STBIDEF int      stbi_info_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* comp);
    STBIDEF int      stbi_is_16_bit_from_memory(stbi_uc const* buffer, int len);
    STBIDEF int      stbi_is_16_bit_from_callbacks(stbi_io_callbacks const* clbk, void* user);

#ifndef STBI_NO_STDIO
    STBIDEF int      stbi_info(char const* filename, int* x, int* y, int* comp);
    STBIDEF int      stbi_info_from_file(FILE* f, int* x, int* y, int* comp);
    STBIDEF int      stbi_is_16_bit(char const* filename);
    STBIDEF int      stbi_is_16_bit_from_file(FILE* f);
#endif



    // for image formats that explicitly notate that they have premultiplied alpha,
    // we just return the colors as stored in the file. set this flag to force
    // unpremultiplication. results are undefined if the unpremultiply overflow.
    STBIDEF void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply);

    // indicate whether we should process iphone images back to canonical format,
    // or just pass them through "as-is"
    STBIDEF void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert);

    // flip the image vertically, so the first pixel in the output array is the bottom left
    STBIDEF void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

    // as above, but only applies to images loaded on the thread that calls the function
    // this function is only available if your compiler supports thread-local variables;
    // calling it will fail to link if your compiler doesn't
    STBIDEF void stbi_set_unpremultiply_on_load_thread(int flag_true_if_should_unpremultiply);
    STBIDEF void stbi_convert_iphone_png_to_rgb_thread(int flag_true_if_should_convert);
    STBIDEF void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip);

    // ZLIB client - used by PNG, available for other purposes

    STBIDEF char* stbi_zlib_decode_malloc_guesssize(const char* buffer, int len, int initial_size, int* outlen);
    STBIDEF char* stbi_zlib_decode_malloc_guesssize_headerflag(const char* buffer, int len, int initial_size, int* outlen, int parse_header);
    STBIDEF char* stbi_zlib_decode_malloc(const char* buffer, int len, int* outlen);
    STBIDEF int   stbi_zlib_decode_buffer(char* obuffer, int olen, const char* ibuffer, int ilen);

    STBIDEF char* stbi_zlib_decode_noheader_malloc(const char* buffer, int len, int* outlen);
    STBIDEF int   stbi_zlib_decode_noheader_buffer(char* obuffer, int olen, const char* ibuffer, int ilen);


#ifdef __cplusplus
}
#endif

#if defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
#ifndef STBI_ONLY_JPEG
#define STBI_NO_JPEG
#endif
#ifndef STBI_ONLY_PNG
#define STBI_NO_PNG
#endif
#ifndef STBI_ONLY_BMP
#define STBI_NO_BMP
#endif
#ifndef STBI_ONLY_PSD
#define STBI_NO_PSD
#endif
#ifndef STBI_ONLY_TGA
#define STBI_NO_TGA
#endif
#ifndef STBI_ONLY_GIF
#define STBI_NO_GIF
#endif
#ifndef STBI_ONLY_HDR
#define STBI_NO_HDR
#endif
#ifndef STBI_ONLY_PIC
#define STBI_NO_PIC
#endif
#ifndef STBI_ONLY_PNM
#define STBI_NO_PNM
#endif
#endif

#if defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#define STBI_NO_ZLIB
#endif


#include <stdarg.h>
#include <stddef.h> // ptrdiff_t on osx
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR)
#include <math.h>  // ldexp, pow
#endif

#ifndef STBI_NO_STDIO
#include <stdio.h>
#endif

#ifndef STBI_ASSERT
#include <assert.h>
#define STBI_ASSERT(x) assert(x)
#endif

#ifdef __cplusplus
#define STBI_EXTERN extern "C"
#else
#define STBI_EXTERN extern
#endif


#ifndef _MSC_VER
#ifdef __cplusplus
#define stbi_inline inline
#else
#define stbi_inline
#endif
#else
#define stbi_inline __forceinline
#endif

#ifndef STBI_NO_THREAD_LOCALS
#if defined(__cplusplus) &&  __cplusplus >= 201103L
#define STBI_THREAD_LOCAL       thread_local
#elif defined(__GNUC__) && __GNUC__ < 5
#define STBI_THREAD_LOCAL       __thread
#elif defined(_MSC_VER)
#define STBI_THREAD_LOCAL       __declspec(thread)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#define STBI_THREAD_LOCAL       _Thread_local
#endif

#ifndef STBI_THREAD_LOCAL
#if defined(__GNUC__)
#define STBI_THREAD_LOCAL       __thread
#endif
#endif
#endif

#ifdef _MSC_VER
typedef unsigned short stbi__uint16;
typedef   signed short stbi__int16;
typedef unsigned int   stbi__uint32;
typedef   signed int   stbi__int32;
#else
#include <stdint.h>
typedef uint16_t stbi__uint16;
typedef int16_t  stbi__int16;
typedef uint32_t stbi__uint32;
typedef int32_t  stbi__int32;
#endif

// should produce compiler error if size is wrong
typedef unsigned char validate_uint32[sizeof(stbi__uint32) == 4 ? 1 : -1];

#ifdef _MSC_VER
#define STBI_NOTUSED(v)  (void)(v)
#else
#define STBI_NOTUSED(v)  (void)sizeof(v)
#endif

#ifdef _MSC_VER
#define STBI_HAS_LROTL
#endif

#ifdef STBI_HAS_LROTL
#define stbi_lrot(x,y)  _lrotl(x,y)
#else
#define stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (-(y) & 31)))
#endif

#if defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#elif !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#else
#error "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#endif

#ifndef STBI_MALLOC
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)
#endif

#ifndef STBI_REALLOC_SIZED
#define STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#endif

// x86/x64 detection
#if defined(__x86_64__) || defined(_M_X64)
#define STBI__X64_TARGET
#elif defined(__i386) || defined(_M_IX86)
#define STBI__X86_TARGET
#endif

typedef struct
{
    stbi__uint32 img_x, img_y;
    int img_n, img_out_n;

    stbi_io_callbacks io;
    void* io_user_data;

    int read_from_callbacks;
    int buflen;
    stbi_uc buffer_start[128];
    int callback_already_read;

    stbi_uc* img_buffer, * img_buffer_end;
    stbi_uc* img_buffer_original, * img_buffer_original_end;
} stbi__context;

enum
{
    STBI_ORDER_RGB,
    STBI_ORDER_BGR
};

typedef struct
{
    int bits_per_channel;
    int num_channels;
    int channel_order;
} stbi__result_info;

#ifndef STBI_NO_JPEG
static int      stbi__jpeg_test(stbi__context* s);
static void* stbi__jpeg_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__jpeg_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PNG
static int      stbi__png_test(stbi__context* s);
static void* stbi__png_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__png_info(stbi__context* s, int* x, int* y, int* comp);
static int      stbi__png_is16(stbi__context* s);
#endif

#ifndef STBI_NO_BMP
static int stbi__bmp_test_raw(stbi__context* s);
static int      stbi__bmp_test(stbi__context* s);
static void* stbi__bmp_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__bmp_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_TGA
static int      stbi__tga_test(stbi__context* s);
static void* stbi__tga_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__tga_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PSD
static int      stbi__psd_test(stbi__context* s);
static void* stbi__psd_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri, int bpc);
static int      stbi__psd_info(stbi__context* s, int* x, int* y, int* comp);
static int      stbi__psd_is16(stbi__context* s);
#endif

#ifndef STBI_NO_HDR
static int      stbi__hdr_test(stbi__context* s);
static float* stbi__hdr_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__hdr_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PIC
static int      stbi__pic_test(stbi__context* s);
static void* stbi__pic_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__pic_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_GIF
static int      stbi__gif_test(stbi__context* s);
static void* stbi__gif_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static void* stbi__load_gif_main(stbi__context* s, int** delays, int* x, int* y, int* z, int* comp, int req_comp);
static int      stbi__gif_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PNM
static int      stbi__pnm_test(stbi__context* s);
static void* stbi__pnm_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
static int      stbi__pnm_info(stbi__context* s, int* x, int* y, int* comp);
static int      stbi__pnm_is16(stbi__context* s);
#endif

#ifndef STBI_NO_STDIO
static FILE* stbi__fopen(char const* filename, char const* mode);
static void stbi__start_file(stbi__context* s, FILE* f);
#endif // STBI_NO_STDIO

#ifndef STBI_NO_LINEAR
static float* stbi__ldr_to_hdr(stbi_uc* data, int x, int y, int comp);
#endif

#ifndef STBI_NO_HDR
static stbi_uc* stbi__hdr_to_ldr(float* data, int x, int y, int comp);
#endif

static stbi_uc* stbi__convert_16_to_8(stbi__uint16* orig, int w, int h, int channels);
static stbi__uint16* stbi__convert_8_to_16(stbi_uc* orig, int w, int h, int channels);
static void stbi__vertical_flip(void* image, int w, int h, int bytes_per_pixel);


// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#ifndef STBI_NO_FAILURE_STRINGS
static int stbi__err(const char* str);
#endif 

#ifdef STBI_NO_FAILURE_STRINGS
#define stbi__err(x,y)  0
#elif defined(STBI_FAILURE_USERMSG)
#define stbi__err(x,y)  stbi__err(y)
#else
#define stbi__err(x,y)  stbi__err(x)
#endif

#define stbi__errpf(x,y)   ((float *)(size_t) (stbi__err(x,y)?NULL:NULL))
#define stbi__errpuc(x,y)  ((unsigned char *)(size_t) (stbi__err(x,y)?NULL:NULL))

#ifndef STBI_THREAD_LOCAL
#define stbi__vertically_flip_on_load  stbi__vertically_flip_on_load_global
#else
static STBI_THREAD_LOCAL int stbi__vertically_flip_on_load_local, stbi__vertically_flip_on_load_set;

STBIDEF void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip)
{
    stbi__vertically_flip_on_load_local = flag_true_if_should_flip;
    stbi__vertically_flip_on_load_set = 1;
}

static int stbi__vertically_flip_on_load_global = 0;
#define stbi__vertically_flip_on_load  (stbi__vertically_flip_on_load_set       \
                                         ? stbi__vertically_flip_on_load_local  \
                                         : stbi__vertically_flip_on_load_global)
#endif // STBI_THREAD_LOCAL
