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
#include "aw_task.h"
#include "aw_delay.h"
#include "aw_mem.h"
#include "aw_cache.h"
#include "aw_emwin_fb.h"
#include "base/idle.h"
#include "base/timer.h"
#include "aw_prj_params.h"
#include "lcd/lcd_mem_bgr565.h"
#include "main_loop/main_loop_simple.h"

static aw_ts_id ts_app_init(void) {
#if defined(AW_DEV_HW480272F)
  char TS_SERVER_ID[] = "480x272";
#elif defined(AW_DEV_HW800480F)
  char TS_SERVER_ID[] = "800x480";
#elif defined(AW_DEV_HWCAP480272F)
  char TS_SERVER_ID[] = "480x272";
#else
//"not supported"
#endif

  aw_ts_id sys_ts = aw_ts_serv_id_get(TS_SERVER_ID, 0, 0);
  return_value_if_fail(sys_ts != NULL, NULL);
  
#if defined(AW_DEV_BU21029MUV) || defined(AW_DEV_IMX1050_TS)
  /* 电阻触摸屏需要读取校准数据, 电容屏无需读校准数据 */
  return_value_if_fail(aw_ts_calc_data_read(sys_ts) == AW_OK, NULL);
#else
  /* 电容屏需要进行XY转换 */
  aw_ts_set_orientation(sys_ts, AW_TS_SWAP_XY);
#endif

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
extern aw_emwin_fb_info_t* aworks_get_fb(void);
extern int aworks_get_fb_size();
static lcd_flush_t s_lcd_flush_default = NULL;

static ret_t lcd_aworks_fb_flush(lcd_t* lcd) {
#if 0  // 是否等待垂直同步
  // aw_emwin_fb_vram_addr_set 与 aw_cache_flush 配合用效果最好，但有等待时间
  aw_emwin_fb_vram_addr_set(aworks_get_fb(), aworks_get_online_fb()); // max 13ms wait
#endif

  if (s_lcd_flush_default != NULL) {
    s_lcd_flush_default(lcd);
  }

  // 单用 aw_cache_flush 能极大改善干扰线问题，但不能完全去除
  aw_cache_flush(aworks_get_online_fb(), aworks_get_fb_size()); // max 2ms wait
  return RET_OK;
}

#ifndef WITH_THREE_FB

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

  uint8_t* next_online_fb = mem->offline_fb;
  mem->offline_fb = mem->online_fb;
  mem->online_fb = next_online_fb;

  aw_cache_flush(next_online_fb, aworks_get_fb_size()); // max 2ms wait
  aw_emwin_fb_vram_addr_set(aworks_get_fb(), (uintptr_t)next_online_fb); // max 13ms wait, 等待垂直同步并交换fb
  return RET_OK;
}

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = lcd_mem_bgr565_create_double_fb(w, h, (uint8_t*) aworks_get_online_fb(),
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

static uint32_t* s_current_vram = NULL;
static uint32_t* s_fblist_readys = NULL;
static uint32_t* s_fblist_frees = NULL;
static uint8_t* s_dirty_offline = NULL;  // 指向mem->offline_fb, 代表当前offline缓冲是否已经被写脏

static uint32_t* aworks_fblist_pop_ready() {
  uint32_t* fb = s_fblist_readys;
  s_fblist_readys = NULL;
  return fb;
}

static void aworks_fblist_push_ready(uint32_t* fb) {
  assert(s_fblist_readys == NULL);
  s_fblist_readys = fb;
}

static uint32_t* aworks_fblist_pop_free() {
  uint32_t* fb = s_fblist_frees;
  s_fblist_frees = NULL;
  return fb;
}

static void aworks_fblist_push_free(uint32_t* fb) {
  assert(s_fblist_frees == NULL);
  s_fblist_frees = fb;
}

#define SWAP_STACK_SIZE 1 * 1024
aw_local void __swap_task_entry(void *p_arg)
{
  while (1) {
    uint32_t* ready = aworks_fblist_pop_ready();
    if (ready) {
      uint32_t* last_online = s_current_vram;
      aw_emwin_fb_vram_addr_set(aworks_get_fb(), (uintptr_t)(s_current_vram = ready));
      aworks_fblist_push_free(last_online);
    } else {
      aw_mdelay(2);
    }
  }
}

static ret_t __swap_idle_entry(const idle_info_t* idle) {
  lcd_mem_t* mem = (lcd_mem_t*)idle->ctx;

  // 检查是否有脏的offline滞留, 如果有, 则强制更新到ready, 下一次循环把最新的帧刷新到online
  // 并且将mem->offline_fb指向最新的free区域
  if (s_dirty_offline) {
    uint32_t* freefb = aworks_fblist_pop_free();
    if (freefb) {
      aw_cache_flush(mem->offline_fb, aworks_get_fb_size());
      aworks_fblist_push_ready((uint32_t*)mem->offline_fb);

      mem->offline_fb = (uint8_t*)freefb;
      s_dirty_offline = NULL;
    }
  }
  return RET_REPEAT;
}

static void aworks_fblist_init(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint32_t* frame_buffer = (uint32_t*)mem->online_fb;
  uint32_t* next_frame_buffer = (uint32_t*)mem->next_fb;
  int fb_size = aworks_get_fb_size();

  assert(frame_buffer && next_frame_buffer);
  memset(next_frame_buffer, 0x00, fb_size);

  s_current_vram = frame_buffer;
  s_fblist_frees = next_frame_buffer;
  s_fblist_readys = NULL;
  s_dirty_offline = NULL;

  AW_TASK_DECL_STATIC(swap_task, SWAP_STACK_SIZE);
  AW_TASK_INIT(swap_task,      /* 任务实体 */
               "swap_task",   /* 任务名字 */
               1,             /* 任务优先级 */
               SWAP_STACK_SIZE, /* 任务堆栈大小 */
               __swap_task_entry,  /* 任务入口函数 */
               NULL);         /* 任务入口参数 */
  AW_TASK_STARTUP(swap_task); /* 启动任务 */

  // 创建idle任务(同gui线程), 检查是否有滞留的脏offline缓冲, 并刷新到online
  idle_add(__swap_idle_entry, lcd);
}

static ret_t lcd_aworks_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  if (lcd_is_swappable(lcd)) {
    lcd_mem_t* mem = (lcd_mem_t*)lcd;
    (void)mem;
    s_dirty_offline = NULL; // 新的一帧开始绘制
  }
  return RET_OK;
}

static ret_t lcd_aworks_swap(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  uint32_t* freefb = aworks_fblist_pop_free();
  if (freefb) {
    aw_cache_flush(mem->offline_fb, aworks_get_fb_size());
    aworks_fblist_push_ready((uint32_t*)mem->offline_fb);

    mem->offline_fb = (uint8_t*)freefb;
  } else {
    // 当前帧已经被更新到offline, 但由于没有可切换的free缓冲器(正在swap过程中)
    // 所以当前offline没有被及时更新到ready区, 会导致最后一帧内容滞留在offline中
    // 需要swap线程检测出来并强制将最新的offline放回ready, 保证最新的内容更新到online
    // 
    s_dirty_offline = mem->offline_fb;
  }
  return RET_OK;
}

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = lcd_mem_bgr565_create_three_fb(w, h, (uint8_t*) aworks_get_online_fb(),
        (uint8_t*) aworks_get_offline_fb(), aw_mem_align(aworks_get_fb_size(), AW_CACHE_LINE_SIZE));

  if (lcd != NULL) {
    // 改进flush机制, 每次flush后加入cache_flush (旋转屏幕方向后进入flush流程)
    s_lcd_flush_default = lcd->flush;
    lcd->flush = lcd_aworks_fb_flush;

    // 使用swap机制(正常屏幕方向进入swap流程)
    lcd->begin_frame = lcd_aworks_begin_frame;
    lcd->swap = lcd_aworks_swap;
  }

  aworks_fblist_init(lcd);
  return lcd;
}

#endif // WITH_THREE_FB

#include "main_loop/main_loop_raw.inc"
