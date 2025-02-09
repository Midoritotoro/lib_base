#pragma once 

#include <cstdint>
#include <base/system/Platform.h>

#ifdef BIG_ENDIAN
#   define FOURCC( a, b, c, d ) \
        ( ((uint32_t)d) | ( ((uint32_t)c) << 8 ) \
           | ( ((uint32_t)b) << 16 ) | ( ((uint32_t)a) << 24 ) )
#   define TWOCC( a, b ) \
        ( (uint16_t)(b) | ( (uint16_t)(a) << 8 ) )

#else
#   define FOURCC( a, b, c, d ) \
        ( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) \
           | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )
#   define TWOCC( a, b ) \
        ( (uint16_t)(a) | ( (uint16_t)(b) << 8 ) )

#endif

#define CODEC_UNKNOWN         FOURCC('u','n','d','f')

    /* Video codec */
#define CODEC_MPGV            FOURCC('m','p','g','v')
#define CODEC_MP4V            FOURCC('m','p','4','v')
#define CODEC_DIV1            FOURCC('D','I','V','1')
#define CODEC_DIV2            FOURCC('D','I','V','2')
#define CODEC_DIV3            FOURCC('D','I','V','3')
#define CODEC_SVQ1            FOURCC('S','V','Q','1')
#define CODEC_SVQ3            FOURCC('S','V','Q','3')
#define CODEC_H264            FOURCC('h','2','6','4')
#define CODEC_H263            FOURCC('h','2','6','3')
#define CODEC_H263I           FOURCC('I','2','6','3')
#define CODEC_H263P           FOURCC('I','L','V','R')
#define CODEC_FLV1            FOURCC('F','L','V','1')
#define CODEC_H261            FOURCC('h','2','6','1')
#define CODEC_VVC             FOURCC('h','2','6','6')
#define CODEC_MJPG            FOURCC('M','J','P','G')
#define CODEC_MJPGB           FOURCC('m','j','p','b')
#define CODEC_LJPG            FOURCC('L','J','P','G')
#define CODEC_WMV1            FOURCC('W','M','V','1')
#define CODEC_WMV2            FOURCC('W','M','V','2')
#define CODEC_WMV3            FOURCC('W','M','V','3')
#define CODEC_WMVA            FOURCC('W','M','V','A')
#define CODEC_WMVP            FOURCC('W','M','V','P')
#define CODEC_WMVP2           FOURCC('W','V','P','2')
#define CODEC_VC1             FOURCC('V','C','-','1')
#define CODEC_DAALA           FOURCC('d','a','a','l')
#define CODEC_THEORA          FOURCC('t','h','e','o')
#define CODEC_TARKIN          FOURCC('t','a','r','k')
#define CODEC_DIRAC           FOURCC('d','r','a','c')
#define CODEC_OGGSPOTS        FOURCC('S','P','O','T')
#define CODEC_CAVS            FOURCC('C','A','V','S')
#define CODEC_CAVS2           FOURCC('A','V','S','2')
#define CODEC_CAVS3           FOURCC('A','V','S','3')
#define CODEC_NUV             FOURCC('N','J','P','G')
#define CODEC_RV10            FOURCC('R','V','1','0')
#define CODEC_RV13            FOURCC('R','V','1','3')
#define CODEC_RV20            FOURCC('R','V','2','0')
#define CODEC_RV30            FOURCC('R','V','3','0')
#define CODEC_RV40            FOURCC('R','V','4','0')
#define CODEC_VP3             FOURCC('V','P','3',' ')
#define CODEC_VP5             FOURCC('V','P','5',' ')
#define CODEC_VP6             FOURCC('V','P','6','2')
#define CODEC_VP6F            FOURCC('V','P','6','F')
#define CODEC_VP6A            FOURCC('V','P','6','A')
#define CODEC_MSVIDEO1        FOURCC('M','S','V','C')
#define CODEC_FLIC            FOURCC('F','L','I','C')
#define CODEC_SP5X            FOURCC('S','P','5','X')
#define CODEC_DV              FOURCC('d','v',' ',' ')
#define CODEC_MSRLE           FOURCC('m','r','l','e')
#define CODEC_HUFFYUV         FOURCC('H','F','Y','U')
#define CODEC_FFVHUFF         FOURCC('F','F','V','H')
#define CODEC_ASV1            FOURCC('A','S','V','1')
#define CODEC_ASV2            FOURCC('A','S','V','2')
#define CODEC_FFV1            FOURCC('F','F','V','1')
#define CODEC_VCR1            FOURCC('V','C','R','1')
#define CODEC_CLJR            FOURCC('C','L','J','R')
#define CODEC_RPZA            FOURCC('r','p','z','a')
#define CODEC_SMC             FOURCC('s','m','c',' ')
#define CODEC_CINEPAK         FOURCC('C','V','I','D')
#define CODEC_TSCC            FOURCC('T','S','C','C')
#define CODEC_CSCD            FOURCC('C','S','C','D')
#define CODEC_ZMBV            FOURCC('Z','M','B','V')
#define CODEC_VMNC            FOURCC('V','M','n','c')
#define CODEC_FMVC            FOURCC('F','M','V','C')
#define CODEC_FRAPS           FOURCC('F','P','S','1')
#define CODEC_TRUEMOTION1     FOURCC('D','U','C','K')
#define CODEC_TRUEMOTION2     FOURCC('T','M','2','0')
#define CODEC_QTRLE           FOURCC('r','l','e',' ')
#define CODEC_QDRAW           FOURCC('q','d','r','w')
#define CODEC_QOI             FOURCC('q','o','i','f')
#define CODEC_QPEG            FOURCC('Q','P','E','G')
#define CODEC_ULTI            FOURCC('U','L','T','I')
#define CODEC_VIXL            FOURCC('V','I','X','L')
#define CODEC_LOCO            FOURCC('L','O','C','O')
#define CODEC_WNV1            FOURCC('W','N','V','1')
#define CODEC_AASC            FOURCC('A','A','S','C')
#define CODEC_INDEO2          FOURCC('I','V','2','0')
#define CODEC_INDEO3          FOURCC('I','V','3','1')
#define CODEC_INDEO4          FOURCC('I','V','4','1')
#define CODEC_INDEO5          FOURCC('I','V','5','0')
#define CODEC_FLASHSV         FOURCC('F','S','V','1')
#define CODEC_KMVC            FOURCC('K','M','V','C')
#define CODEC_SMACKVIDEO      FOURCC('S','M','K','2')
#define CODEC_DNXHD           FOURCC('A','V','d','n')
#define CODEC_8BPS            FOURCC('8','B','P','S')
#define CODEC_MIMIC           FOURCC('M','L','2','O')
#define CODEC_INTERPLAY       FOURCC('i','m','v','e')
#define CODEC_IDCIN           FOURCC('I','D','C','I')
#define CODEC_4XM             FOURCC('4','X','M','V')
#define CODEC_ROQ             FOURCC('R','o','Q','v')
#define CODEC_MDEC            FOURCC('M','D','E','C')
#define CODEC_VMDVIDEO        FOURCC('V','M','D','V')
#define CODEC_CDG             FOURCC('C','D','G',' ')
#define CODEC_FRWU            FOURCC('F','R','W','U')
#define CODEC_AMV             FOURCC('A','M','V',' ')
#define CODEC_VP4             FOURCC('V','P','4','0')
#define CODEC_VP7             FOURCC('V','P','7','0')
#define CODEC_VP8             FOURCC('V','P','8','0')
#define CODEC_VP8ALPHA_ES     FOURCC('V','P','8','a')
#define CODEC_VP9             FOURCC('V','P','9','0')
#define CODEC_VP9ALPHA_ES     FOURCC('V','P','9','a')
#define CODEC_VP10            FOURCC('V','P',':','0')
#define CODEC_AV1             FOURCC('a','v','0','1')
#define CODEC_JPEG2000        FOURCC('J','P','2','K')
#define CODEC_LAGARITH        FOURCC('L','A','G','S')
#define CODEC_FLASHSV2        FOURCC('F','S','V','2')
#define CODEC_PRORES          FOURCC('a','p','c','n')
#define CODEC_MXPEG           FOURCC('M','X','P','G')
#define CODEC_CDXL            FOURCC('C','D','X','L')
#define CODEC_BMVVIDEO        FOURCC('B','M','V','V')
#define CODEC_UTVIDEO         FOURCC('U','L','R','A')
#define CODEC_VBLE            FOURCC('V','B','L','E')
#define CODEC_DXTORY          FOURCC('x','t','o','r')
#define CODEC_MSS1            FOURCC('M','S','S','1')
#define CODEC_MSS2            FOURCC('M','S','S','2')
#define CODEC_MSA1            FOURCC('M','S','A','1')
#define CODEC_TSC2            FOURCC('T','S','C','2')
#define CODEC_MTS2            FOURCC('M','T','S','2')
#define CODEC_HEVC            FOURCC('h','e','v','c')
#define CODEC_ICOD            FOURCC('i','c','o','d')
#define CODEC_G2M2            FOURCC('G','2','M','2')
#define CODEC_G2M3            FOURCC('G','2','M','3')
#define CODEC_G2M4            FOURCC('G','2','M','4')
#define CODEC_BINKVIDEO       FOURCC('B','I','K','f')
#define CODEC_BINKAUDIO_DCT   FOURCC('B','A','U','1')
#define CODEC_BINKAUDIO_RDFT  FOURCC('B','A','U','2')
#define CODEC_XAN_WC4         FOURCC('X','x','a','n')
#define CODEC_LCL_MSZH        FOURCC('M','S','Z','H')
#define CODEC_LCL_ZLIB        FOURCC('Z','L','I','B')
#define CODEC_THP             FOURCC('T','H','P','0')
#define CODEC_ESCAPE124       FOURCC('E','1','2','4')
#define CODEC_KGV1            FOURCC('K','G','V','1')
#define CODEC_CLLC            FOURCC('C','L','L','C')
#define CODEC_AURA            FOURCC('A','U','R','A')
#define CODEC_FIC             FOURCC('F','I','C','V')
#define CODEC_TMV             FOURCC('T','M','A','V')
#define CODEC_XAN_WC3         FOURCC('X','A','N','3')
#define CODEC_WS_VQA          FOURCC('W','V','Q','A')
#define CODEC_MMVIDEO         FOURCC('M','M','V','I')
#define CODEC_AVS             FOURCC('A','V','S','V')
#define CODEC_DSICINVIDEO     FOURCC('D','C','I','V')
#define CODEC_TIERTEXSEQVIDEO FOURCC('T','S','E','Q')
#define CODEC_DXA             FOURCC('D','E','X','A')
#define CODEC_C93             FOURCC('I','C','9','3')
#define CODEC_BETHSOFTVID     FOURCC('B','V','I','D')
#define CODEC_VB              FOURCC('V','B','V','1')
#define CODEC_RL2             FOURCC('R','L','V','2')
#define CODEC_BFI             FOURCC('B','F','&','I')
#define CODEC_CMV             FOURCC('E','C','M','V')
#define CODEC_MOTIONPIXELS    FOURCC('M','P','I','X')
#define CODEC_TGV             FOURCC('T','G','V','V')
#define CODEC_TGQ             FOURCC('T','G','Q','V')
#define CODEC_TQI             FOURCC('T','Q','I','V')
#define CODEC_MAD             FOURCC('M','A','D','V')
#define CODEC_ANM             FOURCC('A','N','I','M')
#define CODEC_YOP             FOURCC('Y','O','P','V')
#define CODEC_JV              FOURCC('J','V','0','0')
#define CODEC_DFA             FOURCC('D','F','I','A')
#define CODEC_HNM4_VIDEO      FOURCC('H','N','M','4')
#define CODEC_TDSC            FOURCC('T','D','S','C')
#define CODEC_HQX             FOURCC('C','H','Q','X')
#define CODEC_HQ_HQA          FOURCC('C','U','V','C')
#define CODEC_HAP             FOURCC('H','A','P','1')
#define CODEC_DXV             FOURCC('D','X','D','3')
#define CODEC_CINEFORM        FOURCC('C','F','H','D')
#define CODEC_SPEEDHQ         FOURCC('S','H','Q','2')
#define CODEC_PIXLET          FOURCC('p','x','l','t')
#define CODEC_MAGICYUV        FOURCC('M','8','Y','0')
#define CODEC_IMM4            FOURCC('I','M','M','4')
#define CODEC_IMM5            FOURCC('I','M','M','5')
#define CODEC_AGM             FOURCC('A','G','M','0')
#define CODEC_NOTCHLC         FOURCC('n','c','l','c')
#define CODEC_RTP_VIDEO_RAW   FOURCC('R','T','P','V')

/***********
 * Chromas
 ***********/

 /* Planar YUV */

 /* Planar YUV 4:1:0 Y:U:V */
#define CODEC_I410            FOURCC('I','4','1','0')
/* Planar YUV 4:1:1 Y:U:V */
#define CODEC_I411            FOURCC('I','4','1','1')

/* Planar YUV 4:2:0 Y:V:U */
#define CODEC_YV12            FOURCC('Y','V','1','2')
/* Planar YUV 4:2:0 Y:U:V 8-bit */
#define CODEC_I420            FOURCC('I','4','2','0')
/* Planar YUV 4:2:0 Y:U:V  9-bit stored on 16 bits */
#define CODEC_I420_9L         FOURCC('I','0','9','L')
#define CODEC_I420_9B         FOURCC('I','0','9','B')
/* Planar YUV 4:2:0 Y:U:V 10-bit stored on 16 bits LSB */
#define CODEC_I420_10L        FOURCC('I','0','A','L')
#define CODEC_I420_10B        FOURCC('I','0','A','B')
/* Planar YUV 4:2:0 Y:U:V 12-bit stored on 16 bits LSB */
#define CODEC_I420_12L        FOURCC('I','0','C','L')
#define CODEC_I420_12B        FOURCC('I','0','C','B')

/* Planar YUV 4:2:0 Y:U:V 16-bit stored on 16 bits */
#define CODEC_I420_16L        FOURCC('I','0','F','L')
#define CODEC_I420_16B        FOURCC('I','0','F','B')

/* Planar YUV 4:2:2 Y:U:V 8-bit */
#define CODEC_I422            FOURCC('I','4','2','2')
/* Planar YUV 4:2:2 Y:U:V  9-bit stored on 16 bits LSB */
#define CODEC_I422_9L         FOURCC('I','2','9','L')
#define CODEC_I422_9B         FOURCC('I','2','9','B')
/* Planar YUV 4:2:2 Y:U:V 10-bit stored on 16 bits LSB */
#define CODEC_I422_10L        FOURCC('I','2','A','L')
#define CODEC_I422_10B        FOURCC('I','2','A','B')
/* Planar YUV 4:2:2 Y:U:V 12-bit stored on 16 bits LSB */
#define CODEC_I422_12L        FOURCC('I','2','C','L')
#define CODEC_I422_12B        FOURCC('I','2','C','B')
/* Planar YUV 4:2:2 Y:U:V 16-bit stored on 16 bits */
#define CODEC_I422_16L        FOURCC('I','2','F','L')
#define CODEC_I422_16B        FOURCC('I','2','F','B')

/* Planar YUV 4:4:0 Y:U:V */
#define CODEC_I440            FOURCC('I','4','4','0')
/* Planar YUV 4:4:4 Y:U:V 8-bit */
#define CODEC_I444            FOURCC('I','4','4','4')
/* Planar YUV 4:4:4 Y:U:V  9-bit stored on 16 bits LSB */
#define CODEC_I444_9L         FOURCC('I','4','9','L')
#define CODEC_I444_9B         FOURCC('I','4','9','B')
/* Planar YUV 4:4:4 Y:U:V 10-bit stored on 16 bits LSB */
#define CODEC_I444_10L        FOURCC('I','4','A','L')
#define CODEC_I444_10B        FOURCC('I','4','A','B')
/* Planar YUV 4:4:4 Y:U:V 12-bit stored on 16 bits LSB */
#define CODEC_I444_12L        FOURCC('I','4','C','L')
#define CODEC_I444_12B        FOURCC('I','4','C','B')
/* Planar YUV 4:4:4 Y:U:V 16-bit */
#define CODEC_I444_16L        FOURCC('I','4','F','L')
#define CODEC_I444_16B        FOURCC('I','4','F','B')

/* Palettized YUV with palette 8-bit Y:U:V:A in memory order */
#define CODEC_YUVP            FOURCC('Y','U','V','P')

/* Planar YUV 4:4:4 Y:U:V:A */
#define CODEC_YUVA            FOURCC('Y','U','V','A')
/* Planar YUV 4:2:2 Y:U:V:A */
#define CODEC_YUV422A         FOURCC('I','4','2','A')
/* Planar YUV 4:2:0 Y:U:V:A */
#define CODEC_YUV420A         FOURCC('I','4','0','A')

/* Planar Y:U:V:A 4:4:4 10bits */
#define CODEC_YUVA_444_10L    FOURCC('Y','A','0','L')
#define CODEC_YUVA_444_10B    FOURCC('Y','A','0','B')

/* Planar Y:U:V:A 4:4:4 12bits */
#define CODEC_YUVA_444_12L    FOURCC('Y','A','2','L')
#define CODEC_YUVA_444_12B    FOURCC('Y','A','2','B')

/* Semi-planar Y/UV */

/* 2 planes Y/UV 4:2:0 */
#define CODEC_NV12            FOURCC('N','V','1','2')
/* 2 planes Y/VU 4:2:0 */
#define CODEC_NV21            FOURCC('N','V','2','1')
/* 2 planes Y/UV 4:2:2 */
#define CODEC_NV16            FOURCC('N','V','1','6')
/* 2 planes Y/VU 4:2:2 */
#define CODEC_NV61            FOURCC('N','V','6','1')
/* 2 planes Y/UV 4:4:4 */
#define CODEC_NV24            FOURCC('N','V','2','4')
/* 2 planes Y/VU 4:4:4 */
#define CODEC_NV42            FOURCC('N','V','4','2')
/* 2 planes Y/UV 4:2:0 10-bit MSB, little endian */
#define CODEC_P010            FOURCC('P','0','1','0')
/* 2 planes Y/UV 4:2:0 16-bit, little endian */
#define CODEC_P016            FOURCC('P','0','1','6')

/* Packed YUV */

/* Packed YUV 4:2:0, U:V:Y */
#define CODEC_YUV4            FOURCC('y','u','v','4')
/* Packed YUV 4:2:2, U:Y:V:Y */
#define CODEC_UYVY            FOURCC('U','Y','V','Y')
/* Packed YUV 4:2:2, V:Y:U:Y */
#define CODEC_VYUY            FOURCC('V','Y','U','Y')
/* Packed YUV 4:2:2, Y:U:Y:V */
#define CODEC_YUYV            FOURCC('Y','U','Y','2')
/* Packed YUV 4:2:2, Y:U:Y:V, signed */
#define CODEC_YUV2            FOURCC('y','u','v','2')
/* Packed YUV 4:2:2, Y:V:Y:U */
#define CODEC_YVYU            FOURCC('Y','V','Y','U')
/* Packed YUV 2:1:1, Y:U:Y:V */
#define CODEC_Y211            FOURCC('Y','2','1','1')
/* Packed YUV 4:2:2, U:Y:V:Y, reverted */
#define CODEC_CYUV            FOURCC('c','y','u','v')
/* Packed YUV 4:2:2 10-bit U10:Y10:V10:Y10:X2 (12 on 4*32bits) */
#define CODEC_V210            FOURCC('v','2','1','0')
/* Packed YUV 4:4:4 */
#define CODEC_V308            FOURCC('v','3','0','8')
/* Packed YUVA 4:4:4:4 */
#define CODEC_V408            FOURCC('v','4','0','8')
/* Packed YUV 4:4:4 10-bit X2:U10:Y10:V10:Y10 */
#define CODEC_V410            FOURCC('v','4','1','0')
/* I420 packed for RTP (RFC 4175) */
#define CODEC_R420            FOURCC('r','4','2','0')
/* Packed YUV 4:2:2 10-bit V10:U10:Y10:A2 */
#define CODEC_Y210            FOURCC('Y','2','1','0')
/* Packed YUV 4:4:4 10-bit V10:U10:Y10:A2 */
#define CODEC_Y410            FOURCC('Y','4','1','0')
/* Packed YUV 4:4:4 V:U:Y:A */
#define CODEC_VUYA            FOURCC('V','U','Y','A')

/* RGB / RGBA */

/* Palettized 8-bit RGB with palette element 8-bit R:G:B:A in memory order */
#define CODEC_RGBP            FOURCC('R','G','B','P')

/* 32-bit RGB, in memory address order: "XRGB" ignoring the x component */
#define CODEC_XRGB            FOURCC('R','V','3','2')
/* 32-bit RGB, in memory address order: "RGBX" ignoring the x component */
#define CODEC_RGBX            FOURCC('R','G','B','X')
/* 32-bit BGR, in memory address order: "BGRX" ignoring the x component */
#define CODEC_BGRX            FOURCC('B','G','R','X')
/* 32-bit BGR, in memory address order: "XBGR" ignoring the x component */
#define CODEC_XBGR            FOURCC('X','B','G','R')

/* 24-bit RGB, in memory address order: "RGB" */
#define CODEC_RGB24           FOURCC('R','V','2','4')
/* 24-bit BGR, in memory address order: "BGR" */
#define CODEC_BGR24           FOURCC('B','G','R','3')

/* 16-bit RGB Big-Endian: "R5G6B5" */
#define CODEC_RGB565BE          FOURCC('R','B','1','6')
/* 16-bit RGB Little-Endian: "g3B5 R5G3" */
#define CODEC_RGB565LE          FOURCC('R','V','1','6')
/* 16-bit BGR Big-Endian: "B5G6R5" */
#define CODEC_BGR565BE          FOURCC('B','B','1','6')
/* 16-bit BGR Little-Endian: "g3R5 B5G3" */
#define CODEC_BGR565LE          FOURCC('B','L','1','6')

#ifdef WORDS_BIGENDIAN
/* 16-bit RGB with host endianness */
#define CODEC_RGB565    CODEC_RGB565BE
/* 16-bit BGR with host endianness */
#define CODEC_BGR565    CODEC_BGR565BE
#else
/* 16-bit RGB with host endianness */
#define CODEC_RGB565    CODEC_RGB565LE
/* 16-bit BGR with host endianness */
#define CODEC_BGR565    CODEC_BGR565LE
#endif

/* 15-bit RGB Big-Endian: "0R5G5B5" */
#define CODEC_RGB555BE          FOURCC('R','B','1','5')
/* 15-bit RGB Little-Endian: "g3B5 0R5G2" */
#define CODEC_RGB555LE          FOURCC('R','V','1','5')
/* 15-bit BGR Big-Endian: "0B5G5R5" */
#define CODEC_BGR555BE          FOURCC('B','B','1','5')
/* 15-bit BGR Little-Endian: "g3R5 0B5G2" */
#define CODEC_BGR555LE          FOURCC('B','L','1','5')

#ifdef WORDS_BIGENDIAN
/* 15-bit RGB with host endianness */
#define CODEC_RGB555    CODEC_RGB555BE
/* 15-bit BGR with host endianness */
#define CODEC_BGR555    CODEC_BGR555BE
#else
/* 15-bit RGB with host endianness */
#define CODEC_RGB555    CODEC_RGB555LE
/* 15-bit BGR with host endianness */
#define CODEC_BGR555    CODEC_BGR555LE
#endif


/* 8-bit RGB "R3G3B2" */
#define CODEC_RGB332           FOURCC('R','3','3','2')
/* 8-bit RGB "R2G3B3" */
#define CODEC_RGB233           FOURCC('R','G','B','8')
/* 8-bit BGR "B2G3R3" */
#define CODEC_BGR233           FOURCC('B','2','3','3')

/* 32-bit RGBA, in memory address order: "RGBA" */
#define CODEC_RGBA            FOURCC('R','G','B','A')
/* 32-bit ARGB, in memory address order: "ARGB" */
#define CODEC_ARGB            FOURCC('A','R','G','B')
/* 32-bit ARGB, in memory address order: "BGRA" */
#define CODEC_BGRA            FOURCC('B','G','R','A')
/* 32-bit RGBA, in memory address order: "ABGR" */
#define CODEC_ABGR            FOURCC('A','B','G','R')
/* 32 bits Little-Endian RGBA with 10-bit colors "A2B6 b4G4 g6R2 r8" */
#define CODEC_RGBA10LE        FOURCC('R','G','A','0')
/* 64 bits RGBA */
#define CODEC_RGBA64          FOURCC('R','G','A','4')

/* Planar GBR 4:4:4 8 bits */
#define CODEC_GBR_PLANAR      FOURCC('G','B','R','8')
#define CODEC_GBR_PLANAR_9B   FOURCC('G','B','9','B')
#define CODEC_GBR_PLANAR_9L   FOURCC('G','B','9','L')
#define CODEC_GBR_PLANAR_10B  FOURCC('G','B','A','B')
#define CODEC_GBR_PLANAR_10L  FOURCC('G','B','A','L')
#define CODEC_GBR_PLANAR_12B  FOURCC('G','B','B','B')
#define CODEC_GBR_PLANAR_12L  FOURCC('G','B','B','L')
#define CODEC_GBR_PLANAR_14B  FOURCC('G','B','D','B')
#define CODEC_GBR_PLANAR_14L  FOURCC('G','B','D','L')
#define CODEC_GBR_PLANAR_16L  FOURCC('G','B','F','L')
#define CODEC_GBR_PLANAR_16B  FOURCC('G','B','F','B')
#define CODEC_GBRA_PLANAR     FOURCC('G','B','0','8')
#define CODEC_GBRA_PLANAR_10B FOURCC('G','B','0','B')
#define CODEC_GBRA_PLANAR_10L FOURCC('G','B','0','L')
#define CODEC_GBRA_PLANAR_12B FOURCC('G','B','C','B')
#define CODEC_GBRA_PLANAR_12L FOURCC('G','B','C','L')
#define CODEC_GBRA_PLANAR_16L FOURCC('G','B','E','L')
#define CODEC_GBRA_PLANAR_16B FOURCC('G','B','E','B')

/* 8 bits grey */
#define CODEC_GREY            FOURCC('G','R','E','Y')
/* 10 bits grey */
#define CODEC_GREY_10L        FOURCC('G','0','F','L')
#define CODEC_GREY_10B        FOURCC('G','0','F','B')
/* 12 bits grey */
#define CODEC_GREY_12L        FOURCC('G','2','F','L')
#define CODEC_GREY_12B        FOURCC('G','2','F','B')
/* 16 bits grey */
#define CODEC_GREY_16L        FOURCC('G','R','F','L')
#define CODEC_GREY_16B        FOURCC('G','R','F','B')

/* VDPAU video surface YCbCr */
#define CODEC_VDPAU_VIDEO     FOURCC('V','D','V','Y')
/* VDPAU output surface RGBA */
#define CODEC_VDPAU_OUTPUT    FOURCC('V','D','O','R')

/* VAAPI opaque surface */
#define CODEC_VAAPI_420 FOURCC('V','A','O','P') /* 4:2:0  8 bpc */
#define CODEC_VAAPI_420_10BPP FOURCC('V','A','O','0') /* 4:2:0 10 bpc */

/* MediaCodec/IOMX opaque buffer type */
#define CODEC_ANDROID_OPAQUE  FOURCC('A','N','O','P')

/* Broadcom MMAL opaque buffer type */
#define CODEC_MMAL_OPAQUE     FOURCC('M','M','A','L')

/* DXVA2 opaque video surface for use with D3D9 */
#define CODEC_D3D9_OPAQUE     FOURCC('D','X','A','9') /* 4:2:0  8 bpc */
#define CODEC_D3D9_OPAQUE_10B FOURCC('D','X','A','0') /* 4:2:0 10 bpc */

/* D3D11VA opaque video surface for use with D3D11 */
#define CODEC_D3D11_OPAQUE          FOURCC('D','X','1','1') /* 4:2:0  8 bpc */
#define CODEC_D3D11_OPAQUE_10B      FOURCC('D','X','1','0') /* 4:2:0 10 bpc */
#define CODEC_D3D11_OPAQUE_RGBA     FOURCC('D','X','R','G')
#define CODEC_D3D11_OPAQUE_BGRA     FOURCC('D','A','G','R')
#define CODEC_D3D11_OPAQUE_ALPHA    FOURCC('D','A','1','1') /* 4:2:0  8 bpc with extra alpha plane */

/* NVDEC opaque video format for use the NVDec API */
#define CODEC_NVDEC_OPAQUE          FOURCC('N','V','D','8') /* 4:2:0  8 bpc */
#define CODEC_NVDEC_OPAQUE_10B      FOURCC('N','V','D','0') /* 4:2:0 10 bpc */
#define CODEC_NVDEC_OPAQUE_16B      FOURCC('N','V','D','6') /* 4:2:0 16 bpc */
#define CODEC_NVDEC_OPAQUE_444      FOURCC('N','V','4','8') /* 4:4:4  8 bpc */
#define CODEC_NVDEC_OPAQUE_444_16B  FOURCC('N','V','4','6') /* 4:4:4 16 bpc */

/* CVPixelBuffer opaque buffer type */
#define CODEC_CVPX_NV12       FOURCC('C','V','P','N')
#define CODEC_CVPX_UYVY       FOURCC('C','V','P','Y')
#define CODEC_CVPX_I420       FOURCC('C','V','P','I')
#define CODEC_CVPX_BGRA       FOURCC('C','V','P','B')
#define CODEC_CVPX_P010       FOURCC('C','V','P','P')

/* GStreamer Memory opaque buffer type */
#define CODEC_GST_MEM_OPAQUE  FOURCC('G','S','T','M')

/* Image codec (video) */
#define CODEC_PNG             FOURCC('p','n','g',' ')
#define CODEC_PPM             FOURCC('p','p','m',' ')
#define CODEC_PGM             FOURCC('p','g','m',' ')
#define CODEC_PGMYUV          FOURCC('p','g','m','y')
#define CODEC_PAM             FOURCC('p','a','m',' ')
#define CODEC_JPEG            FOURCC('j','p','e','g')
#define CODEC_BPG             FOURCC('B','P','G',0xFB)
#define CODEC_JPEGLS          FOURCC('M','J','L','S')
#define CODEC_BMP             FOURCC('b','m','p',' ')
#define CODEC_TIFF            FOURCC('t','i','f','f')
#define CODEC_GIF             FOURCC('g','i','f',' ')
#define CODEC_TARGA           FOURCC('t','g','a',' ')
#define CODEC_SVG             FOURCC('s','v','g',' ')
#define CODEC_SGI             FOURCC('s','g','i',' ')
#define CODEC_PNM             FOURCC('p','n','m',' ')
#define CODEC_PCX             FOURCC('p','c','x',' ')
#define CODEC_XWD             FOURCC('X','W','D',' ')
#define CODEC_TXD             FOURCC('T','X','D',' ')
#define CODEC_WEBP            FOURCC('W','E','B','P')


/* Audio codec */
#define CODEC_MPGA                       FOURCC('m','p','g','a')
#define CODEC_MP4A                       FOURCC('m','p','4','a')
#define CODEC_MPEGH                      FOURCC('M','H','A','S')
#define CODEC_ALS                        FOURCC('a','l','s',' ')
#define CODEC_A52                        FOURCC('a','5','2',' ')
#define CODEC_EAC3                       FOURCC('e','a','c','3')
#define CODEC_AC4                        FOURCC('a','c','-','4')
#define CODEC_DTS                        FOURCC('d','t','s',' ')
/* Only used by outputs and filters */
#define CODEC_DTSHD                      FOURCC('d','t','s','h')
#define CODEC_WMA1                       FOURCC('W','M','A','1')
#define CODEC_WMA2                       FOURCC('W','M','A','2')
#define CODEC_WMAP                       FOURCC('W','M','A','P')
#define CODEC_WMAL                       FOURCC('W','M','A','L')
#define CODEC_WMAS                       FOURCC('W','M','A','S')
#define CODEC_FLAC                       FOURCC('f','l','a','c')
#define CODEC_MLP                        FOURCC('m','l','p',' ')
#define CODEC_TRUEHD                     FOURCC('m','l','p','a')
#define CODEC_DVAUDIO                    FOURCC('d','v','a','u')
#define CODEC_SPEEX                      FOURCC('s','p','x',' ')
#define CODEC_CODEC2                     FOURCC('C','0','D','E')
#define CODEC_OPUS                       FOURCC('O','p','u','s')
#define CODEC_VORBIS                     FOURCC('v','o','r','b')
#define CODEC_MACE3                      FOURCC('M','A','C','3')
#define CODEC_MACE6                      FOURCC('M','A','C','6')
#define CODEC_MUSEPACK7                  FOURCC('M','P','C',' ')
#define CODEC_MUSEPACK8                  FOURCC('M','P','C','K')
#define CODEC_RA_144                     FOURCC('1','4','_','4')
#define CODEC_RA_288                     FOURCC('2','8','_','8')
#define CODEC_INTERPLAY_DPCM             FOURCC('i','d','p','c')
#define CODEC_ROQ_DPCM                   FOURCC('R','o','Q','a')
#define CODEC_DSICINAUDIO                FOURCC('D','C','I','A')
#define CODEC_ADPCM_4XM                  FOURCC('4','x','m','a')
#define CODEC_ADPCM_EA                   FOURCC('A','D','E','A')
#define CODEC_ADPCM_XA                   FOURCC('x','a',' ',' ')
#define CODEC_ADPCM_ADX                  FOURCC('a','d','x',' ')
#define CODEC_ADPCM_IMA_WS               FOURCC('A','I','W','S')
#define CODEC_ADPCM_G722                 FOURCC('g','7','2','2')
#define CODEC_ADPCM_G726                 FOURCC('g','7','2','6')
#define CODEC_ADPCM_G726_LE              FOURCC('g','7','2','s')
#define CODEC_ADPCM_SWF                  FOURCC('S','W','F','a')
#define CODEC_ADPCM_MS                   FOURCC('m','s',0x00,0x02)
#define CODEC_ADPCM_IMA_WAV              FOURCC('m','s',0x00,0x11)
#define CODEC_ADPCM_IMA_AMV              FOURCC('i','m','a','v')
#define CODEC_ADPCM_IMA_QT               FOURCC('i','m','a','4')
#define CODEC_ADPCM_YAMAHA               FOURCC('m','s',0x00,0x20)
#define CODEC_ADPCM_DK3                  FOURCC('m','s',0x00,0x62)
#define CODEC_ADPCM_DK4                  FOURCC('m','s',0x00,0x61)
#define CODEC_ADPCM_CREATIVE             FOURCC('m','s',0x00,0xC0)
#define CODEC_ADPCM_SBPRO_2              FOURCC('m','s',0x00,0xC2)
#define CODEC_ADPCM_SBPRO_3              FOURCC('m','s',0x00,0xC3)
#define CODEC_ADPCM_SBPRO_4              FOURCC('m','s',0x00,0xC4)
#define CODEC_ADPCM_THP                  FOURCC('T','H','P','A')
#define CODEC_ADPCM_XA_EA                FOURCC('X','A','J', 0)
#define CODEC_G723_1                     FOURCC('g','7','2', 0x31)
#define CODEC_G729                       FOURCC('g','7','2','9')
#define CODEC_VMDAUDIO                   FOURCC('v','m','d','a')
#define CODEC_AMR_NB                     FOURCC('s','a','m','r')
#define CODEC_AMR_WB                     FOURCC('s','a','w','b')
#define CODEC_ALAC                       FOURCC('a','l','a','c')
#define CODEC_QDM2                       FOURCC('Q','D','M','2')
#define CODEC_QDMC                       FOURCC('Q','D','M','C')
#define CODEC_COOK                       FOURCC('c','o','o','k')
#define CODEC_SIPR                       FOURCC('s','i','p','r')
#define CODEC_TTA                        FOURCC('T','T','A','1')
#define CODEC_SHORTEN                    FOURCC('s','h','n',' ')
#define CODEC_WAVPACK                    FOURCC('W','V','P','K')
#define CODEC_GSM                        FOURCC('g','s','m',' ')
#define CODEC_GSM_MS                     FOURCC('a','g','s','m')
#define CODEC_ATRAC1                     FOURCC('a','t','r','1')
#define CODEC_ATRAC3                     FOURCC('a','t','r','c')
#define CODEC_ATRAC3P                    FOURCC('a','t','r','p')
#define CODEC_IMC                        FOURCC(0x1,0x4,0x0,0x0)
#define CODEC_TRUESPEECH                 FOURCC(0x22,0x0,0x0,0x0)
#define CODEC_NELLYMOSER                 FOURCC('N','E','L','L')
#define CODEC_APE                        FOURCC('A','P','E',' ')
#define CODEC_QCELP                      FOURCC('Q','c','l','p')
#define CODEC_302M                       FOURCC('3','0','2','m')
#define CODEC_DVD_LPCM                   FOURCC('l','p','c','m')
#define CODEC_DVDA_LPCM                  FOURCC('a','p','c','m')
#define CODEC_BD_LPCM                    FOURCC('b','p','c','m')
#define CODEC_WIDI_LPCM                  FOURCC('w','p','c','m')
#define CODEC_SDDS                       FOURCC('s','d','d','s')
#define CODEC_MIDI                       FOURCC('M','I','D','I')
#define CODEC_RALF                       FOURCC('R','A','L','F')

#define CODEC_S8                         FOURCC('s','8',' ',' ')
#define CODEC_U8                         FOURCC('u','8',' ',' ')
#define CODEC_S16L                       FOURCC('s','1','6','l')
#define CODEC_S16L_PLANAR                FOURCC('s','1','l','p')
#define CODEC_S16B                       FOURCC('s','1','6','b')
#define CODEC_U16L                       FOURCC('u','1','6','l')
#define CODEC_U16B                       FOURCC('u','1','6','b')
#define CODEC_S20B                       FOURCC('s','2','0','b')
#define CODEC_S24L                       FOURCC('s','2','4','l')
#define CODEC_S24B                       FOURCC('s','2','4','b')
#define CODEC_U24L                       FOURCC('u','2','4','l')
#define CODEC_U24B                       FOURCC('u','2','4','b')
#define CODEC_S24L32                     FOURCC('s','2','4','4')
#define CODEC_S24B32                     FOURCC('S','2','4','4')
#define CODEC_S32L                       FOURCC('s','3','2','l')
#define CODEC_S32B                       FOURCC('s','3','2','b')
#define CODEC_U32L                       FOURCC('u','3','2','l')
#define CODEC_U32B                       FOURCC('u','3','2','b')
#define CODEC_F32L                       FOURCC('f','3','2','l')
#define CODEC_F32B                       FOURCC('f','3','2','b')
#define CODEC_F64L                       FOURCC('f','6','4','l')
#define CODEC_F64B                       FOURCC('f','6','4','b')

#define CODEC_ALAW                       FOURCC('a','l','a','w')
#define CODEC_MULAW                      FOURCC('m','l','a','w')
#define CODEC_DAT12                      FOURCC('L','P','1','2')
#define CODEC_S24DAUD                    FOURCC('d','a','u','d')
#define CODEC_TWINVQ                     FOURCC('T','W','I','N')
#define CODEC_BMVAUDIO                   FOURCC('B','M','V','A')
#define CODEC_ULEAD_DV_AUDIO_NTSC        FOURCC('m','s',0x02,0x15)
#define CODEC_ULEAD_DV_AUDIO_PAL         FOURCC('m','s',0x02,0x16)
#define CODEC_INDEO_AUDIO                FOURCC('m','s',0x04,0x02)
#define CODEC_METASOUND                  FOURCC('m','s',0x00,0x75)
#define CODEC_ON2AVC                     FOURCC('m','s',0x05,0x00)
#define CODEC_TAK                        FOURCC('t','a','k',' ')
#define CODEC_SMACKAUDIO                 FOURCC('S','M','K','A')
#define CODEC_ADPCM_IMA_EA_SEAD          FOURCC('S','E','A','D')
#define CODEC_ADPCM_EA_R1                FOURCC('E','A','R','1')
#define CODEC_ADPCM_EA_R3                FOURCC('E','A','R','3')
#define CODEC_ADPCM_IMA_APC              FOURCC('A','I','P','C')
#define CODEC_DSD_LSBF                   FOURCC('D','S','D','l')
#define CODEC_DSD_LSBF_PLANAR            FOURCC('D','S','F','l')
#define CODEC_DSD_MSBF                   FOURCC('D','S','D',' ')
#define CODEC_DSD_MSBF_PLANAR            FOURCC('D','S','F','m')

/* Subtitle */
#define CODEC_SPU       FOURCC('s','p','u',' ')
#define CODEC_DVBS      FOURCC('d','v','b','s')
#define CODEC_SUBT      FOURCC('s','u','b','t')
#define CODEC_XSUB      FOURCC('X','S','U','B')
#define CODEC_SSA       FOURCC('s','s','a',' ')
#define CODEC_TEXT      FOURCC('T','E','X','T')
#define CODEC_TELETEXT  FOURCC('t','e','l','x')
#define CODEC_KATE      FOURCC('k','a','t','e')
#define CODEC_CMML      FOURCC('c','m','m','l')
#define CODEC_ITU_T140  FOURCC('t','1','4','0')
#define CODEC_USF       FOURCC('u','s','f',' ')
#define CODEC_OGT       FOURCC('o','g','t',' ')
#define CODEC_CVD       FOURCC('c','v','d',' ')
#define CODEC_QTXT      FOURCC('q','t','x','t')
#define CODEC_TX3G      FOURCC('t','x','3','g')
#define CODEC_ARIB_A    FOURCC('a','r','b','a')
#define CODEC_ARIB_C    FOURCC('a','r','b','c')
/* Blu-ray Presentation Graphics */
#define CODEC_BD_PG     FOURCC('b','d','p','g')
#define CODEC_BD_TEXT   FOURCC('b','d','t','x')
/* EBU STL (TECH. 3264-E) */
#define CODEC_EBU_STL   FOURCC('S','T','L',' ')
#define CODEC_SCTE_18   FOURCC('S','C','1','8')
#define CODEC_SCTE_27   FOURCC('S','C','2','7')
/* EIA/CEA-608/708 */
#define CODEC_CEA608    FOURCC('c','6','0','8')
#define CODEC_CEA708    FOURCC('c','7','0','8')
#define CODEC_TTML      FOURCC('s','t','p','p')
#define CODEC_TTML_TS   FOURCC('s','t','p','P') /* special for EN.303.560 */
#define CODEC_WEBVTT    FOURCC('w','v','t','t')

/* XYZ colorspace 12 bits packed in 16 bits, organisation |XXX0|YYY0|ZZZ0| */
#define CODEC_XYZ_12L   FOURCC('X','Y','c','L')
#define CODEC_XYZ_12B   FOURCC('X','Y','c','B')


/* Special endian dependent values
 * The suffic N means Native
 * The suffix I means Inverted (ie non native) */
#ifdef WORDS_BIGENDIAN
#   define CODEC_S16N CODEC_S16B
#   define CODEC_U16N CODEC_U16B
#   define CODEC_S24N CODEC_S24B
#   define CODEC_U24N CODEC_U24B
#   define CODEC_S32N CODEC_S32B
#   define CODEC_U32N CODEC_U32B
#   define CODEC_FL32 CODEC_F32B
#   define CODEC_FL64 CODEC_F64B

#   define CODEC_S16I CODEC_S16L
#   define CODEC_U16I CODEC_U16L
#   define CODEC_S24I CODEC_S24L
#   define CODEC_U24I CODEC_U24L
#   define CODEC_S32I CODEC_S32L
#   define CODEC_U32I CODEC_U32L

#else
#   define CODEC_S16N CODEC_S16L
#   define CODEC_U16N CODEC_U16L
#   define CODEC_S24N CODEC_S24L
#   define CODEC_U24N CODEC_U24L
#   define CODEC_S32N CODEC_S32L
#   define CODEC_U32N CODEC_U32L
#   define CODEC_FL32 CODEC_F32L
#   define CODEC_FL64 CODEC_F64L

#   define CODEC_S16I CODEC_S16B
#   define CODEC_U16I CODEC_U16B
#   define CODEC_S24I CODEC_S24B
#   define CODEC_U24I CODEC_U24B
#   define CODEC_S32I CODEC_S32B
#   define CODEC_U32I CODEC_U32B
#endif

 /* Non official codecs, used to force a profile in an encoder */
 /* MPEG-1 video */
#define CODEC_MP1V      FOURCC('m','p','1','v')
/* MPEG-2 video */
#define CODEC_MP2V      FOURCC('m','p','2','v')
/* MPEG-I/II layer 2 audio */
#define CODEC_MP2       FOURCC('m','p','2',' ')
/* MPEG-I/II layer 3 audio */
#define CODEC_MP3       FOURCC('m','p','3',' ')

#define PLANAR(n, w_den, h_den, bits) \
  n, \
  {  {{1,  1}, {1,  1}}, \
    {{1,w_den}, {1,h_den}}, \
    {{1,w_den}, {1,h_den}}, \
    {{1,  1}, {1,  1}} }, \
  ((bits + 7) / 8), \
  bits


#define SEMIPLANAR(w_den, h_den, bits) \
     2, \
     { {1,  1}, {1,  1}, \
      {2,w_den}, {1,h_den} }, \
     ((bits + 7) / 8), \
     bits

#define PACKED_FMT(size, bits) \
     1, \
     { {1,1}, {1,1} }, \
     size, \
     bits

#define FAKE_FMT() \
     0, \
     { {1,1}, {1,1} }, \
     0, \
     0

#define PLANAR_8(n, w_den, h_den)        PLANAR(n, w_den, h_den, 8)
#define PLANAR_16(n, w_den, h_den, bits) PLANAR(n, w_den, h_den, bits)