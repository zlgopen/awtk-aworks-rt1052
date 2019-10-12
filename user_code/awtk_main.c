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
#include "aw_vdebug.h"
#include "aw_delay.h"
#include "aw_time.h"
#include "aw_emwin_fb.h"

#define TK_STACK_SIZE 64 * 1024

extern aw_emwin_fb_info_t* aworks_lcd_init(void);
extern int gui_app_start(int lcd_w, int lcd_h) ;
extern void ts_init (aw_emwin_fb_info_t* p_fb);

aw_local void __task_entry(void *p_arg)
{
  aw_emwin_fb_info_t* fb = aworks_lcd_init();

  ts_init(fb);

  gui_app_start(fb->x_res, fb->y_res);
}

aw_local void __awtk_startup(void)
{
  /* 定义任务实体，分配栈空间大小为4096  */
  AW_TASK_DECL_STATIC(lcd_task, TK_STACK_SIZE);

  AW_TASK_INIT(lcd_task,      /* 任务实体 */
               "awtk_task",   /* 任务名字 */
               5,             /* 任务优先级 */
               TK_STACK_SIZE, /* 任务堆栈大小 */
               __task_entry,  /* 任务入口函数 */
               NULL);         /* 任务入口参数 */

  AW_TASK_STARTUP(lcd_task); /* 启动任务 */
}

int awtk_main() {
  aw_kprintf("gui_main start...");

  __awtk_startup();
  
  return 0;
}

/* end of file*/
