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

#include "aworks.h"
#include "aw_int.h"
#include "driver/pxp/awbl_imx1050_pxp.h"
#include "driver/pxp/imx1050_pxp_regs.h"
#include "aw_assert.h"
#include "imx1050_reg_base.h"


/* 设置背景颜色 */
void pxp_set_process_surface_back_ground_color(uint32_t bg_color)
{
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;
    writel(bg_color, &p_hw_pxp->PS_BACKGROUND);
}

/* 设置Buffer */
void pxp_set_process_surface_buffer_config(const pxp_ps_buffer_config_t *p_config)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    aw_assert(p_config);
    reg_val = readl(&p_hw_pxp->PS_CTRL);
    reg_val &= ~(__PXP_PS_CTRL_FORMAT_MASK | __PXP_PS_CTRL_WB_SWAP_MASK);
    reg_val |=  (__PXP_PS_CTRL_FORMAT(p_config->pixel_format) |
                 __PXP_PS_CTRL_WB_SWAP(p_config->swap_byte));
    writel(reg_val, &p_hw_pxp->PS_CTRL);

    writel(p_config->buffer_addr,   &p_hw_pxp->PS_BUF);
    writel(p_config->buffer_addr_u, &p_hw_pxp->PS_UBUF);
    writel(p_config->buffer_addr_v, &p_hw_pxp->PS_VBUF);
    writel(p_config->pitch_bytes,   &p_hw_pxp->PS_PITCH);
}


void pxp_set_alpha_surface_buffer_config(const pxp_as_buffer_config_t *p_config)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;
    aw_assert(p_config);

    reg_val = readl(&p_hw_pxp->AS_CTRL);
    reg_val = (reg_val & ~__PXP_AS_CTRL_FORMAT_MASK) | __PXP_AS_CTRL_FORMAT(p_config->pixel_format);
    writel(reg_val, &p_hw_pxp->AS_CTRL);

    writel(p_config->buffer_addr, &p_hw_pxp->AS_BUF);
    writel(p_config->pitch_bytes, &p_hw_pxp->AS_PITCH);
}


void pxp_set_alpha_surface_blend_config(const pxp_as_blend_config_t *p_config)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;
    aw_assert(p_config);

    reg_val = readl(&p_hw_pxp->AS_CTRL);
    reg_val &=
           ~(__PXP_AS_CTRL_ALPHA_INVERT_MASK  |
             __PXP_AS_CTRL_ROP_MASK           |
             __PXP_AS_CTRL_ALPHA_MASK         |
             __PXP_AS_CTRL_ALPHA_CTRL_MASK);

    reg_val |= (__PXP_AS_CTRL_ROP(p_config->rop_mode) |
                __PXP_AS_CTRL_ALPHA(p_config->alpha)  |
                __PXP_AS_CTRL_ALPHA_CTRL(p_config->alpha_mode));

    if (p_config->invert_alpha) {
        reg_val |= __PXP_AS_CTRL_ALPHA_INVERT_MASK;
    }

    writel(reg_val, &p_hw_pxp->AS_CTRL);
}


void pxp_set_output_buffer_config(const pxp_output_buffer_config_t *p_config)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    aw_assert(p_config);
    reg_val = readl(&p_hw_pxp->OUT_CTRL);
    reg_val = (reg_val & ~(__PXP_OUT_CTRL_FORMAT_MASK        |
               __PXP_OUT_CTRL_INTERLACED_OUTPUT_MASK))       |
               __PXP_OUT_CTRL_FORMAT(p_config->pixel_format) |
               __PXP_OUT_CTRL_INTERLACED_OUTPUT(p_config->interlaced_mode);

    writel(reg_val, &p_hw_pxp->OUT_CTRL );
    writel(p_config->buffer0_addr, &p_hw_pxp->OUT_BUF );
    writel(p_config->buffer1_addr, &p_hw_pxp->OUT_BUF2 );
    writel(p_config->pitch_bytes, &p_hw_pxp->OUT_PITCH );

    reg_val =  __PXP_OUT_LRC_Y(p_config->height - 1) |  __PXP_OUT_LRC_X(p_config->width - 1);
    writel(reg_val, &p_hw_pxp->OUT_LRC );
}


void pxp_enable_csc1(uint8_t enable)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    reg_val = readl(&p_hw_pxp->CSC1_COEF0);
    if (enable) {
        reg_val &= ~__PXP_CSC1_COEF0_BYPASS_MASK;
    } else {
        reg_val |= __PXP_CSC1_COEF0_BYPASS_MASK;
    }
    writel(reg_val, &p_hw_pxp->CSC1_COEF0);
}


void pxp_set_process_surface_position(uint16_t ul_x,
                                      uint16_t ul_y,
                                      uint16_t lr_x,
                                      uint16_t lr_y)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    reg_val = __PXP_OUT_PS_ULC_Y(ul_y) | __PXP_OUT_PS_ULC_X(ul_x);
    writel(reg_val, &p_hw_pxp->OUT_PS_ULC);

    reg_val =  __PXP_OUT_PS_LRC_Y(lr_y) | __PXP_OUT_PS_LRC_X(lr_x);
    writel(reg_val, &p_hw_pxp->OUT_PS_LRC);
}


void pxp_set_alpha_surface_position(uint16_t ul_x,
                                    uint16_t ul_y,
                                    uint16_t lr_x,
                                    uint16_t lr_y)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    reg_val =   __PXP_OUT_AS_ULC_Y(ul_y) | __PXP_OUT_AS_ULC_X(ul_x);
    writel(reg_val, &p_hw_pxp->OUT_AS_ULC);

    reg_val =  __PXP_OUT_AS_LRC_Y(lr_y) | __PXP_OUT_AS_LRC_X(lr_x);
    writel(reg_val, &p_hw_pxp->OUT_AS_LRC);
}



static void pxp_get_scaler_param (uint16_t input_dimension,
                                  uint16_t output_dimension,
                                  uint8_t  *dec,
                                  uint32_t *scale)
{
    uint32_t scale_fact = 0;
    if (output_dimension == 0) {
        output_dimension = 1;
    }

    scale_fact = ((uint32_t)input_dimension << 12U) / output_dimension;
    if (scale_fact >= (16U << 12U)) {

        /* Desired fact is two large, use the largest support value. */
        *dec = 3U;
        *scale = 0x2000U;
    } else {
        if (scale_fact > (8U << 12U)) {
            *dec = 3U;
        } else if (scale_fact > (4U << 12U)) {
            *dec = 2U;
        } else if (scale_fact > (2U << 12U)) {
            *dec = 1U;
        } else {
            *dec = 0U;
        }

        *scale = scale_fact >> (*dec);
        if (0U == *scale) {
            *scale = 1U;
        }
    }
}


void pxp_set_process_surface_scaler(uint16_t input_width,
                                    uint16_t input_height,
                                    uint16_t output_width,
                                    uint16_t output_height)
{

    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;
    uint8_t  dec_x   = 0;
    uint8_t  dec_y   = 0;
    uint32_t scale_x = 0;
    uint32_t scale_y = 0;
    uint32_t reg_val = 0;

    pxp_get_scaler_param(input_width, output_width, &dec_x, &scale_x);
    pxp_get_scaler_param(input_height, output_height, &dec_y, &scale_y);

    reg_val = readl(&p_hw_pxp->PS_CTRL);
    reg_val = (reg_val & ~(__PXP_PS_CTRL_DECX_MASK | __PXP_PS_CTRL_DECY_MASK)) |
               __PXP_PS_CTRL_DECX(dec_x) | __PXP_PS_CTRL_DECY(dec_y);

    writel(reg_val, &p_hw_pxp->PS_CTRL);

    reg_val = __PXP_PS_SCALE_XSCALE(scale_x) | __PXP_PS_SCALE_YSCALE(scale_y);
    writel(reg_val, &p_hw_pxp->PS_SCALE);
}


void pxp_set_rotate_config(pxp_rotate_position_t position,
                           pxp_rotate_degree_t   degree,
                           pxp_flip_mode_t       flip_mode)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    reg_val = readl(&p_hw_pxp->CTRL) & ~(__PXP_CTRL_ROTATE_MASK |
              __PXP_CTRL_ROT_POS_MASK | __PXP_CTRL_VFLIP_MASK | __PXP_CTRL_HFLIP_MASK);

    reg_val |= __PXP_CTRL_ROTATE(degree) | __PXP_CTRL_ROT_POS(position) |
               ((uint32_t)flip_mode << __PXP_CTRL_HFLIP_SHIFT);

    writel(reg_val, &p_hw_pxp->CTRL);
}

void pxp_start(void)
{
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;
    writel(__PXP_CTRL_ENABLE_MASK, &p_hw_pxp->CTRL_SET);
}



/* 等待完成 */
void pxp_complete_status_sync(void)
{
    uint32_t reg_val = 0;
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    while (!((reg_val = readl(&p_hw_pxp->STAT)) & __PXP_STAT_IRQ_MASK));
    writel(reg_val, &p_hw_pxp->STAT);
}


/* 复位PXP模块 */
void pxp_hard_reset(void)
{
    imx1050_pxp_regs_t *p_hw_pxp = (imx1050_pxp_regs_t *)IMX1050_PXP_BASE;

    /* 复位pxp模块 */
    writel(__PXP_CTRL_SFTRST_MASK, &p_hw_pxp->CTRL_SET);
    writel((__PXP_CTRL_SFTRST_MASK | __PXP_CTRL_CLKGATE_MASK), &p_hw_pxp->CTRL_CLR);
}

/* end of file */
