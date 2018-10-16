/*******************************************************************************
*                                 AWorks
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief iMX RT1050 Pixel Pipeline driver
 *
 * \internal
 * \par Modification History
 * - 1.00  18-04-28  mex, first implementation.
 * \endinternal
 */

#ifndef __AWBL_IMX1050_PXP_H
#define __AWBL_IMX1050_PXP_H

#include "apollo.h"
#include "aw_gpio.h"
#include "awbus_lite.h"
#include "aw_sem.h"
#include "aw_gpio.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */

/** \brief iMX RT1050 PXP 驱动名 */
#define IMX1050_PXP_NAME            "imx1050_pxp"


/**
 * \brief PXP process surface buffer pixel format.
 */
typedef enum pxp_ps_pixel_format {
    kPXP_PsPixelFormatRGB888 = 0x4,      /**< \brief 32-bit pixels without alpha (unpacked 24-bit format) */
    kPXP_PsPixelFormatRGB555 = 0xC,      /**< \brief 16-bit pixels without alpha. */
    kPXP_PsPixelFormatRGB444 = 0xD,      /**< \brief 16-bit pixels without alpha. */
    kPXP_PsPixelFormatRGB565 = 0xE,      /**< \brief 16-bit pixels without alpha. */
    kPXP_PsPixelFormatYUV1P444 = 0x10,   /**< \brief 32-bit pixels (1-plane XYUV unpacked). */
    kPXP_PsPixelFormatUYVY1P422 = 0x12,  /**< \brief 16-bit pixels (1-plane U0,Y0,V0,Y1 interleaved bytes) */
    kPXP_PsPixelFormatVYUY1P422 = 0x13,  /**< \brief 16-bit pixels (1-plane V0,Y0,U0,Y1 interleaved bytes) */
    kPXP_PsPixelFormatY8 = 0x14,        /**< \brief 8-bit monochrome pixels (1-plane Y luma output) */
    kPXP_PsPixelFormatY4 = 0x15,        /**< \brief 4-bit monochrome pixels (1-plane Y luma, 4 bit truncation) */
    kPXP_PsPixelFormatYUV2P422 = 0x18,   /**< \brief 16-bit pixels (2-plane UV interleaved bytes) */
    kPXP_PsPixelFormatYUV2P420 = 0x19,   /**< \brief 16-bit pixels (2-plane UV) */
    kPXP_PsPixelFormatYVU2P422 = 0x1A,   /**< \brief 16-bit pixels (2-plane VU interleaved bytes) */
    kPXP_PsPixelFormatYVU2P420 = 0x1B,   /**< \brief 16-bit pixels (2-plane VU) */
    kPXP_PsPixelFormatYVU422 = 0x1E,     /**< \brief 16-bit pixels (3-plane) */
    kPXP_PsPixelFormatYVU420 = 0x1F,     /**< \brief 16-bit pixels (3-plane) */
} pxp_ps_pixel_format_t;


/**
 * \brief PXP process surface buffer configuration.
 */
typedef struct pxp_ps_buffer_config {

    pxp_ps_pixel_format_t pixel_format; /**< \brief PS buffer pixel format. */
    uint8_t  swap_byte;                 /**< \brief For each 16 bit word, set true to swap the two bytes. */
    uint32_t buffer_addr;               /**< \brief Input buffer address for the first panel. */
    uint32_t buffer_addr_u;             /**< \brief Input buffer address for the second panel. */
    uint32_t buffer_addr_v;             /**< \brief Input buffer address for the third panel. */
    uint16_t pitch_bytes;               /**< \brief Number of bytes between two vertically adjacent pixels. */
} pxp_ps_buffer_config_t;


/**
 * \brief  PXP alpha surface buffer pixel format.
 */
typedef enum pxp_as_pixel_format {
    kPXP_AsPixelFormatARGB8888 = 0x0,  /**< \brief 32-bit pixels with alpha. */
    kPXP_AsPixelFormatRGB888 = 0x4,    /**< \brief 32-bit pixels without alpha (unpacked 24-bit format) */
    kPXP_AsPixelFormatARGB1555 = 0x8,  /**< \brief 16-bit pixels with alpha. */
    kPXP_AsPixelFormatARGB4444 = 0x9,  /**< \brief 16-bit pixels with alpha. */
    kPXP_AsPixelFormatRGB555 = 0xC,    /**< \brief 16-bit pixels without alpha. */
    kPXP_AsPixelFormatRGB444 = 0xD,    /**< \brief 16-bit pixels without alpha. */
    kPXP_AsPixelFormatRGB565 = 0xE,    /**< \brief 16-bit pixels without alpha. */
} pxp_as_pixel_format_t;


/**
 * \brief  PXP alphs surface buffer configuration.
 */
typedef struct pxp_as_buffer_config {
    pxp_as_pixel_format_t pixel_format; /**< \brief AS buffer pixel format. */
    uint32_t buffer_addr;               /**< \brief Input buffer address. */
    uint16_t pitch_bytes;               /**< \brief Number of bytes between two vertically adjacent pixels. */
} pxp_as_buffer_config_t;


/**
 * \brief  PXP ROP mode during blending.
 *
 * Explanation:
 * - AS: Alpha surface
 * - PS: Process surface
 * - nAS: Alpha surface NOT value
 * - nPS: Process surface NOT value
 */
typedef enum pxp_rop_mode {
    kPXP_RopMaskAs = 0x0,     /**< \brief AS AND PS. */
    kPXP_RopMaskNotAs = 0x1,  /**< \brief nAS AND PS. */
    kPXP_RopMaskAsNot = 0x2,  /**< \brief AS AND nPS. */
    kPXP_RopMergeAs = 0x3,    /**< \brief AS OR PS. */
    kPXP_RopMergeNotAs = 0x4, /**< \brief nAS OR PS. */
    kPXP_RopMergeAsNot = 0x5, /**< \brief AS OR nPS. */
    kPXP_RopNotCopyAs = 0x6,  /**< \brief nAS. */
    kPXP_RopNot = 0x7,        /**< \brief nPS. */
    kPXP_RopNotMaskAs = 0x8,  /**< \brief AS NAND PS. */
    kPXP_RopNotMergeAs = 0x9, /**< \brief AS NOR PS. */
    kPXP_RopXorAs = 0xA,      /**< \brief AS XOR PS. */
    kPXP_RopNotXorAs = 0xB    /**< \brief AS XNOR PS. */
} pxp_rop_mode_t;


/**
 * \brief PXP alpha mode during blending.
 */
typedef enum pxp_alpha_mode {
    kPXP_AlphaEmbedded, /**< \brief The alpha surface pixel alpha value will be used for blend. */
    kPXP_AlphaOverride, /**< \brief The user defined alpha value will be used for blend directly. */
    kPXP_AlphaMultiply, /**< \brief The alpha surface pixel alpha value scaled the user defined
                             alpha value will be used for blend, for example, pixel alpha set
                             set to 200, user defined alpha set to 100, then the reault alpha
                             is 200 * 100 / 255. */
    kPXP_AlphaRop       /**< \brief Raster operation. */
} pxp_alpha_mode_t;


/**
 * \brief PXP interlaced output mode.
 */
typedef enum pxp_interlaced_output_mode {
    kPXP_OutputProgressive = 0,     /**< \brief All data written in progressive format to output buffer 0. */
    kPXP_OutputField0,              /**< \brief Only write field 0 data to output buffer 0. */
    kPXP_OutputField1,              /**< \brief Only write field 1 data to output buffer 0. */
    kPXP_OutputInterlaced,          /**< \brief Field 0 write to buffer 0, field 1 write to buffer 1. */
} pxp_interlaced_output_mode_t;


/**
 * \brief PXP rotate mode.
 */
typedef enum pxp_rotate_position {

    kPXP_RotateOutputBuffer = 0,      /**< \brief rotate the output buffer. */
    kPXP_RotateProcessSurface,        /**< \brief Rotate the process surface. */
} pxp_rotate_position_t;


/**
 * \brief PXP rotate degree.
 */
typedef enum pxp_rotate_degree {

    kPXP_Rotate0 = 0,      /**< \brief Clock wise rotate 0 deg. */
    kPXP_Rotate90,         /**< \brief Clock wise rotate 90 deg. */
    kPXP_Rotate180,        /**< \brief Clock wise rotate 180 deg. */
    kPXP_Rotate270,        /**< \brief Clock wise rotate 270 deg. */
} pxp_rotate_degree_t;

/**
 * \brief PXP output flip mode.
 */
typedef enum pxp_flip_mode {
    kPXP_FlipDisable = 0,         /**< \brief Flip disable. */
    kPXP_FlipHorizontal = 0x01,   /**< \brief Horizontal flip. */
    kPXP_FlipVertical = 0x02,     /**< \brief Vertical flip. */
    kPXP_FlipBoth = 0x03,        /**< \brief Flip both directions. */
} pxp_flip_mode_t;

/**
 * \brief  PXP output buffer format.
 */
typedef enum _pxp_output_pixel_format {
    kPXP_OutputPixelFormatARGB8888 = 0x0,  /**< \brief 32-bit pixels with alpha. */
    kPXP_OutputPixelFormatRGB888 = 0x4,    /**< \brief 32-bit pixels without alpha (unpacked 24-bit format) */
    kPXP_OutputPixelFormatRGB888P = 0x5,   /**< \brief 24-bit pixels without alpha (packed 24-bit format) */
    kPXP_OutputPixelFormatARGB1555 = 0x8,  /**< \brief 16-bit pixels with alpha. */
    kPXP_OutputPixelFormatARGB4444 = 0x9,  /**< \brief 16-bit pixels with alpha. */
    kPXP_OutputPixelFormatRGB555 = 0xC,    /**< \brief 16-bit pixels without alpha. */
    kPXP_OutputPixelFormatRGB444 = 0xD,    /**< \brief 16-bit pixels without alpha. */
    kPXP_OutputPixelFormatRGB565 = 0xE,    /**< \brief 16-bit pixels without alpha. */
    kPXP_OutputPixelFormatYUV1P444 = 0x10, /**< \brief 32-bit pixels (1-plane XYUV unpacked). */
    kPXP_OutputPixelFormatUYVY1P422 = 0x12,/**< \brief 16-bit pixels (1-plane U0,Y0,V0,Y1 interleaved bytes) */
    kPXP_OutputPixelFormatVYUY1P422 = 0x13,/**< \brief 16-bit pixels (1-plane V0,Y0,U0,Y1 interleaved bytes) */
    kPXP_OutputPixelFormatY8 = 0x14,       /**< \brief 8-bit monochrome pixels (1-plane Y luma output) */
    kPXP_OutputPixelFormatY4 = 0x15,       /**< \brief 4-bit monochrome pixels (1-plane Y luma, 4 bit truncation) */
    kPXP_OutputPixelFormatYUV2P422 = 0x18, /**< \brief 16-bit pixels (2-plane UV interleaved bytes) */
    kPXP_OutputPixelFormatYUV2P420 = 0x19, /**< \brief 16-bit pixels (2-plane UV) */
    kPXP_OutputPixelFormatYVU2P422 = 0x1A, /**< \brief 16-bit pixels (2-plane VU interleaved bytes) */
    kPXP_OutputPixelFormatYVU2P420 = 0x1B, /**< \brief 16-bit pixels (2-plane VU) */
} pxp_output_pixel_format_t;


/**
 * \brief  PXP output buffer configuration.
 */
typedef struct _pxp_output_buffer_config {
    pxp_output_pixel_format_t pixel_format;      /**< \brief Output buffer pixel format. */
    pxp_interlaced_output_mode_t interlaced_mode;/**< \brief Interlaced output mode. */
    uint32_t buffer0_addr;                       /**< \brief Output buffer 0 address. */
    uint32_t buffer1_addr;                       /**< \brief Output buffer 1 address, used for UV data in YUV 2-plane mode, or
                                                        field 1 in output interlaced mode. */
    uint16_t pitch_bytes;                        /**< \brief Number of bytes between two vertically adjacent pixels. */
    uint16_t width;                              /**< \brief Pixels per line. */
    uint16_t height;                             /**< \brief How many lines in output buffer. */
} pxp_output_buffer_config_t;


/**
 * \brief  PXP alpha surface blending configuration.
 */
typedef struct _pxp_as_blend_config
{
    uint8_t alpha;               /**< \brief User defined alpha value, only used when @ref alphaMode is @ref kPXP_AlphaOverride or @ref
                                       kPXP_AlphaRop. */
    uint8_t invert_alpha;        /**< \brief Set true to invert the alpha. */
    pxp_alpha_mode_t alpha_mode; /**< \brief Alpha mode. */
    pxp_rop_mode_t rop_mode;     /**< \brief ROP mode, only valid when @ref alphaMode is @ref kPXP_AlphaRop. */
} pxp_as_blend_config_t;


/**
 * \brief 设置PS的背景颜色
 *
 * \param[in] bg_color 背景颜色值(最大支持24位)
 *
 * \return    无
 *
 */
void pxp_set_process_surface_back_ground_color(uint32_t bg_color);

/**
 * \brief PS Buffer配置
 *
 * \param[in] p_config  buffer配置信息
 *
 * \return    无
 *
 */
void pxp_set_process_surface_buffer_config(const pxp_ps_buffer_config_t *p_config);


/**
 * \brief AS Buffer配置
 *
 * \param[in] p_config  buffer配置信息
 *
 * \return    无
 *
 */
void pxp_set_alpha_surface_buffer_config(const pxp_as_buffer_config_t *p_config);


/**
 * \brief AS 颜色混合配置
 *
 * \param[in] p_config  配置信息
 *
 * \return    无
 *
 */
void pxp_set_alpha_surface_blend_config(const pxp_as_blend_config_t *p_config);


/**
 * \brief 配置输出缓冲区
 *
 * \param[in] p_config  配置信息
 *
 * \return    无
 *
 */
void pxp_set_output_buffer_config(const pxp_output_buffer_config_t *p_config);



/**
 * \brief PS 图形位置设置
 *
 * \param[in] ul_x  左上角x坐标
 * \param[in] ul_y  左上角y坐标
 * \param[in] lr_x  右下脚x坐标
 * \param[in] lr_y  右下脚y坐标
 *
 * \return    无
 *
 */
void pxp_set_process_surface_position(uint16_t ul_x,
                                      uint16_t ul_y,
                                      uint16_t lr_x,
                                      uint16_t lr_y);

/**
 * \brief AS 图形位置设置
 *
 * \param[in] ul_x  左上角x坐标
 * \param[in] ul_y  左上角y坐标
 * \param[in] lr_x  右下脚x坐标
 * \param[in] lr_y  右下脚y坐标
 *
 * \return    无
 *
 */
void pxp_set_alpha_surface_position(uint16_t ul_x,
                                    uint16_t ul_y,
                                    uint16_t lr_x,
                                    uint16_t lr_y);

/**
 * \brief PS 缩放功能时, 输入输出图形宽度、高度属性设置
 *
 * \param[in] input_width   输入图形的宽度
 * \param[in] input_height  输入图形的高度
 * \param[in] output_width  输出图形的宽度
 * \param[in] output_height 输出图形的宽度
 *
 * \return    无
 *
 */
void pxp_set_process_surface_scaler(uint16_t input_width,
                                    uint16_t input_height,
                                    uint16_t output_width,
                                    uint16_t output_height);

/**
 * \brief 图形旋转配置
 *
 * \param[in] position   旋转图形位置选择
 * \param[in] degree     旋转的角度
 * \param[in] flip_mode  Flip模式设置
 *
 * \return    无
 *
 */
void pxp_set_rotate_config(pxp_rotate_position_t position,
                           pxp_rotate_degree_t   degree,
                           pxp_flip_mode_t       flip_mode);


/**
 * \brief 设置是否使能颜色空间转换
 *
 * \param[in] enable  0:禁能  1:使能
 *
 * \return    无
 *
 */
void pxp_enable_csc1(uint8_t enable);


/**
 * \brief 启动PXP图形处理
 *
 * \return    无
 *
 */
void pxp_start(void);


/**
 * \brief 等待PXP图形处理完成 (注意该函数会阻塞)
 *
 * \return    无
 */
void pxp_complete_status_sync(void);


/**
 * \brief 复位PXP模块
 *
 * \return    无
 */
void pxp_hard_reset(void);


#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif /* __AWBL_IMX1050_PXP_H */

/* end of file */
