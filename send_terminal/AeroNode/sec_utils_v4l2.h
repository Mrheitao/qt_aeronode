/*
 * Copyright@ Samsung Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*
 * file sec_utils_v4l2.h
 * brief header file for sec_utils_v4l2.h
 * author Sangwoo, Park(sw5771.park@samsung.com)
 * date 2011/06/02
 *
 * Revision History:
 * - 2010/06/03 : Sangwoo, Park(sw5771.park@samsung.com)
 * Initial version
 *
 * - 2011/12/07 : Jeonghee, Kim(jhhhh.kim@samsung.com)
 * use V4L2_PIX_FMT_NV12M as HAL_PIXEL_FORMAT_YCbCr_420_SP, HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP
 *
 * - 2011/12/07 : Hyeonmyeong Choi( hyeon.choi@samsung.com)
 * Add V4L2_PIX_FMT_YVU420M
 *
 */
#ifndef __SAMSUNG_SYSLSI_SEC_COMMON_H__
#define __SAMSUNG_SYSLSI_SEC_COMMON_H__

//---------------------------------------------------------//
// Include
//---------------------------------------------------------//

#include <hardware/hardware.h>
#include "sec_format.h"

#include <linux/videodev2.h>

#ifdef __cplusplus
}
#endif

//#include <utils/Log.h>
#include <linux/videodev2.h>


/* cacheable configuration */
#define V4L2_CID_CACHEABLE                      (V4L2_CID_BASE+40)

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG     v4l2_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV       v4l2_fourcc('d', 'v', 's', 'd') /* 1394          */
#define V4L2_PIX_FMT_MPEG     v4l2_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4    */


#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264    */
#define V4L2_PIX_FMT_H263     v4l2_fourcc('H', '2', '6', '3') /* H263    */
#define V4L2_PIX_FMT_MPEG12   v4l2_fourcc('M', 'P', '1', '2') /* MPEG-1/2  */
#define V4L2_PIX_FMT_MPEG4    v4l2_fourcc('M', 'P', 'G', '4') /* MPEG-4  */
#define V4L2_PIX_FMT_FIMV     v4l2_fourcc('F', 'I', 'M', 'V') /* FIMV  */
#define V4L2_PIX_FMT_FIMV1    v4l2_fourcc('F', 'I', 'M', '1') /* FIMV1 */
#define V4L2_PIX_FMT_FIMV2    v4l2_fourcc('F', 'I', 'M', '2') /* FIMV2 */
#define V4L2_PIX_FMT_FIMV3    v4l2_fourcc('F', 'I', 'M', '3') /* FIMV3 */
#define V4L2_PIX_FMT_FIMV4    v4l2_fourcc('F', 'I', 'M', '4') /* FIMV4 */
#define V4L2_PIX_FMT_XVID     v4l2_fourcc('X', 'V', 'I', 'D') /* Xvid */
#define V4L2_PIX_FMT_VC1      v4l2_fourcc('V', 'C', '1', 'A') /* VC-1 */
#define V4L2_PIX_FMT_VC1_RCV      v4l2_fourcc('V', 'C', '1', 'R') /* VC-1 RCV */
#define V4L2_PIX_FMT_VP8      v4l2_fourcc('V', 'P', '8', '0') /* VP8 */

/* common */
enum v4l2_codec_mfc5x_enc_switch {
        V4L2_CODEC_MFC5X_ENC_SW_DISABLE = 0,
        V4L2_CODEC_MFC5X_ENC_SW_ENABLE  = 1,
};
enum v4l2_codec_mfc5x_enc_switch_inv {
        V4L2_CODEC_MFC5X_ENC_SW_INV_ENABLE      = 0,
        V4L2_CODEC_MFC5X_ENC_SW_INV_DISABLE     = 1,
};

#define V4L2_CID_CODEC_MFC5X_ENC_GOP_SIZE               (V4L2_CID_CODEC_MFC5X_BASE+300)
#define V4L2_CID_CODEC_MFC5X_ENC_MULTI_SLICE_MODE       (V4L2_CID_CODEC_MFC5X_BASE+301)
enum v4l2_codec_mfc5x_enc_multi_slice_mode {
        V4L2_CODEC_MFC5X_ENC_MULTI_SLICE_MODE_DISABLE           = 0,
        V4L2_CODEC_MFC5X_ENC_MULTI_SLICE_MODE_MACROBLOCK_COUNT  = 1,
        V4L2_CODEC_MFC5X_ENC_MULTI_SLICE_MODE_BIT_COUNT         = 3,
};

#define V4L2_CID_CODEC_MFC5X_ENC_MULTI_SLICE_MB         (V4L2_CID_CODEC_MFC5X_BASE+302)
#define V4L2_CID_CODEC_MFC5X_ENC_MULTI_SLICE_BIT        (V4L2_CID_CODEC_MFC5X_BASE+303)
#define V4L2_CID_CODEC_MFC5X_ENC_INTRA_REFRESH_MB       (V4L2_CID_CODEC_MFC5X_BASE+304)
#define V4L2_CID_CODEC_MFC5X_ENC_PAD_CTRL_ENABLE        (V4L2_CID_CODEC_MFC5X_BASE+305)
#define V4L2_CID_CODEC_MFC5X_ENC_PAD_LUMA_VALUE         (V4L2_CID_CODEC_MFC5X_BASE+306)
#define V4L2_CID_CODEC_MFC5X_ENC_PAD_CB_VALUE           (V4L2_CID_CODEC_MFC5X_BASE+307)
#define V4L2_CID_CODEC_MFC5X_ENC_PAD_CR_VALUE           (V4L2_CID_CODEC_MFC5X_BASE+308)
#define V4L2_CID_CODEC_MFC5X_ENC_RC_FRAME_ENABLE        (V4L2_CID_CODEC_MFC5X_BASE+309)
#define V4L2_CID_CODEC_MFC5X_ENC_RC_BIT_RATE            (V4L2_CID_CODEC_MFC5X_BASE+310)
#define V4L2_CID_CODEC_MFC5X_ENC_RC_REACTION_COEFF      (V4L2_CID_CODEC_MFC5X_BASE+311)
enum v4l2_codec_mfc5x_enc_frame_type {
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_NOT_CODED       = 0,
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_I_FRAME         = 1,
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_P_FRAME         = 2,
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_B_FRAME         = 3,
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_SKIPPED         = 4,
        V4L2_CODEC_MFC5X_ENC_FRAME_TYPE_OTHERS          = 5,
};
enum v4l2_codec_mfc5x_enc_force_frame_type {
        V4L2_CODEC_MFC5X_ENC_FORCE_FRAME_TYPE_I_FRAME   = 1,
        V4L2_CODEC_MFC5X_ENC_FORCE_FRAME_TYPE_NOT_CODED = 2,
};
#define V4L2_CID_CODEC_MFC5X_ENC_VBV_BUF_SIZE           (V4L2_CID_CODEC_MFC5X_BASE+316)
#define V4L2_CID_CODEC_MFC5X_ENC_SEQ_HDR_MODE           (V4L2_CID_CODEC_MFC5X_BASE+317)
enum v4l2_codec_mfc5x_enc_seq_hdr_mode {
        V4L2_CODEC_MFC5X_ENC_SEQ_HDR_MODE_SEQ           = 0,
        V4L2_CODEC_MFC5X_ENC_SEQ_HDR_MODE_SEQ_FRAME     = 1,
};
#define V4L2_CID_CODEC_MFC5X_ENC_FRAME_SKIP_MODE        (V4L2_CID_CODEC_MFC5X_BASE+318)
enum v4l2_codec_mfc5x_enc_frame_skip_mode {
        V4L2_CODEC_MFC5X_ENC_FRAME_SKIP_MODE_DISABLE            = 0,
        V4L2_CODEC_MFC5X_ENC_FRAME_SKIP_MODE_LEVEL              = 1,
        V4L2_CODEC_MFC5X_ENC_FRAME_SKIP_MODE_VBV_BUF_SIZE       = 2,
};
#define V4L2_CID_CODEC_MFC5X_ENC_RC_FIXED_TARGET_BIT    (V4L2_CID_CODEC_MFC5X_BASE+319)

/* codec specific */
#define V4L2_CID_CODEC_MFC5X_ENC_H264_B_FRAMES          (V4L2_CID_CODEC_MFC5X_BASE+400)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_PROFILE           (V4L2_CID_CODEC_MFC5X_BASE+401)
enum v4l2_codec_mfc5x_enc_h264_profile {
        V4L2_CODEC_MFC5X_ENC_H264_PROFILE_MAIN          = 0,
        V4L2_CODEC_MFC5X_ENC_H264_PROFILE_HIGH          = 1,
        V4L2_CODEC_MFC5X_ENC_H264_PROFILE_BASELINE      = 2,
};
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LEVEL             (V4L2_CID_CODEC_MFC5X_BASE+402)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_INTERLACE         (V4L2_CID_CODEC_MFC5X_BASE+403)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LOOP_FILTER_MODE  (V4L2_CID_CODEC_MFC5X_BASE+404)
enum v4l2_codec_mfc5x_enc_h264_loop_filter {
        V4L2_CODEC_MFC5X_ENC_H264_LOOP_FILTER_ENABLE                    = 0,
        V4L2_CODEC_MFC5X_ENC_H264_LOOP_FILTER_DISABLE                   = 1,
        V4L2_CODEC_MFC5X_ENC_H264_LOOP_FILTER_DISABLE_AT_BOUNDARY       = 2,
};
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LOOP_FILTER_ALPHA (V4L2_CID_CODEC_MFC5X_BASE+405)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LOOP_FILTER_BETA  (V4L2_CID_CODEC_MFC5X_BASE+406)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_ENTROPY_MODE      (V4L2_CID_CODEC_MFC5X_BASE+407)
enum v4l2_codec_mfc5x_enc_h264_entropy_mode {
        V4L2_CODEC_MFC5X_ENC_H264_ENTROPY_MODE_CAVLC    = 0,
        V4L2_CODEC_MFC5X_ENC_H264_ENTROPY_MODE_CABAC    = 1,
};

#define V4L2_CID_CODEC_MFC5X_ENC_H264_MAX_REF_PIC       (V4L2_CID_CODEC_MFC5X_BASE+408)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_NUM_REF_PIC_4P    (V4L2_CID_CODEC_MFC5X_BASE+409)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_8X8_TRANSFORM     (V4L2_CID_CODEC_MFC5X_BASE+410)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MB_ENABLE      (V4L2_CID_CODEC_MFC5X_BASE+411)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_FRAME_RATE     (V4L2_CID_CODEC_MFC5X_BASE+412)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_FRAME_QP       (V4L2_CID_CODEC_MFC5X_BASE+413)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MIN_QP         (V4L2_CID_CODEC_MFC5X_BASE+414)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MAX_QP         (V4L2_CID_CODEC_MFC5X_BASE+415)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MB_DARK        (V4L2_CID_CODEC_MFC5X_BASE+416)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MB_SMOOTH      (V4L2_CID_CODEC_MFC5X_BASE+417)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MB_STATIC      (V4L2_CID_CODEC_MFC5X_BASE+418)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_MB_ACTIVITY    (V4L2_CID_CODEC_MFC5X_BASE+419)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_P_FRAME_QP     (V4L2_CID_CODEC_MFC5X_BASE+420)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_RC_B_FRAME_QP     (V4L2_CID_CODEC_MFC5X_BASE+421)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_AR_VUI_ENABLE     (V4L2_CID_CODEC_MFC5X_BASE+422)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_AR_VUI_IDC        (V4L2_CID_CODEC_MFC5X_BASE+423)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_EXT_SAR_WIDTH     (V4L2_CID_CODEC_MFC5X_BASE+424)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_EXT_SAR_HEIGHT    (V4L2_CID_CODEC_MFC5X_BASE+425)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_OPEN_GOP          (V4L2_CID_CODEC_MFC5X_BASE+426)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_I_PERIOD          (V4L2_CID_CODEC_MFC5X_BASE+427)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_HIER_P_ENABLE     (V4L2_CID_CODEC_MFC5X_BASE+428)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LAYER0_QP         (V4L2_CID_CODEC_MFC5X_BASE+429)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LAYER1_QP         (V4L2_CID_CODEC_MFC5X_BASE+430)
#define V4L2_CID_CODEC_MFC5X_ENC_H264_LAYER2_QP         (V4L2_CID_CODEC_MFC5X_BASE+431)

#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_B_FRAMES         (V4L2_CID_CODEC_MFC5X_BASE+440)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_PROFILE          (V4L2_CID_CODEC_MFC5X_BASE+441)
enum v4l2_codec_mfc5x_enc_mpeg4_profile {
        V4L2_CODEC_MFC5X_ENC_MPEG4_PROFILE_SIMPLE               = 0,
        V4L2_CODEC_MFC5X_ENC_MPEG4_PROFILE_ADVANCED_SIMPLE      = 1,
};
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_LEVEL            (V4L2_CID_CODEC_MFC5X_BASE+442)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_FRAME_QP      (V4L2_CID_CODEC_MFC5X_BASE+443)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_MIN_QP        (V4L2_CID_CODEC_MFC5X_BASE+444)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_MAX_QP        (V4L2_CID_CODEC_MFC5X_BASE+445)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_QUARTER_PIXEL    (V4L2_CID_CODEC_MFC5X_BASE+446)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_P_FRAME_QP    (V4L2_CID_CODEC_MFC5X_BASE+447)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_B_FRAME_QP    (V4L2_CID_CODEC_MFC5X_BASE+448)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_VOP_TIME_RES     (V4L2_CID_CODEC_MFC5X_BASE+449)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_VOP_FRM_DELTA    (V4L2_CID_CODEC_MFC5X_BASE+450)
#define V4L2_CID_CODEC_MFC5X_ENC_MPEG4_RC_MB_ENABLE     (V4L2_CID_CODEC_MFC5X_BASE+451)

#define V4L2_CID_CODEC_MFC5X_ENC_H263_RC_FRAME_RATE     (V4L2_CID_CODEC_MFC5X_BASE+460)
#define V4L2_CID_CODEC_MFC5X_ENC_H263_RC_FRAME_QP       (V4L2_CID_CODEC_MFC5X_BASE+461)
#define V4L2_CID_CODEC_MFC5X_ENC_H263_RC_MIN_QP         (V4L2_CID_CODEC_MFC5X_BASE+462)
#define V4L2_CID_CODEC_MFC5X_ENC_H263_RC_MAX_QP         (V4L2_CID_CODEC_MFC5X_BASE+463)
#define V4L2_CID_CODEC_MFC5X_ENC_H263_RC_P_FRAME_QP     (V4L2_CID_CODEC_MFC5X_BASE+464)

#define V4L2_CTRL_CLASS_CODEC 0x009c0000 

/* For codecs */

#define V4L2_CID_CODEC_BASE                     (V4L2_CTRL_CLASS_CODEC | 0x900)
#define V4L2_CID_CODEC_CLASS                    (V4L2_CTRL_CLASS_CODEC | 1)

/* Codec class control IDs specific to the MFC5X driver */
#define V4L2_CID_CODEC_MFC5X_BASE               (V4L2_CTRL_CLASS_CODEC | 0x1000)

/* For decoding */

#define V4L2_CID_CODEC_LOOP_FILTER_MPEG4_ENABLE (V4L2_CID_CODEC_BASE + 110)
#define V4L2_CID_CODEC_DISPLAY_DELAY            (V4L2_CID_CODEC_BASE + 137)
#define V4L2_CID_CODEC_SLICE_INTERFACE          (V4L2_CID_CODEC_BASE + 141)
#define V4L2_CID_CODEC_CRC_ENABLE               (V4L2_CID_CODEC_BASE + 144)
#define V4L2_CID_CODEC_CRC_DATA_LUMA            (V4L2_CID_CODEC_BASE + 145)
#define V4L2_CID_CODEC_CRC_DATA_CHROMA          (V4L2_CID_CODEC_BASE + 146)

//---------------------------------------------------------//
// Common structure                                        //
//---------------------------------------------------------//
struct ADDRS {
    unsigned int addr_y;
    unsigned int addr_cbcr;
    unsigned int buf_idx;
    unsigned int reserved;
};

#define V4L2_PIX_FMT_YVU420M v4l2_fourcc('Y', 'V', 'U', 'M') /* 12  YVU420 planar */
#define V4L2_PIX_FMT_NV21M        v4l2_fourcc('N', 'M', '2', '1') /* 21  Y/CrCb 4:2:0  */
//---------------------------------------------------------//
// Common function                                         //
//---------------------------------------------------------//
inline int HAL_PIXEL_FORMAT_2_V4L2_PIX(int HAL_PIXEL_FORMAT)
{
    int V4L2_PIX = -1;

    switch (HAL_PIXEL_FORMAT) {
    case HAL_PIXEL_FORMAT_RGBA_8888:
    case HAL_PIXEL_FORMAT_RGBX_8888:
        V4L2_PIX = V4L2_PIX_FMT_RGB32;
        break;

    case HAL_PIXEL_FORMAT_RGB_888:
        V4L2_PIX = V4L2_PIX_FMT_RGB24;
        break;

    case HAL_PIXEL_FORMAT_RGB_565:
        V4L2_PIX = V4L2_PIX_FMT_RGB565;
        break;

    case HAL_PIXEL_FORMAT_BGRA_8888:
        V4L2_PIX = V4L2_PIX_FMT_BGR32;
        break;

    case HAL_PIXEL_FORMAT_RGBA_5551:
        V4L2_PIX = V4L2_PIX_FMT_RGB555X;
        break;

    case HAL_PIXEL_FORMAT_RGBA_4444:
        V4L2_PIX = V4L2_PIX_FMT_RGB444;
        break;

    case HAL_PIXEL_FORMAT_YV12:
        V4L2_PIX = V4L2_PIX_FMT_NV12M;
        break;

    case HAL_PIXEL_FORMAT_YCbCr_420_P:
        V4L2_PIX = V4L2_PIX_FMT_YUV420M;
        break;

    case HAL_PIXEL_FORMAT_YCbCr_422_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_SP:
        V4L2_PIX = V4L2_PIX_FMT_NV61;
        break;

    case HAL_PIXEL_FORMAT_YCbCr_420_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP:
        V4L2_PIX = V4L2_PIX_FMT_NV12M;
        break;

    case HAL_PIXEL_FORMAT_YCbCr_422_I:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_I:
        V4L2_PIX = V4L2_PIX_FMT_YUYV;
        break;

    case HAL_PIXEL_FORMAT_YCbCr_422_P:
        V4L2_PIX = V4L2_PIX_FMT_YUV422P;
        break;

    case HAL_PIXEL_FORMAT_CbYCrY_422_I:
    case HAL_PIXEL_FORMAT_CUSTOM_CbYCrY_422_I:
        V4L2_PIX = V4L2_PIX_FMT_UYVY;
        break;

    case HAL_PIXEL_FORMAT_YCrCb_422_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_SP:
        V4L2_PIX = V4L2_PIX_FMT_NV16;
        break;

    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_420_SP:
        V4L2_PIX = V4L2_PIX_FMT_NV21;
        break;

    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP_TILED:
        V4L2_PIX = V4L2_PIX_FMT_NV12MT;
        break;

   case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_I:
        V4L2_PIX = V4L2_PIX_FMT_YVYU;
        break;

   case HAL_PIXEL_FORMAT_CUSTOM_CrYCbY_422_I:
        V4L2_PIX = V4L2_PIX_FMT_VYUY;
        break;

   case HAL_PIXEL_FORMAT_JPEG:
        V4L2_PIX = V4L2_PIX_FMT_JPEG;
       break;

    default:
        printf("%s::unmatched HAL_PIXEL_FORMAT color_space(0x%x)\n",
                __func__, HAL_PIXEL_FORMAT);
        break;
    }

    return V4L2_PIX;
}

inline int V4L2_PIX_2_HAL_PIXEL_FORMAT(int V4L2_PIX)
{
    int HAL_PIXEL_FORMAT = -1;

    switch (V4L2_PIX) {
    case V4L2_PIX_FMT_RGB32:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_RGBA_8888;
        break;

    case V4L2_PIX_FMT_RGB24:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_RGB_888;
        break;

    case V4L2_PIX_FMT_RGB565:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_RGB_565;
        break;

    case V4L2_PIX_FMT_BGR32:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_BGRA_8888;
        break;

    case V4L2_PIX_FMT_RGB555X:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_RGBA_5551;
        break;

    case V4L2_PIX_FMT_RGB444:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_RGBA_4444;
        break;

    case V4L2_PIX_FMT_YUV420:
    case V4L2_PIX_FMT_YUV420M:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_YCbCr_420_P;
        break;

    case V4L2_PIX_FMT_YVU420:
    case V4L2_PIX_FMT_YVU420M:
         HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_YV12;
         break;

    case V4L2_PIX_FMT_NV16:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_SP;
        break;

    case V4L2_PIX_FMT_NV12:
    case V4L2_PIX_FMT_NV12M:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP;
        break;

    case V4L2_PIX_FMT_YUYV:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_I;
        break;

    case V4L2_PIX_FMT_YUV422P:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_YCbCr_422_P;
        break;

    case V4L2_PIX_FMT_UYVY:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_CbYCrY_422_I;
        break;

    case V4L2_PIX_FMT_NV21:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCrCb_420_SP;
        break;
    case V4L2_PIX_FMT_NV12MT:

        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP_TILED;
        break;

    case V4L2_PIX_FMT_NV61:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_SP;
        break;

    case V4L2_PIX_FMT_YVYU:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_I;
        break;

    case V4L2_PIX_FMT_VYUY:
        HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_CUSTOM_CrYCbY_422_I;
        break;

    case V4L2_PIX_FMT_JPEG:
       HAL_PIXEL_FORMAT = HAL_PIXEL_FORMAT_JPEG;
       break;

    default:
        printf("%s::unmatched V4L2_PIX color_space(%d)\n",
                __func__, V4L2_PIX);
        break;
    }

    return HAL_PIXEL_FORMAT;
}

#define ALIGN_TO_32B(x)   ((((x) + (1 <<  5) - 1) >>  5) <<  5)
#define ALIGN_TO_128B(x)  ((((x) + (1 <<  7) - 1) >>  7) <<  7)
#define ALIGN_TO_8KB(x)   ((((x) + (1 << 13) - 1) >> 13) << 13)
#define ALIGN(x, a)       (((x) + (a) - 1) & ~((a) - 1))

#define GET_32BPP_FRAME_SIZE(w, h)  (((w) * (h)) << 2)
#define GET_24BPP_FRAME_SIZE(w, h)  (((w) * (h)) * 3)
#define GET_16BPP_FRAME_SIZE(w, h)  (((w) * (h)) << 1)

inline unsigned int FRAME_SIZE(int HAL_PIXEL_FORMAT, int w, int h)
{
    unsigned int frame_size = 0;
    unsigned int size       = 0;

    switch (HAL_PIXEL_FORMAT) {
    // 16bpp
    case HAL_PIXEL_FORMAT_RGB_565:
    case HAL_PIXEL_FORMAT_RGBA_5551:
    case HAL_PIXEL_FORMAT_RGBA_4444:
        frame_size = GET_16BPP_FRAME_SIZE(w, h);
        break;

    // 24bpp
    case HAL_PIXEL_FORMAT_RGB_888:
        frame_size = GET_24BPP_FRAME_SIZE(w, h);
        break;

    // 32bpp
    case HAL_PIXEL_FORMAT_RGBA_8888:
    case HAL_PIXEL_FORMAT_BGRA_8888:
    case HAL_PIXEL_FORMAT_RGBX_8888:
        frame_size = GET_32BPP_FRAME_SIZE(w, h);
        break;

    // 12bpp
    case HAL_PIXEL_FORMAT_YV12:
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
    case HAL_PIXEL_FORMAT_YCbCr_420_P:
    case HAL_PIXEL_FORMAT_YCbCr_420_I:
    case HAL_PIXEL_FORMAT_CbYCrY_420_I:
    case HAL_PIXEL_FORMAT_YCbCr_420_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_420_SP:
        size = w * h;
        frame_size = size + ((size >> 2) << 1);
        break;

    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP_TILED:
        frame_size =   ALIGN_TO_8KB(ALIGN_TO_128B(w) * ALIGN_TO_32B(h))
                     + ALIGN_TO_8KB(ALIGN_TO_128B(w) * ALIGN_TO_32B(h >> 1));
        break;

    // 16bpp
    case HAL_PIXEL_FORMAT_YCbCr_422_SP:
    case HAL_PIXEL_FORMAT_YCbCr_422_I:
    case HAL_PIXEL_FORMAT_YCbCr_422_P:
    case HAL_PIXEL_FORMAT_CbYCrY_422_I:
    case HAL_PIXEL_FORMAT_YCrCb_422_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_SP:
    case HAL_PIXEL_FORMAT_CUSTOM_YCbCr_422_I:
    case HAL_PIXEL_FORMAT_CUSTOM_YCrCb_422_I:
    case HAL_PIXEL_FORMAT_CUSTOM_CbYCrY_422_I:
    case HAL_PIXEL_FORMAT_CUSTOM_CrYCbY_422_I:
        frame_size = GET_16BPP_FRAME_SIZE(w, h);
        break;

    case HAL_PIXEL_FORMAT_JPEG:
       /* TODO: Verify if frame size calculation is right or not */
       frame_size = w * h;
       break;

    default:
        printf("%s::no matching source colorformat(0x%x), w(%d), h(%d) fail\n",
                __func__, HAL_PIXEL_FORMAT, w, h);
        break;
    }

    return frame_size;
}

#endif //__SAMSUNG_SYSLSI_SEC_COMMON_H__
