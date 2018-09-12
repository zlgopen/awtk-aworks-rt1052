/*******************************************************************************
 *                                 AWorks
 *                       ---------------------------
 *                       innovating embedded platform
 *
 * Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Co., Ltd.
 * All rights reserved.
 *
 * Contact information:
 * web site:    http://www.zlg.cn/
 *******************************************************************************/

#include "aworks.h"
#include "aw_emwin_fb.h"
#include "aw_sem.h"
#include "aw_cache.h"
#include "aw_delay.h"
#include "aw_vdebug.h"
#include <string.h>

aw_emwin_fb_info_t* aworks_lcd_init(void);
int gui_app_start(int lcd_w, int lcd_h);

aw_local void __task_entry(void *p_arg) {
  aw_emwin_fb_info_t* fb = aworks_lcd_init();

  gui_app_start(fb->x_res, fb->y_res);
}

void awtk_demo(void) {
#define TK_STACK_SIZE 64 * 1024
  /* 定义任务实体，分配栈空间大小为4096  */
  AW_TASK_DECL_STATIC(lcd_task, TK_STACK_SIZE);

  AW_TASK_INIT(lcd_task, /* 任务实体 */
  "awtk_task", /* 任务名字 */
  5, /* 任务优先级 */
  TK_STACK_SIZE, /* 任务堆栈大小 */
  __task_entry, /* 任务入口函数 */
  NULL); /* 任务入口参数 */

  AW_TASK_STARTUP(lcd_task); /* 启动任务 */
}

/* end of file */
void test_g2d(void);
extern void app_emwin_demo();
extern void test_aworks_desktop();

int aw_main() {
  aw_kprintf("AWTK DEMO...");
  //test_g2d();

  awtk_demo();

  //test_aworks_desktop();
  //app_emwin_demo ();

  return 0;
}

/* end of file*/
