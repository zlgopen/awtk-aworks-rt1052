#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

# XXX: make sure no no ascii chars in the path name.

SRC_ROOT_DIR=os.getcwd();
DST_ROOT='D:\\work\\AWorks\\sdk\\aworks_m105x_sdk_1.0.1-alpha\\img_rt1050_debug';

def copyFile(src, dst):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(dst, src)
    print(s + '->' + d)
    dir=os.path.dirname(d)
    if os.path.exists(dir):
        shutil.copyfile(s, d)
    else:
        os.makedirs(dir)
        shutil.copyfile(s, d)

def copyFiles(src, dst):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(dst, src)
    print(s + '->' + d)
    shutil.rmtree(d, True)
    shutil.copytree(s, d)

copyFiles('awtk-port', DST_ROOT)
copyFiles('awtk/3rd/cairo/cairo', DST_ROOT)
copyFiles('awtk/3rd/pixman/pixman', DST_ROOT)
copyFiles('awtk/3rd/gpinyin/src', DST_ROOT)
copyFiles('awtk/3rd/gpinyin/include', DST_ROOT)
copyFiles('awtk/3rd/libunibreak/src', DST_ROOT)

copyFiles('awtk/src/misc', DST_ROOT)
copyFiles('awtk/src/xml', DST_ROOT)
copyFiles('awtk/src/base', DST_ROOT)
copyFiles('awtk/src/font', DST_ROOT)
copyFiles('awtk/src/blend', DST_ROOT)
copyFiles('awtk/src/ui_loader', DST_ROOT)
copyFiles('awtk/src/image_loader', DST_ROOT)
copyFiles('awtk/src/ext_widgets', DST_ROOT)
copyFiles('awtk/src/widget_animators', DST_ROOT)
copyFiles('awtk/src/window_animators', DST_ROOT)
copyFiles('awtk/demos/assets', DST_ROOT)

copyFile('awtk/src/awtk.c', DST_ROOT)
copyFile('awtk/src/awtk.h', DST_ROOT)
copyFile('awtk/src/platforms/raw/fs_os.c', DST_ROOT)
copyFile('awtk/src/vgcanvas/vgcanvas_cairo.c', DST_ROOT)

LCD_FILES=['lcd_mem.h', 
    'lcd_mem.inc', 
    'lcd_mem_rgba8888.h', 
    'lcd_mem_rgba8888.c', 
    'lcd_mem_bgra8888.h', 
    'lcd_mem_bgra8888.c', 
    'lcd_mem_rgb565.h', 
    'lcd_mem_rgb565.c']
for f in LCD_FILES:
    filename=os.path.join('awtk/src/lcd/', f);
    copyFile(filename, DST_ROOT)

INPUT_METHODS_FILES=['input_engines/input_engine_pinyin.cpp',
        'input_methods/input_method_creator.c',
        'input_methods/input_method_default.inc',
        'input_methods/suggest_words.inc']
for f in INPUT_METHODS_FILES:
    filename=os.path.join('awtk/src/', f);
    copyFile(filename, DST_ROOT)

MAIN_LOOP_FILES=['main_loop_simple.h', 'main_loop_simple.c', 'main_loop_raw.inc']
for f in MAIN_LOOP_FILES:
    filename=os.path.join('awtk/src/main_loop', f);
    copyFile(filename, DST_ROOT)

DEMO_FILES=['assets.h', 'assets.c', 'demo_main.c', 'demo_ui_app.c', 'common.inc']
for f in DEMO_FILES:
    filename=os.path.join('awtk/demos', f);
    copyFile(filename, DST_ROOT)

