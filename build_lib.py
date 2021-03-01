import subprocess
import sys
import os
import shutil

GCC_PREFIX = r"arm-none-eabi-"
GCC_PATH = r"D:\eclipse_neon_2016q3_x86\GNU Tools ARM Embedded\2016q3\bin;"
ARMCC_PATH = r"C:\Keil_v5\ARM\ARMCC\bin;"
#ARMCLANG_PATH = r"C:\Keil_v5\ARM\ARMCLANG\bin;"
SCONS_RUN = r"scons.bat"
EXE_SUFFIX = '.exe'

#for linux
#GCC_PREFIX = r"arm-none-eabi-"
#GCC_PATH = r"/opt/arm-none-eabi/bin:"
#ARMCC_PATH = r""
#ARMCLANG_PATH = r""
#SCONS_RUN = r"scons"
#EXE_SUFFIX = ''


AWTK_GCC_LIB_NAME = 'libawtk_imx1050.a'
AWTK_ARMCC_LIB_NAME = 'awtk_imx1050.lib'
AWTK_ARMCLANG_LIB_NAME = 'awtk_imx1050_clang.lib'

OUTPUT = './output/lib'
AWTK_ROOT = './output/awtk'


def run_scons_for_gcc():
    os.environ['PATH'] =GCC_PATH + os.environ['PATH']
    scons_gcc_cmd = []
    scons_gcc_cmd.append(SCONS_RUN)
    scons_gcc_cmd.append('COMPILER_TOOLS=gcc')
    scons_gcc_cmd.append('GCC_PREFIX=' + GCC_PREFIX)
    scons_gcc_cmd.append('AWTK_LIB_NAME=' + AWTK_GCC_LIB_NAME)
    
    p_gcc = subprocess.Popen(scons_gcc_cmd)
    p_gcc.wait()

    if p_gcc.returncode == 0:
        shutil.move(AWTK_GCC_LIB_NAME,OUTPUT)
        scons_gcc_cmd.append('-c')
        p_clean = subprocess.Popen(scons_gcc_cmd)
        p_clean.wait()
    else:
        sys.exit(1)

def run_scons_for_armcc():
    os.environ['PATH'] = ARMCC_PATH + os.environ['PATH']
    scons_armcc_cmd = []
    scons_armcc_cmd.append(SCONS_RUN)
    scons_armcc_cmd.append('COMPILER_TOOLS=armcc')
    scons_armcc_cmd.append('AWTK_LIB_NAME=' + AWTK_ARMCC_LIB_NAME)
    
    p_armcc = subprocess.Popen(scons_armcc_cmd)
    p_armcc.wait()

    if p_armcc.returncode == 0:
        shutil.move(AWTK_ARMCC_LIB_NAME,OUTPUT)
        scons_armcc_cmd.append('-c')
        p_clean = subprocess.Popen(scons_armcc_cmd)
        p_clean.wait()
    else:
        sys.exit(1)

def run_scons_for_armclang():
    os.environ['PATH'] = ARMCLANG_PATH + os.environ['PATH']
    scons_armclang_cmd = []
    scons_armclang_cmd.append(SCONS_RUN)
    scons_armclang_cmd.append('COMPILER_TOOLS=armclang')
    scons_armclang_cmd.append('AWTK_LIB_NAME=' + AWTK_ARMCLANG_LIB_NAME)
    
    p_armclang = subprocess.Popen(scons_armclang_cmd)
    p_armclang.wait()

    if p_armclang.returncode == 0:
        shutil.move(AWTK_ARMCLANG_LIB_NAME,OUTPUT)
        scons_armclang_cmd.append('-c')
        p_clean = subprocess.Popen(scons_armclang_cmd)
        p_clean.wait()
    else:
        sys.exit(1)

def compiler_path_is_valid(path, compiler_name):
    is_valid = True
    if path and not os.path.exists(os.path.join(path, compiler_name + EXE_SUFFIX)):
        print('\033[31m '+ 'error: ' + compiler_name + ' is not exist!' + ' \033[0m')
        is_valid = False
    return is_valid

def main():
    GCC_PATH_TRIM = GCC_PATH.strip().strip(';')
    ARMCC_PATH_TRIM = ARMCC_PATH.strip().strip(';')
    ARMCLANG_PATH_TRIM = ARMCLANG_PATH.strip().strip(';')
    
    COMPILER_IS_VALID = compiler_path_is_valid(GCC_PATH_TRIM, GCC_PREFIX + 'gcc')
    COMPILER_IS_VALID = compiler_path_is_valid(ARMCC_PATH_TRIM, 'armcc') and COMPILER_IS_VALID
    COMPILER_IS_VALID = compiler_path_is_valid(ARMCLANG_PATH_TRIM, 'armclang') and COMPILER_IS_VALID
    
    if not COMPILER_IS_VALID:
        exit(0);

    if os.path.exists(OUTPUT):
        shutil.rmtree(OUTPUT)

    os.mkdir(OUTPUT)

    #arm-none-eabi-gcc
    if GCC_PATH_TRIM:
        run_scons_for_gcc()

    #ARMCC
    if ARMCC_PATH_TRIM:
        run_scons_for_armcc()

    #ARMCLANG
    if ARMCLANG_PATH_TRIM:
        run_scons_for_armclang()
        
if __name__ == "__main__":
    main();