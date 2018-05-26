/**
 * file:   main_loop_aworks_raw.c
 * Author: AWTK Develop Team
 * brief:  main loop for aworks
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-23 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "aw_ts.h"
#include "base/idle.h"
#include "base/timer.h"
#include "lcd/lcd_mem_rgb565.h"
#include "main_loop/main_loop_simple.h"

static aw_ts_id ts_app_init(void) {
  aw_ts_id sys_ts = sys_ts = aw_ts_serv_id_get("480x272", 0, 0);
  return_value_if_fail(sys_ts != NULL, NULL);
  return_value_if_fail(aw_ts_calc_data_read(sys_ts) == AW_OK, NULL);

  return sys_ts;
}

ret_t platform_disaptch_input(main_loop_t* loop) {
  int tsret = 0;
  static aw_ts_id ts_id = NULL;
  struct aw_ts_state ts_state;

  memset(&ts_state, 0x00, sizeof(ts_state));
  if (ts_id == NULL) {
    ts_id = ts_app_init();
  }

  return_value_if_fail(ts_id != NULL, RET_FAIL);

  tsret = aw_ts_exec(ts_id, &ts_state, 1);
  if (tsret >= 0) {
    if (ts_state.pressed) {
      main_loop_post_pointer_event(loop, ts_state.pressed, ts_state.x,
          ts_state.y);
    } else {
      main_loop_post_pointer_event(loop, ts_state.pressed, ts_state.x,
          ts_state.y);
    }
  }

  return RET_OK;
}

extern uint32_t* aworks_get_online_fb(void);
extern uint32_t* aworks_get_offline_fb(void);

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = lcd_mem_rgb565_create_double_fb(w, h, (uint8_t*) aworks_get_online_fb(),
      (uint8_t*) aworks_get_offline_fb());

  return lcd;
}

#include "main_loop/main_loop_raw.inc"
