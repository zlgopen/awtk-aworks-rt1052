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
VGCANVAS = 'NANOVG'
NANOVG_BACKEND = 'AGGE'
```

#### 1.3 生成源文件或 lib

##### 1.3.1 执行 copy\_files.py

输出编译AWorks工程需要用的源文件，默认输出到 output 文件夹

```
cd E:\zlgopen\awtk-aworks-rt1052
python copy_files.py
```

完成后将 output 文件夹内的所有文件拷贝到 Eclipse 工程目录

##### 1.3.1 执行 build_lib.py（可选）

如果工程中需要以源码的方式引用 AWTK，执行完 1.3.1 步骤即可，但这种方式在每次修改工程时，会浪费大量时间在编译 AWTK 源码上面。因此可以使用预编译好 lib 的方式：

```
cd E:\zlgopen\awtk-aworks-rt1052
python copy_files.py
python build_lib.py
```

完成后，会在 output/lib 文件夹下生成相应的库文件。如果执行失败，可能需要修改 build_lib.py 编译器路径。

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

## 二、其他设置问题

#### 2.1 内存配置

Eclipse 工程的 rt1050_sdram.ld 有以下两种配置，根据工程需要选择

##### code = 4M的配置

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

##### code = 8M的配置

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

#### 2.2 渲染模式选择

##### 使用 AGGE 模式（默认）

- 修改 copy\_files.py

  ```
  VGCANVAS = 'NANOVG'
  NANOVG_BACKEND = 'AGGE'
  ```

- 修改 awtk_config.h

  ```
  #define WITH_NANOVG_AGGE 1
  ```

- 添加工程 include 路径

  ```
  "${workspace_loc:/${ProjName}/awtk/3rd/agge}"
  ```

##### 使用 AGG 模式

- 修改 copy\_files.py

  ```
  VGCANVAS = 'NANOVG'
  NANOVG_BACKEND = 'AGG'
  ```

- 修改 awtk_config.h

  ```
  #define WITH_NANOVG_AGG 1
  ```

- 添加工程 include 路径

  ```
  "${workspace_loc:/${ProjName}/awtk/3rd/agg}"
  ```

##### 使用 CAIRO 模式

- 修改 copy\_files.py

  ```
  VGCANVAS = 'CAIRO'
  ```

- 添加工程 include 路径

  ```
  "${workspace_loc:/${ProjName}/awtk/3rd/cairo}"
  "${workspace_loc:/${ProjName}/awtk/3rd/pixman}"
  ```

## 三、Keil 环境设置问题

AWorks 默认提供的环境是 Eclipse，如果需要在 Keil 环境下使用 AWorks 和 AWTK，则有些额外的设置工作：

1. 请在 C/C++ -> Misc Controls 中加上 **--gnu** 标志 

2. 禁用 **Use MicroLib**

   