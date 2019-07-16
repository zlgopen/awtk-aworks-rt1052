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

  if ((strcmp(p_ts_name, "imx105x-ts") == 0) || 
      (strcmp(p_ts_name, "bu21029muv") == 0)) {
    /* 电阻触摸屏需要读取校准数据, 电容屏无需读校准数据 */
    return_value_if_fail(aw_ts_calc_data_read(sys_ts) == AW_OK, NULL);
  }

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

extern uint32_t* aworks_get_online_fb(void);
extern uint32_t* aworks_get_offline_fb(void);
extern void*     aworks_get_fb(void);
extern int       aworks_get_fb_size();
static lcd_flush_t s_lcd_flush_default = NULL;

static ret_t lcd_aworks_fb_flush(lcd_t* lcd) {
  if (s_lcd_flush_default != NULL) {
    s_lcd_flush_default(lcd);
  }

  // 单用 aw_cache_flush 能极大改善干扰线问题，但不能完全去除
  aw_cache_flush(aworks_get_online_fb(), aworks_get_fb_size()); // max 2ms wait
  return RET_OK;
}

#ifndef WITH_THREE_FB

/*----------------------------------------------------------------------------*/
/* 双缓冲模式                                                                 */
/*----------------------------------------------------------------------------*/

static ret_t lcd_aworks_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  if (lcd_is_swappable(lcd)) {
    lcd_mem_t* mem = (lcd_mem_t*)lcd;
    (void)mem;

#if 0 // 拷贝上一屏数据到offline fb作为背景, begin_frame之后只绘制脏矩形区域
    // 当前的awtk脏矩形实现机制: 每帧begin_frame时的脏矩形是与上一帧的脏矩形合并一次
    // 这样, 当前帧绘制时也会把上一帧的脏区域也绘制一次, 这样就无需执行这里的memcpy(拷贝上一屏数据到offline fb作为背景)
    // 但如果以后awtk修改了这个机制, 就必须执行这里的memcpy了
    memcpy(mem->offline_fb, mem->online_fb, aworks_get_fb_size());
#endif

#if 0 // 测试用代码, offline fb 填充空白, 这样可以观察每次绘制的脏矩形
    memset(mem->offline_fb, 0, aworks_get_fb_size());
#endif
  }

  return RET_OK;
}

static ret_t lcd_aworks_swap(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  void     *p_fb = aworks_get_fb();

  /* 交换缓冲带有垂直同步功能 */
  aw_fb_swap_buf(p_fb);
  mem->offline_fb = (uint8_t*)aw_fb_get_offline_buf(p_fb);
  mem->online_fb = (uint8_t*)aw_fb_get_online_buf(p_fb);
  return RET_OK;
}

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
    lcd_t* lcd = lcd_mem_bgr565_create_double_fb(w,
                                                 h,
                                      (uint8_t*) aworks_get_online_fb(),
                                      (uint8_t*) aworks_get_offline_fb());

  if (lcd != NULL) {
    // 改进flush机制, 每次flush后加入cache_flush (旋转屏幕方向后进入flush流程)
    s_lcd_flush_default = lcd->flush;
    lcd->flush = lcd_aworks_fb_flush;

    // 使用swap机制(正常屏幕方向进入swap流程)
    lcd->begin_frame = lcd_aworks_begin_frame;
    lcd->swap = lcd_aworks_swap;
  }

  return lcd;
}

#else // WITH_THREE_FB

/*----------------------------------------------------------------------------*/
/* 三缓冲模式                                                                 */
/*----------------------------------------------------------------------------*/

static ret_t lcd_aworks_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  if (lcd_is_swappable(lcd)) {
    lcd_mem_t* mem = (lcd_mem_t*)lcd;
    (void)mem;
  }
  return RET_OK;
}

static ret_t lcd_aworks_swap(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  void     *p_fb = aworks_get_fb();

  if (AW_OK == aw_fb_try_swap_buf(p_fb)) {
      mem->offline_fb = (uint8_t*)aw_fb_get_offline_buf(p_fb);
  }
  return RET_OK;
}

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
    lcd_t* lcd = lcd_mem_bgr565_create_three_fb(w,                          /* 宽分辨率 */
                                                h,                          /* 高分辨率 */
                                     (uint8_t *)aworks_get_online_fb(),     /* 缓存区地址1 */
                                     (uint8_t *)aworks_get_offline_fb(),    /* 缓存区地址2 */
                                     (uint8_t *)aworks_get_offline_fb() + \
                                                aworks_get_fb_size());      /* 缓存区地址3 */

  if (lcd != NULL) {
    // 改进flush机制, 每次flush后加入cache_flush (旋转屏幕方向后进入flush流程)
    s_lcd_flush_default = lcd->flush;
    lcd->flush = lcd_aworks_fb_flush;

    // 使用swap机制(正常屏幕方向进入swap流程)
    lcd->begin_frame = lcd_aworks_begin_frame;
    lcd->swap = lcd_aworks_swap;
  }

  return lcd;
}

#endif // WITH_THREE_FB

#include "main_loop/main_loop_raw.inc"
