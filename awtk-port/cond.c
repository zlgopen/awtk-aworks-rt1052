/**
 * File:   cond.c
 * Author: AWTK Develop Team
 * Brief:  cond
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-23 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "aw_sem.h"
#include "aw_mem.h"
#include "tkc/mutex.h"
#include "tkc/cond.h"

struct _tk_cond_t {
  tk_mutex_t* lock;
  int waiting;
  int signals;
  aw_semc_id_t wait_sem;
  aw_semc_id_t wait_done;
  AW_SEMC_DECL(__wait_sem);
  AW_SEMC_DECL(__wait_done);
};

tk_cond_t* tk_cond_create(void) {
  tk_cond_t* cond = (tk_cond_t*)aw_mem_calloc(1, sizeof(tk_cond_t));

  if (cond) {
    cond->lock = tk_mutex_create();
    cond->wait_sem = AW_SEMC_INIT(cond->__wait_sem, 0, AW_SEM_Q_FIFO);
    cond->wait_done = AW_SEMC_INIT(cond->__wait_done, 0, AW_SEM_Q_FIFO);
    cond->waiting = cond->signals = 0;

    if (cond->lock == NULL || cond->wait_sem == NULL || cond->wait_done == NULL) {
      tk_cond_destroy(cond);
      cond = NULL;
    }
  }

  return cond;
}

ret_t tk_cond_destroy(tk_cond_t* cond) {
  if (cond) {
    if (cond->lock) {
      tk_mutex_destroy(cond->lock);
    }
    if (cond->wait_sem) {
      aw_semc_terminate(cond->wait_sem);
    }
    if (cond->wait_done) {
      aw_semc_terminate(cond->wait_done);
    }
    aw_mem_free(cond);
  }
  return RET_OK;
}

ret_t tk_cond_signal(tk_cond_t* cond) {
  assert(cond);

  tk_mutex_lock(cond->lock);
  if (cond->waiting > cond->signals) {
    ++cond->signals;
    aw_semc_give(cond->wait_sem);
    tk_mutex_unlock(cond->lock);
    aw_semc_take(cond->wait_done, AW_SEM_WAIT_FOREVER);
  } else {
    tk_mutex_unlock(cond->lock);
  }
  return RET_OK;
}

ret_t tk_cond_broadcast(tk_cond_t* cond) {
  assert(cond);

  tk_mutex_lock(cond->lock);
  if (cond->waiting > cond->signals) {
    int i, num_waiting;

    num_waiting = (cond->waiting - cond->signals);
    cond->signals = cond->waiting;
    for (i = 0; i < num_waiting; ++i) {
      aw_semc_give(cond->wait_sem);
    }

    tk_mutex_unlock(cond->lock);
    for (i = 0; i < num_waiting; ++i) {
      aw_semc_take(cond->wait_done, AW_SEM_WAIT_FOREVER);
    }
  } else {
    tk_mutex_unlock(cond->lock);
  }
  return RET_OK;
}

ret_t tk_cond_wait_timeout(tk_cond_t* cond, tk_mutex_t* mutex, uint32_t ms) {
  assert(cond);
  assert(mutex);

  tk_mutex_lock(cond->lock);
  ++cond->waiting;
  tk_mutex_unlock(cond->lock);

  tk_mutex_unlock(mutex);

  aw_err_t retval;
  if (ms == -1) {
    retval = aw_semc_take(cond->wait_sem, AW_SEM_WAIT_FOREVER);
  } else {
    retval = aw_semc_take(cond->wait_sem, aw_ms_to_ticks(ms));
  }

  tk_mutex_lock(cond->lock);
  if (cond->signals > 0) {
    /* If we timed out, we need to eat a condition signal */
    if (retval == -AW_ETIME) {
      aw_semc_take(cond->wait_sem, AW_SEM_WAIT_FOREVER);
    }
    /* We always notify the signal thread that we are done */
    aw_semc_give(cond->wait_done);
    --cond->signals;
  }
  --cond->waiting;
  tk_mutex_unlock(cond->lock);

  tk_mutex_lock(mutex);
  switch (retval) {
    case AW_OK: return RET_OK;
    case -AW_ETIME: return RET_TIMEOUT;
    default: return RET_FAIL;
  }
}

ret_t tk_cond_wait(tk_cond_t* cond, tk_mutex_t* mutex) {
  return tk_cond_wait_timeout(cond, mutex, -1);
}
