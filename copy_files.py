#!/usr/bin/python

import os
import glob
import copy
import shutil
import platform


# XXX: make sure no no ascii chars in the path name.

AWTK_ROOT_DIR = 'D:/z_lab';
PORT_ROOT_DIR = os.getcwd();
DST_ROOT_DIR = 'D:/z_lab/awtk-aworks-rt1052/broad';
NANOVG_BACKEND = 'AGGE'


def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

def copyFile(src_root_dir, src, dst_root_dir, dst):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    dir=os.path.dirname(d)
    if os.path.exists(dir):
        shutil.copyfile(s, d)
    else:
        os.makedirs(dir)
        shutil.copyfile(s, d)

def copyAwtkFile(src, dst):
    copyFile(AWTK_ROOT_DIR, src, DST_ROOT_DIR, dst)

def copyPortFile(src, dst):
    copyFile(PORT_ROOT_DIR, src, DST_ROOT_DIR, dst)

def copyFiles(src_root_dir, src, dst_root_dir, dst):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    shutil.rmtree(d, True)
    shutil.copytree(s, d, ignore=shutil.ignore_patterns("*.obj"))

def copyAwtkFiles(src, dst):
    copyFiles(AWTK_ROOT_DIR, src, DST_ROOT_DIR, dst)

def copyPortFiles(src, dst):
    copyFiles(PORT_ROOT_DIR, src, DST_ROOT_DIR, dst)

copyPortFiles('awtk-port', 'awtk-port')
copyAwtkFiles('awtk/3rd/stb', 'awtk/3rd/stb')
copyAwtkFiles('awtk/3rd/libunibreak', 'awtk/3rd/libunibreak')

copyAwtkFiles('awtk/3rd/nanovg/base', 'awtk/3rd/nanovg/base')
if NANOVG_BACKEND == 'AGG':
  copyAwtkFiles('awtk/3rd/agg', 'awtk/3rd/agg')
  copyAwtkFiles('awtk/3rd/nanovg/agg', 'awtk/3rd/nanovg/agg')
elif NANOVG_BACKEND == 'AGGE':
  copyAwtkFiles('awtk/3rd/agge', 'awtk/3rd/agge')
  copyAwtkFiles('awtk/3rd/nanovg/agge', 'awtk/3rd/nanovg/agge')
elif NANOVG_BACKEND == 'BGFX':
  copyAwtkFiles('awtk/3rd/nanovg/bgfx', 'awtk/3rd/nanovg/bgfx')
else:
  copyAwtkFiles('awtk/3rd/nanovg/gl', 'awtk/3rd/nanovg/gl')
copyAwtkFiles('awtk/3rd/gpinyin/src', 'awtk/3rd/gpinyin/src')
copyAwtkFiles('awtk/3rd/gpinyin/include', 'awtk/3rd/gpinyin/include')

copyAwtkFiles('awtk/src/misc', 'awtk/src/misc')
copyAwtkFiles('awtk/src/xml', 'awtk/src/xml')
copyAwtkFiles('awtk/src/base', 'awtk/src/base')
copyAwtkFiles('awtk/src/font_loader', 'awtk/src/font_loader')
copyAwtkFiles('awtk/src/blend', 'awtk/src/blend')
copyAwtkFiles('awtk/src/ui_loader', 'awtk/src/ui_loader')
copyAwtkFiles('awtk/src/image_loader', 'awtk/src/image_loader')
copyAwtkFiles('awtk/src/ext_widgets', 'awtk/src/ext_widgets')
copyAwtkFiles('awtk/src/widget_animators', 'awtk/src/widget_animators')

copyAwtkFile('awtk/src/awtk.c', 'awtk/src/awtk.c')
copyAwtkFile('awtk/src/awtk.h', 'awtk/src/awtk.h')
copyAwtkFile('awtk/src/vgcanvas/vgcanvas_nanovg.c', 'awtk/src/vgcanvas/vgcanvas_nanovg.c')

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
    copyAwtkFile(filename, filename)

INPUT_METHODS_FILES=['input_engines/input_engine_pinyin.cpp',
        'input_methods/input_method_creator.c',
        'input_methods/input_method_default.inc',
        'input_methods/input_method_null.inc',
        'input_methods/suggest_words.inc']
for f in INPUT_METHODS_FILES:
    filename=os.path.join('awtk/src/', f);
    copyAwtkFile(filename, filename)

MAIN_LOOP_FILES=['main_loop_simple.h', 'main_loop_simple.c', 'main_loop_raw.inc']
for f in MAIN_LOOP_FILES:
    filename=os.path.join('awtk/src/main_loop', f);
    copyAwtkFile(filename, filename)


WINDOW_ANIMATORS_FILES=['bottom_to_top.inc', 'top_to_bottom.inc', 'center_scale.inc', 'vtranslate.inc',
'common.inc', 'window_animator_fb.c', 'fade.inc', 'htranslate.inc']
for f in WINDOW_ANIMATORS_FILES:
    filename=os.path.join('awtk/src/window_animators', f);
    copyAwtkFile(filename, filename)

DEMO_FILES=['assets.h', 'assets.c', 'demo_main.c', 'demo_ui_app.c', 'common.inc']
for f in DEMO_FILES:
    sfilename=os.path.join('awtk/demos', f);
    dfilename=os.path.join('awtk-demos/', f);
    copyAwtkFile(sfilename, dfilename)

copyAwtkFiles('awtk/demos/assets', 'awtk-demos/assets')

copyPortFiles('pxp', 'pxp')
# copyPortFile('user_code/main.c', 'awtk-demos/main.c')	

