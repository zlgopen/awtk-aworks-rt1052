#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

AWTK_DIR=os.path.normpath('awtk')
TARGET_AWTK_DIR=joinPath('../awtk-aworks-sdk', 'awtk')

def copyFile(src, dst):
    s = joinPath(AWTK_DIR, src)
    d = joinPath(dst, src)
    print(s + '->' + d)
    dir=os.path.dirname(d)
    if os.path.exists(dir):
        shutil.copyfile(s, d)
    else:
        os.makedirs(dir)
        shutil.copyfile(s, d)

def copyFiles(src, dst):
    s = joinPath(AWTK_DIR, src)
    d = joinPath(dst, src)
    print(s + '->' + d)
    shutil.rmtree(d, True)
    shutil.copytree(s, d)

copyFiles('3rd/cairo/cairo', TARGET_AWTK_DIR)
copyFiles('3rd/pixman/pixman', TARGET_AWTK_DIR)
copyFiles('3rd/gpinyin/src', TARGET_AWTK_DIR)
copyFiles('3rd/gpinyin/include', TARGET_AWTK_DIR)
copyFiles('3rd/libunibreak/src', TARGET_AWTK_DIR)

copyFiles('src/misc', TARGET_AWTK_DIR)
copyFiles('src/xml', TARGET_AWTK_DIR)
copyFiles('src/base', TARGET_AWTK_DIR)
copyFiles('src/font', TARGET_AWTK_DIR)
copyFiles('src/blend', TARGET_AWTK_DIR)
copyFiles('src/ui_loader', TARGET_AWTK_DIR)
copyFiles('src/image_loader', TARGET_AWTK_DIR)
copyFiles('src/ext_widgets', TARGET_AWTK_DIR)
copyFiles('src/widget_animators', TARGET_AWTK_DIR)
copyFiles('src/window_animators', TARGET_AWTK_DIR)
copyFiles('demos/assets', TARGET_AWTK_DIR)

copyFile('src/awtk.c', TARGET_AWTK_DIR)
copyFile('src/awtk.h', TARGET_AWTK_DIR)
copyFile('src/platforms/raw/fs_os.c', TARGET_AWTK_DIR)
copyFile('src/vgcanvas/vgcanvas_cairo.c', TARGET_AWTK_DIR)

LCD_FILES=['lcd_mem.h', 
    'lcd_mem.inc', 
    'lcd_mem_rgba8888.h', 
    'lcd_mem_rgba8888.c', 
    'lcd_mem_bgra8888.h', 
    'lcd_mem_bgra8888.c', 
    'lcd_mem_rgb565.h', 
    'lcd_mem_rgb565.c']
for f in LCD_FILES:
    filename=os.path.join('src/lcd/', f);
    copyFile(filename, TARGET_AWTK_DIR)

INPUT_METHODS_FILES=['input_engines/input_engine_pinyin.cpp',
        'input_methods/input_method_creator.c',
        'input_methods/input_method_default.inc',
        'input_methods/suggest_words.inc']
for f in INPUT_METHODS_FILES:
    filename=os.path.join('src/', f);
    copyFile(filename, TARGET_AWTK_DIR)

MAIN_LOOP_FILES=['main_loop_simple.h', 'main_loop_simple.c', 'main_loop_raw.inc']
for f in MAIN_LOOP_FILES:
    filename=os.path.join('src/main_loop', f);
    copyFile(filename, TARGET_AWTK_DIR)

DEMO_FILES=['assets.h', 'assets.c', 'demo_main.c', 'demo_ui_app.c', 'common.inc']
for f in DEMO_FILES:
    filename=os.path.join('demos', f);
    copyFile(filename, TARGET_AWTK_DIR)

