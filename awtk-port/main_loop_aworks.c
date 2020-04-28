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
/* 触摸屏输入消息分派                                                         */
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
  /* 定义任务实体，分配栈空间大小为4096  */
  AW_TASK_DECL_STATIC(ts_task, TS_STACK_SIZE);

  AW_TASK_INIT(ts_task,      /* 任务实体 */
               "ts_task",   /* 任务名字 */
               4,             /* 任务优先级 */
               TS_STACK_SIZE, /* 任务堆栈大小 */
               __ts_task_entry,  /* 任务入口函数 */
               sys_ts);         /* 任务入口参数 */

  AW_TASK_STARTUP(ts_task); /* 启动任务 */
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
/* frame buffer刷新操作                                                       */
/*----------------------------------------------------------------------------*/

/**
 * 三缓冲模式下, 是否限制帧率FPS
 *
 * 如果定义该宏，FPS将被限制在LCD的刷新率之内
 * 如果不定义该宏，GUI将不限制帧率，但CPU压力会变大
 */
#ifndef THREE_FB_WITH_FPS_LIMITED
#define THREE_FB_WITH_FPS_LIMITED 1
#endif

extern void* aworks_get_fb(void);
extern int aworks_get_fb_number(void);

/*----------------------------------------------------------------------------*/
/* 垂直同步交换方式（FPS限制在LCD刷新率）                                     */
/*----------------------------------------------------------------------------*/

static ret_t lcd_aworks_swap_sync(lcd_t* lcd) {
  void* p_fb = aworks_get_fb();
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  /* 交换缓冲带有垂直同步功能 */
  aw_fb_swap_buf(p_fb);
  mem->offline_fb = (uint8_t*)aw_fb_get_offline_buf(p_fb);
  mem->online_fb = (uint8_t*)aw_fb_get_online_buf(p_fb);
  return RET_OK;
}

/*----------------------------------------------------------------------------*/
/* 异步交换方式，不等待垂直同步（不限制FPS）                                  */
/*----------------------------------------------------------------------------*/

static bool_t s_dirty_offline = FALSE;

static ret_t lcd_aworks_swap_async(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  void     *p_fb = aworks_get_fb();

  /* 异步缓冲区交换, 不等待 */
  if (AW_OK == aw_fb_try_swap_buf(p_fb)) {
    mem->offline_fb = (uint8_t*)aw_fb_get_offline_buf(p_fb);
    s_dirty_offline = FALSE;
  } else {
    s_dirty_offline = TRUE;
  }
  return RET_OK;
}

static ret_t __swap_idle_entry(const idle_info_t* idle) {
  void* p_fb = aworks_get_fb();
  lcd_mem_t* mem = (lcd_mem_t*)idle->ctx;

  if (s_dirty_offline) {
    if (AW_OK == aw_fb_try_swap_buf(p_fb)) {
      mem->offline_fb = (uint8_t*)aw_fb_get_offline_buf(p_fb);
      s_dirty_offline = FALSE;
    }
  }
  return RET_REPEAT;
}

/*----------------------------------------------------------------------------*/
/* 双缓冲模式/三缓冲模式 根据aworks_get_fb_number自动切换                     */
/*----------------------------------------------------------------------------*/

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  void* p_fb = aworks_get_fb();

  /* 检查显存是否足够分配两个以上的缓冲区, 如果失败, 请检查分散加载文件的VRAM_SIZE分配
   * VRAM_SIZE >= 2 * lcd_width*lcd_height*bpp
   */
  assert(p_fb && aw_fb_get_online_buf(p_fb));
  assert(p_fb && aw_fb_get_offline_buf(p_fb));
  
  lcd_t* lcd = lcd_mem_bgr565_create_double_fb(w,
                                               h,
                                    (uint8_t*) aw_fb_get_online_buf(p_fb),
                                    (uint8_t*) aw_fb_get_offline_buf(p_fb));

#ifdef WITH_THREE_FB
  if (lcd != NULL) {
    if (aworks_get_fb_number() > 2) {
      /* 三缓冲模式，不支持脏矩形 */
      lcd->support_dirty_rect = 0;
      
#if THREE_FB_WITH_FPS_LIMITED
      lcd->swap = lcd_aworks_swap_sync;
#else // THREE_FB_WITH_FPS_LIMITED
      lcd->swap = lcd_aworks_swap_async;
      idle_add(__swap_idle_entry, lcd);
#endif // THREE_FB_WITH_FPS_LIMITED
    } else {
      /* 单缓冲或双缓冲模式（VRAM不足三缓冲） */
      lcd->support_dirty_rect = 1;
      lcd->swap = NULL;
      assert(lcd->flush); // swap=NULL则使用默认的flush方法
    }
  }
#endif // WITH_THREE_FB

  return lcd;
}

#include "main_loop/main_loop_raw.inc"
