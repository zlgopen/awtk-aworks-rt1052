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
#include "aw_sem.h"
#include "aw_task.h"
#include "aw_delay.h"
#include "aw_mem.h"
#include "aw_cache.h"
#include "aw_fb.h"
#include "base/idle.h"
#include "base/timer.h"
#include "aw_prj_params.h"
#include "lcd/lcd_mem_bgr565.h"
#include "main_loop/main_loop_simple.h"

#include <string.h>

/*----------------------------------------------------------------------------*/
/* ������������Ϣ����                                                         */
/*----------------------------------------------------------------------------*/

static struct aw_ts_state s_ts_state = {0};

#define TS_STACK_SIZE 2 * 1024
aw_local void __ts_task_entry(void *p_arg)
{
  int tsret = 0;
  struct aw_ts_state ts_state;

  while (1) {
    memset(&ts_state, 0x00, sizeof(ts_state));
    tsret = aw_ts_exec(p_arg, &ts_state, 1);

    if (tsret >= 0) {
      s_ts_state = ts_state;

      if (ts_state.pressed) {
        main_loop_post_pointer_event(main_loop(), ts_state.pressed, ts_state.x,
            ts_state.y);
      } else {
        main_loop_post_pointer_event(main_loop(), ts_state.pressed, ts_state.x,
            ts_state.y);
      }
    }

    if (ts_state.pressed) {
      aw_mdelay(2);
    } else {
      aw_mdelay(10);
    }
  }
}

static void ts_task_init(aw_ts_id sys_ts) {
  /* ��������ʵ�壬����ջ�ռ��СΪ4096  */
  AW_TASK_DECL_STATIC(ts_task, TS_STACK_SIZE);

  AW_TASK_INIT(ts_task,      /* ����ʵ�� */
               "ts_task",   /* �������� */
               4,             /* �������ȼ� */
               TS_STACK_SIZE, /* �����ջ��С */
               __ts_task_entry,  /* ������ں��� */
               sys_ts);         /* ������ڲ��� */

  AW_TASK_STARTUP(ts_task); /* �������� */
}

char *aw_get_ts_name (void);

static aw_ts_id ts_app_init(void) {
  aw_ts_id    sys_ts;
  
  char       *p_ts_name = aw_get_ts_name();
  return_value_if_fail(strcmp(p_ts_name, "none") != 0, NULL);

  sys_ts = aw_ts_serv_id_get(p_ts_name, 0, 0);
  return_value_if_fail(sys_ts != NULL, NULL);

  ts_task_init(sys_ts);
  return sys_ts;
}

ret_t platform_disaptch_input(main_loop_t* loop) {
  static aw_ts_id ts_id = NULL;
  if (ts_id == NULL) {
    ts_id = ts_app_init();
  }

  return RET_OK;
}

/*----------------------------------------------------------------------------*/
/* frame bufferˢ�²���                                                       */
/*----------------------------------------------------------------------------*/

#define __FB_WAIT_VSYNC    1

extern void* aworks_get_fb(void);
extern int aworks_get_fb_number(void);

/*----------------------------------------------------------------------------*/
/* ��ֱͬ��������ʽ��FPS������LCDˢ���ʣ�                                     */
/*----------------------------------------------------------------------------*/

static ret_t lcd_aworks_swap_sync(lcd_t* lcd) {
  void* p_fb = aworks_get_fb();
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  /* ����������д�ֱͬ������ */
  aw_fb_swap_buf(p_fb);
  lcd_mem_set_offline_fb(mem, (uint8_t*)aw_fb_get_offline_buf(p_fb));
  lcd_mem_set_online_fb(mem, (uint8_t*)aw_fb_get_online_buf(p_fb));
  return RET_OK;
}

/*----------------------------------------------------------------------------*/
/* ˫����ģʽ/������ģʽ ����aworks_get_fb_number�Զ��л�                     */
/*----------------------------------------------------------------------------*/

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  void* p_fb = aworks_get_fb();

  /* ����Դ��Ƿ��㹻�����������ϵĻ�����, ���ʧ��, �����ɢ�����ļ���VRAM_SIZE����
   * VRAM_SIZE >= 2 * lcd_width*lcd_height*bpp
   */
  assert(p_fb && aw_fb_get_online_buf(p_fb));
  assert(p_fb && aw_fb_get_offline_buf(p_fb));

  lcd_t* lcd = lcd_mem_bgr565_create_double_fb(w,
                                               h,
                                    (uint8_t*) aw_fb_get_online_buf(p_fb),
                                    (uint8_t*) aw_fb_get_offline_buf(p_fb));

#if __FB_WAIT_VSYNC
  if (lcd != NULL) {
    lcd->support_dirty_rect = 0;
    lcd->swap = lcd_aworks_swap_sync;
  }
#endif // __FB_WAIT_VSYNC
  return lcd;
}

#include "main_loop/main_loop_raw.inc"
