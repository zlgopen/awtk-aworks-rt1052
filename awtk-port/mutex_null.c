/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex do nothing
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
 * 2018-05-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "aw_sem.h"
#include "aw_mem.h"
#include "tkc/mutex.h"

struct _tk_mutex_t {
  AW_MUTEX_DECL(__lock);
};

//static tk_mutex_t s_tk_mutex_null;

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = (tk_mutex_t*)aw_mem_alloc(sizeof(tk_mutex_t));
  if (mutex) {
  	AW_MUTEX_INIT(mutex->__lock, AW_SEM_INVERSION_SAFE);
  }
  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
	assert(mutex);
	aw_err_t err = AW_MUTEX_LOCK( mutex->__lock, AW_SEM_WAIT_FOREVER );

  switch (err) {
    case AW_OK: return RET_OK;
    default: return RET_FAIL;
  }
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
	assert(mutex);
	aw_err_t err = AW_MUTEX_UNLOCK( mutex->__lock );

  switch (err) {
    case AW_OK: return RET_OK;
    default: return RET_FAIL;
  }
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
	assert(mutex);
	AW_MUTEX_TERMINATE( mutex->__lock );

	aw_mem_free(mutex);
  return RET_OK;
}
