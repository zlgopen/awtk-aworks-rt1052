/**
 * File:   platform.c
 * Author: AWTK Develop Team
 * Brief:  platform dependent function of aworks
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
 * 2018-05-23 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "aw_int.h"
#include "aw_system.h"
#include "aw_delay.h"
#include "base/timer.h"
#include "tkc/mem.h"
#include "tkc/date_time.h"

#if 1
// for compatible with ellipse(gcc) and keil(armcc)

#include "aw_time.h"
static ret_t date_time_get_now_impl(date_time_t* dt) {
  
  aw_err_t ret;
  aw_tm_t t;
  
  ret = aw_tm_get(&t);

  if (ret == AW_OK) {
    dt->second = t.tm_sec;
    dt->minute = t.tm_min;
    dt->hour = t.tm_hour;
    dt->day = t.tm_mday;
    dt->month = t.tm_mon + 1;
    dt->year = t.tm_year + 1900;
    dt->wday = t.tm_wday;
  } else {
    dt->second = 0;
    dt->minute = 0;
    dt->hour = 0;
    dt->day = 11;
    dt->month = 11;
    dt->year = 2018;
    dt->wday = 0;
  }

  return RET_OK;
}
#else
// only for ellipse(gcc)

#include <time.h>
#include <sys/time.h>
static ret_t date_time_get_now_impl(date_time_t* dt) {
  time_t now = time(0);
  struct tm* t = localtime(&now);

  if (t) {
    dt->second = t->tm_sec;
    dt->minute = t->tm_min;
    dt->hour = t->tm_hour;
    dt->day = t->tm_mday;
    dt->month = t->tm_mon + 1;
    dt->year = t->tm_year + 1900;
    dt->wday = t->tm_wday;
  } else {
    dt->second = 0;
    dt->minute = 0;
    dt->hour = 0;
    dt->day = 11;
    dt->month = 11;
    dt->year = 2018;
    dt->wday = 0;
  }

  return RET_OK;
}
#endif

uint64_t get_time_ms64() {
  uint32_t tick = 0;
  uint64_t time_ms64 = 0;
  static volatile uint32_t g_last_tick = 0;
  static volatile uint64_t g_tick64_base = 0;

  AW_INT_CPU_LOCK_DECL(cpu_lock_key);
  AW_INT_CPU_LOCK(cpu_lock_key);
  tick = aw_sys_tick_get();
  if (g_last_tick > tick) {
    g_tick64_base += 0xFFFFFFFFu;
    g_tick64_base += 1;
  }
  g_last_tick = tick;
  time_ms64 = g_tick64_base + tick;
  AW_INT_CPU_UNLOCK(cpu_lock_key);
  return time_ms64;
}

void sleep_ms(uint32_t ms) {
  aw_mdelay(ms);
}

#define TK_MEM_SIZE 6 * 1024 * 1024

ret_t platform_prepare(void) {
#ifndef HAS_STD_MALLOC
  uint32_t* mem = (uint32_t*) aw_mem_alloc(TK_MEM_SIZE);
  return_value_if_fail(mem != NULL, RET_OOM);

  tk_mem_init(mem, TK_MEM_SIZE);
#endif

  date_time_set_impl(date_time_get_now_impl);
  return RET_OK;
}
