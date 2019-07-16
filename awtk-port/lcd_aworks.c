/**
 * File:   lcd_aworks.c
 * Author: AWTK Develop Team
 * Brief:  lcd for aworks
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "aworks.h"
#include "aw_fb.h"
#include "aw_sem.h"
#include "aw_cache.h"
#include "aw_mem.h"
#include "aw_delay.h"
#include "aw_vdebug.h"
#include <string.h>
#include "aw_prj_params.h"
#include "base/types_def.h"

static uint32_t *s_frame_buffer = 0;
static uint32_t *s_offline_frame_buffer = 0;
static void     *s_awtk_fb = NULL;
static int s_fb_size = 0;

uint32_t* aworks_get_online_fb(void) {
  return s_frame_buffer;
}

uint32_t* aworks_get_offline_fb(void) {
  return s_offline_frame_buffer;
}

void* aworks_get_fb(void) {
  return s_awtk_fb;
}

int aworks_get_fb_size() {
  return s_fb_size;
}

void* aworks_lcd_init(void) {
  aw_fb_fix_info_t fix_info;

#ifdef WITH_PXP_G2D
// add AW_DEV_IMX1050_PXP check for compatible with aworks sdk v1.0.5 and v1.0.4
#ifndef AW_DEV_IMX1050_PXP
  awbl_imx1050_pxp_init();
#endif
#endif

  s_awtk_fb = aw_fb_open("imx1050_fb", 0);
  return_value_if_fail(s_awtk_fb != NULL, NULL);

  aw_fb_init(s_awtk_fb);

  aw_fb_ioctl(s_awtk_fb, AW_FB_CMD_GET_FINFO, &fix_info);

  s_fb_size              = fix_info.buffer_size;
  s_frame_buffer         = (uint32_t *)(fix_info.vram_addr);
  s_offline_frame_buffer = (uint32_t *)(fix_info.vram_addr) + \
                           aworks_get_fb_size() / 4;
  aw_fb_start(s_awtk_fb);

  aw_fb_backlight(s_awtk_fb, 100);

  return s_awtk_fb;
}

