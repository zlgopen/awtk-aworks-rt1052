import subprocess
import sys
import os
import shutil

GCC_PREFIX = r"arm-none-eabi-"
GCC_PATH = r"D:\eclipse_neon_2016q3_x86\GNU Tools ARM Embedded\2016q3\bin;"
ARMCC_PATH = r"C:\Keil_v5\ARM\ARMCC\bin;"
SCONS_RUN = r"scons.bat"

#for linux
#GCC_PREFIX = r"arm-none-eabi-"
#GCC_PATH = r"/opt/arm-none-eabi/bin:"
#ARMCC_PATH = r""
#SCONS_RUN = r"scons"


AWTK_GCC_LIB_NAME = 'libawtk_imx1050.a'
AWTK_ARMCC_LIB_NAME = 'awtk_imx1050.lib'
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

def main():
    if os.path.exists(OUTPUT):
        shutil.rmtree(OUTPUT)

    os.mkdir(OUTPUT)

    #arm-none-eabi-gcc
    if GCC_PATH.strip():
        run_scons_for_gcc()

    #ARMCC
    if ARMCC_PATH.strip():
        run_scons_for_armcc()

        
if __name__ == "__main__":
    main();