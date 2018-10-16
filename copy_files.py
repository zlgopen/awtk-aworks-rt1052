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
DST_ROOT_DIR='D:\\work\\m1052_board\\broad';

def copyFile(src, dst):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(DST_ROOT_DIR, dst)
    print(s + '->' + d)
    dir=os.path.dirname(d)
    if os.path.exists(dir):
        shutil.copyfile(s, d)
    else:
        os.makedirs(dir)
        shutil.copyfile(s, d)

def copyFiles(src, dst):
    s = joinPath(SRC_ROOT_DIR, src)
    d = joinPath(DST_ROOT_DIR, dst)
    print(s + '->' + d)
    shutil.rmtree(d, True)
    shutil.copytree(s, d)

copyFiles('awtk-port', 'awtk-port')
copyFiles('awtk/3rd/stb', 'awtk/3rd/stb')
copyFiles('awtk/3rd/libunibreak', 'awtk/3rd/libunibreak')

copyFiles('awtk/3rd/agge', 'awtk/3rd/agge')
copyFiles('awtk/3rd/nanovg/base', 'awtk/3rd/nanovg/base')
copyFiles('awtk/3rd/nanovg/agge', 'awtk/3rd/nanovg/agge')
copyFiles('awtk/3rd/gpinyin/src', 'awtk/3rd/gpinyin/src')
copyFiles('awtk/3rd/gpinyin/include', 'awtk/3rd/gpinyin/include')

copyFiles('awtk/src/misc', 'awtk/src/misc')
copyFiles('awtk/src/xml', 'awtk/src/xml')
copyFiles('awtk/src/base', 'awtk/src/base')
copyFiles('awtk/src/font', 'awtk/src/font')
copyFiles('awtk/src/blend', 'awtk/src/blend')
copyFiles('awtk/src/ui_loader', 'awtk/src/ui_loader')
copyFiles('awtk/src/image_loader', 'awtk/src/image_loader')
copyFiles('awtk/src/ext_widgets', 'awtk/src/ext_widgets')
copyFiles('awtk/src/widget_animators', 'awtk/src/widget_animators')

copyFile('awtk/src/awtk.c', 'awtk/src/awtk.c')
copyFile('awtk/src/awtk.h', 'awtk/src/awtk.h')
copyFile('awtk/src/vgcanvas/vgcanvas_nanovg.c', 'awtk/src/vgcanvas/vgcanvas_nanovg.c')

LCD_FILES=['lcd_mem.h', 
    'lcd_mem.inc', 
    'lcd_mem_rgba8888.h', 
    'lcd_mem_rgba8888.c', 
    'lcd_mem_bgra8888.h', 
    'lcd_mem_bgra8888.c', 
    'lcd_mem_bgr565.h', 
    'lcd_mem_bgr565.c']
for f in LCD_FILES:
    filename=os.path.join('awtk/src/lcd/', f);
    copyFile(filename, filename)

INPUT_METHODS_FILES=['input_engines/input_engine_pinyin.cpp',
        'input_methods/input_method_creator.c',
        'input_methods/input_method_default.inc',
		'input_methods/input_method_null.inc',
        'input_methods/suggest_words.inc']
for f in INPUT_METHODS_FILES:
    filename=os.path.join('awtk/src/', f);
    copyFile(filename, filename)

MAIN_LOOP_FILES=['main_loop_simple.h', 'main_loop_simple.c', 'main_loop_raw.inc']
for f in MAIN_LOOP_FILES:
    filename=os.path.join('awtk/src/main_loop', f);
    copyFile(filename, filename)


WINDOW_ANIMATORS_FILES=['bottom_to_top.inc', 'top_to_bottom.inc', 'center_scale.inc', 'vtranslate.inc',
'common.inc', 'window_animator_fb.c', 'fade.inc', 'htranslate.inc']
for f in WINDOW_ANIMATORS_FILES:
    filename=os.path.join('awtk/src/window_animators', f);
    copyFile(filename, filename)

copyFiles('pxp', 'pxp')

DEMO_FILES=['assets.h', 'assets.c', 'demo_main.c', 'demo_ui_app.c', 'common.inc']
for f in DEMO_FILES:
    sfilename=os.path.join('awtk/demos', f);
    dfilename=os.path.join('awtk-demos/', f);
    copyFile(sfilename, dfilename)
copyFile('user_code/main.c', 'awtk-demos/main.c')	
copyFiles('awtk/demos/assets', 'awtk-demos/assets')

