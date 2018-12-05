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

#include "aw_system.h"
#include "aw_delay.h"
#include "base/mem.h"
#include "base/timer.h"

uint32_t get_time_ms() {
  return aw_sys_tick_get();
}

void sleep_ms(uint32_t ms) {
  aw_mdelay(ms);
}

#define TK_MEM_SIZE 6 * 1024 * 1024

ret_t platform_prepare(void) {
  uint32_t* mem = (uint32_t*) aw_mem_alloc(TK_MEM_SIZE);
  return_value_if_fail(mem != NULL, RET_OOM);

  tk_mem_init(mem, TK_MEM_SIZE);

  return RET_OK;
}
