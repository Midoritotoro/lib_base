//#include <QApplication>
//#include <QWidget>

#include <base/core/AtomicInteger.h>
#include <base/core/Thread.h>

#include <stdio.h>
#include <corecrt.h>

//#include <base/qt/style/StyleCore.h>
//#include <base/qt/common/MainQueueProcessor.h>

#include <base/io/File.h>
#include <thread>

//#include <base/qt/ui/FlatLabel.h>

#include <base/utility/CommonMacros.h>
#include <memory>

/*****************************************************************************
 * vlc_fourcc.h: Definition of various FOURCC and helpers
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

typedef struct
{
    bool klass;
    char fourcc[5];
    const char* description;
} staticentry_t;

struct entry
{
    char fourcc[5];
    char alias[5];
    const char* desc;
};

#define VLC_FOURCC(a,b,c,d) { a, b, c, d }
#define VLC_CODEC_UNKNOWN         VLC_FOURCC('u','n','d','f')

 /* Video codec */
#define VLC_CODEC_MPGV            VLC_FOURCC('m','p','g','v')
#define VLC_CODEC_MP4V            VLC_FOURCC('m','p','4','v')
#define VLC_CODEC_DIV1            VLC_FOURCC('D','I','V','1')
#define VLC_CODEC_DIV2            VLC_FOURCC('D','I','V','2')
#define VLC_CODEC_DIV3            VLC_FOURCC('D','I','V','3')
#define VLC_CODEC_SVQ1            VLC_FOURCC('S','V','Q','1')
#define VLC_CODEC_SVQ3            VLC_FOURCC('S','V','Q','3')
#define VLC_CODEC_H264            VLC_FOURCC('h','2','6','4')
#define VLC_CODEC_H263            VLC_FOURCC('h','2','6','3')
#define VLC_CODEC_H263I           VLC_FOURCC('I','2','6','3')
#define VLC_CODEC_H263P           VLC_FOURCC('I','L','V','R')
#define VLC_CODEC_FLV1            VLC_FOURCC('F','L','V','1')
#define VLC_CODEC_H261            VLC_FOURCC('h','2','6','1')
#define VLC_CODEC_VVC             VLC_FOURCC('h','2','6','6')
#define VLC_CODEC_MJPG            VLC_FOURCC('M','J','P','G')
#define VLC_CODEC_MJPGB           VLC_FOURCC('m','j','p','b')
#define VLC_CODEC_LJPG            VLC_FOURCC('L','J','P','G')
#define VLC_CODEC_WMV1            VLC_FOURCC('W','M','V','1')
#define VLC_CODEC_WMV2            VLC_FOURCC('W','M','V','2')
#define VLC_CODEC_WMV3            VLC_FOURCC('W','M','V','3')
#define VLC_CODEC_WMVA            VLC_FOURCC('W','M','V','A')
#define VLC_CODEC_WMVP            VLC_FOURCC('W','M','V','P')
#define VLC_CODEC_WMVP2           VLC_FOURCC('W','V','P','2')
#define VLC_CODEC_VC1             VLC_FOURCC('V','C','-','1')
#define VLC_CODEC_DAALA           VLC_FOURCC('d','a','a','l')
#define VLC_CODEC_THEORA          VLC_FOURCC('t','h','e','o')
#define VLC_CODEC_TARKIN          VLC_FOURCC('t','a','r','k')
#define VLC_CODEC_DIRAC           VLC_FOURCC('d','r','a','c')
#define VLC_CODEC_OGGSPOTS        VLC_FOURCC('S','P','O','T')
#define VLC_CODEC_CAVS            VLC_FOURCC('C','A','V','S')
#define VLC_CODEC_CAVS2           VLC_FOURCC('A','V','S','2')
#define VLC_CODEC_CAVS3           VLC_FOURCC('A','V','S','3')
#define VLC_CODEC_NUV             VLC_FOURCC('N','J','P','G')
#define VLC_CODEC_RV10            VLC_FOURCC('R','V','1','0')
#define VLC_CODEC_RV13            VLC_FOURCC('R','V','1','3')
#define VLC_CODEC_RV20            VLC_FOURCC('R','V','2','0')
#define VLC_CODEC_RV30            VLC_FOURCC('R','V','3','0')
#define VLC_CODEC_RV40            VLC_FOURCC('R','V','4','0')
#define VLC_CODEC_VP3             VLC_FOURCC('V','P','3',' ')
#define VLC_CODEC_VP5             VLC_FOURCC('V','P','5',' ')
#define VLC_CODEC_VP6             VLC_FOURCC('V','P','6','2')
#define VLC_CODEC_VP6F            VLC_FOURCC('V','P','6','F')
#define VLC_CODEC_VP6A            VLC_FOURCC('V','P','6','A')
#define VLC_CODEC_MSVIDEO1        VLC_FOURCC('M','S','V','C')
#define VLC_CODEC_FLIC            VLC_FOURCC('F','L','I','C')
#define VLC_CODEC_SP5X            VLC_FOURCC('S','P','5','X')
#define VLC_CODEC_DV              VLC_FOURCC('d','v',' ',' ')
#define VLC_CODEC_MSRLE           VLC_FOURCC('m','r','l','e')
#define VLC_CODEC_HUFFYUV         VLC_FOURCC('H','F','Y','U')
#define VLC_CODEC_FFVHUFF         VLC_FOURCC('F','F','V','H')
#define VLC_CODEC_ASV1            VLC_FOURCC('A','S','V','1')
#define VLC_CODEC_ASV2            VLC_FOURCC('A','S','V','2')
#define VLC_CODEC_FFV1            VLC_FOURCC('F','F','V','1')
#define VLC_CODEC_VCR1            VLC_FOURCC('V','C','R','1')
#define VLC_CODEC_CLJR            VLC_FOURCC('C','L','J','R')
#define VLC_CODEC_RPZA            VLC_FOURCC('r','p','z','a')
#define VLC_CODEC_SMC             VLC_FOURCC('s','m','c',' ')
#define VLC_CODEC_CINEPAK         VLC_FOURCC('C','V','I','D')
#define VLC_CODEC_TSCC            VLC_FOURCC('T','S','C','C')
#define VLC_CODEC_CSCD            VLC_FOURCC('C','S','C','D')
#define VLC_CODEC_ZMBV            VLC_FOURCC('Z','M','B','V')
#define VLC_CODEC_VMNC            VLC_FOURCC('V','M','n','c')
#define VLC_CODEC_FMVC            VLC_FOURCC('F','M','V','C')
#define VLC_CODEC_FRAPS           VLC_FOURCC('F','P','S','1')
#define VLC_CODEC_TRUEMOTION1     VLC_FOURCC('D','U','C','K')
#define VLC_CODEC_TRUEMOTION2     VLC_FOURCC('T','M','2','0')
#define VLC_CODEC_QTRLE           VLC_FOURCC('r','l','e',' ')
#define VLC_CODEC_QDRAW           VLC_FOURCC('q','d','r','w')
#define VLC_CODEC_QOI             VLC_FOURCC('q','o','i','f')
#define VLC_CODEC_QPEG            VLC_FOURCC('Q','P','E','G')
#define VLC_CODEC_ULTI            VLC_FOURCC('U','L','T','I')
#define VLC_CODEC_VIXL            VLC_FOURCC('V','I','X','L')
#define VLC_CODEC_LOCO            VLC_FOURCC('L','O','C','O')
#define VLC_CODEC_WNV1            VLC_FOURCC('W','N','V','1')
#define VLC_CODEC_AASC            VLC_FOURCC('A','A','S','C')
#define VLC_CODEC_INDEO2          VLC_FOURCC('I','V','2','0')
#define VLC_CODEC_INDEO3          VLC_FOURCC('I','V','3','1')
#define VLC_CODEC_INDEO4          VLC_FOURCC('I','V','4','1')
#define VLC_CODEC_INDEO5          VLC_FOURCC('I','V','5','0')
#define VLC_CODEC_FLASHSV         VLC_FOURCC('F','S','V','1')
#define VLC_CODEC_KMVC            VLC_FOURCC('K','M','V','C')
#define VLC_CODEC_SMACKVIDEO      VLC_FOURCC('S','M','K','2')
#define VLC_CODEC_DNXHD           VLC_FOURCC('A','V','d','n')
#define VLC_CODEC_8BPS            VLC_FOURCC('8','B','P','S')
#define VLC_CODEC_MIMIC           VLC_FOURCC('M','L','2','O')
#define VLC_CODEC_INTERPLAY       VLC_FOURCC('i','m','v','e')
#define VLC_CODEC_IDCIN           VLC_FOURCC('I','D','C','I')
#define VLC_CODEC_4XM             VLC_FOURCC('4','X','M','V')
#define VLC_CODEC_ROQ             VLC_FOURCC('R','o','Q','v')
#define VLC_CODEC_MDEC            VLC_FOURCC('M','D','E','C')
#define VLC_CODEC_VMDVIDEO        VLC_FOURCC('V','M','D','V')
#define VLC_CODEC_CDG             VLC_FOURCC('C','D','G',' ')
#define VLC_CODEC_FRWU            VLC_FOURCC('F','R','W','U')
#define VLC_CODEC_AMV             VLC_FOURCC('A','M','V',' ')
#define VLC_CODEC_VP4             VLC_FOURCC('V','P','4','0')
#define VLC_CODEC_VP7             VLC_FOURCC('V','P','7','0')
#define VLC_CODEC_VP8             VLC_FOURCC('V','P','8','0')
#define VLC_CODEC_VP8ALPHA_ES     VLC_FOURCC('V','P','8','a')
#define VLC_CODEC_VP9             VLC_FOURCC('V','P','9','0')
#define VLC_CODEC_VP9ALPHA_ES     VLC_FOURCC('V','P','9','a')
#define VLC_CODEC_VP10            VLC_FOURCC('V','P',':','0')
#define VLC_CODEC_AV1             VLC_FOURCC('a','v','0','1')
#define VLC_CODEC_JPEG2000        VLC_FOURCC('J','P','2','K')
#define VLC_CODEC_LAGARITH        VLC_FOURCC('L','A','G','S')
#define VLC_CODEC_FLASHSV2        VLC_FOURCC('F','S','V','2')
#define VLC_CODEC_PRORES          VLC_FOURCC('a','p','c','n')
#define VLC_CODEC_MXPEG           VLC_FOURCC('M','X','P','G')
#define VLC_CODEC_CDXL            VLC_FOURCC('C','D','X','L')
#define VLC_CODEC_BMVVIDEO        VLC_FOURCC('B','M','V','V')
#define VLC_CODEC_UTVIDEO         VLC_FOURCC('U','L','R','A')
#define VLC_CODEC_VBLE            VLC_FOURCC('V','B','L','E')
#define VLC_CODEC_DXTORY          VLC_FOURCC('x','t','o','r')
#define VLC_CODEC_MSS1            VLC_FOURCC('M','S','S','1')
#define VLC_CODEC_MSS2            VLC_FOURCC('M','S','S','2')
#define VLC_CODEC_MSA1            VLC_FOURCC('M','S','A','1')
#define VLC_CODEC_TSC2            VLC_FOURCC('T','S','C','2')
#define VLC_CODEC_MTS2            VLC_FOURCC('M','T','S','2')
#define VLC_CODEC_HEVC            VLC_FOURCC('h','e','v','c')
#define VLC_CODEC_ICOD            VLC_FOURCC('i','c','o','d')
#define VLC_CODEC_G2M2            VLC_FOURCC('G','2','M','2')
#define VLC_CODEC_G2M3            VLC_FOURCC('G','2','M','3')
#define VLC_CODEC_G2M4            VLC_FOURCC('G','2','M','4')
#define VLC_CODEC_BINKVIDEO       VLC_FOURCC('B','I','K','f')
#define VLC_CODEC_BINKAUDIO_DCT   VLC_FOURCC('B','A','U','1')
#define VLC_CODEC_BINKAUDIO_RDFT  VLC_FOURCC('B','A','U','2')
#define VLC_CODEC_XAN_WC4         VLC_FOURCC('X','x','a','n')
#define VLC_CODEC_LCL_MSZH        VLC_FOURCC('M','S','Z','H')
#define VLC_CODEC_LCL_ZLIB        VLC_FOURCC('Z','L','I','B')
#define VLC_CODEC_THP             VLC_FOURCC('T','H','P','0')
#define VLC_CODEC_ESCAPE124       VLC_FOURCC('E','1','2','4')
#define VLC_CODEC_KGV1            VLC_FOURCC('K','G','V','1')
#define VLC_CODEC_CLLC            VLC_FOURCC('C','L','L','C')
#define VLC_CODEC_AURA            VLC_FOURCC('A','U','R','A')
#define VLC_CODEC_FIC             VLC_FOURCC('F','I','C','V')
#define VLC_CODEC_TMV             VLC_FOURCC('T','M','A','V')
#define VLC_CODEC_XAN_WC3         VLC_FOURCC('X','A','N','3')
#define VLC_CODEC_WS_VQA          VLC_FOURCC('W','V','Q','A')
#define VLC_CODEC_MMVIDEO         VLC_FOURCC('M','M','V','I')
#define VLC_CODEC_AVS             VLC_FOURCC('A','V','S','V')
#define VLC_CODEC_DSICINVIDEO     VLC_FOURCC('D','C','I','V')
#define VLC_CODEC_TIERTEXSEQVIDEO VLC_FOURCC('T','S','E','Q')
#define VLC_CODEC_DXA             VLC_FOURCC('D','E','X','A')
#define VLC_CODEC_C93             VLC_FOURCC('I','C','9','3')
#define VLC_CODEC_BETHSOFTVID     VLC_FOURCC('B','V','I','D')
#define VLC_CODEC_VB              VLC_FOURCC('V','B','V','1')
#define VLC_CODEC_RL2             VLC_FOURCC('R','L','V','2')
#define VLC_CODEC_BFI             VLC_FOURCC('B','F','&','I')
#define VLC_CODEC_CMV             VLC_FOURCC('E','C','M','V')
#define VLC_CODEC_MOTIONPIXELS    VLC_FOURCC('M','P','I','X')
#define VLC_CODEC_TGV             VLC_FOURCC('T','G','V','V')
#define VLC_CODEC_TGQ             VLC_FOURCC('T','G','Q','V')
#define VLC_CODEC_TQI             VLC_FOURCC('T','Q','I','V')
#define VLC_CODEC_MAD             VLC_FOURCC('M','A','D','V')
#define VLC_CODEC_ANM             VLC_FOURCC('A','N','I','M')
#define VLC_CODEC_YOP             VLC_FOURCC('Y','O','P','V')
#define VLC_CODEC_JV              VLC_FOURCC('J','V','0','0')
#define VLC_CODEC_DFA             VLC_FOURCC('D','F','I','A')
#define VLC_CODEC_HNM4_VIDEO      VLC_FOURCC('H','N','M','4')
#define VLC_CODEC_TDSC            VLC_FOURCC('T','D','S','C')
#define VLC_CODEC_HQX             VLC_FOURCC('C','H','Q','X')
#define VLC_CODEC_HQ_HQA          VLC_FOURCC('C','U','V','C')
#define VLC_CODEC_HAP             VLC_FOURCC('H','A','P','1')
#define VLC_CODEC_DXV             VLC_FOURCC('D','X','D','3')
#define VLC_CODEC_CINEFORM        VLC_FOURCC('C','F','H','D')
#define VLC_CODEC_SPEEDHQ         VLC_FOURCC('S','H','Q','2')
#define VLC_CODEC_PIXLET          VLC_FOURCC('p','x','l','t')
#define VLC_CODEC_MAGICYUV        VLC_FOURCC('M','8','Y','0')
#define VLC_CODEC_IMM4            VLC_FOURCC('I','M','M','4')
#define VLC_CODEC_IMM5            VLC_FOURCC('I','M','M','5')
#define VLC_CODEC_AGM             VLC_FOURCC('A','G','M','0')
#define VLC_CODEC_NOTCHLC         VLC_FOURCC('n','c','l','c')
#define VLC_CODEC_RTP_VIDEO_RAW   VLC_FOURCC('R','T','P','V')

/***********
 * Chromas
 ***********/

 /* Planar YUV */

 /* Planar YUV 4:1:0 Y:U:V */
#define VLC_CODEC_I410            VLC_FOURCC('I','4','1','0')
/* Planar YUV 4:1:1 Y:U:V */
#define VLC_CODEC_I411            VLC_FOURCC('I','4','1','1')

/* Planar YUV 4:2:0 Y:V:U */
#define VLC_CODEC_YV12            VLC_FOURCC('Y','V','1','2')
/* Planar YUV 4:2:0 Y:U:V 8-bit */
#define VLC_CODEC_I420            VLC_FOURCC('I','4','2','0')
/* Planar YUV 4:2:0 Y:U:V  9-bit stored on 16 bits */
#define VLC_CODEC_I420_9L         VLC_FOURCC('I','0','9','L')
#define VLC_CODEC_I420_9B         VLC_FOURCC('I','0','9','B')
/* Planar YUV 4:2:0 Y:U:V 10-bit stored on 16 bits LSB */
#define VLC_CODEC_I420_10L        VLC_FOURCC('I','0','A','L')
#define VLC_CODEC_I420_10B        VLC_FOURCC('I','0','A','B')
/* Planar YUV 4:2:0 Y:U:V 12-bit stored on 16 bits LSB */
#define VLC_CODEC_I420_12L        VLC_FOURCC('I','0','C','L')
#define VLC_CODEC_I420_12B        VLC_FOURCC('I','0','C','B')

/* Planar YUV 4:2:0 Y:U:V 16-bit stored on 16 bits */
#define VLC_CODEC_I420_16L        VLC_FOURCC('I','0','F','L')
#define VLC_CODEC_I420_16B        VLC_FOURCC('I','0','F','B')

/* Planar YUV 4:2:2 Y:U:V 8-bit */
#define VLC_CODEC_I422            VLC_FOURCC('I','4','2','2')
/* Planar YUV 4:2:2 Y:U:V  9-bit stored on 16 bits LSB */
#define VLC_CODEC_I422_9L         VLC_FOURCC('I','2','9','L')
#define VLC_CODEC_I422_9B         VLC_FOURCC('I','2','9','B')
/* Planar YUV 4:2:2 Y:U:V 10-bit stored on 16 bits LSB */
#define VLC_CODEC_I422_10L        VLC_FOURCC('I','2','A','L')
#define VLC_CODEC_I422_10B        VLC_FOURCC('I','2','A','B')
/* Planar YUV 4:2:2 Y:U:V 12-bit stored on 16 bits LSB */
#define VLC_CODEC_I422_12L        VLC_FOURCC('I','2','C','L')
#define VLC_CODEC_I422_12B        VLC_FOURCC('I','2','C','B')
/* Planar YUV 4:2:2 Y:U:V 16-bit stored on 16 bits */
#define VLC_CODEC_I422_16L        VLC_FOURCC('I','2','F','L')
#define VLC_CODEC_I422_16B        VLC_FOURCC('I','2','F','B')

/* Planar YUV 4:4:0 Y:U:V */
#define VLC_CODEC_I440            VLC_FOURCC('I','4','4','0')
/* Planar YUV 4:4:4 Y:U:V 8-bit */
#define VLC_CODEC_I444            VLC_FOURCC('I','4','4','4')
/* Planar YUV 4:4:4 Y:U:V  9-bit stored on 16 bits LSB */
#define VLC_CODEC_I444_9L         VLC_FOURCC('I','4','9','L')
#define VLC_CODEC_I444_9B         VLC_FOURCC('I','4','9','B')
/* Planar YUV 4:4:4 Y:U:V 10-bit stored on 16 bits LSB */
#define VLC_CODEC_I444_10L        VLC_FOURCC('I','4','A','L')
#define VLC_CODEC_I444_10B        VLC_FOURCC('I','4','A','B')
/* Planar YUV 4:4:4 Y:U:V 12-bit stored on 16 bits LSB */
#define VLC_CODEC_I444_12L        VLC_FOURCC('I','4','C','L')
#define VLC_CODEC_I444_12B        VLC_FOURCC('I','4','C','B')
/* Planar YUV 4:4:4 Y:U:V 16-bit */
#define VLC_CODEC_I444_16L        VLC_FOURCC('I','4','F','L')
#define VLC_CODEC_I444_16B        VLC_FOURCC('I','4','F','B')

/* Palettized YUV with palette 8-bit Y:U:V:A in memory order */
#define VLC_CODEC_YUVP            VLC_FOURCC('Y','U','V','P')

/* Planar YUV 4:4:4 Y:U:V:A */
#define VLC_CODEC_YUVA            VLC_FOURCC('Y','U','V','A')
/* Planar YUV 4:2:2 Y:U:V:A */
#define VLC_CODEC_YUV422A         VLC_FOURCC('I','4','2','A')
/* Planar YUV 4:2:0 Y:U:V:A */
#define VLC_CODEC_YUV420A         VLC_FOURCC('I','4','0','A')

/* Planar Y:U:V:A 4:4:4 10bits */
#define VLC_CODEC_YUVA_444_10L    VLC_FOURCC('Y','A','0','L')
#define VLC_CODEC_YUVA_444_10B    VLC_FOURCC('Y','A','0','B')

/* Planar Y:U:V:A 4:4:4 12bits */
#define VLC_CODEC_YUVA_444_12L    VLC_FOURCC('Y','A','2','L')
#define VLC_CODEC_YUVA_444_12B    VLC_FOURCC('Y','A','2','B')

/* Semi-planar Y/UV */

/* 2 planes Y/UV 4:2:0 */
#define VLC_CODEC_NV12            VLC_FOURCC('N','V','1','2')
/* 2 planes Y/VU 4:2:0 */
#define VLC_CODEC_NV21            VLC_FOURCC('N','V','2','1')
/* 2 planes Y/UV 4:2:2 */
#define VLC_CODEC_NV16            VLC_FOURCC('N','V','1','6')
/* 2 planes Y/VU 4:2:2 */
#define VLC_CODEC_NV61            VLC_FOURCC('N','V','6','1')
/* 2 planes Y/UV 4:4:4 */
#define VLC_CODEC_NV24            VLC_FOURCC('N','V','2','4')
/* 2 planes Y/VU 4:4:4 */
#define VLC_CODEC_NV42            VLC_FOURCC('N','V','4','2')
/* 2 planes Y/UV 4:2:0 10-bit MSB, little endian */
#define VLC_CODEC_P010            VLC_FOURCC('P','0','1','0')
/* 2 planes Y/UV 4:2:0 12-bit MSB, little endian */
#define VLC_CODEC_P012            VLC_FOURCC('P','0','1','2')
/* 2 planes Y/UV 4:2:0 16-bit, little endian */
#define VLC_CODEC_P016            VLC_FOURCC('P','0','1','6')

/* Packed YUV */

/* Packed YUV 4:2:0, U:V:Y */
#define VLC_CODEC_YUV4            VLC_FOURCC('y','u','v','4')
/* Packed YUV 4:2:2, U:Y:V:Y */
#define VLC_CODEC_UYVY            VLC_FOURCC('U','Y','V','Y')
/* Packed YUV 4:2:2, V:Y:U:Y */
#define VLC_CODEC_VYUY            VLC_FOURCC('V','Y','U','Y')
/* Packed YUV 4:2:2, Y:U:Y:V */
#define VLC_CODEC_YUYV            VLC_FOURCC('Y','U','Y','2')
/* Packed YUV 4:2:2, Y:U:Y:V, signed */
#define VLC_CODEC_YUV2            VLC_FOURCC('y','u','v','2')
/* Packed YUV 4:2:2, Y:V:Y:U */
#define VLC_CODEC_YVYU            VLC_FOURCC('Y','V','Y','U')
/* Packed YUV 2:1:1, Y:U:Y:V */
#define VLC_CODEC_Y211            VLC_FOURCC('Y','2','1','1')
/* Packed YUV 4:2:2, U:Y:V:Y, reverted */
#define VLC_CODEC_CYUV            VLC_FOURCC('c','y','u','v')
/* Packed YUV 4:2:2 10-bit U10:Y10:V10:Y10:X2 (12 on 4*32bits) */
#define VLC_CODEC_V210            VLC_FOURCC('v','2','1','0')
/* Packed YUV 4:4:4 */
#define VLC_CODEC_V308            VLC_FOURCC('v','3','0','8')
/* Packed YUVA 4:4:4:4 */
#define VLC_CODEC_V408            VLC_FOURCC('v','4','0','8')
/* Packed YUV 4:4:4 10-bit X2:U10:Y10:V10:Y10 */
#define VLC_CODEC_V410            VLC_FOURCC('v','4','1','0')
/* I420 packed for RTP (RFC 4175) */
#define VLC_CODEC_R420            VLC_FOURCC('r','4','2','0')
/* Packed YUV 4:2:2 10-bit V10:U10:Y10:A2 */
#define VLC_CODEC_Y210            VLC_FOURCC('Y','2','1','0')
/* Packed YUV 4:4:4 10-bit V10:U10:Y10:A2 */
#define VLC_CODEC_Y410            VLC_FOURCC('Y','4','1','0')
/* Packed YUV 4:4:4 V:U:Y:A */
#define VLC_CODEC_VUYA            VLC_FOURCC('V','U','Y','A')

/* RGB / RGBA */

/* Palettized 8-bit RGB with palette element 8-bit R:G:B:A in memory order */
#define VLC_CODEC_RGBP            VLC_FOURCC('R','G','B','P')

/* 32-bit RGB, in memory address order: "XRGB" ignoring the x component */
#define VLC_CODEC_XRGB            VLC_FOURCC('R','V','3','2')
/* 32-bit RGB, in memory address order: "RGBX" ignoring the x component */
#define VLC_CODEC_RGBX            VLC_FOURCC('R','G','B','X')
/* 32-bit BGR, in memory address order: "BGRX" ignoring the x component */
#define VLC_CODEC_BGRX            VLC_FOURCC('B','G','R','X')
/* 32-bit BGR, in memory address order: "XBGR" ignoring the x component */
#define VLC_CODEC_XBGR            VLC_FOURCC('X','B','G','R')

/* 24-bit RGB, in memory address order: "RGB" */
#define VLC_CODEC_RGB24           VLC_FOURCC('R','V','2','4')
/* 24-bit BGR, in memory address order: "BGR" */
#define VLC_CODEC_BGR24           VLC_FOURCC('B','G','R','3')

/* 16-bit RGB Big-Endian: "R5G6B5" */
#define VLC_CODEC_RGB565BE          VLC_FOURCC('R','B','1','6')
/* 16-bit RGB Little-Endian: "g3B5 R5G3" */
#define VLC_CODEC_RGB565LE          VLC_FOURCC('R','V','1','6')
/* 16-bit BGR Big-Endian: "B5G6R5" */
#define VLC_CODEC_BGR565BE          VLC_FOURCC('B','B','1','6')
/* 16-bit BGR Little-Endian: "g3R5 B5G3" */
#define VLC_CODEC_BGR565LE          VLC_FOURCC('B','L','1','6')

#ifdef WORDS_BIGENDIAN
/* 16-bit RGB with host endianness */
#define VLC_CODEC_RGB565    VLC_CODEC_RGB565BE
/* 16-bit BGR with host endianness */
#define VLC_CODEC_BGR565    VLC_CODEC_BGR565BE
#else
/* 16-bit RGB with host endianness */
#define VLC_CODEC_RGB565    VLC_CODEC_RGB565LE
/* 16-bit BGR with host endianness */
#define VLC_CODEC_BGR565    VLC_CODEC_BGR565LE
#endif

/* 15-bit RGB Big-Endian: "0R5G5B5" */
#define VLC_CODEC_RGB555BE          VLC_FOURCC('R','B','1','5')
/* 15-bit RGB Little-Endian: "g3B5 0R5G2" */
#define VLC_CODEC_RGB555LE          VLC_FOURCC('R','V','1','5')
/* 15-bit BGR Big-Endian: "0B5G5R5" */
#define VLC_CODEC_BGR555BE          VLC_FOURCC('B','B','1','5')
/* 15-bit BGR Little-Endian: "g3R5 0B5G2" */
#define VLC_CODEC_BGR555LE          VLC_FOURCC('B','L','1','5')

#ifdef WORDS_BIGENDIAN
/* 15-bit RGB with host endianness */
#define VLC_CODEC_RGB555    VLC_CODEC_RGB555BE
/* 15-bit BGR with host endianness */
#define VLC_CODEC_BGR555    VLC_CODEC_BGR555BE
#else
/* 15-bit RGB with host endianness */
#define VLC_CODEC_RGB555    VLC_CODEC_RGB555LE
/* 15-bit BGR with host endianness */
#define VLC_CODEC_BGR555    VLC_CODEC_BGR555LE
#endif


/* 8-bit RGB "R3G3B2" */
#define VLC_CODEC_RGB332           VLC_FOURCC('R','3','3','2')
/* 8-bit RGB "R2G3B3" */
#define VLC_CODEC_RGB233           VLC_FOURCC('R','G','B','8')
/* 8-bit BGR "B2G3R3" */
#define VLC_CODEC_BGR233           VLC_FOURCC('B','2','3','3')

/* 32-bit RGBA, in memory address order: "RGBA" */
#define VLC_CODEC_RGBA            VLC_FOURCC('R','G','B','A')
/* 32-bit ARGB, in memory address order: "ARGB" */
#define VLC_CODEC_ARGB            VLC_FOURCC('A','R','G','B')
/* 32-bit ARGB, in memory address order: "BGRA" */
#define VLC_CODEC_BGRA            VLC_FOURCC('B','G','R','A')
/* 32-bit RGBA, in memory address order: "ABGR" */
#define VLC_CODEC_ABGR            VLC_FOURCC('A','B','G','R')
/* 32 bits Little-Endian RGBA with 10-bit colors "A2B6 b4G4 g6R2 r8" */
#define VLC_CODEC_RGBA10LE        VLC_FOURCC('R','G','A','0')
/* 64 bits RGBA */
#define VLC_CODEC_RGBA64          VLC_FOURCC('R','G','A','4')

/* Planar GBR 4:4:4 8 bits */
#define VLC_CODEC_GBR_PLANAR      VLC_FOURCC('G','B','R','8')
#define VLC_CODEC_GBR_PLANAR_9B   VLC_FOURCC('G','B','9','B')
#define VLC_CODEC_GBR_PLANAR_9L   VLC_FOURCC('G','B','9','L')
#define VLC_CODEC_GBR_PLANAR_10B  VLC_FOURCC('G','B','A','B')
#define VLC_CODEC_GBR_PLANAR_10L  VLC_FOURCC('G','B','A','L')
#define VLC_CODEC_GBR_PLANAR_12B  VLC_FOURCC('G','B','B','B')
#define VLC_CODEC_GBR_PLANAR_12L  VLC_FOURCC('G','B','B','L')
#define VLC_CODEC_GBR_PLANAR_14B  VLC_FOURCC('G','B','D','B')
#define VLC_CODEC_GBR_PLANAR_14L  VLC_FOURCC('G','B','D','L')
#define VLC_CODEC_GBR_PLANAR_16L  VLC_FOURCC('G','B','F','L')
#define VLC_CODEC_GBR_PLANAR_16B  VLC_FOURCC('G','B','F','B')
#define VLC_CODEC_GBRA_PLANAR     VLC_FOURCC('G','B','0','8')
#define VLC_CODEC_GBRA_PLANAR_10B VLC_FOURCC('G','B','0','B')
#define VLC_CODEC_GBRA_PLANAR_10L VLC_FOURCC('G','B','0','L')
#define VLC_CODEC_GBRA_PLANAR_12B VLC_FOURCC('G','B','C','B')
#define VLC_CODEC_GBRA_PLANAR_12L VLC_FOURCC('G','B','C','L')
#define VLC_CODEC_GBRA_PLANAR_16L VLC_FOURCC('G','B','E','L')
#define VLC_CODEC_GBRA_PLANAR_16B VLC_FOURCC('G','B','E','B')

/* 8 bits grey */
#define VLC_CODEC_GREY            VLC_FOURCC('G','R','E','Y')
/* 10 bits grey */
#define VLC_CODEC_GREY_10L        VLC_FOURCC('G','0','F','L')
#define VLC_CODEC_GREY_10B        VLC_FOURCC('G','0','F','B')
/* 12 bits grey */
#define VLC_CODEC_GREY_12L        VLC_FOURCC('G','2','F','L')
#define VLC_CODEC_GREY_12B        VLC_FOURCC('G','2','F','B')
/* 16 bits grey */
#define VLC_CODEC_GREY_16L        VLC_FOURCC('G','R','F','L')
#define VLC_CODEC_GREY_16B        VLC_FOURCC('G','R','F','B')

/* VDPAU video surface YCbCr */
#define VLC_CODEC_VDPAU_VIDEO     VLC_FOURCC('V','D','V','Y')
/* VDPAU output surface RGBA */
#define VLC_CODEC_VDPAU_OUTPUT    VLC_FOURCC('V','D','O','R')

/* VAAPI opaque surface */
#define VLC_CODEC_VAAPI_420 VLC_FOURCC('V','A','O','P') /* 4:2:0  8 bpc */
#define VLC_CODEC_VAAPI_420_10BPP VLC_FOURCC('V','A','O','0') /* 4:2:0 10 bpc */
#define VLC_CODEC_VAAPI_420_12BPP VLC_FOURCC('V','A','O','2') /* 4:2:0 12 bpc */

/* MediaCodec/IOMX opaque buffer type */
#define VLC_CODEC_ANDROID_OPAQUE  VLC_FOURCC('A','N','O','P')

/* Broadcom MMAL opaque buffer type */
#define VLC_CODEC_MMAL_OPAQUE     VLC_FOURCC('M','M','A','L')

/* DXVA2 opaque video surface for use with D3D9 */
#define VLC_CODEC_D3D9_OPAQUE     VLC_FOURCC('D','X','A','9') /* 4:2:0  8 bpc */
#define VLC_CODEC_D3D9_OPAQUE_10B VLC_FOURCC('D','X','A','0') /* 4:2:0 10 bpc */

/* D3D11VA opaque video surface for use with D3D11 */
#define VLC_CODEC_D3D11_OPAQUE          VLC_FOURCC('D','X','1','1') /* 4:2:0  8 bpc */
#define VLC_CODEC_D3D11_OPAQUE_10B      VLC_FOURCC('D','X','1','0') /* 4:2:0 10 bpc */
#define VLC_CODEC_D3D11_OPAQUE_RGBA     VLC_FOURCC('D','X','R','G')
#define VLC_CODEC_D3D11_OPAQUE_BGRA     VLC_FOURCC('D','A','G','R')
#define VLC_CODEC_D3D11_OPAQUE_ALPHA    VLC_FOURCC('D','A','1','1') /* 4:2:0  8 bpc with extra alpha plane */

/* NVDEC opaque video format for use the NVDec API */
#define VLC_CODEC_NVDEC_OPAQUE          VLC_FOURCC('N','V','D','8') /* 4:2:0  8 bpc */
#define VLC_CODEC_NVDEC_OPAQUE_10B      VLC_FOURCC('N','V','D','0') /* 4:2:0 10 bpc */
#define VLC_CODEC_NVDEC_OPAQUE_16B      VLC_FOURCC('N','V','D','6') /* 4:2:0 16 bpc */
#define VLC_CODEC_NVDEC_OPAQUE_444      VLC_FOURCC('N','V','4','8') /* 4:4:4  8 bpc */
#define VLC_CODEC_NVDEC_OPAQUE_444_16B  VLC_FOURCC('N','V','4','6') /* 4:4:4 16 bpc */

/* CVPixelBuffer opaque buffer type */
#define VLC_CODEC_CVPX_NV12       VLC_FOURCC('C','V','P','N')
#define VLC_CODEC_CVPX_UYVY       VLC_FOURCC('C','V','P','Y')
#define VLC_CODEC_CVPX_I420       VLC_FOURCC('C','V','P','I')
#define VLC_CODEC_CVPX_BGRA       VLC_FOURCC('C','V','P','B')
#define VLC_CODEC_CVPX_P010       VLC_FOURCC('C','V','P','P')

/* GStreamer Memory opaque buffer type */
#define VLC_CODEC_GST_MEM_OPAQUE  VLC_FOURCC('G','S','T','M')

/* Image codec (video) */
#define VLC_CODEC_PNG             VLC_FOURCC('p','n','g',' ')
#define VLC_CODEC_PPM             VLC_FOURCC('p','p','m',' ')
#define VLC_CODEC_PGM             VLC_FOURCC('p','g','m',' ')
#define VLC_CODEC_PGMYUV          VLC_FOURCC('p','g','m','y')
#define VLC_CODEC_PAM             VLC_FOURCC('p','a','m',' ')
#define VLC_CODEC_JPEG            VLC_FOURCC('j','p','e','g')
#define VLC_CODEC_BPG             VLC_FOURCC('B','P','G',0xFB)
#define VLC_CODEC_JPEGLS          VLC_FOURCC('M','J','L','S')
#define VLC_CODEC_BMP             VLC_FOURCC('b','m','p',' ')
#define VLC_CODEC_FARBFELD        VLC_FOURCC('f','a','r','b')
#define VLC_CODEC_TIFF            VLC_FOURCC('t','i','f','f')
#define VLC_CODEC_GIF             VLC_FOURCC('g','i','f',' ')
#define VLC_CODEC_TARGA           VLC_FOURCC('t','g','a',' ')
#define VLC_CODEC_SVG             VLC_FOURCC('s','v','g',' ')
#define VLC_CODEC_SGI             VLC_FOURCC('s','g','i',' ')
#define VLC_CODEC_PNM             VLC_FOURCC('p','n','m',' ')
#define VLC_CODEC_PCX             VLC_FOURCC('p','c','x',' ')
#define VLC_CODEC_XWD             VLC_FOURCC('X','W','D',' ')
#define VLC_CODEC_TXD             VLC_FOURCC('T','X','D',' ')
#define VLC_CODEC_WEBP            VLC_FOURCC('W','E','B','P')


/* Audio codec */
#define VLC_CODEC_MPGA                       VLC_FOURCC('m','p','g','a')
#define VLC_CODEC_MP4A                       VLC_FOURCC('m','p','4','a')
#define VLC_CODEC_MPEGH                      VLC_FOURCC('M','H','A','S')
#define VLC_CODEC_ALS                        VLC_FOURCC('a','l','s',' ')
#define VLC_CODEC_A52                        VLC_FOURCC('a','5','2',' ')
#define VLC_CODEC_EAC3                       VLC_FOURCC('e','a','c','3')
#define VLC_CODEC_AC4                        VLC_FOURCC('a','c','-','4')
#define VLC_CODEC_DTS                        VLC_FOURCC('d','t','s',' ')
/* Only used by outputs and filters */
#define VLC_CODEC_DTSHD                      VLC_FOURCC('d','t','s','h')
#define VLC_CODEC_WMA1                       VLC_FOURCC('W','M','A','1')
#define VLC_CODEC_WMA2                       VLC_FOURCC('W','M','A','2')
#define VLC_CODEC_WMAP                       VLC_FOURCC('W','M','A','P')
#define VLC_CODEC_WMAL                       VLC_FOURCC('W','M','A','L')
#define VLC_CODEC_WMAS                       VLC_FOURCC('W','M','A','S')
#define VLC_CODEC_FLAC                       VLC_FOURCC('f','l','a','c')
#define VLC_CODEC_MLP                        VLC_FOURCC('m','l','p',' ')
#define VLC_CODEC_TRUEHD                     VLC_FOURCC('m','l','p','a')
#define VLC_CODEC_DVAUDIO                    VLC_FOURCC('d','v','a','u')
#define VLC_CODEC_SPEEX                      VLC_FOURCC('s','p','x',' ')
#define VLC_CODEC_CODEC2                     VLC_FOURCC('C','0','D','E')
#define VLC_CODEC_OPUS                       VLC_FOURCC('O','p','u','s')
#define VLC_CODEC_VORBIS                     VLC_FOURCC('v','o','r','b')
#define VLC_CODEC_MACE3                      VLC_FOURCC('M','A','C','3')
#define VLC_CODEC_MACE6                      VLC_FOURCC('M','A','C','6')
#define VLC_CODEC_MUSEPACK7                  VLC_FOURCC('M','P','C',' ')
#define VLC_CODEC_MUSEPACK8                  VLC_FOURCC('M','P','C','K')
#define VLC_CODEC_RA_144                     VLC_FOURCC('1','4','_','4')
#define VLC_CODEC_RA_288                     VLC_FOURCC('2','8','_','8')
#define VLC_CODEC_INTERPLAY_DPCM             VLC_FOURCC('i','d','p','c')
#define VLC_CODEC_ROQ_DPCM                   VLC_FOURCC('R','o','Q','a')
#define VLC_CODEC_DSICINAUDIO                VLC_FOURCC('D','C','I','A')
#define VLC_CODEC_ADPCM_4XM                  VLC_FOURCC('4','x','m','a')
#define VLC_CODEC_ADPCM_EA                   VLC_FOURCC('A','D','E','A')
#define VLC_CODEC_ADPCM_XA                   VLC_FOURCC('x','a',' ',' ')
#define VLC_CODEC_ADPCM_ADX                  VLC_FOURCC('a','d','x',' ')
#define VLC_CODEC_ADPCM_IMA_WS               VLC_FOURCC('A','I','W','S')
#define VLC_CODEC_ADPCM_G722                 VLC_FOURCC('g','7','2','2')
#define VLC_CODEC_ADPCM_G726                 VLC_FOURCC('g','7','2','6')
#define VLC_CODEC_ADPCM_G726_LE              VLC_FOURCC('g','7','2','s')
#define VLC_CODEC_ADPCM_SWF                  VLC_FOURCC('S','W','F','a')
#define VLC_CODEC_ADPCM_MS                   VLC_FOURCC('m','s',0x00,0x02)
#define VLC_CODEC_ADPCM_IMA_WAV              VLC_FOURCC('m','s',0x00,0x11)
#define VLC_CODEC_ADPCM_IMA_AMV              VLC_FOURCC('i','m','a','v')
#define VLC_CODEC_ADPCM_IMA_QT               VLC_FOURCC('i','m','a','4')
#define VLC_CODEC_ADPCM_YAMAHA               VLC_FOURCC('m','s',0x00,0x20)
#define VLC_CODEC_ADPCM_DK3                  VLC_FOURCC('m','s',0x00,0x62)
#define VLC_CODEC_ADPCM_DK4                  VLC_FOURCC('m','s',0x00,0x61)
#define VLC_CODEC_ADPCM_CREATIVE             VLC_FOURCC('m','s',0x00,0xC0)
#define VLC_CODEC_ADPCM_SBPRO_2              VLC_FOURCC('m','s',0x00,0xC2)
#define VLC_CODEC_ADPCM_SBPRO_3              VLC_FOURCC('m','s',0x00,0xC3)
#define VLC_CODEC_ADPCM_SBPRO_4              VLC_FOURCC('m','s',0x00,0xC4)
#define VLC_CODEC_ADPCM_THP                  VLC_FOURCC('T','H','P','A')
#define VLC_CODEC_ADPCM_XA_EA                VLC_FOURCC('X','A','J', 0)
#define VLC_CODEC_G723_1                     VLC_FOURCC('g','7','2', 0x31)
#define VLC_CODEC_G729                       VLC_FOURCC('g','7','2','9')
#define VLC_CODEC_VMDAUDIO                   VLC_FOURCC('v','m','d','a')
#define VLC_CODEC_AMR_NB                     VLC_FOURCC('s','a','m','r')
#define VLC_CODEC_AMR_WB                     VLC_FOURCC('s','a','w','b')
#define VLC_CODEC_ALAC                       VLC_FOURCC('a','l','a','c')
#define VLC_CODEC_QDM2                       VLC_FOURCC('Q','D','M','2')
#define VLC_CODEC_QDMC                       VLC_FOURCC('Q','D','M','C')
#define VLC_CODEC_QOA                        VLC_FOURCC('q','o','a','f')
#define VLC_CODEC_COOK                       VLC_FOURCC('c','o','o','k')
#define VLC_CODEC_SIPR                       VLC_FOURCC('s','i','p','r')
#define VLC_CODEC_TTA                        VLC_FOURCC('T','T','A','1')
#define VLC_CODEC_SHORTEN                    VLC_FOURCC('s','h','n',' ')
#define VLC_CODEC_WAVPACK                    VLC_FOURCC('W','V','P','K')
#define VLC_CODEC_GSM                        VLC_FOURCC('g','s','m',' ')
#define VLC_CODEC_GSM_MS                     VLC_FOURCC('a','g','s','m')
#define VLC_CODEC_ATRAC1                     VLC_FOURCC('a','t','r','1')
#define VLC_CODEC_ATRAC3                     VLC_FOURCC('a','t','r','c')
#define VLC_CODEC_ATRAC3P                    VLC_FOURCC('a','t','r','p')
#define VLC_CODEC_IMC                        VLC_FOURCC(0x1,0x4,0x0,0x0)
#define VLC_CODEC_TRUESPEECH                 VLC_FOURCC(0x22,0x0,0x0,0x0)
#define VLC_CODEC_NELLYMOSER                 VLC_FOURCC('N','E','L','L')
#define VLC_CODEC_APE                        VLC_FOURCC('A','P','E',' ')
#define VLC_CODEC_QCELP                      VLC_FOURCC('Q','c','l','p')
#define VLC_CODEC_302M                       VLC_FOURCC('3','0','2','m')
#define VLC_CODEC_DVD_LPCM                   VLC_FOURCC('l','p','c','m')
#define VLC_CODEC_DVDA_LPCM                  VLC_FOURCC('a','p','c','m')
#define VLC_CODEC_BD_LPCM                    VLC_FOURCC('b','p','c','m')
#define VLC_CODEC_WIDI_LPCM                  VLC_FOURCC('w','p','c','m')
#define VLC_CODEC_SDDS                       VLC_FOURCC('s','d','d','s')
#define VLC_CODEC_MIDI                       VLC_FOURCC('M','I','D','I')
#define VLC_CODEC_RALF                       VLC_FOURCC('R','A','L','F')

#define VLC_CODEC_S8                         VLC_FOURCC('s','8',' ',' ')
#define VLC_CODEC_U8                         VLC_FOURCC('u','8',' ',' ')
#define VLC_CODEC_S16L                       VLC_FOURCC('s','1','6','l')
#define VLC_CODEC_S16L_PLANAR                VLC_FOURCC('s','1','l','p')
#define VLC_CODEC_S16B                       VLC_FOURCC('s','1','6','b')
#define VLC_CODEC_U16L                       VLC_FOURCC('u','1','6','l')
#define VLC_CODEC_U16B                       VLC_FOURCC('u','1','6','b')
#define VLC_CODEC_S20B                       VLC_FOURCC('s','2','0','b')
#define VLC_CODEC_S24L                       VLC_FOURCC('s','2','4','l')
#define VLC_CODEC_S24B                       VLC_FOURCC('s','2','4','b')
#define VLC_CODEC_U24L                       VLC_FOURCC('u','2','4','l')
#define VLC_CODEC_U24B                       VLC_FOURCC('u','2','4','b')
#define VLC_CODEC_S24L32                     VLC_FOURCC('s','2','4','4')
#define VLC_CODEC_S24B32                     VLC_FOURCC('S','2','4','4')
#define VLC_CODEC_S32L                       VLC_FOURCC('s','3','2','l')
#define VLC_CODEC_S32B                       VLC_FOURCC('s','3','2','b')
#define VLC_CODEC_U32L                       VLC_FOURCC('u','3','2','l')
#define VLC_CODEC_U32B                       VLC_FOURCC('u','3','2','b')
#define VLC_CODEC_F32L                       VLC_FOURCC('f','3','2','l')
#define VLC_CODEC_F32B                       VLC_FOURCC('f','3','2','b')
#define VLC_CODEC_F64L                       VLC_FOURCC('f','6','4','l')
#define VLC_CODEC_F64B                       VLC_FOURCC('f','6','4','b')

#define VLC_CODEC_ALAW                       VLC_FOURCC('a','l','a','w')
#define VLC_CODEC_MULAW                      VLC_FOURCC('m','l','a','w')
#define VLC_CODEC_DAT12                      VLC_FOURCC('L','P','1','2')
#define VLC_CODEC_S24DAUD                    VLC_FOURCC('d','a','u','d')
#define VLC_CODEC_TWINVQ                     VLC_FOURCC('T','W','I','N')
#define VLC_CODEC_BMVAUDIO                   VLC_FOURCC('B','M','V','A')
#define VLC_CODEC_ULEAD_DV_AUDIO_NTSC        VLC_FOURCC('m','s',0x02,0x15)
#define VLC_CODEC_ULEAD_DV_AUDIO_PAL         VLC_FOURCC('m','s',0x02,0x16)
#define VLC_CODEC_INDEO_AUDIO                VLC_FOURCC('m','s',0x04,0x02)
#define VLC_CODEC_METASOUND                  VLC_FOURCC('m','s',0x00,0x75)
#define VLC_CODEC_ON2AVC                     VLC_FOURCC('m','s',0x05,0x00)
#define VLC_CODEC_TAK                        VLC_FOURCC('t','a','k',' ')
#define VLC_CODEC_SMACKAUDIO                 VLC_FOURCC('S','M','K','A')
#define VLC_CODEC_ADPCM_IMA_EA_SEAD          VLC_FOURCC('S','E','A','D')
#define VLC_CODEC_ADPCM_EA_R1                VLC_FOURCC('E','A','R','1')
#define VLC_CODEC_ADPCM_EA_R3                VLC_FOURCC('E','A','R','3')
#define VLC_CODEC_ADPCM_IMA_APC              VLC_FOURCC('A','I','P','C')
#define VLC_CODEC_DSD_LSBF                   VLC_FOURCC('D','S','D','l')
#define VLC_CODEC_DSD_LSBF_PLANAR            VLC_FOURCC('D','S','F','l')
#define VLC_CODEC_DSD_MSBF                   VLC_FOURCC('D','S','D',' ')
#define VLC_CODEC_DSD_MSBF_PLANAR            VLC_FOURCC('D','S','F','m')

/* Subtitle */
#define VLC_CODEC_SPU       VLC_FOURCC('s','p','u',' ')
#define VLC_CODEC_DVBS      VLC_FOURCC('d','v','b','s')
#define VLC_CODEC_SUBT      VLC_FOURCC('s','u','b','t')
#define VLC_CODEC_XSUB      VLC_FOURCC('X','S','U','B')
#define VLC_CODEC_SSA       VLC_FOURCC('s','s','a',' ')
#define VLC_CODEC_TEXT      VLC_FOURCC('T','E','X','T')
#define VLC_CODEC_TELETEXT  VLC_FOURCC('t','e','l','x')
#define VLC_CODEC_KATE      VLC_FOURCC('k','a','t','e')
#define VLC_CODEC_CMML      VLC_FOURCC('c','m','m','l')
#define VLC_CODEC_ITU_T140  VLC_FOURCC('t','1','4','0')
#define VLC_CODEC_USF       VLC_FOURCC('u','s','f',' ')
#define VLC_CODEC_OGT       VLC_FOURCC('o','g','t',' ')
#define VLC_CODEC_CVD       VLC_FOURCC('c','v','d',' ')
#define VLC_CODEC_QTXT      VLC_FOURCC('q','t','x','t')
#define VLC_CODEC_TX3G      VLC_FOURCC('t','x','3','g')
#define VLC_CODEC_ARIB_A    VLC_FOURCC('a','r','b','a')
#define VLC_CODEC_ARIB_C    VLC_FOURCC('a','r','b','c')
/* Blu-ray Presentation Graphics */
#define VLC_CODEC_BD_PG     VLC_FOURCC('b','d','p','g')
#define VLC_CODEC_BD_TEXT   VLC_FOURCC('b','d','t','x')
/* EBU STL (TECH. 3264-E) */
#define VLC_CODEC_EBU_STL   VLC_FOURCC('S','T','L',' ')
#define VLC_CODEC_SCTE_18   VLC_FOURCC('S','C','1','8')
#define VLC_CODEC_SCTE_27   VLC_FOURCC('S','C','2','7')
/* EIA/CEA-608/708 */
#define VLC_CODEC_CEA608    VLC_FOURCC('c','6','0','8')
#define VLC_CODEC_CEA708    VLC_FOURCC('c','7','0','8')
#define VLC_CODEC_TTML      VLC_FOURCC('s','t','p','p')
#define VLC_CODEC_TTML_TS   VLC_FOURCC('s','t','p','P') /* special for EN.303.560 */
#define VLC_CODEC_WEBVTT    VLC_FOURCC('w','v','t','t')

/* XYZ colorspace 12 bits packed in 16 bits, organisation |XXX0|YYY0|ZZZ0| */
#define VLC_CODEC_XYZ_12L   VLC_FOURCC('X','Y','c','L')
#define VLC_CODEC_XYZ_12B   VLC_FOURCC('X','Y','c','B')


/* Special endian dependent values
 * The suffic N means Native
 * The suffix I means Inverted (ie non native) */
#ifdef WORDS_BIGENDIAN
#   define VLC_CODEC_S16N VLC_CODEC_S16B
#   define VLC_CODEC_U16N VLC_CODEC_U16B
#   define VLC_CODEC_S24N VLC_CODEC_S24B
#   define VLC_CODEC_U24N VLC_CODEC_U24B
#   define VLC_CODEC_S32N VLC_CODEC_S32B
#   define VLC_CODEC_U32N VLC_CODEC_U32B
#   define VLC_CODEC_FL32 VLC_CODEC_F32B
#   define VLC_CODEC_FL64 VLC_CODEC_F64B

#   define VLC_CODEC_S16I VLC_CODEC_S16L
#   define VLC_CODEC_U16I VLC_CODEC_U16L
#   define VLC_CODEC_S24I VLC_CODEC_S24L
#   define VLC_CODEC_U24I VLC_CODEC_U24L
#   define VLC_CODEC_S32I VLC_CODEC_S32L
#   define VLC_CODEC_U32I VLC_CODEC_U32L

#else
#   define VLC_CODEC_S16N VLC_CODEC_S16L
#   define VLC_CODEC_U16N VLC_CODEC_U16L
#   define VLC_CODEC_S24N VLC_CODEC_S24L
#   define VLC_CODEC_U24N VLC_CODEC_U24L
#   define VLC_CODEC_S32N VLC_CODEC_S32L
#   define VLC_CODEC_U32N VLC_CODEC_U32L
#   define VLC_CODEC_FL32 VLC_CODEC_F32L
#   define VLC_CODEC_FL64 VLC_CODEC_F64L

#   define VLC_CODEC_S16I VLC_CODEC_S16B
#   define VLC_CODEC_U16I VLC_CODEC_U16B
#   define VLC_CODEC_S24I VLC_CODEC_S24B
#   define VLC_CODEC_U24I VLC_CODEC_U24B
#   define VLC_CODEC_S32I VLC_CODEC_S32B
#   define VLC_CODEC_U32I VLC_CODEC_U32B
#endif
 /* MPEG-1 video */
#define VLC_CODEC_MP1V      VLC_FOURCC('m','p','1','v')
/* MPEG-2 video */
#define VLC_CODEC_MP2V      VLC_FOURCC('m','p','2','v')
/* MPEG-I/II layer 2 audio */
#define VLC_CODEC_MP2       VLC_FOURCC('m','p','2',' ')
/* MPEG-I/II layer 3 audio */
#define VLC_CODEC_MP3       VLC_FOURCC('m','p','3',' ')


#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



#define VLC_API
#define VLC_USED
typedef uint32_t vlc_fourcc_t;
typedef struct { unsigned num, den; } vlc_rational_t;

#define B(fcc,dsc) { true, fcc, dsc }
#define E(sfcc,dsc) { false, sfcc, dsc }

#define A(sfcc) E(sfcc, NULL)


static const staticentry_t p_list_video[] = {

    B(VLC_CODEC_MP1V, "MPEG-1 Video"),
        A("mp1v"),
        A("m1v "),
        A("mpg1"),
        A("BW10"),
        E("XMPG", "Xing MPEG-1 Intra"),

    B(VLC_CODEC_MPGV, "MPEG-1/2 Video"),
    B(VLC_CODEC_MP2V, "MPEG-2 Video"),
        A("mpeg"),
        A("mp2v"),
        A("MPEG"),
        A("mpg2"),
        A("MPG2"),
        A("H262"),

        E("PIM1", "Pinnacle DC1000 (MPEG-1 Video)"),

        E("hdv1", "HDV 720p30 (MPEG-2 Video)"),
        E("hdv2", "Sony HDV 1080i60 (MPEG-2 Video)"),
        E("hdv3", "FCP HDV 1080i50 (MPEG-2 Video)"),
        E("hdv4", "HDV 720p24 (MPEG-2 Video)"),
        E("hdv5", "HDV 720p25 (MPEG-2 Video)"),
        E("hdv6", "HDV 1080p24 (MPEG-2 Video)"),
        E("hdv7", "HDV 1080p25 (MPEG-2 Video)"),
        E("hdv8", "HDV 1080p30 (MPEG-2 Video)"),
        E("hdv9", "HDV 720p60 JVC (MPEG-2 Video)"),
        E("hdva", "HDV 720p50 (MPEG-2 Video)"),

        E("mx5n", "MPEG2 IMX NTSC 525/60 50Mb/s (FCP)"),
        E("mx5p", "MPEG2 IMX PAL 625/60 50Mb/s (FCP)"),
        E("mx4n", "MPEG2 IMX NTSC 525/60 40Mb/s (FCP)"),
        E("mx4p", "MPEG2 IMX PAL 625/50 40Mb/s (FCP)"),
        E("mx3n", "MPEG2 IMX NTSC 525/60 30Mb/s (FCP)"),
        E("mx3p", "MPEG2 IMX NTSC 625/50 30Mb/s (FCP)"),

        E("xdv1", "XDCAM HD 720p30 35Mb/s"),
        E("xdv2", "XDCAM HD 1080i60 35Mb/s"),
        E("xdv3", "XDCAM HD 1080i50 35Mb/s"),
        E("xdv4", "XDCAM HD 720p24 35Mb/s"),
        E("xdv5", "XDCAM HD 720p25 35Mb/s"),
        E("xdv6", "XDCAM HD 1080p24 35Mb/s"),
        E("xdv7", "XDCAM HD 1080p25 35Mb/s"),
        E("xdv8", "XDCAM HD 1080p30 35Mb/s"),
        E("xdv9", "XDCAM HD 720p60 35Mb/s"),
        E("xdva", "XDCAM HD 720p50 35Mb/s"),

        E("xdvb", "XDCAM EX 1080i60 50Mb/s CBR"),
        E("xdvc", "XDCAM EX 1080i50 50Mb/s CBR"),
        E("xdvd", "XDCAM EX 1080p24 50Mb/s CBR"),
        E("xdve", "XDCAM EX 1080p25 50Mb/s CBR"),
        E("xdvf", "XDCAM EX 1080p30 50Mb/s CBR"),

        E("xd51", "XDCAM HD422 720p30 50Mb/s CBR"),
        E("xd54", "XDCAM HD422 720p24 50Mb/s CBR"),
        E("xd55", "XDCAM HD422 720p25 50Mb/s CBR"),
        E("xd59", "XDCAM HD422 720p60 50Mb/s CBR"),
        E("xd5a", "XDCAM HD422 720p50 50Mb/s CBR"),
        E("xd5b", "XDCAM HD422 1080i60 50Mb/s CBR"),
        E("xd5c", "XDCAM HD422 1080i50 50Mb/s CBR"),
        E("xd5d", "XDCAM HD422 1080p24 50Mb/s CBR"),
        E("xd5e", "XDCAM HD422 1080p25 50Mb/s CBR"),
        E("xd5f", "XDCAM HD422 1080p30 50Mb/s CBR"),

        E("xdhd", "XDCAM HD 540p"),
        E("xdh2", "XDCAM HD422 540p"),

        E("AVmp", "AVID IMX PAL"),
        E("MMES", "Matrox MPEG-2"),
        E("mmes", "Matrox MPEG-2"),
        E("PIM2", "Pinnacle MPEG-2"),
        E("LMP2", "Lead MPEG-2"),

        E("VCR2", "ATI VCR-2"),

    B(VLC_CODEC_MP4V, "MPEG-4 Video"),
        A("mp4v"),
        A("DIVX"),
        A("divx"),
        A("MP4S"),
        A("mp4s"),
        A("M4S2"),
        A("m4s2"),
        A("MP4V"),
        A("\x04\x00\x00\x00"),
        A("m4cc"),
        A("M4CC"),
        A("FMP4"),
        A("fmp4"),
        A("DCOD"),
        A("MVXM"),
        A("PM4V"),
        A("M4T3"),
        A("GEOX"),
        A("GEOV"),
        A("DMK2"),
        A("WV1F"),
        A("DIGI"),
        A("INMC"),
        A("SN40"),
        A("EPHV"),
        A("DM4V"),
        A("SM4V"),
        A("DYM4"),
        /* XVID flavours */
        E("xvid", "Xvid MPEG-4 Video"),
        E("XVID", "Xvid MPEG-4 Video"),
        E("XviD", "Xvid MPEG-4 Video"),
        E("XVIX", "Xvid MPEG-4 Video"),
        E("xvix", "Xvid MPEG-4 Video"),
        /* DX50 */
        E("DX50", "DivX MPEG-4 Video"),
        E("dx50", "DivX MPEG-4 Video"),
        E("BLZ0", "Blizzard MPEG-4 Video"),
        E("DXGM", "Electronic Arts Game MPEG-4 Video"),
        E("DreX", "DreX Mpeg-4"),
        /* 3ivx delta 4 */
        E("3IV2", "3ivx MPEG-4 Video"),
        E("3iv2", "3ivx MPEG-4 Video"),
        /* Various */
        E("UMP4", "UB MPEG-4 Video"),
        E("SEDG", "Samsung MPEG-4 Video"),
        E("RMP4", "REALmagic MPEG-4 Video"),
        E("LMP4", "Lead MPEG-4 Video"),
        E("HDX4", "Jomigo HDX4 (MPEG-4 Video)"),
        E("hdx4", "Jomigo HDX4 (MPEG-4 Video)"),
        E("SMP4", "Samsung SMP4 (MPEG-4 Video)"),
        E("smp4", "Samsung SMP4 (MPEG-4 Video)"),
        E("fvfw", "libavcodec MPEG-4"),
        E("FVFW", "libavcodec MPEG-4"),
        E("FFDS", "FFDShow MPEG-4"),
        E("VIDM", "vidm 4.01 codec"),
        E("DP02", "DynaPel MPEG-4 codec"),
        E("PLV1", "Pelco DVR MPEG-4"),
        E("QMP4", "QNAP Systems MPEG-4"),
        E("qMP4", "QNAP Systems MPEG-4"),
        E("wMP4", "QNAP Systems MPEG-4"),
        E("G264", "GeoVision MPEG-4"),
        E("GM40", "GeoVision MPEG-4"),
        E("GMP4", "GeoVision MPEG-4"),
        /* 3ivx delta 3.5 Unsupported
         * putting it here gives extreme distorted images */
         //E("3IV1", "3ivx delta 3.5 MPEG-4 Video"),
         //E("3iv1", "3ivx delta 3.5 MPEG-4 Video"),

     /* MSMPEG4 v1 */
     B(VLC_CODEC_DIV1, "MS MPEG-4 Video v1"),
         A("DIV1"),
         A("div1"),
         A("MPG4"),
         A("mpg4"),
         A("mp41"),

         /* MSMPEG4 v2 */
         B(VLC_CODEC_DIV2, "MS MPEG-4 Video v2"),
             A("DIV2"),
             A("div2"),
             A("MP42"),
             A("mp42"),

             /* MSMPEG4 v3 / M$ mpeg4 v3 */
             B(VLC_CODEC_DIV3, "MS MPEG-4 Video v3"),
                 A("DIV3"),
                 A("MPG3"),
                 A("mpg3"),
                 A("div3"),
                 A("MP43"),
                 A("mp43"),
                 /* DivX 3.20 */
                 A("DIV4"),
                 A("div4"),
                 A("DIV5"),
                 A("div5"),
                 A("DIV6"),
                 A("div6"),
                 E("divf", "DivX 4.12"),
                 E("DIVF", "DivX 4.12"),
                 /* Cool Codec */
                 A("COL1"),
                 A("col1"),
                 A("COL0"),
                 A("col0"),
                 /* AngelPotion stuff */
                 A("AP41"),
                 /* 3ivx doctered divx files */
                 A("3IVD"),
                 A("3ivd"),
                 /* who knows? */
                 A("3VID"),
                 A("3vid"),
                 A("DVX1"),
                 A("DVX3"),

                 /* Sorenson v1 */
                 B(VLC_CODEC_SVQ1, "SVQ-1 (Sorenson Video v1)"),
                     A("SVQ1"),
                     A("svq1"),
                     A("svqi"),

                     /* Sorenson v3 */
                     B(VLC_CODEC_SVQ3, "SVQ-3 (Sorenson Video v3)"),
                         A("SVQ3"),

                         /* VVC / H.266 */
                         B(VLC_CODEC_VVC, "MPEG-I Part3/VVC (H.266)"),
                             A("vvc1"),

                             /* HEVC / H.265 */
                             B(VLC_CODEC_HEVC, "MPEG-H Part2/HEVC (H.265)"),
                                 A("hevc"),
                                 A("HEVC"),
                                 A("h265"),
                                 A("H265"),
                                 A("x265"),
                                 A("X265"),
                                 A("hev1"),
                                 A("hvc1"),
                                 A("HM10"),
                                 E("dvhe", "Dolby Vision HEVC (H.265)"),
                                 E("GHEC", "GeoVision HEVC"),
                                 /* E("dvh1", "Dolby Vision HEVC (H.265)"), Collides with DV */

                             /* h264 */
                             B(VLC_CODEC_H264, "H264 - MPEG-4 AVC (part 10)"),
                                 A("H264"),
                                 A("h264"),
                                 A("x264"),
                                 A("X264"),
                                 A("V264"),
                                 /* avc1: special case h264 */
                                 A("avc1"),
                                 A("AVC1"),
                                 A("AVCB"), /* smooth streaming alias */
                                 A("avc3"),
                                 E("ai5p", "AVC-Intra  50M 720p24/30/60"),
                                 E("ai5q", "AVC-Intra  50M 720p25/50"),
                                 E("ai52", "AVC-Intra  50M 1080p25/50"),
                                 E("ai53", "AVC-Intra  50M 1080p24/30/60"),
                                 E("ai55", "AVC-Intra  50M 1080i50"),
                                 E("ai56", "AVC-Intra  50M 1080i60"),
                                 E("ai1p", "AVC-Intra 100M 720p24/30/60"),
                                 E("ai1q", "AVC-Intra 100M 720p25/50"),
                                 E("ai12", "AVC-Intra 100M 1080p25/50"),
                                 E("ai13", "AVC-Intra 100M 1080p24/30/60"),
                                 E("ai15", "AVC-Intra 100M 1080i50"),
                                 E("ai16", "AVC-Intra 100M 1080i60"),
                                 E("dvav", "Dolby Vision H264"),
                                 E("dva1", "Dolby Vision H264"),
                                 E("VSSH", "Vanguard VSS H264"),
                                 E("VSSW", "Vanguard VSS H264"),
                                 E("vssh", "Vanguard VSS H264"),
                                 E("DAVC", "Dicas MPEGable H.264/MPEG-4 AVC"),
                                 E("davc", "Dicas MPEGable H.264/MPEG-4 AVC"),
                                 E("x3eV", "DreX H.264"),
                                 E("GAVC", "GeoVision MPEG-4 AVC"),
                                 E("Q264", "QNAP H.264/MPEG-4 AVC"),
                                 E("q264", "QNAP H.264/MPEG-4 AVC"),
                                 E("w264", "QNAP H.264/MPEG-4 AVC"),
                                 A("UMSV"),
                                 A("SMV2"),
                                 A("tshd"),
                                 A("rv64"),

                                 /* H263 and H263i */
                                 /* H263(+) is also known as Real Video 1.0 */

                                 /* H263 */
                                 B(VLC_CODEC_H263, "H263"),
                                     A("H263"),
                                     A("h263"),
                                     A("VX1K"),
                                     A("s263"),
                                     A("S263"),
                                     A("u263"),
                                     A("lsvm"),
                                     E("T263", "March H263"),
                                     E("D263", "DEC H263"),
                                     E("d263", "DEC H263"),
                                     E("L263", "LEAD H263"),
                                     E("M263", "Microsoft H263"),
                                     E("X263", "Xirlink H263"),
                                     /* Zygo (partial) */
                                     E("ZyGo", "ITU H263+"),
                                     A("HAVI"),

                                     /* H263i */
                                     B(VLC_CODEC_H263I, "I263.I"),
                                         A("I263"),
                                         A("i263"),

                                         /* H263P */
                                         B(VLC_CODEC_H263P, "ITU H263+"),
                                             E("ILVR", "ITU H263+"),
                                             E("viv1", "H263+"),
                                             E("vivO", "H263+"),
                                             E("viv2", "H263+"),
                                             E("VIVO", "H263+"),
                                             E("U263", "UB H263+"),

                                             /* Flash (H263) variant */
                                             B(VLC_CODEC_FLV1, "Flash Video"),
                                                 A("FLV1"),
                                                 A("flv "),

                                                 /* H261 */
                                                 B(VLC_CODEC_H261, "H.261"),
                                                     A("H261"),
                                                     A("h261"),

                                                 B(VLC_CODEC_FLIC, "Flic Video"),
                                                     A("FLIC"),
                                                     A("AFLC"),

                                                     /* MJPEG */
                                                     B(VLC_CODEC_MJPG, "Motion JPEG Video"),
                                                         A("MJPG"),
                                                         A("MJPx"),
                                                         A("mjpg"),
                                                         A("mJPG"),
                                                         A("mjpa"),
                                                         A("JFIF"),
                                                         A("JPGL"),
                                                         A("MMJP"),
                                                         A("FLJP"),
                                                         A("FMJP"),
                                                         A("SJPG"),
                                                         E("QIVG", "QNAP Motion JPEG"),
                                                         E("qIVG", "QNAP Motion JPEG"),
                                                         E("wIVG", "QNAP Motion JPEG"),
                                                         E("AVRn", "Avid Motion JPEG"),
                                                         E("AVDJ", "Avid Motion JPEG"),
                                                         E("ADJV", "Avid Motion JPEG"),
                                                         E("dmb1", "Motion JPEG OpenDML Video"),
                                                         E("DMB1", "Motion JPEG OpenDML Video"),
                                                         E("ijpg", "Intergraph JPEG Video"),
                                                         E("IJPG", "Intergraph JPEG Video"),
                                                         E("ACDV", "ACD Systems Digital"),
                                                         E("SLMJ", "SL M-JPEG"),

                                                     B(VLC_CODEC_MJPGB, "Motion JPEG B Video"),
                                                         A("mjpb"),

                                                     B(VLC_CODEC_LJPG, "Lead Motion JPEG Video"),
                                                         E("Ljpg", "Lead Motion JPEG"),

                                                         /* SP5x */
                                                         B(VLC_CODEC_SP5X, "Sunplus Motion JPEG Video"),
                                                             A("SP5X"),
                                                             A("SP53"),
                                                             A("SP54"),
                                                             A("SP55"),
                                                             A("SP56"),
                                                             A("SP57"),
                                                             A("SP58"),

                                                             /* DV */
                                                             B(VLC_CODEC_DV, "DV Video"),
                                                                 A("dv  "),
                                                                 A("dvsl"),
                                                                 A("DVSD"),
                                                                 A("dvsd"),
                                                                 A("DVCS"),
                                                                 A("dvcs"),
                                                                 A("dvhd"),
                                                                 A("dvhq"),
                                                                 A("dvh1"),
                                                                 E("dvh2", "DV Video 720p24"),
                                                                 E("dvh3", "DV Video 720p25"),
                                                                 E("dvh4", "DV Video 720p30"),
                                                                 A("dv25"),
                                                                 A("dc25"),
                                                                 A("dvs1"),
                                                                 A("dvis"),
                                                                 A("CDV2"),
                                                                 A("CDVP"),
                                                                 A("PDVC"),
                                                                 A("IPDV"),
                                                                 A("ipdv"),
                                                                 A("pdvc"),
                                                                 A("SL25"),
                                                                 E("dvcp", "DV Video PAL"),
                                                                 E("dvc ", "DV Video NTSC"),
                                                                 E("dvp ", "DV Video Pro"),
                                                                 E("dvpp", "DV Video Pro PAL"),
                                                                 E("dv50", "DV Video C Pro 50"),
                                                                 E("dv5p", "DV Video C Pro 50 PAL"),
                                                                 E("dv5n", "DV Video C Pro 50 NTSC"),
                                                                 E("dv1p", "DV Video C Pro 100 PAL"),
                                                                 E("dv1n", "DV Video C Pro 100 NTSC"),
                                                                 E("dvhp", "DV Video C Pro HD 720p"),
                                                                 E("dvh5", "DV Video C Pro HD 1080i50"),
                                                                 E("dvh6", "DV Video C Pro HD 1080i60"),
                                                                 E("AVdv", "AVID DV"),
                                                                 E("AVd1", "AVID DV"),
                                                                 E("CDVC", "Canopus DV Video"),
                                                                 E("cdvc", "Canopus DV Video"),
                                                                 E("CDVH", "Canopus DV Video"),
                                                                 E("cdvh", "Canopus DV Video"),
                                                                 E("CDV5", "Canopus DV Video"),
                                                                 E("SLDV", "SoftLab DVCAM codec"),

                                                                 /* Windows Media Video */
                                                                 B(VLC_CODEC_WMV1, "Windows Media Video 7"),
                                                                     A("WMV1"),
                                                                     A("wmv1"),

                                                                 B(VLC_CODEC_WMV2, "Windows Media Video 8"),
                                                                     A("WMV2"),
                                                                     A("wmv2"),
                                                                     A("GXVE"),

                                                                 B(VLC_CODEC_WMV3, "Windows Media Video 9"),
                                                                     A("WMV3"),
                                                                     A("wmv3"),

                                                                     /* WMVA is the VC-1 codec before the standardization process,
                                                                      * it is not bitstream compatible and deprecated  */
                                                                     B(VLC_CODEC_WMVA, "Windows Media Video Advanced Profile"),
                                                                         A("WMVA"),
                                                                         A("wmva"),

                                                                     B(VLC_CODEC_VC1, "Windows Media Video VC1"),
                                                                         A("WVC1"),
                                                                         A("wvc1"),
                                                                         A("vc-1"),
                                                                         A("VC-1"),

                                                                     B(VLC_CODEC_WMVP, "Windows Media Video Presentation"),
                                                                         A("WMVP"),
                                                                         A("wmvp"),

                                                                     B(VLC_CODEC_WMVP2, "Windows Media Video Presentation, v2"),
                                                                         A("WVP2"),
                                                                         A("wvp2"),

                                                                         /* Microsoft Video 1 */
                                                                         B(VLC_CODEC_MSVIDEO1, "Microsoft Video 1"),
                                                                             A("MSVC"),
                                                                             A("msvc"),
                                                                             A("CRAM"),
                                                                             A("cram"),
                                                                             A("WHAM"),
                                                                             A("wham"),

                                                                             /* Microsoft RLE */
                                                                             B(VLC_CODEC_MSRLE, "Microsoft RLE Video"),
                                                                                 A("mrle"),
                                                                                 A("WRLE"),
                                                                                 A("\x01\x00\x00\x00"),
                                                                                 A("\x02\x00\x00\x00"),

                                                                                 /* Indeo Video Codecs (Quality of this decoder on ppc is not good) */
                                                                                 B(VLC_CODEC_INDEO3, "Indeo Video v3"),
                                                                                     A("IV31"),
                                                                                     A("iv31"),
                                                                                     A("IV32"),
                                                                                     A("iv32"),

                                                                                     /* Huff YUV */
                                                                                     B(VLC_CODEC_HUFFYUV, "Huff YUV Video"),
                                                                                         A("HFYU"),

                                                                                     B(VLC_CODEC_FFVHUFF, "Huff YUV Video"),
                                                                                         A("FFVH"),

                                                                                         /* On2 VP3 Video Codecs */
                                                                                         B(VLC_CODEC_VP3, "On2's VP3 Video"),
                                                                                             A("VP3 "),
                                                                                             A("VP30"),
                                                                                             A("vp30"),
                                                                                             A("VP31"),
                                                                                             A("vp31"),

                                                                                             /* On2 VP4 Video Codec */
                                                                                             B(VLC_CODEC_VP4, "On2's VP4 Video"),
                                                                                                 A("VP40"),

                                                                                                 /* On2  VP5, VP6 codecs */
                                                                                                 B(VLC_CODEC_VP5, "On2's VP5 Video"),
                                                                                                     A("VP5 "),
                                                                                                     A("VP50"),

                                                                                                 B(VLC_CODEC_VP6, "On2's VP6.2 Video"),
                                                                                                     A("VP62"),
                                                                                                     A("vp62"),
                                                                                                     E("VP60", "On2's VP6.0 Video"),
                                                                                                     E("VP61", "On2's VP6.1 Video"),

                                                                                                 B(VLC_CODEC_VP6F, "On2's VP6.2 Video (Flash)"),
                                                                                                     A("VP6F"),
                                                                                                     A("FLV4"),

                                                                                                 B(VLC_CODEC_VP6A, "On2's VP6 A Video"),
                                                                                                     A("VP6A"),

                                                                                                 B(VLC_CODEC_VP7, "Google/On2's VP7 Video"),
                                                                                                     A("VP70"),
                                                                                                     A("VP71"),

                                                                                                 B(VLC_CODEC_VP8, "Google/On2's VP8 Video"),
                                                                                                     A("VP80"),

                                                                                                 B(VLC_CODEC_VP8ALPHA_ES, "Google/On2's VP8 Alpha"),
                                                                                                     A("VP8a"),

                                                                                                 B(VLC_CODEC_VP9, "Google/On2's VP9 Video"),
                                                                                                     A("VP90"),

                                                                                                 B(VLC_CODEC_VP9ALPHA_ES, "Google/On2's VP9 Alpha"),
                                                                                                     A("VP9a"),

                                                                                                 B(VLC_CODEC_AV1, "AOMedia's AV1 Video"),
                                                                                                     A("av10"),
                                                                                                     A("AV01"),

                                                                                                     /* Xiph.org theora */
                                                                                                     B(VLC_CODEC_THEORA, "Xiph.org's Theora Video"),
                                                                                                         A("theo"),
                                                                                                         A("THEO"),
                                                                                                         A("Thra"),

                                                                                                         /* Xiph.org tarkin */
                                                                                                         B(VLC_CODEC_TARKIN, "Xiph.org's Tarkin Video"),
                                                                                                             A("tark"),

                                                                                                             /* Asus Video (Another thing that doesn't work on PPC) */
                                                                                                             B(VLC_CODEC_ASV1, "Asus V1 Video"),
                                                                                                                 A("ASV1"),
                                                                                                             B(VLC_CODEC_ASV2, "Asus V2 Video"),
                                                                                                                 A("ASV2"),

                                                                                                                 /* FF video codec 1 (lossless codec) */
                                                                                                                 B(VLC_CODEC_FFV1, "FF video codec 1"),
                                                                                                                     A("FFV1"),

                                                                                                                     /* ATI VCR1 */
                                                                                                                     B(VLC_CODEC_VCR1, "ATI VCR1 Video"),
                                                                                                                         A("VCR1"),

                                                                                                                         /* Cirrus Logic AccuPak */
                                                                                                                         B(VLC_CODEC_CLJR, "Creative Logic AccuPak"),
                                                                                                                             A("CLJR"),

                                                                                                                             /* Real Video */
                                                                                                                             B(VLC_CODEC_RV10, "RealVideo 1.0"),
                                                                                                                                 A("RV10"),
                                                                                                                                 A("rv10"),

                                                                                                                             B(VLC_CODEC_RV13, "RealVideo 1.3"),
                                                                                                                                 A("RV13"),
                                                                                                                                 A("rv13"),

                                                                                                                             B(VLC_CODEC_RV20, "RealVideo G2 (2.0)"),
                                                                                                                                 A("RV20"),
                                                                                                                                 A("rv20"),

                                                                                                                             B(VLC_CODEC_RV30, "RealVideo 8 (3.0)"),
                                                                                                                                 A("RV30"),
                                                                                                                                 A("rv30"),

                                                                                                                             B(VLC_CODEC_RV40, "RealVideo 9/10 (4.0)"),
                                                                                                                                 A("RV40"),
                                                                                                                                 A("rv40"),

                                                                                                                                 /* Apple Video */
                                                                                                                                 B(VLC_CODEC_RPZA, "Apple Video"),
                                                                                                                                     A("rpza"),
                                                                                                                                     A("azpr"),
                                                                                                                                     A("RPZA"),
                                                                                                                                     A("AZPR"),

                                                                                                                                 B(VLC_CODEC_SMC, "Apple graphics"),
                                                                                                                                     A("smc "),

                                                                                                                                 B(VLC_CODEC_CINEPAK, "Cinepak Video"),
                                                                                                                                     A("CVID"),
                                                                                                                                     A("cvid"),

                                                                                                                                     /* Screen Capture Video Codecs */
                                                                                                                                     B(VLC_CODEC_TSCC, "TechSmith Camtasia Screen Capture"),
                                                                                                                                         A("TSCC"),
                                                                                                                                         A("tscc"),

                                                                                                                                     B(VLC_CODEC_CSCD, "CamStudio Screen Codec"),
                                                                                                                                         A("CSCD"),
                                                                                                                                         A("cscd"),

                                                                                                                                     B(VLC_CODEC_ZMBV, "DosBox Capture Codec"),
                                                                                                                                         A("ZMBV"),

                                                                                                                                     B(VLC_CODEC_VMNC, "VMware Video"),
                                                                                                                                         A("VMnc"),

                                                                                                                                     B(VLC_CODEC_FMVC, "FM Screen Capture Codec"),
                                                                                                                                         A("FMVC"),

                                                                                                                                     B(VLC_CODEC_FRAPS, "FRAPS: Realtime Video Capture"),
                                                                                                                                         A("FPS1"),
                                                                                                                                         A("fps1"),

                                                                                                                                         /* Duck TrueMotion */
                                                                                                                                         B(VLC_CODEC_TRUEMOTION1, "Duck TrueMotion v1 Video"),
                                                                                                                                             A("DUCK"),
                                                                                                                                             A("PVEZ"),
                                                                                                                                         B(VLC_CODEC_TRUEMOTION2, "Duck TrueMotion v2.0 Video"),
                                                                                                                                             A("TM20"),

                                                                                                                                         B(VLC_CODEC_QTRLE, "Apple QuickTime RLE Video"),
                                                                                                                                             A("rle "),

                                                                                                                                         B(VLC_CODEC_QDRAW, "Apple QuickDraw Video"),
                                                                                                                                             A("qdrw"),

                                                                                                                                         B(VLC_CODEC_QOI, "Quite OK Image Format"),
                                                                                                                                             A("qoif"),

                                                                                                                                         B(VLC_CODEC_QPEG, "QPEG Video"),
                                                                                                                                             A("QPEG"),
                                                                                                                                             A("Q1.0"),
                                                                                                                                             A("Q1.1"),

                                                                                                                                         B(VLC_CODEC_ULTI, "IBM Ultimotion Video"),
                                                                                                                                             A("ULTI"),

                                                                                                                                         B(VLC_CODEC_VIXL, "Miro/Pinnacle VideoXL Video"),
                                                                                                                                             A("VIXL"),
                                                                                                                                             A("XIXL"),
                                                                                                                                             E("PIXL", "Pinnacle VideoXL Video"),

                                                                                                                                         B(VLC_CODEC_LOCO, "LOCO Video"),
                                                                                                                                             A("LOCO"),

                                                                                                                                         B(VLC_CODEC_WNV1, "Winnov WNV1 Video"),
                                                                                                                                             A("WNV1"),
                                                                                                                                             A("YUV8"),

                                                                                                                                         B(VLC_CODEC_AASC, "Autodesc RLE Video"),
                                                                                                                                             A("AASC"),
                                                                                                                                             E("AAS4", "Autodesc RLE Video 24bit"),

                                                                                                                                         B(VLC_CODEC_INDEO2, "Indeo Video v2"),
                                                                                                                                             A("IV20"),
                                                                                                                                             E("RT21", "Indeo Video 2.1"),

                                                                                                                                             /* Flash Screen Video */
                                                                                                                                             B(VLC_CODEC_FLASHSV, "Flash Screen Video"),
                                                                                                                                                 A("FSV1"),
                                                                                                                                             B(VLC_CODEC_FLASHSV2, "Flash Screen Video 2"),
                                                                                                                                                 A("FSV2"),

                                                                                                                                             B(VLC_CODEC_KMVC, "Karl Morton's Video Codec (Worms)"),
                                                                                                                                                 A("KMVC"),

                                                                                                                                             B(VLC_CODEC_NUV, "Nuppel Video"),
                                                                                                                                                 A("RJPG"),
                                                                                                                                                 A("NUV1"),

                                                                                                                                                 /* CODEC_ID_SMACKVIDEO */
                                                                                                                                                 B(VLC_CODEC_SMACKVIDEO, "Smacker Video"),
                                                                                                                                                     A("SMK2"),
                                                                                                                                                     A("SMK4"),

                                                                                                                                                     /* Chinese AVS - Untested */
                                                                                                                                                     B(VLC_CODEC_CAVS, "Chinese AVS"),
                                                                                                                                                         A("CAVS"),
                                                                                                                                                         A("AVs2"),
                                                                                                                                                         A("avs2"),

                                                                                                                                                     B(VLC_CODEC_CAVS2, "AVS2-P2 video (IEEE 1857.4)"),
                                                                                                                                                         A("AVS2"),

                                                                                                                                                     B(VLC_CODEC_CAVS3, "AVS3-P2 video (IEEE 1857.10)"),

                                                                                                                                                     B(VLC_CODEC_AMV, "AMV"),

                                                                                                                                                     B(VLC_CODEC_BINKVIDEO, "Bink Video"),

                                                                                                                                                     B(VLC_CODEC_BINKAUDIO_DCT, "Bink Audio (DCT)"),

                                                                                                                                                     B(VLC_CODEC_BINKAUDIO_RDFT, "Bink Audio (RDFT)"),

                                                                                                                                                     /* */
                                                                                                                                                     B(VLC_CODEC_DNXHD, "DNxHD"),
                                                                                                                                                         A("AVdn"),
                                                                                                                                                         E("AVdh", "DNxHR"),
                                                                                                                                                     B(VLC_CODEC_8BPS, "8BPS"),
                                                                                                                                                         A("8BPS"),
                                                                                                                                                     B(VLC_CODEC_MIMIC, "Mimic"),
                                                                                                                                                         A("ML2O"),

                                                                                                                                                     B(VLC_CODEC_CDG, "CD-G Video"),
                                                                                                                                                         A("CDG "),

                                                                                                                                                     B(VLC_CODEC_FRWU, "Forward Uncompressed"),
                                                                                                                                                         A("FRWU"),

                                                                                                                                                     B(VLC_CODEC_INDEO4, "Indeo Video v4"),
                                                                                                                                                         A("IV41"),
                                                                                                                                                         A("iv41"),

                                                                                                                                                     B(VLC_CODEC_INDEO5, "Indeo Video v5"),
                                                                                                                                                         A("IV50"),
                                                                                                                                                         A("iv50"),

                                                                                                                                                     B(VLC_CODEC_PRORES, "Apple ProRes"),
                                                                                                                                                         E("apcn", "Apple ProRes 422 Standard"),
                                                                                                                                                         E("apch", "Apple ProRes 422 HQ"),
                                                                                                                                                         E("apcs", "Apple ProRes 422 LT"),
                                                                                                                                                         E("apco", "Apple ProRes 422 Proxy"),
                                                                                                                                                         E("ap4c", "Apple ProRes 4444"),
                                                                                                                                                         E("ap4h", "Apple ProRes 4444"),
                                                                                                                                                         E("ap4x", "Apple ProRes 4444 XQ"),

                                                                                                                                                     B(VLC_CODEC_ICOD, "Apple Intermediate Codec"),
                                                                                                                                                         A("icod"),

                                                                                                                                                     B(VLC_CODEC_G2M2, "GoTo Meeting Codec 2"),
                                                                                                                                                         A("G2M2"),

                                                                                                                                                     B(VLC_CODEC_G2M3, "GoTo Meeting Codec 3"),
                                                                                                                                                         A("G2M3"),

                                                                                                                                                     B(VLC_CODEC_G2M4, "GoTo Meeting Codec 4"),
                                                                                                                                                         A("G2M4"),
                                                                                                                                                         A("G2M5"),

                                                                                                                                                     B(VLC_CODEC_FIC, "Mirillis FIC video"),
                                                                                                                                                         A("FICV"),

                                                                                                                                                     B(VLC_CODEC_TDSC, "TDSC"),

                                                                                                                                                     B(VLC_CODEC_HQX, "Canopus HQX"),

                                                                                                                                                     B(VLC_CODEC_HQ_HQA, "Canopus HQ"),

                                                                                                                                                     B(VLC_CODEC_HAP, "Vidvox Hap"),
                                                                                                                                                         A("Hap1"),
                                                                                                                                                         E("Hap5", "Vidvox Hap Alpha"),
                                                                                                                                                         E("HapY", "Vidvox Hap Q"),

                                                                                                                                                     B(VLC_CODEC_DXV, "Resolume DXV"),
                                                                                                                                                         A("DXDI"),
                                                                                                                                                         E("DXD3", "Resolume DXV version 3"),

                                                                                                                                                         /* */
                                                                                                                                                         B(VLC_CODEC_YV12, "Planar 4:2:0 YVU"),
                                                                                                                                                             A("YV12"),
                                                                                                                                                             A("yv12"),
                                                                                                                                                         B(VLC_CODEC_I410, "Planar 4:1:0 YUV"),
                                                                                                                                                             A("I410"),
                                                                                                                                                         B(VLC_CODEC_I411, "Planar 4:1:1 YUV"),
                                                                                                                                                             A("I411"),
                                                                                                                                                             A("Y41B"),
                                                                                                                                                         B(VLC_CODEC_I420, "Planar 4:2:0 YUV"),
                                                                                                                                                             A("I420"),
                                                                                                                                                             A("IYUV"),
                                                                                                                                                         B(VLC_CODEC_I422, "Planar 4:2:2 YUV"),
                                                                                                                                                             A("I422"),
                                                                                                                                                             A("Y42B"),
                                                                                                                                                         B(VLC_CODEC_I440, "Planar 4:4:0 YUV"),
                                                                                                                                                             A("I440"),
                                                                                                                                                         B(VLC_CODEC_I444, "Planar 4:4:4 YUV"),
                                                                                                                                                             A("I444"),

                                                                                                                                                         B(VLC_CODEC_YUVP, "Palettized YUV with palette element Y:U:V:A"),
                                                                                                                                                             A("YUVP"),

                                                                                                                                                         B(VLC_CODEC_YUVA, "Planar YUV 4:4:4 Y:U:V:A"),
                                                                                                                                                             A("YUVA"),
                                                                                                                                                         B(VLC_CODEC_YUV420A, "Planar YUV 4:2:0 Y:U:V:A"),
                                                                                                                                                             A("I40A"),
                                                                                                                                                         B(VLC_CODEC_YUV422A, "Planar YUV 4:2:2 Y:U:V:A"),
                                                                                                                                                             A("I42A"),
                                                                                                                                                         B(VLC_CODEC_YUVA_444_10L, "Planar YUV 4:4:4 Y:U:V:A 10bits LE"),
                                                                                                                                                         B(VLC_CODEC_YUVA_444_10B, "Planar YUV 4:4:4 Y:U:V:A 10bits BE"),
                                                                                                                                                         B(VLC_CODEC_YUVA_444_12L, "Planar YUV 4:4:4 Y:U:V:A 12bits LE"),
                                                                                                                                                         B(VLC_CODEC_YUVA_444_12B, "Planar YUV 4:4:4 Y:U:V:A 12bits BE"),

                                                                                                                                                         B(VLC_CODEC_RGBP, "Palettized RGB with palette element R:G:B:A"),
                                                                                                                                                             A("RGBP"),

                                                                                                                                                         B(VLC_CODEC_RGB233, "8 bits RGB 2:3:3"),
                                                                                                                                                             A("RGB8"),
                                                                                                                                                         B(VLC_CODEC_RGB332, "8 bits RGB 3:3:2"),
                                                                                                                                                             A("R332"),
                                                                                                                                                         B(VLC_CODEC_BGR233, "8 bits BGR 2:3:3"),
                                                                                                                                                             A("B233"),
                                                                                                                                                         B(VLC_CODEC_RGBA, "32 bits RGBA"),
                                                                                                                                                             A("RGBA"),
                                                                                                                                                         B(VLC_CODEC_ARGB, "32 bits ARGB"),
                                                                                                                                                             A("ARGB"),
                                                                                                                                                             A("AV32"),
                                                                                                                                                         B(VLC_CODEC_BGRA, "32 bits BGRA"),
                                                                                                                                                             A("BGRA"),
                                                                                                                                                         B(VLC_CODEC_ABGR, "32 bits ABGR"),
                                                                                                                                                         B(VLC_CODEC_RGBA10LE, "32 bits RGB 10bits A 2bits LE"),
                                                                                                                                                             A("RGA0"),
                                                                                                                                                         B(VLC_CODEC_RGBA64, "64 bits RGBA"),
                                                                                                                                                             A("RGA4"),
                                                                                                                                                         B(VLC_CODEC_XRGB, "32 bits xRGB"),
                                                                                                                                                             A("RV32"),
                                                                                                                                                         B(VLC_CODEC_RGBX, "32 bits RGBx"),
                                                                                                                                                             A("RGBX"),
                                                                                                                                                         B(VLC_CODEC_XBGR, "32 bits xBGR"),
                                                                                                                                                             A("XBGR"),
                                                                                                                                                         B(VLC_CODEC_BGRX, "32 bits BGRx"),
                                                                                                                                                             A("BGRX"),
                                                                                                                                                         B(VLC_CODEC_RGB24, "24 bits RGB"),
                                                                                                                                                             A("RV24"),
                                                                                                                                                         B(VLC_CODEC_BGR24, "24 bits BGR"),
                                                                                                                                                             A("BGR3"),
                                                                                                                                                         B(VLC_CODEC_RGB565LE, "16 bits R5G6G5 LE"),
                                                                                                                                                             A("RV16"),
                                                                                                                                                         B(VLC_CODEC_RGB565BE, "16 bits R5G6G5 BE"),
                                                                                                                                                             A("RB16"),
                                                                                                                                                         B(VLC_CODEC_BGR565LE, "16 bits B5G6R5 LE"),
                                                                                                                                                             A("BL16"),
                                                                                                                                                         B(VLC_CODEC_BGR565BE, "16 bits B5G6R5 BE"),
                                                                                                                                                             A("BB16"),
                                                                                                                                                         B(VLC_CODEC_RGB555LE, "15 bits R5G5G5 LE"),
                                                                                                                                                             A("RV15"),
                                                                                                                                                         B(VLC_CODEC_RGB555BE, "15 bits R5G5G5 BE"),
                                                                                                                                                             A("RB15"),
                                                                                                                                                         B(VLC_CODEC_BGR555LE, "15 bits B5G5R5 LE"),
                                                                                                                                                             A("BL15"),
                                                                                                                                                         B(VLC_CODEC_BGR555LE, "15 bits B5G5R5 BE"),
                                                                                                                                                             A("BB15"),

                                                                                                                                                         B(VLC_CODEC_GREY, "8 bits greyscale"),
                                                                                                                                                             A("GREY"),
                                                                                                                                                             A("Y800"),
                                                                                                                                                             A("Y8  "),

                                                                                                                                                         B(VLC_CODEC_GREY_10L, "10 bits greyscale LE"),
                                                                                                                                                             A("G0FL"),

                                                                                                                                                         B(VLC_CODEC_GREY_10B, "10 bits greyscale BE"),
                                                                                                                                                             A("G0FB"),

                                                                                                                                                         B(VLC_CODEC_GREY_12L, "12 bits greyscale LE"),
                                                                                                                                                             A("G2FL"),

                                                                                                                                                         B(VLC_CODEC_GREY_12B, "12 bits greyscale BE"),
                                                                                                                                                             A("G2FB"),

                                                                                                                                                         B(VLC_CODEC_GREY_16L, "16 bits greyscale LE"),
                                                                                                                                                             A("GRFL"),

                                                                                                                                                         B(VLC_CODEC_GREY_16B, "16 bits greyscale BE"),
                                                                                                                                                             A("GRFB"),

                                                                                                                                                         B(VLC_CODEC_UYVY, "Packed YUV 4:2:2, U:Y:V:Y"),
                                                                                                                                                             A("UYVY"),
                                                                                                                                                             A("UYNV"),
                                                                                                                                                             A("UYNY"),
                                                                                                                                                             A("Y422"),
                                                                                                                                                             A("HDYC"),
                                                                                                                                                             A("AVUI"),
                                                                                                                                                             A("uyv1"),
                                                                                                                                                             A("2vuy"),
                                                                                                                                                             A("2Vuy"),
                                                                                                                                                             A("2Vu1"),
                                                                                                                                                         B(VLC_CODEC_VYUY, "Packed YUV 4:2:2, V:Y:U:Y"),
                                                                                                                                                             A("VYUY"),
                                                                                                                                                         B(VLC_CODEC_YUYV, "Packed YUV 4:2:2, Y:U:Y:V"),
                                                                                                                                                             A("YUY2"),
                                                                                                                                                             A("YUYV"),
                                                                                                                                                             A("YUNV"),
                                                                                                                                                             A("V422"),

                                                                                                                                                         B(VLC_CODEC_YUV2, "Packed YUV 4:2:2, Y:U:Y:V signed"),
                                                                                                                                                             A("yuv2"),
                                                                                                                                                             A("yuvu"),

                                                                                                                                                         B(VLC_CODEC_YVYU, "Packed YUV 4:2:2, Y:V:Y:U"),
                                                                                                                                                             A("YVYU"),

                                                                                                                                                         B(VLC_CODEC_Y211, "Packed YUV 2:1:1, Y:U:Y:V "),
                                                                                                                                                             A("Y211"),
                                                                                                                                                         B(VLC_CODEC_CYUV, "Creative Packed YUV 4:2:2, U:Y:V:Y, reverted"),
                                                                                                                                                             A("cyuv"),
                                                                                                                                                             A("CYUV"),

                                                                                                                                                         B(VLC_CODEC_V210, "10-bit 4:2:2 Component YCbCr"),
                                                                                                                                                             A("v210"),

                                                                                                                                                         B(VLC_CODEC_Y210, "Packed 10-bit YUV 4:2:2"),
                                                                                                                                                             A("Y210"),
                                                                                                                                                         B(VLC_CODEC_VUYA, "Packed YUV 4:4:4, V:U:Y:A"),
                                                                                                                                                             A("VUYA"),
                                                                                                                                                         B(VLC_CODEC_Y410, "Packed 10-bit YUV 4:4:4"),
                                                                                                                                                             A("Y410"),

                                                                                                                                                         B(VLC_CODEC_NV12, "Biplanar 4:2:0 Y/UV"),
                                                                                                                                                             A("NV12"),
                                                                                                                                                         B(VLC_CODEC_NV21, "Biplanar 4:2:0 Y/VU"),
                                                                                                                                                             A("NV21"),
                                                                                                                                                         B(VLC_CODEC_P010, "Biplanar 4:2:0 Y/VU 10-bit LE"),
                                                                                                                                                             A("P010"),
                                                                                                                                                         B(VLC_CODEC_P012, "Biplanar 4:2:0 Y/VU 12-bit LE"),
                                                                                                                                                             A("P012"),
                                                                                                                                                         B(VLC_CODEC_P016, "Biplanar 4:2:0 Y/VU 16-bit LE"),
                                                                                                                                                             A("P016"),
                                                                                                                                                         B(VLC_CODEC_NV16, "Biplanar 4:2:2 Y/UV"),
                                                                                                                                                             A("NV16"),
                                                                                                                                                         B(VLC_CODEC_NV61, "Biplanar 4:2:2 Y/VU"),
                                                                                                                                                             A("NV61"),
                                                                                                                                                         B(VLC_CODEC_NV24, "Biplanar 4:4:4 Y/UV"),
                                                                                                                                                             A("NV24"),
                                                                                                                                                         B(VLC_CODEC_NV42, "Biplanar 4:4:4 Y/VU"),
                                                                                                                                                             A("NV42"),

                                                                                                                                                         B(VLC_CODEC_I420_9L, "Planar 4:2:0 YUV 9-bit LE"),
                                                                                                                                                             A("I09L"),
                                                                                                                                                         B(VLC_CODEC_I420_9B, "Planar 4:2:0 YUV 9-bit BE"),
                                                                                                                                                             A("I09B"),
                                                                                                                                                         B(VLC_CODEC_I422_9L, "Planar 4:2:2 YUV 9-bit LE"),
                                                                                                                                                             A("I29L"),
                                                                                                                                                         B(VLC_CODEC_I422_9B, "Planar 4:2:2 YUV 9-bit BE"),
                                                                                                                                                             A("I29B"),
                                                                                                                                                         B(VLC_CODEC_I444_9L, "Planar 4:4:4 YUV 9-bit LE"),
                                                                                                                                                             A("I49L"),
                                                                                                                                                         B(VLC_CODEC_I444_9B, "Planar 4:4:4 YUV 9-bit BE"),
                                                                                                                                                             A("I49B"),

                                                                                                                                                         B(VLC_CODEC_I420_10L, "Planar 4:2:0 YUV 10-bit LE"),
                                                                                                                                                             A("I0AL"),
                                                                                                                                                         B(VLC_CODEC_I420_10B, "Planar 4:2:0 YUV 10-bit BE"),
                                                                                                                                                             A("I0AB"),
                                                                                                                                                         B(VLC_CODEC_I422_10L, "Planar 4:2:2 YUV 10-bit LE"),
                                                                                                                                                             A("I2AL"),
                                                                                                                                                         B(VLC_CODEC_I422_10B, "Planar 4:2:2 YUV 10-bit BE"),
                                                                                                                                                             A("I2AB"),
                                                                                                                                                         B(VLC_CODEC_I444_10L, "Planar 4:4:4 YUV 10-bit LE"),
                                                                                                                                                             A("I4AL"),
                                                                                                                                                         B(VLC_CODEC_I444_10B, "Planar 4:4:4 YUV 10-bit BE"),
                                                                                                                                                             A("I4AB"),

                                                                                                                                                         B(VLC_CODEC_I420_12L, "Planar 4:2:0 YUV 12-bit LE"),
                                                                                                                                                             A("I0CL"),
                                                                                                                                                         B(VLC_CODEC_I420_12B, "Planar 4:2:0 YUV 12-bit BE"),
                                                                                                                                                             A("I0CB"),
                                                                                                                                                         B(VLC_CODEC_I422_12L, "Planar 4:2:2 YUV 12-bit LE"),
                                                                                                                                                             A("I2CL"),
                                                                                                                                                         B(VLC_CODEC_I422_12B, "Planar 4:2:2 YUV 12-bit BE"),
                                                                                                                                                             A("I2CB"),
                                                                                                                                                         B(VLC_CODEC_I444_12L, "Planar 4:4:4 YUV 12-bit LE"),
                                                                                                                                                             A("I4CL"),
                                                                                                                                                         B(VLC_CODEC_I444_12B, "Planar 4:4:4 YUV 12-bit BE"),
                                                                                                                                                             A("I4CB"),

                                                                                                                                                         B(VLC_CODEC_I420_16L, "Planar 4:2:0 YUV 16-bit LE"),
                                                                                                                                                             A("I0FL"),
                                                                                                                                                         B(VLC_CODEC_I420_16B, "Planar 4:2:0 YUV 16-bit BE"),
                                                                                                                                                             A("I0FB"),
                                                                                                                                                         B(VLC_CODEC_I444_16L, "Planar 4:4:4 YUV 16-bit LE"),
                                                                                                                                                             A("I4FL"),
                                                                                                                                                         B(VLC_CODEC_I444_16B, "Planar 4:4:4 YUV 16-bit BE"),
                                                                                                                                                             A("I4FB"),


                                                                                                                                                             /* XYZ color space */
                                                                                                                                                             B(VLC_CODEC_XYZ_12L, "Packed XYZ 12-bit LE"),

                                                                                                                                                             B(VLC_CODEC_XYZ_12B, "Packed XYZ 12-bit BE"),
                                                                                                                                                                 A("XY12"),

                                                                                                                                                                 /* Videogames Codecs */

                                                                                                                                                                 /* Interplay MVE */
                                                                                                                                                                 B(VLC_CODEC_INTERPLAY, "Interplay MVE Video"),
                                                                                                                                                                     A("imve"),
                                                                                                                                                                     A("INPV"),

                                                                                                                                                                     /* Id Quake II CIN */
                                                                                                                                                                     B(VLC_CODEC_IDCIN, "Id Quake II CIN Video"),
                                                                                                                                                                         A("IDCI"),

                                                                                                                                                                         /* 4X Technologies */
                                                                                                                                                                         B(VLC_CODEC_4XM, "4X Technologies Video"),
                                                                                                                                                                             A("4XMV"),
                                                                                                                                                                             A("4xmv"),

                                                                                                                                                                             /* Id RoQ */
                                                                                                                                                                             B(VLC_CODEC_ROQ, "Id RoQ Video"),
                                                                                                                                                                                 A("RoQv"),

                                                                                                                                                                                 /* Sony Playstation MDEC */
                                                                                                                                                                                 B(VLC_CODEC_MDEC, "PSX MDEC Video"),
                                                                                                                                                                                     A("MDEC"),

                                                                                                                                                                                     /* Sierra VMD */
                                                                                                                                                                                     B(VLC_CODEC_VMDVIDEO, "Sierra VMD Video"),
                                                                                                                                                                                         A("VMDV"),
                                                                                                                                                                                         A("vmdv"),

                                                                                                                                                                                     B(VLC_CODEC_DIRAC, "Dirac"),
                                                                                                                                                                                         A("drac"),

                                                                                                                                                                                         /* Image */
                                                                                                                                                                                         B(VLC_CODEC_PNG, "PNG Image"),
                                                                                                                                                                                             A("png "),

                                                                                                                                                                                         B(VLC_CODEC_PPM, "PPM Image"),
                                                                                                                                                                                             A("ppm "),

                                                                                                                                                                                         B(VLC_CODEC_PGM, "PGM Image"),
                                                                                                                                                                                             A("pgm "),

                                                                                                                                                                                         B(VLC_CODEC_PGMYUV, "PGM YUV Image"),
                                                                                                                                                                                             A("pgmy"),

                                                                                                                                                                                         B(VLC_CODEC_PAM, "PAM Image"),
                                                                                                                                                                                             A("pam "),

                                                                                                                                                                                         B(VLC_CODEC_JPEGLS, "JPEG-LS"),
                                                                                                                                                                                             A("MJLS"),

                                                                                                                                                                                         B(VLC_CODEC_JPEG, "JPEG"),
                                                                                                                                                                                             A("jpeg"),
                                                                                                                                                                                             A("JPEG"),

                                                                                                                                                                                         B(VLC_CODEC_BPG, "BPG Image"),
                                                                                                                                                                                             A("BPG "),

                                                                                                                                                                                         B(VLC_CODEC_BMP, "BMP Image"),
                                                                                                                                                                                             A("bmp "),

                                                                                                                                                                                         B(VLC_CODEC_FARBFELD, "Farbfeld Image"),
                                                                                                                                                                                             A("farb"),

                                                                                                                                                                                         B(VLC_CODEC_TIFF, "TIFF Image"),
                                                                                                                                                                                             A("tiff"),

                                                                                                                                                                                         B(VLC_CODEC_GIF, "GIF Image"),
                                                                                                                                                                                             A("gif "),


                                                                                                                                                                                         B(VLC_CODEC_TARGA, "Truevision Targa Image"),
                                                                                                                                                                                             A("tga "),
                                                                                                                                                                                             A("mtga"),
                                                                                                                                                                                             A("MTGA"),

                                                                                                                                                                                         B(VLC_CODEC_SGI, "SGI Image"),
                                                                                                                                                                                             A("sgi "),

                                                                                                                                                                                         B(VLC_CODEC_SVG, "SVG Scalable Vector Graphics Image"),
                                                                                                                                                                                             A("svg "),

                                                                                                                                                                                         B(VLC_CODEC_PNM, "Portable Anymap Image"),
                                                                                                                                                                                             A("pnm "),

                                                                                                                                                                                         B(VLC_CODEC_PCX, "Personal Computer Exchange Image"),
                                                                                                                                                                                             A("pcx "),

                                                                                                                                                                                         B(VLC_CODEC_XWD, "X Window system raster image"),

                                                                                                                                                                                         B(VLC_CODEC_JPEG2000, "JPEG 2000 Image"),
                                                                                                                                                                                             A("JP2K"),
                                                                                                                                                                                             A("mjp2"),
                                                                                                                                                                                             A("MJP2"),
                                                                                                                                                                                             A("MJ2C"),
                                                                                                                                                                                             A("LJ2C"),
                                                                                                                                                                                             A("LJ2K"),

                                                                                                                                                                                         B(VLC_CODEC_WEBP, "WebP Image"),
                                                                                                                                                                                             A("WEBP"),

                                                                                                                                                                                         B(VLC_CODEC_LAGARITH, "Lagarith Lossless"),
                                                                                                                                                                                             A("LAGS"),

                                                                                                                                                                                         B(VLC_CODEC_MXPEG, "Mxpeg"),
                                                                                                                                                                                             A("MXPG"),

                                                                                                                                                                                         B(VLC_CODEC_CDXL, "Commodore CDXL video format"),
                                                                                                                                                                                             A("CDXL"),

                                                                                                                                                                                         B(VLC_CODEC_BMVVIDEO, "Discworld II BMV video"),
                                                                                                                                                                                             A("BMVV"),

                                                                                                                                                                                         B(VLC_CODEC_UTVIDEO, "Ut Video"),
                                                                                                                                                                                             A("ULRA"),
                                                                                                                                                                                             A("ULRG"),
                                                                                                                                                                                             A("ULY0"),
                                                                                                                                                                                             A("ULY2"),
                                                                                                                                                                                             A("ULY4"),
                                                                                                                                                                                             A("ULH0"),
                                                                                                                                                                                             A("ULH2"),
                                                                                                                                                                                             A("ULH4"),
                                                                                                                                                                                             A("UQY2"),
                                                                                                                                                                                             A("UQRA"),
                                                                                                                                                                                             A("UQRG"),

                                                                                                                                                                                         B(VLC_CODEC_VBLE, "VBLE Lossless"),
                                                                                                                                                                                             A("VBLE"),

                                                                                                                                                                                         B(VLC_CODEC_DXTORY, "Dxtory capture format"),
                                                                                                                                                                                             A("xtor"),

                                                                                                                                                                                         B(VLC_CODEC_MSS1, "Windows Media Video 7 Screen"),
                                                                                                                                                                                             A("MSS1"),
                                                                                                                                                                                             A("mss1"),

                                                                                                                                                                                         B(VLC_CODEC_MSS2, "Windows Media Video 9 Screen"),
                                                                                                                                                                                             A("MSS2"),
                                                                                                                                                                                             A("mss2"),

                                                                                                                                                                                         B(VLC_CODEC_MSA1, "Microsoft Application Screen Decoder 1"),
                                                                                                                                                                                             A("MSA1"),

                                                                                                                                                                                         B(VLC_CODEC_TSC2, "TechSmith Screen Codec 2"),
                                                                                                                                                                                             A("tsc2"),

                                                                                                                                                                                         B(VLC_CODEC_MTS2, "Microsoft Expression Encoder Screen"),
                                                                                                                                                                                             A("MTS2"),

                                                                                                                                                                                         B(VLC_CODEC_XAN_WC4, "Wing Commander IV Xan video"),
                                                                                                                                                                                             A("Xxan"),

                                                                                                                                                                                         B(VLC_CODEC_LCL_MSZH, "Loss-Less Codec Library AVImszh"),
                                                                                                                                                                                             A("MSZH"),

                                                                                                                                                                                         B(VLC_CODEC_LCL_ZLIB, "Loss-Less Codec Library AVIzlib"),
                                                                                                                                                                                             A("ZLIB"),

                                                                                                                                                                                         B(VLC_CODEC_THP, "GameCube THP video"),

                                                                                                                                                                                         B(VLC_CODEC_TXD, "RenderWare TXD"),

                                                                                                                                                                                         B(VLC_CODEC_ESCAPE124, "Escape 124 video"),

                                                                                                                                                                                         B(VLC_CODEC_KGV1, "Kega Game Video (KGV1)"),
                                                                                                                                                                                             A("KGV1"),

                                                                                                                                                                                         B(VLC_CODEC_CLLC, "Canopus Lossless"),
                                                                                                                                                                                             A("CLLC"),

                                                                                                                                                                                         B(VLC_CODEC_AURA, "Auravision Aura"),
                                                                                                                                                                                             A("AURA"),

                                                                                                                                                                                         B(VLC_CODEC_TMV, "8088flex TMV"),

                                                                                                                                                                                         B(VLC_CODEC_XAN_WC3, "Wing Commander III video"),

                                                                                                                                                                                         B(VLC_CODEC_WS_VQA, "Westwood Studios VQA"),

                                                                                                                                                                                         B(VLC_CODEC_MMVIDEO, "American Laser Games MM Video"),

                                                                                                                                                                                         B(VLC_CODEC_AVS, "Creature Shock AVS"),

                                                                                                                                                                                         B(VLC_CODEC_DSICINVIDEO, "Delphine CIN video"),

                                                                                                                                                                                         B(VLC_CODEC_TIERTEXSEQVIDEO, "Tiertex Limited SEQ video"),

                                                                                                                                                                                         B(VLC_CODEC_DXA, "Feeble Files/ScummVM DXA"),

                                                                                                                                                                                         B(VLC_CODEC_C93, "Interplay C93"),

                                                                                                                                                                                         B(VLC_CODEC_BETHSOFTVID, "Bethesda VID"),

                                                                                                                                                                                         B(VLC_CODEC_VB, "Beam Software VB"),

                                                                                                                                                                                         B(VLC_CODEC_RL2, "RL2 video"),

                                                                                                                                                                                         B(VLC_CODEC_BFI, "Brute Force & Ignorance (BFI) video"),

                                                                                                                                                                                         B(VLC_CODEC_CMV, "Electronic Arts CMV"),

                                                                                                                                                                                         B(VLC_CODEC_MOTIONPIXELS, "Sirius Publishing Motion Pixels"),

                                                                                                                                                                                         B(VLC_CODEC_TGV, "Electronic Arts TGV"),

                                                                                                                                                                                         B(VLC_CODEC_TGQ, "Electronic Arts TGQ"),

                                                                                                                                                                                         B(VLC_CODEC_TQI, "Electronic Arts TQI"),

                                                                                                                                                                                         B(VLC_CODEC_MAD, "Electronic Arts MAD"),

                                                                                                                                                                                         B(VLC_CODEC_ANM, "DeluxePaint animation"),

                                                                                                                                                                                         B(VLC_CODEC_YOP, "Psygnosis YOP"),

                                                                                                                                                                                         B(VLC_CODEC_JV, "Bitmap Brothers JV"),

                                                                                                                                                                                         B(VLC_CODEC_DFA, "Chronomaster DFA"),

                                                                                                                                                                                         B(VLC_CODEC_HNM4_VIDEO, "Cryo Interactive Entertainment HNM4"),

                                                                                                                                                                                         B(VLC_CODEC_CINEFORM, "CineForm"),

                                                                                                                                                                                         B(VLC_CODEC_SPEEDHQ, "NewTek SpeedHQ"),
                                                                                                                                                                                             A("SHQ0"),
                                                                                                                                                                                             A("SHQ1"),
                                                                                                                                                                                             A("SHQ2"),
                                                                                                                                                                                             A("SHQ3"),
                                                                                                                                                                                             A("SHQ4"),
                                                                                                                                                                                             A("SHQ5"),
                                                                                                                                                                                             A("SHQ7"),
                                                                                                                                                                                             A("SHQ9"),

                                                                                                                                                                                         B(VLC_CODEC_PIXLET, "Apple Pixlet"),
                                                                                                                                                                                             A("pxlt"),

                                                                                                                                                                                         B(VLC_CODEC_MAGICYUV, "MagicYUV lossless"),
                                                                                                                                                                                             A("M8RG"),
                                                                                                                                                                                             A("M8RA"),
                                                                                                                                                                                             A("M8G0"),
                                                                                                                                                                                             A("M8Y0"),
                                                                                                                                                                                             A("M8Y2"),
                                                                                                                                                                                             A("M8Y4"),
                                                                                                                                                                                             A("M8YA"),

                                                                                                                                                                                         B(VLC_CODEC_IMM4, "Infinity IMM4"),
                                                                                                                                                                                             A("IMM4"),

                                                                                                                                                                                         B(VLC_CODEC_IMM5, "Infinity IMM5"),
                                                                                                                                                                                             A("IMM5"),

                                                                                                                                                                                         B(VLC_CODEC_VDPAU_VIDEO, "VDPAU video"),

                                                                                                                                                                                         B(VLC_CODEC_VDPAU_OUTPUT, "VDPAU output"),

                                                                                                                                                                                         B(VLC_CODEC_VAAPI_420, "4:2:0 VAAPI opaque"),
                                                                                                                                                                                             A("VAOP"),

                                                                                                                                                                                         B(VLC_CODEC_VAAPI_420_10BPP, "4:2:0 10bits VAAPI opaque"),
                                                                                                                                                                                             A("VAO0"),

                                                                                                                                                                                         B(VLC_CODEC_VAAPI_420_12BPP, "4:2:0 12bits VAAPI opaque"),
                                                                                                                                                                                             A("VAO2"),

                                                                                                                                                                                         B(VLC_CODEC_ANDROID_OPAQUE, "Android opaque"),
                                                                                                                                                                                             A("ANOP"),

                                                                                                                                                                                         B(VLC_CODEC_MMAL_OPAQUE, "MMAL opaque"),
                                                                                                                                                                                             A("MMAL"),

                                                                                                                                                                                         B(VLC_CODEC_D3D9_OPAQUE, "4:2:0 D3D9 opaque"),
                                                                                                                                                                                             A("DXA9"),

                                                                                                                                                                                         B(VLC_CODEC_D3D9_OPAQUE_10B, "4:2:0 10bits D3D9 opaque"),
                                                                                                                                                                                             A("DXA0"),

                                                                                                                                                                                         B(VLC_CODEC_D3D11_OPAQUE, "4:2:0 D3D11 opaque"),
                                                                                                                                                                                             A("DX11"),

                                                                                                                                                                                         B(VLC_CODEC_D3D11_OPAQUE_10B, "4:2:0 10bits D3D11 opaque"),
                                                                                                                                                                                             A("DX10"),

                                                                                                                                                                                         B(VLC_CODEC_D3D11_OPAQUE_RGBA, "RGBA D3D11 opaque"),
                                                                                                                                                                                             A("DXRG"),

                                                                                                                                                                                         B(VLC_CODEC_D3D11_OPAQUE_BGRA, "BGRA D3D11 opaque"),
                                                                                                                                                                                             A("DAGR"),

                                                                                                                                                                                         B(VLC_CODEC_D3D11_OPAQUE_ALPHA, "4:2:0 D3D11 opaque with alpha"),
                                                                                                                                                                                             A("DA11"),

                                                                                                                                                                                         B(VLC_CODEC_NVDEC_OPAQUE, "4:2:0 NVDEC opaque"),
                                                                                                                                                                                             A("NVD8"),

                                                                                                                                                                                         B(VLC_CODEC_NVDEC_OPAQUE_10B, "4:2:0 10bits NVDEC opaque"),
                                                                                                                                                                                             A("NVD0"),

                                                                                                                                                                                         B(VLC_CODEC_NVDEC_OPAQUE_16B, "4:2:0 16bits NVDEC opaque"),
                                                                                                                                                                                             A("NVD6"),

                                                                                                                                                                                         B(VLC_CODEC_NVDEC_OPAQUE_444, "4:4:4 NVDEC opaque"),
                                                                                                                                                                                             A("NV48"),

                                                                                                                                                                                         B(VLC_CODEC_NVDEC_OPAQUE_444_16B, "4:4:4 16bits NVDEC opaque"),
                                                                                                                                                                                             A("NV46"),

                                                                                                                                                                                         B(VLC_CODEC_AGM, "AmuseGraphics Movie"),
                                                                                                                                                                                             A("AGM0"),
                                                                                                                                                                                             A("AGM1"),
                                                                                                                                                                                             A("AGM2"),
                                                                                                                                                                                             A("AGM3"),
                                                                                                                                                                                             A("AGM4"),
                                                                                                                                                                                             A("AGM5"),
                                                                                                                                                                                             A("AGM6"),
                                                                                                                                                                                             A("AGM7"),

                                                                                                                                                                                         B(VLC_CODEC_NOTCHLC, "NotchLC"),

                                                                                                                                                                                         B(VLC_CODEC_RTP_VIDEO_RAW, "RTP raw video"),
};

static const staticentry_t p_list_audio[] = {

    /* Windows Media Audio 1 */
    B(VLC_CODEC_WMA1, "Windows Media Audio 1"),
        A("WMA1"),
        A("wma1"),

        /* Windows Media Audio 2 */
        B(VLC_CODEC_WMA2, "Windows Media Audio 2"),
            A("WMA2"),
            A("wma2"),
            A("wma "),

            /* Windows Media Audio Professional */
            B(VLC_CODEC_WMAP, "Windows Media Audio Professional"),
                A("WMAP"),
                A("wmap"),

                /* Windows Media Audio Lossless */
                B(VLC_CODEC_WMAL, "Windows Media Audio Lossless"),
                    A("WMAL"),
                    A("wmal"),

                    /* Windows Media Audio Speech */
                    B(VLC_CODEC_WMAS, "Windows Media Audio Voice (Speech)"),
                        A("WMAS"),
                        A("wmas"),

                        /* DV Audio */
                        B(VLC_CODEC_DVAUDIO, "DV Audio"),
                            A("dvau"),
                            A("vdva"),
                            A("dvca"),
                            A("RADV"),

                            /* MACE-3 Audio */
                            B(VLC_CODEC_MACE3, "MACE-3 Audio"),
                                A("MAC3"),

                                /* MACE-6 Audio */
                                B(VLC_CODEC_MACE6, "MACE-6 Audio"),
                                    A("MAC6"),

                                    /* MUSEPACK7 Audio */
                                    B(VLC_CODEC_MUSEPACK7, "MUSEPACK7 Audio"),
                                        A("MPC "),

                                        /* MUSEPACK8 Audio */
                                        B(VLC_CODEC_MUSEPACK8, "MUSEPACK8 Audio"),
                                            A("MPCK"),
                                            A("MPC8"),

                                            /* RealAudio 1.0 */
                                            B(VLC_CODEC_RA_144, "RealAudio 1.0"),
                                                A("14_4"),
                                                A("lpcJ"),

                                                /* RealAudio 2.0 */
                                                B(VLC_CODEC_RA_288, "RealAudio 2.0"),
                                                    A("28_8"),

                                                B(VLC_CODEC_SIPR, "RealAudio Sipr"),
                                                    A("sipr"),

                                                    /* MPEG Audio layer 1/2 */
                                                    B(VLC_CODEC_MPGA, "MPEG Audio layer 1/2"),
                                                        A("mpga"),
                                                        A("mp2a"),
                                                        A(".mp1"),
                                                        A(".mp2"),
                                                        A("LAME"),
                                                        A("ms\x00\x50"),
                                                        A("ms\x00\x55"),

                                                        /* MPEG Audio layer 3 */
                                                        B(VLC_CODEC_MP3, "MPEG Audio layer 3"),
                                                            A("mp3 "),
                                                            A(".mp3"),
                                                            A("MP3 "),

                                                            /* A52 Audio (aka AC3) */
                                                            B(VLC_CODEC_A52, "Audio Coding 3 (AC-3)"),
                                                                A("a52 "),
                                                                A("a52b"),
                                                                A("ac-3"),
                                                                A("sac3"),
                                                                A("ms\x20\x00"),

                                                            B(VLC_CODEC_EAC3, "Enhanced Audio Coding 3 (E-AC-3)"),
                                                                A("ec-3"),

                                                            B(VLC_CODEC_AC4,  "Audio Coding 4 (AC-4)"),

                                                            /* DTS Audio */
                                                            B(VLC_CODEC_DTS, "DTS Audio"),
                                                                A("dts "),
                                                                A("DTS "),
                                                                A("dtsb"),
                                                                A("dtsc"),
                                                                E("dtse", "DTS Express"),
                                                                E("dtsh", "DTS-HD High Resolution Audio"),
                                                                E("dtsl", "DTS-HD Lossless"),
                                                                A("ms\x20\x01"),

                                                                /* AAC audio */
                                                                B(VLC_CODEC_MP4A, "MPEG AAC Audio"),
                                                                    A("mp4a"),
                                                                    A("aac "),
                                                                    A("AACL"),
                                                                    A("AACH"),
                                                                    A("AACP"), /* smooth streaming alias */

                                                                B(VLC_CODEC_MPEGH, "MPEG-H Audio"),
                                                                    A("mha1"),
                                                                    A("mha2"),
                                                                    A("mhm1"),
                                                                    A("mhm2"),

                                                                    /* ALS audio */
                                                                    B(VLC_CODEC_ALS, "MPEG-4 Audio Lossless (ALS)"),
                                                                        A("als "),

                                                                        /* 4X Technologies */
                                                                        B(VLC_CODEC_ADPCM_4XM, "4X Technologies Audio"),
                                                                            A("4xma"),

                                                                            /* EA ADPCM */
                                                                            B(VLC_CODEC_ADPCM_EA, "EA ADPCM Audio"),
                                                                                A("ADEA"),

                                                                                /* Interplay DPCM */
                                                                                B(VLC_CODEC_INTERPLAY_DPCM, "Interplay DPCM Audio"),
                                                                                    A("idpc"),

                                                                                    /* Id RoQ */
                                                                                    B(VLC_CODEC_ROQ_DPCM, "Id RoQ DPCM Audio"),
                                                                                        A("RoQa"),

                                                                                        /* DCIN Audio */
                                                                                        B(VLC_CODEC_DSICINAUDIO, "Delphine CIN Audio"),
                                                                                            A("DCIA"),

                                                                                            /* Sony Playstation XA ADPCM */
                                                                                            B(VLC_CODEC_ADPCM_XA, "PSX XA ADPCM Audio"),
                                                                                                A("xa  "),

                                                                                                /* ADX ADPCM */
                                                                                                B(VLC_CODEC_ADPCM_ADX, "ADX ADPCM Audio"),
                                                                                                    A("adx "),

                                                                                                    /* Westwood ADPCM */
                                                                                                    B(VLC_CODEC_ADPCM_IMA_WS, "Westwood IMA ADPCM audio"),
                                                                                                        A("AIWS"),

                                                                                                        /* MS ADPCM */
                                                                                                        B(VLC_CODEC_ADPCM_MS, "MS ADPCM audio"),
                                                                                                            A("ms\x00\x02"),

                                                                                                            /* Sierra VMD */
                                                                                                            B(VLC_CODEC_VMDAUDIO, "Sierra VMD Audio"),
                                                                                                                A("vmda"),

                                                                                                                /* G.726 ADPCM */
                                                                                                                B(VLC_CODEC_ADPCM_G726, "G.726 ADPCM Audio"),
                                                                                                                    A("g726"),

                                                                                                                    /* G.726 ADPCM */
                                                                                                                    B(VLC_CODEC_ADPCM_G726_LE, "G.726 ADPCM Audio (little-endian)"),
                                                                                                                        A("g72s"),

                                                                                                                        /* G.722 ADPCM */
                                                                                                                        B(VLC_CODEC_ADPCM_G722, "G.722 ADPCM Audio"),
                                                                                                                            A("g722"),

                                                                                                                            /* Flash ADPCM */
                                                                                                                            B(VLC_CODEC_ADPCM_SWF, "Flash ADPCM Audio"),
                                                                                                                                A("SWFa"),

                                                                                                                            B(VLC_CODEC_ADPCM_IMA_WAV, "IMA WAV ADPCM Audio"),
                                                                                                                                A("ms\x00\x11"),

                                                                                                                            B(VLC_CODEC_ADPCM_IMA_AMV, "IMA AMV ADPCM Audio"),
                                                                                                                                A("imav"),

                                                                                                                            B(VLC_CODEC_ADPCM_IMA_QT, "IMA QT ADPCM Audio"),
                                                                                                                                A("ima4"),

                                                                                                                            B(VLC_CODEC_ADPCM_YAMAHA, "Yamaha ADPCM Audio"),
                                                                                                                                A("ms\x00\x20"),

                                                                                                                            B(VLC_CODEC_ADPCM_DK3, "Duck DK3 ADPCM"),
                                                                                                                                A("ms\x00\x62"),

                                                                                                                            B(VLC_CODEC_ADPCM_DK4, "Duck DK4 ADPCM"),
                                                                                                                                A("ms\x00\x61"),

                                                                                                                            B(VLC_CODEC_ADPCM_THP, "GameCube THP ADPCM"),

                                                                                                                            B(VLC_CODEC_ADPCM_XA_EA, "EA-XA ADPCM"),
                                                                                                                                A("XAJ\x00"),

                                                                                                                                /* AMR */
                                                                                                                                B(VLC_CODEC_AMR_NB, "AMR narrow band"),
                                                                                                                                    A("samr"),

                                                                                                                                B(VLC_CODEC_AMR_WB, "AMR wide band"),
                                                                                                                                    A("sawb"),

                                                                                                                                    /* FLAC */
                                                                                                                                    B(VLC_CODEC_FLAC, "FLAC (Free Lossless Audio Codec)"),
                                                                                                                                        A("flac"),

                                                                                                                                        /* ALAC */
                                                                                                                                        B(VLC_CODEC_ALAC, "Apple Lossless Audio Codec"),
                                                                                                                                            A("alac"),

                                                                                                                                            /* QDM2 */
                                                                                                                                            B(VLC_CODEC_QDM2, "QDM2 Audio"),
                                                                                                                                                A("QDM2"),

                                                                                                                                                /* QDMC */
                                                                                                                                                B(VLC_CODEC_QDMC, "QDMC Audio"),
                                                                                                                                                    A("QDMC"),

                                                                                                                                                    /* QOA */
                                                                                                                                                    B(VLC_CODEC_QOA, "Quite OK Audio Format"),
                                                                                                                                                        A("qoaf"),

                                                                                                                                                        /* COOK */
                                                                                                                                                        B(VLC_CODEC_COOK, "Cook Audio"),
                                                                                                                                                            A("cook"),

                                                                                                                                                            /* TTA: The Lossless True Audio */
                                                                                                                                                            B(VLC_CODEC_TTA, "The Lossless True Audio"),
                                                                                                                                                                A("TTA1"),

                                                                                                                                                                /* Shorten */
                                                                                                                                                                B(VLC_CODEC_SHORTEN, "Shorten Lossless Audio"),
                                                                                                                                                                    A("shn "),
                                                                                                                                                                    A("shrn"),

                                                                                                                                                                B(VLC_CODEC_WAVPACK, "WavPack"),
                                                                                                                                                                    A("WVPK"),
                                                                                                                                                                    A("wvpk"),

                                                                                                                                                                B(VLC_CODEC_GSM, "GSM Audio"),
                                                                                                                                                                    A("gsm "),

                                                                                                                                                                B(VLC_CODEC_GSM_MS, "Microsoft GSM Audio"),
                                                                                                                                                                    A("agsm"),

                                                                                                                                                                B(VLC_CODEC_ATRAC1, "atrac 1"),
                                                                                                                                                                    A("atr1"),

                                                                                                                                                                B(VLC_CODEC_ATRAC3, "atrac 3"),
                                                                                                                                                                    A("atrc"),
                                                                                                                                                                    A("\x70\x02\x00\x00"),

                                                                                                                                                                B(VLC_CODEC_ATRAC3P, "atrac 3+"),
                                                                                                                                                                    A("atrp"),

                                                                                                                                                                B(VLC_CODEC_IMC, "IMC"),
                                                                                                                                                                    A("\x01\x04\x00\x00"),

                                                                                                                                                                B(VLC_CODEC_TRUESPEECH,"TrueSpeech"),
                                                                                                                                                                    A("\x22\x00\x00\x00"),

                                                                                                                                                                B(VLC_CODEC_NELLYMOSER, "NellyMoser ASAO"),
                                                                                                                                                                    A("NELL"),
                                                                                                                                                                    A("nmos"),

                                                                                                                                                                B(VLC_CODEC_APE, "Monkey's Audio"),
                                                                                                                                                                    A("APE "),

                                                                                                                                                                B(VLC_CODEC_MLP, "MLP/TrueHD Audio"),
                                                                                                                                                                    A("mlp "),

                                                                                                                                                                B(VLC_CODEC_TRUEHD, "TrueHD Audio"),
                                                                                                                                                                    A("trhd"),

                                                                                                                                                                B(VLC_CODEC_QCELP, "QCELP Audio"),
                                                                                                                                                                    A("Qclp"),
                                                                                                                                                                    A("Qclq"),
                                                                                                                                                                    A("sqcp"),

                                                                                                                                                                B(VLC_CODEC_CODEC2, "Codec 2 Audio"),

                                                                                                                                                                B(VLC_CODEC_SPEEX, "Speex Audio"),
                                                                                                                                                                    A("spx "),
                                                                                                                                                                    A("spxr"),

                                                                                                                                                                B(VLC_CODEC_VORBIS, "Vorbis Audio"),
                                                                                                                                                                    A("vorb"),
                                                                                                                                                                    A("vor1"),

                                                                                                                                                                B(VLC_CODEC_OPUS, "Opus Audio"),
                                                                                                                                                                    A("Opus"),
                                                                                                                                                                    A("opus"),

                                                                                                                                                                B(VLC_CODEC_302M, "302M Audio"),
                                                                                                                                                                    A("302m"),

                                                                                                                                                                B(VLC_CODEC_DVD_LPCM, "DVD LPCM Audio"),
                                                                                                                                                                    A("lpcm"),

                                                                                                                                                                B(VLC_CODEC_DVDA_LPCM, "DVD-Audio LPCM Audio"),
                                                                                                                                                                    A("apcm"),

                                                                                                                                                                B(VLC_CODEC_BD_LPCM, "BD LPCM Audio"),
                                                                                                                                                                    A("bpcm"),

                                                                                                                                                                B(VLC_CODEC_SDDS, "SDDS Audio"),
                                                                                                                                                                    A("sdds"),
                                                                                                                                                                    A("sddb"),

                                                                                                                                                                B(VLC_CODEC_MIDI, "MIDI Audio"),
                                                                                                                                                                    A("MIDI"),

                                                                                                                                                                B(VLC_CODEC_RALF, "RealAudio Lossless"),
                                                                                                                                                                    A("LSD:"),

                                                                                                                                                                    /* G.723.1 */
                                                                                                                                                                    B(VLC_CODEC_G723_1, "G.723.1 Audio"),
                                                                                                                                                                        A("g72\x31"),

                                                                                                                                                                        /* PCM */
                                                                                                                                                                        B(VLC_CODEC_S8, "PCM S8"),
                                                                                                                                                                            A("s8  "),

                                                                                                                                                                        B(VLC_CODEC_U8, "PCM U8"),
                                                                                                                                                                            A("u8  "),

                                                                                                                                                                        B(VLC_CODEC_S16L, "PCM S16 LE"),
                                                                                                                                                                            A("s16l"),

                                                                                                                                                                        B(VLC_CODEC_S16L_PLANAR, "PCM S16 LE planar"),

                                                                                                                                                                        B(VLC_CODEC_S16B, "PCM S16 BE"),
                                                                                                                                                                            A("s16b"),

                                                                                                                                                                        B(VLC_CODEC_U16L, "PCM U16 LE"),
                                                                                                                                                                            A("u16l"),

                                                                                                                                                                        B(VLC_CODEC_U16B, "PCM U16 BE"),
                                                                                                                                                                            A("u16b"),

                                                                                                                                                                        B(VLC_CODEC_S20B, "PCM S20 BE"),

                                                                                                                                                                        B(VLC_CODEC_S24L, "PCM S24 LE"),
                                                                                                                                                                            A("s24l"),
                                                                                                                                                                            A("42ni"),  /* Quicktime */

                                                                                                                                                                        B(VLC_CODEC_S24B, "PCM S24 BE"),
                                                                                                                                                                            A("s24b"),
                                                                                                                                                                            A("in24"),  /* Quicktime */

                                                                                                                                                                        B(VLC_CODEC_U24L, "PCM U24 LE"),
                                                                                                                                                                            A("u24l"),

                                                                                                                                                                        B(VLC_CODEC_U24B, "PCM U24 BE"),
                                                                                                                                                                            A("u24b"),

                                                                                                                                                                        B(VLC_CODEC_S24L32, "PCM S24 in 32 LE"),

                                                                                                                                                                        B(VLC_CODEC_S24B32, "PCM S24 in 32 BE"),

                                                                                                                                                                        B(VLC_CODEC_S32L, "PCM S32 LE"),
                                                                                                                                                                            A("s32l"),
                                                                                                                                                                            A("23ni"),  /* Quicktime */

                                                                                                                                                                        B(VLC_CODEC_S32B, "PCM S32 BE"),
                                                                                                                                                                            A("s32b"),
                                                                                                                                                                            A("in32"),  /* Quicktime */

                                                                                                                                                                        B(VLC_CODEC_U32L, "PCM U32 LE"),
                                                                                                                                                                            A("u32l"),

                                                                                                                                                                        B(VLC_CODEC_U32B, "PCM U32 BE"),
                                                                                                                                                                            A("u32b"),

                                                                                                                                                                        B(VLC_CODEC_ALAW, "PCM ALAW"),
                                                                                                                                                                            A("alaw"),

                                                                                                                                                                        B(VLC_CODEC_MULAW, "PCM MU-LAW"),
                                                                                                                                                                            A("mlaw"),
                                                                                                                                                                            A("ulaw"),

                                                                                                                                                                        B(VLC_CODEC_DAT12, "12 bits DAT audio"),

                                                                                                                                                                        B(VLC_CODEC_S24DAUD, "PCM DAUD"),
                                                                                                                                                                            A("daud"),

                                                                                                                                                                        B(VLC_CODEC_F32L, "32 bits float LE"),
                                                                                                                                                                            A("f32l"),
                                                                                                                                                                            A("fl32"),

                                                                                                                                                                        B(VLC_CODEC_F32B, "32 bits float BE"),
                                                                                                                                                                            A("f32b"),

                                                                                                                                                                        B(VLC_CODEC_F64L, "64 bits float LE"),
                                                                                                                                                                            A("f64l"),

                                                                                                                                                                        B(VLC_CODEC_F64B, "64 bits float BE"),
                                                                                                                                                                            A("f64b"),

                                                                                                                                                                        B(VLC_CODEC_TWINVQ, "TwinVQ"),
                                                                                                                                                                            A("TWIN"),

                                                                                                                                                                        B(VLC_CODEC_BMVAUDIO, "Discworld II BMV audio"),
                                                                                                                                                                            A("BMVA"),

                                                                                                                                                                        B(VLC_CODEC_ULEAD_DV_AUDIO_NTSC, "Ulead DV audio NTSC"),
                                                                                                                                                                            A("ms\x02\x15"),
                                                                                                                                                                        B(VLC_CODEC_ULEAD_DV_AUDIO_PAL, "Ulead DV audio PAL"),
                                                                                                                                                                            A("ms\x02\x16"),

                                                                                                                                                                        B(VLC_CODEC_INDEO_AUDIO, "Indeo Audio Coder"),
                                                                                                                                                                            A("ms\x04\x02"),

                                                                                                                                                                        B(VLC_CODEC_TAK, "TAK (Tom's lossless Audio Kompressor)"),

                                                                                                                                                                        B(VLC_CODEC_SMACKAUDIO, "Smacker audio"),

                                                                                                                                                                        B(VLC_CODEC_ADPCM_IMA_EA_SEAD, "ADPCM IMA Electronic Arts SEAD"),

                                                                                                                                                                        B(VLC_CODEC_ADPCM_EA_R1, "ADPCM Electronic Arts R1"),

                                                                                                                                                                        B(VLC_CODEC_ADPCM_EA_R3, "ADPCM Electronic Arts R3"),

                                                                                                                                                                        B(VLC_CODEC_ADPCM_IMA_APC, "ADPCM APC"),

                                                                                                                                                                        B(VLC_CODEC_DSD_LSBF, "DSD (Direct Stream Digital) LSB first"),

                                                                                                                                                                        B(VLC_CODEC_DSD_MSBF, "DSD (Direct Stream Digital) MSB first"),

                                                                                                                                                                        B(VLC_CODEC_DSD_LSBF_PLANAR, "DSD (Direct Stream Digital) LSB first, planar"),

                                                                                                                                                                        B(VLC_CODEC_DSD_MSBF_PLANAR, "DSD (Direct Stream Digital) MSB first, planar"),
};

static const staticentry_t p_list_spu[] = {

    B(VLC_CODEC_SPU, "DVD Subtitles"),
        A("spu "),
        A("spub"),

    B(VLC_CODEC_DVBS, "DVB Subtitles"),
        A("dvbs"),

    B(VLC_CODEC_SUBT, "Text subtitles with various tags"),
        A("subt"),

    B(VLC_CODEC_XSUB, "DivX XSUB subtitles"),
        A("XSUB"),
        A("xsub"),
        A("DXSB"),
        A("DXSA"),

    B(VLC_CODEC_SSA, "SubStation Alpha subtitles"),
        A("ssa "),

    B(VLC_CODEC_TEXT, "Plain text subtitles"),
        A("TEXT"),

    B(VLC_CODEC_TELETEXT, "Teletext"),
        A("telx"),

    B(VLC_CODEC_KATE, "Kate subtitles"),
        A("kate"),

    B(VLC_CODEC_CMML, "CMML annotations/metadata"),
        A("cmml"),

    B(VLC_CODEC_ITU_T140, "ITU T.140 subtitles"),
        A("t140"),

    B(VLC_CODEC_USF, "USF subtitles"),
        A("usf "),

    B(VLC_CODEC_OGT, "OGT subtitles"),
        A("ogt "),

    B(VLC_CODEC_CVD, "CVD subtitles"),
        A("cvd "),

    B(VLC_CODEC_ARIB_A, "ARIB subtitles (A-profile)"),
        A("arba"),

    B(VLC_CODEC_ARIB_C, "ARIB subtitles (C-profile)"),
        A("arbc"),

    B(VLC_CODEC_BD_PG, "BD PGS subtitles"),
        A("bdpg"),

    B(VLC_CODEC_BD_TEXT, "BD Text subtitles"),
        A("bdtx"),

    B(VLC_CODEC_EBU_STL, "EBU STL subtitles"),
        A("STL "),

    B(VLC_CODEC_SCTE_27, "SCTE-27 subtitles"),
        A("SC27"),

    B(VLC_CODEC_CEA608,  "EIA-608 subtitles"),
        A("cc1 "), /* acquisition devices */
        A("cc2 "),
        A("cc3 "),
        A("cc4 "),

    B(VLC_CODEC_TTML, "TTML subtitles"),
        A("ttml"),

    B(VLC_CODEC_TTML_TS, "TTML subtitles (EN.303.560)"),

    B(VLC_CODEC_WEBVTT, "WEBVTT subtitles"),
};


static int cmp_entry(const void* a, const void* b)
{
    const struct entry* ea = (const entry*)a, * eb = (const entry*)b;
    int d = memcmp(ea->alias, eb->alias, 4);
    if (d == 0)
        d = memcmp(ea->fourcc, eb->fourcc, 4);
    return d;
}

static void process_list(const char* name, const staticentry_t* list, size_t n)
{
    struct entry* entries = (entry*)malloc(sizeof(*entries) * n);
    if (entries == NULL)
        abort();

    const staticentry_t* klass = NULL;

    for (size_t i = 0; i < n; i++)
    {
        if (list[i].klass)
            klass = &list[i];

        if (klass == NULL)
        {
            fprintf(stderr, "Error: FourCC \"%.4s\" not mapped!\n",
                list[i].fourcc);
            exit(1);
        }

        memcpy(entries[i].fourcc, klass->fourcc, 4);
        memcpy(entries[i].alias, list[i].fourcc, 4);
        entries[i].desc = list[i].description;
    }

    qsort(entries, n, sizeof(*entries), cmp_entry);

    size_t dups = 0;
    for (size_t i = 1; i < n; i++)
        if (!memcmp(entries[i - 1].alias, entries[i].alias, 4)
            && memcmp(entries[i - 1].fourcc, entries[i].fourcc, 4))
        {
            fprintf(stderr, "Error: FourCC alias \"%.4s\" conflict: "
                "\"%.4s\" and \"%.4s\"\n", entries[i].alias,
                entries[i - 1].fourcc, entries[i].fourcc);
            dups++;
        }

    if (dups > 0)
        exit(1);

    printf("static const struct fourcc_mapping mapping_%s[] = {\n", name);
    for (size_t i = 0; i < n; i++)
    {
        if (!memcmp(entries[i].fourcc, entries[i].alias, 4))
            continue;
        printf("    { { { 0x%02hhx, 0x%02hhx, 0x%02hhx, 0x%02hhx } }, "
            "{ { 0x%02hhx, 0x%02hhx, 0x%02hhx, 0x%02hhx } } },\n",
            entries[i].alias[0], entries[i].alias[1], entries[i].alias[2],
            entries[i].alias[3], entries[i].fourcc[0], entries[i].fourcc[1],
            entries[i].fourcc[2], entries[i].fourcc[3]);
    }
    puts("};");
    printf("static const struct fourcc_desc desc_%s[] = {\n", name);
    for (size_t i = 0; i < n; i++)
    {
        if (entries[i].desc == NULL)
            continue;
        printf("    { { { 0x%02hhx, 0x%02hhx, 0x%02hhx, 0x%02hhx } }, "
            "\"%s\" },\n", entries[i].alias[0], entries[i].alias[1],
            entries[i].alias[2], entries[i].alias[3], entries[i].desc);
    }
    puts("};");

    free(entries);
    fprintf(stderr, "%s: %zu entries\n", name, n);
}

int main()
{
    puts("/* This file is generated automatically. DO NOT EDIT! */");
    puts("struct fourcc_mapping {");
    puts("    union { unsigned char alias_str[4]; vlc_fourcc_t alias; };");
    puts("    union { unsigned char fourcc_str[4]; vlc_fourcc_t fourcc; };");
    puts("};");
    puts("struct fourcc_desc {");
    puts("    union { unsigned char alias_str[4]; vlc_fourcc_t alias; };");
    puts("    const char desc[52];");
    puts("};");

#define p(t) \
    process_list(#t, p_list_##t, \
                 sizeof (p_list_##t) / sizeof ((p_list_##t)[0]))
    p(video);
    p(audio);
    p(spu);
    return 0;
}


//int main(int argc, char* argv[]) {
//	/*QApplication application(argc, argv);
//	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();
//
//	const auto ratio = application.devicePixelRatio();
//	const auto useRatio = std::clamp(qCeil(ratio), 1, 3);
//
//	base::qt::style::SetDevicePixelRatio(useRatio);
//
//	base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
//	base::qt::style::internal::StartFonts();
//
//	base::qt::style::RegisterStyles();
//
//	base::qt::style::Start();
//
//	QWidget widget;
//
//	widget.resize(1280, 720);
//	widget.setMouseTracking(true);
//
//	base::qt::ui::FlatLabel label(&widget);
//
//	label.setText("Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст *Длинный текст* ```Длинный текст```");
//	widget.show();
//	label.move((widget.width() - label.width()) / 2,
//		(widget.height() - label.height()) / 2);
//
//	return application.exec();*/
//	return 0;
//}