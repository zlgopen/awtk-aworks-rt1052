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
#include "aw_emwin_fb.h"
#include "aw_sem.h"
#include "aw_cache.h"
#include "aw_mem.h"
#include "aw_delay.h"
#include "aw_vdebug.h"
#include <string.h>

#include "base/types_def.h"

static uint32_t *s_frame_buffer = 0;
static uint32_t *s_offline_frame_buffer = 0;
static aw_emwin_fb_info_t *s_awtk_fb = NULL;
static int s_fb_size = 0;

uint32_t* aworks_get_online_fb(void) {
  return s_frame_buffer;
}

uint32_t* aworks_get_offline_fb(void) {
  return s_offline_frame_buffer;
}

aw_emwin_fb_info_t* aworks_get_fb(void) {
  return s_awtk_fb;
}

int aworks_get_fb_size() {
	return s_fb_size;
}

aw_emwin_fb_info_t* aworks_lcd_init(void) {
  int fb_size = 0;
  s_awtk_fb = aw_emwin_fb_open("imx1050_emwin_fb", 0);
  return_value_if_fail(s_awtk_fb != NULL, NULL);

  fb_size = s_awtk_fb->x_res * s_awtk_fb->y_res * 2;
  s_fb_size = fb_size;

  s_frame_buffer = (uint32_t*) aw_mem_align(fb_size,128);
  return_value_if_fail(s_frame_buffer != NULL, NULL);
  memset(s_frame_buffer, 0x00, fb_size);

  s_offline_frame_buffer = (uint32_t*) aw_mem_align(fb_size,128);
  return_value_if_fail(s_offline_frame_buffer != NULL, NULL);
  memset(s_offline_frame_buffer, 0x00, fb_size);

  return_value_if_fail(
      AW_OK == aw_emwin_fb_init(s_awtk_fb, s_frame_buffer, AW_FB_VERTICAL_SCREEN),
      NULL);

  aw_emwin_fb_run(s_awtk_fb);

  aw_emwin_fb_backlight(s_awtk_fb, 100);

  return s_awtk_fb;
}

