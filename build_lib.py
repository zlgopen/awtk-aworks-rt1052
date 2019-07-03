import subprocess
import sys
import os
import shutil

GCC_PATH = r"D:\eclipse_neon_2016q3_x86\GNU Tools ARM Embedded\2016q3\bin;"
ARMCC_PATH = r"C:\Keil_v5\ARM\ARMCC\bin;"
AWTK_ROOT = './output/awtk'



AWTK_GCC_LIB_NAME = 'libawtk_imx1050.a'
AWTK_ARMCC_LIB_NAME = 'awtk_imx1050.lib'
OUTPUT = './output/lib'


def main():
    if os.path.exists(OUTPUT):
        shutil.rmtree(OUTPUT)

    os.mkdir(OUTPUT)    

    #arm-none-eabi-gcc
    os.environ['Path'] =GCC_PATH + os.environ['Path']
    scons_gcc_cmd = []
    scons_gcc_cmd.append('scons.bat')
    scons_gcc_cmd.append('COMPILER_TOOLS=gcc')
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

    #ARMCC
    os.environ['Path'] = ARMCC_PATH + os.environ['Path']
    scons_armcc_cmd = []
    scons_armcc_cmd.append('scons.bat')
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



if __name__ == "__main__":
    main();