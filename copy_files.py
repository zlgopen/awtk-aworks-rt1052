#!/usr/bin/python

import os
import glob
import copy
import shutil
import fnmatch
import platform


# XXX: make sure no no ascii chars in the path name.

AWTK_ROOT_DIR = '../awtk';
PORT_ROOT_DIR = os.getcwd();
DST_ROOT_DIR = './output';

VGCANVAS='NANOVG'
#VGCANVAS='CAIRO'

NANOVG_BACKEND='AGGE'
#NANOVG_BACKEND='AGG'


def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

def copyFile(src_root_dir, src, dst_root_dir, dst):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    if os.path.exists(s) :
        dir=os.path.dirname(d)
        if os.path.exists(dir):
            shutil.copyfile(s, d)
        else:
            os.makedirs(dir)
            shutil.copyfile(s, d)
    else:
        print('!!! copyFile src NOT EXISTS: ' + s)

def copyAwtkFile(src, dst):
    copyFile(AWTK_ROOT_DIR, src, DST_ROOT_DIR, dst)

def copyPortFile(src, dst):
    copyFile(PORT_ROOT_DIR, src, DST_ROOT_DIR, dst)

def ignore_patterns_list(patterns_list):
    def _ignore_patterns(path, names):
        ignored_names = []
        for pattern in patterns_list:
            ignored_names.extend(fnmatch.filter(names, pattern))
        return set(ignored_names)
    return _ignore_patterns

def copyFiles(src_root_dir, src, dst_root_dir, dst, ignore_files = []):
    s = joinPath(src_root_dir, src)
    d = joinPath(dst_root_dir, dst)
    print(s + '->' + d)
    if os.path.exists(s) :
        shutil.rmtree(d, True)
        ignore_files.append('*.o')
        ignore_files.append('*.obj')
        shutil.copytree(s, d, ignore=ignore_patterns_list(ignore_files))
    else:
        print('!!! copyFiles src NOT EXISTS: ' + s)

def copyAwtkFiles(src, dst, ignore_files = []):
    copyFiles(AWTK_ROOT_DIR, src, DST_ROOT_DIR, dst, ignore_files)

def copyPortFiles(src, dst, ignore_files = []):
    copyFiles(PORT_ROOT_DIR, src, DST_ROOT_DIR, dst, ignore_files)

shutil.rmtree(DST_ROOT_DIR, True)

copyAwtkFiles('3rd/stb', 'awtk/3rd/stb')
copyAwtkFiles('3rd/libunibreak', 'awtk/3rd/libunibreak')
copyAwtkFiles('3rd/gpinyin/src', 'awtk/3rd/gpinyin/src')
copyAwtkFiles('3rd/gpinyin/include', 'awtk/3rd/gpinyin/include')

if VGCANVAS == 'NANOVG' and NANOVG_BACKEND == 'AGG':
    copyAwtkFiles('3rd/nanovg/base', 'awtk/3rd/nanovg/base')
    copyAwtkFiles('3rd/agg', 'awtk/3rd/agg')
    copyAwtkFiles('3rd/nanovg/agg', 'awtk/3rd/nanovg/agg')
elif VGCANVAS == 'NANOVG' and NANOVG_BACKEND == 'AGGE':
    copyAwtkFiles('3rd/nanovg/base', 'awtk/3rd/nanovg/base')
    copyAwtkFiles('3rd/agge', 'awtk/3rd/agge')
    copyAwtkFiles('3rd/nanovg/agge', 'awtk/3rd/nanovg/agge')
elif VGCANVAS == 'CAIRO':
    ignorePixmanFiles=['pixman-arm-detect-win32.asm',
        'pixman-mips-dspr2-asm.S',
        'pixman-mips-memcpy-asm.S',
        'pixman-sse2.c',
        'pixman-ssse3.c',
        'pixman-vmx.c']

    ignoreCairoFiles=['stars.c']

    copyAwtkFiles('3rd/cairo', 'awtk/3rd/cairo', ignoreCairoFiles)
    copyAwtkFiles('3rd/pixman', 'awtk/3rd/pixman', ignorePixmanFiles)
else:
    assert 0, " VGCANVAS != {NANOVG, CAIRO} and NANOVG_BACKEND != {AGG, AGGE}"


copyAwtkFiles('src/xml', 'awtk/src/xml')
copyAwtkFiles('src/svg', 'awtk/src/svg')
copyAwtkFiles('src/tkc', 'awtk/src/tkc')
copyAwtkFiles('src/base', 'awtk/src/base')
copyAwtkFiles('src/misc', 'awtk/src/misc')
copyAwtkFiles('src/blend', 'awtk/src/blend')
copyAwtkFiles('src/widgets', 'awtk/src/widgets')
copyAwtkFiles('src/layouters', 'awtk/src/layouters')
copyAwtkFiles('src/ui_loader', 'awtk/src/ui_loader')
copyAwtkFiles('src/clip_board', 'awtk/src/clip_board')
copyAwtkFiles('src/font_loader', 'awtk/src/font_loader')
copyAwtkFiles('src/ext_widgets', 'awtk/src/ext_widgets')
copyAwtkFiles('src/image_loader', 'awtk/src/image_loader')
copyAwtkFiles('src/widget_animators', 'awtk/src/widget_animators')
copyAwtkFiles('src/window_animators', 'awtk/src/window_animators')
copyAwtkFiles('src/dialog_highlighters', 'awtk/src/dialog_highlighters')

copyAwtkFile('src/awtk.h', 'awtk/src/awtk.h')
copyAwtkFile('src/awtk_tkc.h', 'awtk/src/awtk_tkc.h')
copyAwtkFile('src/awtk_base.h', 'awtk/src/awtk_base.h')
copyAwtkFile('src/awtk_global.c', 'awtk/src/awtk_global.c')
copyAwtkFile('src/awtk_global.h', 'awtk/src/awtk_global.h')
copyAwtkFile('src/awtk_widgets.h', 'awtk/src/awtk_widgets.h')
copyAwtkFile('src/awtk_ext_widgets.h', 'awtk/src/awtk_ext_widgets.h')
copyAwtkFile('src/native_window/native_window_raw.c', 'awtk/src/native_window/native_window_raw.c')
copyAwtkFile('src/native_window/native_window_raw.h', 'awtk/src/native_window/native_window_raw.h')
copyAwtkFile('src/window_manager/window_manager_default.c', 'awtk/src/window_manager/window_manager_default.c')
copyAwtkFile('src/window_manager/window_manager_default.h', 'awtk/src/window_manager/window_manager_default.h')
copyAwtkFile('src/graphic_buffer/graphic_buffer_default.c', 'awtk/src/graphic_buffer/graphic_buffer_default.c')

if VGCANVAS == 'NANOVG' and (NANOVG_BACKEND == 'AGG' or NANOVG_BACKEND == 'AGGE'):
    copyAwtkFile('src/vgcanvas/vgcanvas_nanovg_soft.c', 'awtk/src/vgcanvas/vgcanvas_nanovg_soft.c')
    copyAwtkFile('src/vgcanvas/vgcanvas_nanovg_soft.inc', 'awtk/src/vgcanvas/vgcanvas_nanovg_soft.inc')
    copyAwtkFile('src/vgcanvas/vgcanvas_nanovg.inc', 'awtk/src/vgcanvas/vgcanvas_nanovg.inc')
elif VGCANVAS == 'CAIRO':
    copyAwtkFile('src/vgcanvas/vgcanvas_cairo.c', 'awtk/src/vgcanvas/vgcanvas_cairo.c')
else :
    assert 0, " VGCANVAS != {NANOVG, CAIRO} and NANOVG_BACKEND != {AGG, AGGE}"
  
LCD_FILES=['lcd_mem.h',
    'lcd_mem.inc',
    'lcd_mem_rgba8888.h',
    'lcd_mem_rgba8888.c',
    'lcd_mem_bgra8888.h',
    'lcd_mem_bgra8888.c',
    'lcd_mem_rgb565.h',
    'lcd_mem_rgb565.c',
    'lcd_mem_bgr565.h',
    'lcd_mem_bgr565.c']
for f in LCD_FILES:
    sfilename=os.path.join('src/lcd/', f);
    dfilename=os.path.join('awtk/src/lcd/', f);
    copyAwtkFile(sfilename, dfilename)

INPUT_METHODS_FILES=['input_engines/input_engine_pinyin.cpp',
    'input_methods/input_method_creator.c',
    'input_methods/input_method_default.inc',
    'input_methods/input_method_null.inc',
    'input_methods/suggest_words.inc']
for f in INPUT_METHODS_FILES:
    sfilename=os.path.join('src/', f);
    dfilename=os.path.join('awtk/src/', f);
    copyAwtkFile(sfilename, dfilename)

MAIN_LOOP_FILES=['main_loop_simple.h',
    'main_loop_simple.c',
    'main_loop_raw.inc']
for f in MAIN_LOOP_FILES:
    sfilename=os.path.join('src/main_loop', f);
    dfilename=os.path.join('awtk/src/main_loop', f);
    copyAwtkFile(sfilename, dfilename)


copyAwtkFiles('demos/assets', 'awtk-demos/assets')

DEMO_FILES=['assets.h',
    'assets.c',
    'demo_main.c',
    'demo_ui_app.c',
    'common.inc',
    'vg_common.inc']
for f in DEMO_FILES:
    sfilename=os.path.join('demos', f);
    dfilename=os.path.join('awtk-demos/', f);
    copyAwtkFile(sfilename, dfilename)


copyPortFiles('awtk-port', 'awtk-port')
# copyPortFiles('pxp', 'pxp')
# copyPortFile('user_code/main.c', 'awtk-demos/main.c')	
