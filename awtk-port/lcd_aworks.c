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


static void     *s_awtk_fb = NULL;
static int       s_fb_size = 0;
static int       s_fb_number = 0;


void* aworks_get_fb(void) {
  return s_awtk_fb;
}

int aworks_get_fb_size(void) {
  return s_fb_size;
}

int aworks_get_fb_number(void) {
  return s_fb_number;
}

void* aworks_lcd_init(void) {
#ifdef WITH_PXP_G2D
// add AW_DEV_IMX1050_PXP check for compatible with aworks sdk v1.0.5 and v1.0.4
#ifndef AW_DEV_IMX1050_PXP
  awbl_imx1050_pxp_init();
#endif
#endif

  s_awtk_fb = aw_fb_open("imx1050_fb", 0);
  return_value_if_fail(s_awtk_fb != NULL, NULL);

  aw_fb_init(s_awtk_fb);

// add AW_FB_VERSIONS_N1 check for compatible with aworks sdk v2.1.0 and v2.2.0
#ifdef AW_FB_VERSIONS_N1
  aw_fb_var_info_t var_info;
  aw_fb_ioctl(s_awtk_fb, AW_FB_CMD_GET_VINFO, &var_info);
  s_fb_size              = var_info.buffer.buffer_size;
  s_fb_number            = var_info.buffer.buffer_num;
#else
  aw_fb_fix_info_t fix_info;
  aw_fb_ioctl(s_awtk_fb, AW_FB_CMD_GET_FINFO, &fix_info);
  s_fb_size              = fix_info.buffer_size;
  s_fb_number            = fix_info.buffer_num;
#endif
  
  aw_fb_start(s_awtk_fb);
  aw_fb_backlight(s_awtk_fb, 100);

  return s_awtk_fb;
}

