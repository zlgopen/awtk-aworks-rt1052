#include "io/sys/aw_stat.h"
#include "io/sys/aw_statvfs.h"
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

static bool_t fs_os_file_eof(fs_file_t* file) {
  assert(file);
  int fd = (int)(file->data);

  return aw_lseek(fd, 0, SEEK_CUR) == fs_file_size(file) ? TRUE : FALSE;
}

static int64_t fs_os_file_tell(fs_file_t* file) {
  assert(file);
  int fd = (int)(file->data);

  return aw_lseek(fd, 0, SEEK_CUR);
}

static int64_t fs_os_file_size(fs_file_t* file) {
  fs_stat_info_t st;

  if (fs_file_stat(file, &st) == RET_OK && st.is_reg_file) {
    return st.size;
  } else {
    return -1;
  }
}

static ret_t fs_os_file_sync(fs_file_t* file) {
  assert(file);
  int fd = (int)(file->data);

  return aw_fsync(fd) == AW_OK ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_stat(fs_file_t* file, fs_stat_info_t* fst) {
  assert(file);
  ret_t ret = RET_OK;
  struct aw_stat st;
  int fd = (int)(file->data);

  if (aw_fstat(fd, &st) == AW_OK) {
    fst->dev = st.st_dev;
    fst->ino = st.st_ino;
    fst->mode = st.st_mode;
    fst->nlink = st.st_nlink;
    fst->uid = st.st_uid;
    fst->gid = st.st_gid;
    fst->rdev = st.st_rdev;
    fst->size = st.st_size;
    fst->atime = st.st_atim.tv_sec;
    fst->mtime = st.st_mtim.tv_sec;
    fst->ctime = st.st_ctim.tv_sec;
    fst->is_dir = S_ISDIR(st.st_mode);
    fst->is_reg_file = S_ISREG(st.st_mode);
    fst->is_link = S_ISLNK(st.st_mode);
  } else {
    memset(fst, 0x00, sizeof(fs_stat_info_t));
    ret = RET_FAIL;
  }

  return ret;
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
                                                    .printf = NULL,
                                                    .seek = fs_os_file_seek,
                                                    .truncate = fs_os_file_truncate,
                                                    .eof = fs_os_file_eof,
                                                    .tell = fs_os_file_tell,
                                                    .size = fs_os_file_size,
                                                    .sync = fs_os_file_sync,
                                                    .stat = fs_os_file_stat,
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

static ret_t fs_os_file_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  assert(name && new_name);

  return aw_rename(name, new_name) == AW_OK ? RET_OK : RET_FAIL;
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

static ret_t fs_os_create_dir(fs_t* fs, const char* name) {
  (void)fs;
  assert(name);

  return aw_mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO) == AW_OK ? RET_OK : RET_FAIL;
}

static ret_t fs_os_change_dir(fs_t* fs, const char* name) {
  (void)fs;
  assert(name);

  return aw_chdir(name) == AW_OK ? RET_OK : RET_FAIL;
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

static ret_t fs_os_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  assert(name && new_name);

  return aw_rename(name, new_name) == AW_OK ? RET_OK : RET_FAIL;
}

/*----------------------------------------------------------------------------*/
/* 文件系统操作                                                               */
/*----------------------------------------------------------------------------*/

static int32_t fs_os_get_file_size(fs_t* fs, const char* name) {
  struct aw_stat st;

  return_value_if_fail(aw_stat(name, &st) == AW_OK, -1);
  return st.st_size;
}

static ret_t fs_os_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb,
                                 int32_t* total_kb) {
  (void)fs;
  assert(volume && free_kb && total_kb);
  ret_t ret = RET_OK;
  struct aw_statvfs buf;

  if (aw_statvfs(volume, &buf) == AW_OK) {
    /* AWTK 接口返回的空间大小单位为 KB，因此除以 1024 */
    *free_kb = buf.f_bfree * buf.f_bsize / 1024;
    *total_kb = buf.f_blocks * buf.f_bsize / 1024;
  } else {
    *free_kb = 0;
    *total_kb = 0;
    ret = RET_FAIL;
  }

  return ret;
}

static ret_t fs_os_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  (void)path;

  return RET_NOT_IMPL;
}

static ret_t fs_os_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  assert(path);

  *path = '\0';
  return aw_getcwd(path, MAX_PATH) != NULL ? RET_OK : RET_FAIL;
}

static ret_t fs_os_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  (void)path;

  return RET_NOT_IMPL;
}

static ret_t fs_os_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  fs_file_t* file = NULL;
  ret_t ret = RET_FAIL;
  assert(fs && name && fst);

  file = fs_open_file(fs, name, "r");
  if (file) {
    ret = fs_file_stat(file, fst);
    fs_file_close(file);
  }

  return ret;
}

static ret_t fs_os_get_temp_path(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  (void)path;

  return RET_NOT_IMPL;
}

/*----------------------------------------------------------------------------*/
/* 入口              													      */
/*----------------------------------------------------------------------------*/

static const fs_t s_os_fs = {.open_file = fs_os_open_file,
                             .remove_file = fs_os_remove_file,
                             .file_exist = fs_os_file_exist,
                             .file_rename = fs_os_file_rename,

                             .open_dir = fs_os_open_dir,
                             .create_dir = fs_os_create_dir,
                             .change_dir = fs_os_change_dir,
                             .remove_dir = fs_os_remove_dir,
                             .dir_exist = fs_os_dir_exist,
                             .dir_rename = fs_os_dir_rename,

                             .get_file_size = fs_os_get_file_size,
                             .get_disk_info = fs_os_get_disk_info,
                             .get_cwd = fs_os_get_cwd,
                             .get_exe = fs_os_get_exe,
                             .get_user_storage_path = fs_os_get_user_storage_path,
                             .stat = fs_os_stat,
                             .get_temp_path = fs_os_get_temp_path};

fs_t* os_fs(void) {
  return (fs_t*)&s_os_fs;
}
