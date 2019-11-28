# build_lib.py 编译生成 awtk 静态库说明

执行 build_lib.py 可以执行自动编译脚本生成 awtk 的静态库，最终会在 output/lib 文件夹中生成 libawtk_imx1050.a、awtk_imx1050.lib 等静态库文件。

执行 build_lib.py 之前，需要配置目标平台的编译器路径、编译参数、以及选择运行平台等。配置 build_lib 通常需要修改以下两个文件：

- build_lib.py
- SConstruct

其中 build_lib.py 可以运行在 Windows 和 Linux 两种环境下，比如有些芯片编译器只有 Linux 版本，则要按照 Linux 运行的方式修改 build_lib.py，具体的操作方法如下：

## 一、准备工作：安装好编译环境

现在当前系统环境中，安装好编译工具链，比如：

- arm-none-eabi（GCC）
- ARMCC（Keil）

## 二、在 Windows 环境下运行 build_lib.py

#### 1.1 配置 build_lib.py 工具链路径和编译器前缀

修改以下几行代码，配置正确的编译器路径和前缀

```
GCC_PREFIX = r"arm-none-eabi-"
GCC_PATH = r"D:\eclipse_neon_2016q3_x86\GNU Tools ARM Embedded\2016q3\bin;"
ARMCC_PATH = r"C:\Keil_v5\ARM\ARMCC\bin;"
```

#### 1.2 配置 build_lib.py 生成 lib 的文件名

```
AWTK_GCC_LIB_NAME = 'libawtk_imx1050.a'
AWTK_ARMCC_LIB_NAME = 'awtk_imx1050.lib'
```

#### 1.3 配置 SConstruct 编译参数

按照目标平台的 cpu，修改 SConstruct 文件中的对应编译参数，以下是 i.MX RT1052 的 gcc 编译参数

```
CPU_FLAGS =  ' '
CPU_FLAGS += ' -mcpu='+'cortex-m7'
CPU_FLAGS += ' -m' + 'thumb'
CPU_FLAGS += ' -mthumb-interwork '
CPU_FLAGS += ' -mfloat-abi=' + 'hard'
CPU_FLAGS += ' -mfpu=' + 'fpv5-d16'
CPU_FLAGS += ' -mno-unaligned-access'
#CPU_FLAGS += ' -fno-short-enums'
```

如果还需要编译 Keil 的库，则修改对应的 ARMCC 的编译参数

```
env['CFLAGS'] = '--c99'
env['CCFLAGS'] = '--cpu=Cortex-M7.fp.dp --apcs=interwork --gnu -O3 -Ospace'
env['CXXFLAGS'] = '--cpp'
```

## 三、在 Linux 环境下运行 build_lib.py

#### 2.1 修改 build_lib.py 的运行环境为 Linux

将 build_lib.py 以下 2~5 行代码的注释去掉，其他修改见[在 Windows 环境下运行 build_lib.py](#二、在 Windows 环境下运行 build_lib.py)，这里不再赘述

```
#for linux
GCC_PREFIX = r"arm-none-eabi-"
GCC_PATH = r"/opt/arm-none-eabi/bin:"
ARMCC_PATH = r""
SCONS_RUN = r"scons"
```

##  四、执行 build_lib.py 生成 lib

在命令行中，执行 build_lib.py 前要先执行 copy_files.py，拷贝要编译的源文件，最后生成后的静态库在 output/lib 文件夹下

```
python copy_files.py
python build_lib.py
```

