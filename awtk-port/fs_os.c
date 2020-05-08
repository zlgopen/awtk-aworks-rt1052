#include "io/sys/aw_stat.h"
#include "io/aw_unistd.h"
#include "io/aw_dirent.h"
#include "io/aw_fcntl.h"
#include "fs/aw_mount.h"
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

/*----------------------------------------------------------------------------*/
/* 文件操作	                                                                  */
/*----------------------------------------------------------------------------*/

static int32_t fs_os_file_read(fs_file_t* file, void* buffer, uint32_t size) {
	assert(file);
	int fd = (int)(file->data);

  return (int32_t)aw_read(fd, buffer, size);
}

static int32_t fs_os_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
	assert(file);
	int fd = (int)(file->data);

  return aw_write(fd, buffer, size);
}

static ret_t fs_os_file_seek(fs_file_t* file, int32_t offset) {
	assert(file);
	int fd = (int)(file->data);

  return aw_lseek(fd, offset, SEEK_SET) >= 0 ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_truncate(fs_file_t* file, int32_t size) {
	assert(file);
	int fd = (int)(file->data);

  return aw_ftruncate(fd, size) == AW_OK ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_close(fs_file_t* file) {
	assert(file);
  int fd = (int)(file->data);

  aw_close(fd);
  TKMEM_FREE(file);

  return RET_OK;
}

static const fs_file_vtable_t s_fs_file_vtable_t = {.read = fs_os_file_read,
                                                    .write = fs_os_file_write,
                                                    .seek = fs_os_file_seek,
                                                    .truncate = fs_os_file_truncate,
                                                    .close = fs_os_file_close};

static fs_file_t* fs_file_create(int fd) {
  fs_file_t* f = NULL;
  return_value_if_fail(fd >= 0, NULL);

  f = TKMEM_ZALLOC(fs_file_t);
  if (f != NULL) {
    f->vt = &s_fs_file_vtable_t;
    f->data = (void*)fd;
  } else {
    aw_close(fd);
  }

  return f;
}

static fs_file_t* fs_os_open_file(fs_t* fs, const char* name, const char* mode) {
  (void)fs;
  assert(name && mode);
  int oflag = 0;

  if (strchr(mode, 'r')) {
  	oflag |= O_RDONLY;
  }

  if (strchr(mode, 'w')) {
  	oflag |= O_CREAT | O_RDWR;

    if (strchr(mode, '+')) {
    	oflag |= O_APPEND;
    } else {
    	oflag |= O_TRUNC;
    }
  }

  return fs_file_create(aw_open(name, oflag, 0666));
}

static ret_t fs_os_remove_file(fs_t* fs, const char* name) {
  (void)fs;
  assert(name);

  return aw_unlink(name) == AW_OK ? RET_OK : RET_FAIL;
}

static bool_t fs_os_file_exist(fs_t* fs, const char* name) {
	(void)fs;
	assert(name);

  struct aw_stat st;
  return (aw_stat(name, &st) == 0 && S_ISREG(st.st_mode));
}

static bool_t fs_os_file_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  assert(name && new_name);

  return (aw_rename(name, new_name) == AW_OK);
}

/*----------------------------------------------------------------------------*/
/* 文件夹操作                                                                 */
/*----------------------------------------------------------------------------*/

struct fs_dir_ex {
	fs_dir_t dir;
	char dir_name[MAX_PATH + 1];
};

static ret_t fs_os_dir_read(fs_dir_t* dir, fs_item_t* item) {
	assert(dir && dir->data && item);
	char item_path[MAX_PATH];

	struct aw_dir* d = (struct aw_dir*)(dir->data);
  struct aw_dirent* entry = aw_readdir(d);

  memset(item, 0x00, sizeof(fs_item_t));
  if (entry != NULL) {
  	item_path[0] = '\0';
  	strncat(item_path, ((struct fs_dir_ex*)dir)->dir_name, MAX_PATH);
  	strncat(item_path, "/", MAX_PATH);
  	strncat(item_path, entry->d_name, MAX_PATH);

  	struct aw_stat st;
  	return_value_if_fail(aw_stat(item_path, &st) == AW_OK, RET_FAIL);

    item->is_dir = S_ISDIR(st.st_mode);
    item->is_reg_file = S_ISREG(st.st_mode);
    tk_strncpy(item->name, entry->d_name, MAX_PATH);
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t fs_os_dir_rewind(fs_dir_t* dir) {
	assert(dir);
  assert(!"fs_os_dir_rewind not supported yet");

  return RET_NOT_IMPL;
}

static ret_t fs_os_dir_close(fs_dir_t* dir) {
	assert(dir && dir->data);
  struct aw_dir* d = (struct aw_dir*)dir->data;

  aw_closedir(d);
  TKMEM_FREE(dir);

  return RET_OK;
}

static const fs_dir_vtable_t s_fs_dir_vtable_t = {.read = fs_os_dir_read,
                                                  .rewind = fs_os_dir_rewind,
                                                  .close = fs_os_dir_close};

static fs_dir_t* fs_dir_create(struct aw_dir* dir, const char* dir_name) {
  fs_dir_t* d = NULL;
  return_value_if_fail(dir != NULL, NULL);

  struct fs_dir_ex* d_ex = TKMEM_ZALLOC(struct fs_dir_ex);
  if (d_ex != NULL) {
    d = &d_ex->dir;
    d->vt = &s_fs_dir_vtable_t;
    d->data = dir;
    tk_strncpy(d_ex->dir_name, dir_name, MAX_PATH);
  } else {
    aw_closedir(dir);
  }

  return d;
}

static fs_dir_t* fs_os_open_dir(fs_t* fs, const char* name) {
	(void)fs;
	assert(name);

  return fs_dir_create(aw_opendir(name), name);
}

static ret_t fs_os_remove_dir(fs_t* fs, const char* name) {
  (void)fs;
  assert(name);

  return aw_rmdir(name) == AW_OK ? RET_OK : RET_FAIL;
}

static bool_t fs_os_dir_exist(fs_t* fs, const char* name) {
	(void)fs;
	assert(name);

	struct aw_stat st;
  return (aw_stat(name, &st) == AW_OK && S_ISDIR(st.st_mode));
}

static bool_t fs_os_dir_rename(fs_t* fs, const char* name, const char* new_name) {
	(void)fs;
	assert(name && new_name);

  return (aw_rename(name, new_name) == AW_OK);
}

/*----------------------------------------------------------------------------*/
/* 文件系统操作                                                               */
/*----------------------------------------------------------------------------*/

static int32_t fs_os_get_file_size(fs_t* fs, const char* name) {
	struct aw_stat st;

  return_value_if_fail(aw_stat(name, &st) == AW_OK, -1);
  return st.st_size;
}

static ret_t fs_os_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb, int32_t* total_kb) {
	(void)fs;
	assert(free_kb && total_kb);
	assert(!"fs_os_get_disk_info not supported yet");

  *free_kb = 0;
  *total_kb = 0;
  return RET_NOT_IMPL;
}

static ret_t fs_os_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  (void)path;
  assert(!"fs_os_get_exe not supported yet");

  return RET_NOT_IMPL;
}

static ret_t fs_os_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
	(void)fs;
	assert(path);

  *path = '\0';
  return aw_getcwd(path, MAX_PATH) != NULL ? RET_OK : RET_FAIL;
}

/*----------------------------------------------------------------------------*/
/* 入口              													      */
/*----------------------------------------------------------------------------*/

static const fs_t s_os_fs = {.open_file = fs_os_open_file,
                             .remove_file = fs_os_remove_file,
                             .file_exist = fs_os_file_exist,
                             .file_rename = fs_os_file_rename,

                             .open_dir = fs_os_open_dir,
                             .remove_dir = fs_os_remove_dir,
                             .dir_exist = fs_os_dir_exist,
                             .dir_rename = fs_os_dir_rename,

                             .get_file_size = fs_os_get_file_size,
                             .get_disk_info = fs_os_get_disk_info,
                             .get_cwd = fs_os_get_cwd,
                             .get_exe = fs_os_get_exe};

fs_t* os_fs(void) {
	return (fs_t*)&s_os_fs;
}
