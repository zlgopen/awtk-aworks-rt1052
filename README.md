# AWTK针对AWorks平台的移植

[AWTK](https://github.com/zlgopen/awtk)是为嵌入式系统开发的GUI引擎库。

[awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)是AWTK在AWorks(RT1052)上的移植。

## 一、环境配置

#### 1.1 克隆(clone)  AWTK

```
cd E:\zlgopen
git clone https://github.com/zlgopen/awtk.git
git clone https://github.com/zlgopen/awtk-aworks-rt1052.git
```

#### 1.2 修改 copy\_files.py

主要有如下3项，通常无需修改，按默认设置即可：

```
AWTK_ROOT_DIR = '../awtk';
DST_ROOT_DIR = './output';
NANOVG_BACKEND = 'AGGE'
```

#### 1.3 执行 copy\_files.py

输出编译AWorks工程需要用的源文件，默认输出到 output 文件夹

```
cd E:\zlgopen\awtk-aworks-rt1052
python copy_files.py
```

完成后将 output 文件夹内的所有文件拷贝到 Eclipse 工程目录

#### 1.4 添加预定义宏

进入 Eclipse，导入并选中 AWorks 工程，在如下两个地方添加宏：

```
HAS_AWTK_CONFIG
```

"Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> 

- "Cross ARM C Compiler" -> "Preprocessor"

- "Cross ARM C++ Compiler" -> "Preprocessor"


#### 1.5 添加 AWTK 相关的头文件路径

进入eclipse, 导入并选中AWorks工程，在如下两个地方添加路径：

```
"${workspace_loc:/${ProjName}/awtk/src}"
"${workspace_loc:/${ProjName}/awtk/src/ext_widgets}"
"${workspace_loc:/${ProjName}/awtk/3rd}"
"${workspace_loc:/${ProjName}/awtk/3rd/agge}"
"${workspace_loc:/${ProjName}/awtk/3rd/nanovg}"
"${workspace_loc:/${ProjName}/awtk/3rd/nanovg/base}"
"${workspace_loc:/${ProjName}/awtk/3rd/gpinyin/include}"
"${workspace_loc:/${ProjName}/awtk/3rd/libunibreak}"
"${workspace_loc:/${ProjName}/awtk-port}"
```

"Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> 

- "Cross ARM C Compiler" -> "Includes"

- "Cross ARM C++ Compiler" -> "Includes"

## 二、注意事项

1、Eclipse 工程的 rt1050_sdram.ld 有以下两种配置，根据工程需要选择

**code = 4M的配置**

```
MEMORY
{
    data (xrw)             : ORIGIN = 0x20000000, LENGTH = 512K
    flexspi_drv(rx)        : ORIGIN = 0x80000000, LENGTH = 4K
    code (rx)              : ORIGIN = 0x80002000, LENGTH = 4088K
    dma_heap (rw)          : ORIGIN = 0x80400000, LENGTH = 1M
    heap (rw)              : ORIGIN = 0x80500000, LENGTH = 11M
}
```

**code = 8M的配置**

```
MEMORY
{
    data (xrw)             : ORIGIN = 0x20000000, LENGTH = 512K
    flexspi_drv(rx)        : ORIGIN = 0x80000000, LENGTH = 4K
    code (rx)              : ORIGIN = 0x80002000, LENGTH = 8184K
    dma_heap (rw)          : ORIGIN = 0x80800000, LENGTH = 1M
    heap (rw)              : ORIGIN = 0x80900000, LENGTH = 7M
}
```

2、关于awtk_config.h的常用配置

| 宏               | 说明                                                         |
| ---------------- | ------------------------------------------------------------ |
| WITH_PXP_G2D     | 开启2D加速，可以加快贴图速度                                 |
| WITH_THREE_FB    | 开启三缓冲机制，提高刷新帧率，但如果使用 tk_set_lcd_orientation 旋转屏幕，则应该注释该行，使用默认的双缓冲机制 |
| WITH_NANOVG_AGGE | 使用 AGGE 作为渲染后端，要使用 AGG 则可以改为 WITH_NANOVG_AGG |


## 三、Keil 环境设置问题

AWorks 默认提供的环境是 Eclipse，如果需要在 Keil 环境下使用 AWorks 和 AWTK，则有些额外的设置工作：

1. 请在 C/C++ -> Misc Controls 中加上 **--gnu** 标志 

2. 禁用 **Use MicroLib**

   