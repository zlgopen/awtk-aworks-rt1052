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
 * \brief iMX RT1050 Quadrature Decoder regs
 *
 * \internal
 * \par modification history:
 * - 1.00 18-03-21  mex, first implementation
 * \endinternal
 */

#ifndef __IMX1050_PXP_REGS_H
#define __IMX1050_PXP_REGS_H

#include "stdint.h"

#define __IO  volatile
#define __O   volatile
#define __I   volatile const

typedef struct {

    __IO uint32_t CTRL;                     /**< \brief \brief Control Register 0, offset: 0x0 */
    __IO uint32_t CTRL_SET;                 /**< \brief \brief Control Register 0, offset: 0x4 */
    __IO uint32_t CTRL_CLR;                 /**< \brief \brief Control Register 0, offset: 0x8 */
    __IO uint32_t CTRL_TOG;                 /**< \brief \brief Control Register 0, offset: 0xC */
    __IO uint32_t STAT;                     /**< \brief \brief Status Register, offset: 0x10 */
    __IO uint32_t STAT_SET;                 /**< \brief Status Register, offset: 0x14 */
    __IO uint32_t STAT_CLR;                 /**< \brief Status Register, offset: 0x18 */
    __IO uint32_t STAT_TOG;                 /**< \brief Status Register, offset: 0x1C */
    __IO uint32_t OUT_CTRL;                 /**< \brief Output Buffer Control Register, offset: 0x20 */
    __IO uint32_t OUT_CTRL_SET;             /**< \brief Output Buffer Control Register, offset: 0x24 */
    __IO uint32_t OUT_CTRL_CLR;             /**< \brief Output Buffer Control Register, offset: 0x28 */
    __IO uint32_t OUT_CTRL_TOG;             /**< \brief Output Buffer Control Register, offset: 0x2C */
    __IO uint32_t OUT_BUF;                  /**< \brief Output Frame Buffer Pointer, offset: 0x30 */
         uint8_t RESERVED_0[12];
    __IO uint32_t OUT_BUF2;                 /**< \brief Output Frame Buffer Pointer #2, offset: 0x40 */
         uint8_t RESERVED_1[12];
    __IO uint32_t OUT_PITCH;                /**< \brief Output Buffer Pitch, offset: 0x50 */
         uint8_t RESERVED_2[12];
    __IO uint32_t OUT_LRC;                  /**< \brief Output Surface Lower Right Coordinate, offset: 0x60 */
         uint8_t RESERVED_3[12];
    __IO uint32_t OUT_PS_ULC;               /**< \brief Processed Surface Upper Left Coordinate, offset: 0x70 */
         uint8_t RESERVED_4[12];
    __IO uint32_t OUT_PS_LRC;               /**< \brief Processed Surface Lower Right Coordinate, offset: 0x80 */
         uint8_t RESERVED_5[12];
    __IO uint32_t OUT_AS_ULC;               /**< \brief Alpha Surface Upper Left Coordinate, offset: 0x90 */
         uint8_t RESERVED_6[12];
    __IO uint32_t OUT_AS_LRC;               /**< \brief Alpha Surface Lower Right Coordinate, offset: 0xA0 */
         uint8_t RESERVED_7[12];
    __IO uint32_t PS_CTRL;                  /**< \brief Processed Surface (PS) Control Register, offset: 0xB0 */
    __IO uint32_t PS_CTRL_SET;              /**< \brief Processed Surface (PS) Control Register, offset: 0xB4 */
    __IO uint32_t PS_CTRL_CLR;              /**< \brief Processed Surface (PS) Control Register, offset: 0xB8 */
    __IO uint32_t PS_CTRL_TOG;              /**< \brief Processed Surface (PS) Control Register, offset: 0xBC */
    __IO uint32_t PS_BUF;                   /**< \brief PS Input Buffer Address, offset: 0xC0 */
         uint8_t RESERVED_8[12];
    __IO uint32_t PS_UBUF;                  /**< \brief PS U/Cb or 2 Plane UV Input Buffer Address, offset: 0xD0 */
         uint8_t RESERVED_9[12];
    __IO uint32_t PS_VBUF;                  /**< \brief PS V/Cr Input Buffer Address, offset: 0xE0 */
         uint8_t RESERVED_10[12];
    __IO uint32_t PS_PITCH;                 /**< \brief Processed Surface Pitch, offset: 0xF0 */
         uint8_t RESERVED_11[12];
    __IO uint32_t PS_BACKGROUND;            /**< \brief PS Background Color, offset: 0x100 */
         uint8_t RESERVED_12[12];
    __IO uint32_t PS_SCALE;                 /**< \brief PS Scale Factor Register, offset: 0x110 */
         uint8_t RESERVED_13[12];
    __IO uint32_t PS_OFFSET;                /**< \brief PS Scale Offset Register, offset: 0x120 */
         uint8_t RESERVED_14[12];
    __IO uint32_t PS_CLRKEYLOW;             /**< \brief PS Color Key Low, offset: 0x130 */
         uint8_t RESERVED_15[12];
    __IO uint32_t PS_CLRKEYHIGH;            /**< \brief PS Color Key High, offset: 0x140 */
         uint8_t RESERVED_16[12];
    __IO uint32_t AS_CTRL;                  /**< \brief Alpha Surface Control, offset: 0x150 */
         uint8_t RESERVED_17[12];
    __IO uint32_t AS_BUF;                   /**< \brief Alpha Surface Buffer Pointer, offset: 0x160 */
         uint8_t RESERVED_18[12];
    __IO uint32_t AS_PITCH;                 /**< \brief Alpha Surface Pitch, offset: 0x170 */
         uint8_t RESERVED_19[12];
    __IO uint32_t AS_CLRKEYLOW;             /**< \brief Overlay Color Key Low, offset: 0x180 */
         uint8_t RESERVED_20[12];
    __IO uint32_t AS_CLRKEYHIGH;            /**< \brief Overlay Color Key High, offset: 0x190 */
         uint8_t RESERVED_21[12];
    __IO uint32_t CSC1_COEF0;               /**< \brief Color Space Conversion Coefficient Register 0, offset: 0x1A0 */
         uint8_t RESERVED_22[12];
    __IO uint32_t CSC1_COEF1;               /**< \brief Color Space Conversion Coefficient Register 1, offset: 0x1B0 */
         uint8_t RESERVED_23[12];
    __IO uint32_t CSC1_COEF2;               /**< \brief Color Space Conversion Coefficient Register 2, offset: 0x1C0 */
         uint8_t RESERVED_24[348];
    __IO uint32_t POWER;                    /**< \brief PXP Power Control Register, offset: 0x320 */
         uint8_t RESERVED_25[220];
    __IO uint32_t NEXT;                     /**< \brief Next Frame Pointer, offset: 0x400 */
         uint8_t RESERVED_26[60];
    __IO uint32_t PORTER_DUFF_CTRL;         /**< \brief PXP Alpha Engine A Control Register., offset: 0x440 */

} imx1050_pxp_regs_t;


/*******************************************************************************
  Control Register 0
*******************************************************************************/

/* Enables PXP operation with specified parameters. */
#define __PXP_CTRL_ENABLE_SHIFT            0
#define __PXP_CTRL_ENABLE_MASK            (0x1 << __PXP_CTRL_ENABLE_SHIFT)

/* Interrupt enable. */
#define __PXP_CTRL_IRQ_ENABLE_SHIFT        1
#define __PXP_CTRL_IRQ_ENABLE_MASK        (0x1 << __PXP_CTRL_IRQ_ENABLE_SHIFT)

/* Next command interrupt enable. */
#define __PXP_CTRL_NEXT_IRQ_EN_SHIFT       2
#define __PXP_CTRL_NEXT_IRQ_EN_MASK       (0x1 << __PXP_CTRL_NEXT_IRQ_EN_SHIFT)

/* Next command interrupt enable. */
#define __PXP_CTRL_EN_LCD_HANDSHAKE_SHIFT  4
#define __PXP_CTRL_EN_LCD_HANDSHAKE_MASK  (0x1 << __PXP_CTRL_EN_LCD_HANDSHAKE_SHIFT)

/* Indicates the clockwise rotation to be applied at the output buffer. */
#define __PXP_CTRL_ROTATE_SHIFT            8
#define __PXP_CTRL_ROTATE_MASK            (0X3 << __PXP_CTRL_ROTATE_SHIFT)
#define __PXP_CTRL_ROTATE(x)              (((x) & 0x3) << __PXP_CTRL_ROTATE_SHIFT)

/* Indicates that the output buffer should be flipped horizontally. */
#define __PXP_CTRL_HFLIP_SHIFT             10
#define __PXP_CTRL_HFLIP_MASK             (0x1 << __PXP_CTRL_HFLIP_SHIFT)

/* Indicates that the output buffer should be flipped vertically. */
#define __PXP_CTRL_VFLIP_SHIFT             10
#define __PXP_CTRL_VFLIP_MASK             (0x1 << __PXP_CTRL_VFLIP_SHIFT)

/* This bit controls where rotation will occur in the PXP datapath. */
#define __PXP_CTRL_ROT_POS_SHIFT           22
#define __PXP_CTRL_ROT_POS_MASK           (0x1 << __PXP_CTRL_ROT_POS_SHIFT)
#define __PXP_CTRL_ROT_POS(x)             (((x) & 0x1) << __PXP_CTRL_ROT_POS_SHIFT)

/* Select the block size to process. */
#define __PXP_CTRL_BLOCK_SIZE_SHIFT        23
#define __PXP_CTRL_BLOCK_SIZE_MASK        (0x1 << __PXP_CTRL_BLOCK_SIZE_SHIFT)

/* Enable the PXP to run continuously.. */
#define __PXP_CTRL_EN_REPEAT_SHIFT         28
#define __PXP_CTRL_EN_REPEAT_MASK         (0x1 << __PXP_CTRL_EN_REPEAT_SHIFT)

/* This bit must be set to zero for normal operation.
 * When set to one it gates off the clocks to the block.
 */
#define __PXP_CTRL_CLKGATE_SHIFT           30
#define __PXP_CTRL_CLKGATE_MASK           (0x1 << __PXP_CTRL_CLKGATE_SHIFT)

/* Set this bit to zero to enable normal PXP operation. */
#define __PXP_CTRL_SFTRST_SHIFT            31
#define __PXP_CTRL_SFTRST_MASK            (0x1 << __PXP_CTRL_SFTRST_SHIFT)


/*******************************************************************************
  Status Register Register
*******************************************************************************/

/* Indicates current PXP interrupt status. */
#define __PXP_STAT_IRQ_SHIFT              0
#define __PXP_STAT_IRQ_MASK              (0x1 << __PXP_STAT_IRQ_SHIFT)

/* Indicates PXP encountered an AXI write error and processing has been terminated. */
#define __PXP_STAT_AXI_WRITE_ERR_SHIFT    1
#define __PXP_STAT_AXI_WRITE_ERR_MASK    (0x1 << __PXP_STAT_AXI_WRITE_ERR_SHIFT)

/* Indicates PXP encountered an AXI read error and processing has been terminated. */
#define __PXP_STAT_AXI_READ_ERR_SHIFT     2
#define __PXP_STAT_AXI_READ_ERR_MASK     (0x1 << __PXP_STAT_AXI_READ_ERR_SHIFT)

/* Indicates that a command issued with the "Next Command" functionality has been
 * issued and that a new command may be initiated with a write to the PXP_NEXT register.
 */
#define __PXP_STAT_NEXT_IRQ_SHIFT         3
#define __PXP_STAT_NEXT_IRQ_MASK         (0x1 << __PXP_STAT_NEXT_IRQ_SHIFT)

/* Indicates the AXI ID of the failing bus operation. */
#define __PXP_STAT_AXI_ERR_ID_SHIFT       4
#define __PXP_STAT_AXI_ERR_ID_MASK       (0xF << __PXP_STAT_AXI_ERR_ID_SHIFT)

/* Indicates that the LUT DMA transfer has completed. */
#define __PXP_STAT_LUT_DMA_LOAD_DONE_IRQ_SHIFT   8
#define __PXP_STAT_LUT_DMA_LOAD_DONE_IRQ_MASK   (0x1 << __PXP_STAT_LUT_DMA_LOAD_DONE_IRQ_SHIFT)

/* Indicates the Y coordinate of the block currently being rendered. */
#define __PXP_STAT_BLOCKY_SHIFT           16
#define __PXP_STAT_BLOCKY_MASK           (0xFF << __PXP_STAT_BLOCKY_SHIFT)

/* Indicates the X coordinate of the block currently being rendered. */
#define __PXP_STAT_BLOCKX_SHIFT           24
#define __PXP_STAT_BLOCKX_MASK           (0xFF << __PXP_STAT_BLOCKX_SHIFT)


/*******************************************************************************
  Processed Surface (PS) Control Register
*******************************************************************************/

/* PS buffer format. */
#define __PXP_PS_CTRL_FORMAT_SHIFT         0
#define __PXP_PS_CTRL_FORMAT_MASK         (0x1F << __PXP_PS_CTRL_FORMAT_SHIFT)
#define __PXP_PS_CTRL_FORMAT(x)           (((x) & 0x1F) << __PXP_PS_CTRL_FORMAT_SHIFT)

/* Swap bytes in words. For each 16 bit word, the two bytes will be swapped. */
#define __PXP_PS_CTRL_WB_SWAP_SHIFT        5
#define __PXP_PS_CTRL_WB_SWAP_MASK        (0x1 << __PXP_PS_CTRL_WB_SWAP_SHIFT)
#define __PXP_PS_CTRL_WB_SWAP(x)          (((x) & 0x1) << __PXP_PS_CTRL_WB_SWAP_SHIFT)

/* Verticle pre decimation filter control. */
#define __PXP_PS_CTRL_DECY_SHIFT           8
#define __PXP_PS_CTRL_DECY_MASK           (0x3 << __PXP_PS_CTRL_DECY_SHIFT)
#define __PXP_PS_CTRL_DECY(x)             (((x) & 0x3) << __PXP_PS_CTRL_DECY_SHIFT)

/* Horizontal pre decimation filter control. */
#define __PXP_PS_CTRL_DECX_SHIFT           10
#define __PXP_PS_CTRL_DECX_MASK           (0x3 << __PXP_PS_CTRL_DECX_SHIFT)
#define __PXP_PS_CTRL_DECX(x)             (((x) & 0x3) << __PXP_PS_CTRL_DECX_SHIFT)


/*******************************************************************************
  Alpha Surface Control
*******************************************************************************/

/* Determines how the alpha value is constructed for this alpha surface. */
#define __PXP_AS_CTRL_ALPHA_CTRL_SHIFT      1
#define __PXP_AS_CTRL_ALPHA_CTRL_MASK      (0x3 << __PXP_AS_CTRL_ALPHA_CTRL_SHIFT)
#define __PXP_AS_CTRL_ALPHA_CTRL(x)        (((x) & 0x3) << __PXP_AS_CTRL_ALPHA_CTRL_SHIFT)

/* Indicates that colorkey functionality is enabled for this alpha surface.. */
#define __PXP_AS_CTRL_EN_COLORKEY_SHIFT     3
#define __PXP_AS_CTRL_EN_COLORKEY_MASK     (0x3 << __PXP_AS_CTRL_EN_COLORKEY_SHIFT)

/* Indicates the input buffer format for AS. */
#define __PXP_AS_CTRL_FORMAT_SHIFT          4
#define __PXP_AS_CTRL_FORMAT_MASK          (0xF << __PXP_AS_CTRL_FORMAT_SHIFT)
#define __PXP_AS_CTRL_FORMAT(x)            (((x) & 0xF) << __PXP_AS_CTRL_FORMAT_SHIFT)

/* Alpha modifier used when the ALPHA_MULTIPLY or ALPHA_OVERRIDE values are
 * programmed in PXP_AS_CTRL[ALPHA_CTRL].
 */
#define __PXP_AS_CTRL_ALPHA_SHIFT           8
#define __PXP_AS_CTRL_ALPHA_MASK           (0xFF << __PXP_AS_CTRL_ALPHA_SHIFT)
#define __PXP_AS_CTRL_ALPHA(x)             (((x) & 0xFF) << __PXP_AS_CTRL_ALPHA_SHIFT)

/* Indicates a raster operation to perform when enabled. Raster operations are
 * enabled through the ALPHA_CTRL field.
 */
#define __PXP_AS_CTRL_ROP_SHIFT             16
#define __PXP_AS_CTRL_ROP_MASK             (0xF << __PXP_AS_CTRL_ROP_SHIFT)
#define __PXP_AS_CTRL_ROP(x)               (((x) & 0xF) << __PXP_AS_CTRL_ROP_SHIFT)


/* Setting this bit to logic 0 will not alter the alpha value.. */
#define __PXP_AS_CTRL_ALPHA_INVERT_SHIFT    20
#define __PXP_AS_CTRL_ALPHA_INVERT_MASK    (0xF << __PXP_AS_CTRL_ALPHA_INVERT_SHIFT)
#define __PXP_AS_CTRL_ALPHA_INVERT(x)      (((x) & 0xF) << __PXP_AS_CTRL_ALPHA_INVERT_SHIFT)


/*******************************************************************************
  Output Buffer Control Register
*******************************************************************************/

/* Output framebuffer format. */
#define __PXP_OUT_CTRL_FORMAT_SHIFT         0
#define __PXP_OUT_CTRL_FORMAT_MASK         (0x1F << __PXP_OUT_CTRL_FORMAT_SHIFT)
#define __PXP_OUT_CTRL_FORMAT(x)           (((x) & 0x1F) << __PXP_OUT_CTRL_FORMAT_SHIFT)

/* Determines how the PXP writes it's output data. */
#define __PXP_OUT_CTRL_INTERLACED_OUTPUT_SHIFT  8
#define __PXP_OUT_CTRL_INTERLACED_OUTPUT_MASK  (0x3 << __PXP_OUT_CTRL_INTERLACED_OUTPUT_SHIFT)
#define __PXP_OUT_CTRL_INTERLACED_OUTPUT(x)    (((x) & 0x3) << __PXP_OUT_CTRL_INTERLACED_OUTPUT_SHIFT)

/* Indicates that alpha component in output buffer pixels should be overwritten
 * by PXP_OUT_CTRL[ALPHA]. If 0, retain their alpha value from the computed alpha
 * for that pixel.
 */
#define __PXP_OUT_CTRL_ALPHA_OUTPUT_SHIFT    23
#define __PXP_OUT_CTRL_ALPHA_OUTPUT_MASK    (0x1 << __PXP_OUT_CTRL_ALPHA_OUTPUT_SHIFT)
#define __PXP_OUT_CTRL_ALPHA_OUTPUT(x)      (((x) & 0x1) << __PXP_OUT_CTRL_ALPHA_OUTPUT_SHIFT)

/* When generating an output buffer with an alpha component, the value in this
 *  field will be used when enabled to override the alpha passed through the pixel
 *  data pipeline.
 */
#define __PXP_OUT_CTRL_ALPHA_SHIFT           24
#define __PXP_OUT_CTRL_ALPHA_MASK           (0xF << __PXP_OUT_CTRL_ALPHA_SHIFT)
#define __PXP_OUT_CTRL_ALPHA(x)             (((x) & 0xF) << __PXP_OUT_CTRL_ALPHA_SHIFT)

/*******************************************************************************
  Output Surface Lower Right Coordinate
*******************************************************************************/

/* Indicates the number of vertical PIXELS in the output surface (non-rotated). */
#define __PXP_OUT_LRC_Y_SHIFT             0
#define __PXP_OUT_LRC_Y_MASK             (0x3FFF << __PXP_OUT_LRC_Y_SHIFT)
#define __PXP_OUT_LRC_Y(x)               (((x) & 0x3FFF) << __PXP_OUT_LRC_Y_SHIFT)

/* Indicates the number of vertical PIXELS in the output surface (non-rotated). */
#define __PXP_OUT_LRC_X_SHIFT             16
#define __PXP_OUT_LRC_X_MASK             (0x3FFF << __PXP_OUT_LRC_X_SHIFT)
#define __PXP_OUT_LRC_X(x)               (((x) & 0x3FFF) << __PXP_OUT_LRC_X_SHIFT)


/*******************************************************************************
  Color Space Conversion Coefficient Register 0
*******************************************************************************/

#define __PXP_CSC1_COEF0_BYPASS_SHIFT     30
#define __PXP_CSC1_COEF0_BYPASS_MASK     (0x1 << __PXP_CSC1_COEF0_BYPASS_SHIFT)


/*******************************************************************************
    Processed Surface Upper Left Coordinate
*******************************************************************************/

/* This field indicates the upper left Y-coordinate (in pixels) of the processed
 *  surface in the output buffer.
 */
#define __PXP_OUT_PS_ULC_Y_SHIFT           0
#define __PXP_OUT_PS_ULC_Y_MASK           (0x3FFF << __PXP_OUT_PS_ULC_Y_SHIFT)
#define __PXP_OUT_PS_ULC_Y(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_ULC_Y_SHIFT)


/* This field indicates the upper left X-coordinate (in pixels) of the processed
 *  surface (PS) in the output buffer.
 */
#define __PXP_OUT_PS_ULC_X_SHIFT           16
#define __PXP_OUT_PS_ULC_X_MASK           (0x3FFF << __PXP_OUT_PS_ULC_X_SHIFT)
#define __PXP_OUT_PS_ULC_X(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_ULC_X_SHIFT)



/*******************************************************************************
    Processed Surface Lower Right Coordinate
*******************************************************************************/

/* This field indicates the lower right Y-coordinate (in pixels) of the processed
 *  surface in the output frame buffer.
 */
#define __PXP_OUT_PS_LRC_Y_SHIFT           0
#define __PXP_OUT_PS_LRC_Y_MASK           (0x3FFF << __PXP_OUT_PS_LRC_Y_SHIFT)
#define __PXP_OUT_PS_LRC_Y(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_LRC_Y_SHIFT)


/* This field indicates the lower right X-coordinate (in pixels) of the processed
 * surface (PS) in the output frame buffer.
 */
#define __PXP_OUT_PS_LRC_X_SHIFT           16
#define __PXP_OUT_PS_LRC_X_MASK           (0x3FFF << __PXP_OUT_PS_LRC_X_SHIFT)
#define __PXP_OUT_PS_LRC_X(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_LRC_X_SHIFT)


/*******************************************************************************
    Alpha Surface Upper Left Coordinate
*******************************************************************************/

/* This field indicates the upper left X-coordinate (in pixels) of the alpha surface
 *  (AS) in the output frame buffer.
 */
#define __PXP_OUT_AS_ULC_Y_SHIFT           0
#define __PXP_OUT_AS_ULC_Y_MASK           (0x3FFF << __PXP_OUT_AS_ULC_Y_SHIFT)
#define __PXP_OUT_AS_ULC_Y(x)             (((x) & 0x3FFF) << __PXP_OUT_AS_ULC_Y_SHIFT)

/* This field indicates the upper left Y-coordinate (in pixels) of the alpha surface
 * in the output frame buffer.
 */
#define __PXP_OUT_AS_ULC_X_SHIFT           16
#define __PXP_OUT_AS_ULC_X_MASK           (0x3FFF << __PXP_OUT_AS_ULC_X_SHIFT)
#define __PXP_OUT_AS_ULC_X(x)             (((x) & 0x3FFF) << __PXP_OUT_AS_ULC_X_SHIFT)

/*******************************************************************************
    Alpha Surface Lower Right Coordinate
*******************************************************************************/

/* This field indicates the lower right Y-coordinate (in pixels) of the alpha surface
 *  in the output frame buffer.
 */
#define __PXP_OUT_AS_LRC_Y_SHIFT           0
#define __PXP_OUT_AS_LRC_Y_MASK           (0x3FFF << __PXP_OUT_PS_LRC_Y_SHIFT)
#define __PXP_OUT_AS_LRC_Y(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_LRC_Y_SHIFT)


/* This field indicates the lower right X-coordinate (in pixels) of the alpha surface
 *  (AS) in the output frame buffer.
 */
#define __PXP_OUT_AS_LRC_X_SHIFT           16
#define __PXP_OUT_AS_LRC_X_MASK           (0x3FFF << __PXP_OUT_PS_LRC_X_SHIFT)
#define __PXP_OUT_AS_LRC_X(x)             (((x) & 0x3FFF) << __PXP_OUT_PS_LRC_X_SHIFT)


/*******************************************************************************
    PS Scale Factor Register
*******************************************************************************/

/* This is a two bit integer and 12 bit fractional representation (##.####_####_####)
 * of the X scaling factor for the PS source buffer.
 */
#define __PXP_PS_SCALE_XSCALE_SHIFT         0
#define __PXP_PS_SCALE_XSCALE_MASK         (0x7FFF << __PXP_PS_SCALE_XSCALE_SHIFT)
#define __PXP_PS_SCALE_XSCALE(x)           (((x) & 0x7FFF) << __PXP_PS_SCALE_XSCALE_SHIFT)

/* This is a two bit integer and 12 bit fractional representation (##.####_####_####)
 * of the Y scaling factor for the PS source buffer.
 */
#define __PXP_PS_SCALE_YSCALE_SHIFT         16
#define __PXP_PS_SCALE_YSCALE_MASK         (0x7FFF << __PXP_PS_SCALE_YSCALE_SHIFT)
#define __PXP_PS_SCALE_YSCALE(x)           (((x) & 0x7FFF) << __PXP_PS_SCALE_YSCALE_SHIFT)


#endif /* __IMX1050_ENC_REGS_H */

/* end of file */
