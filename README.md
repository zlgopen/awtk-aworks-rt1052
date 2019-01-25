# AWTK针对AWorks平台的移植

[AWTK](https://github.com/zlgopen/awtk)是为嵌入式系统开发的GUI引擎库。

[awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)是AWTK在AWorks(RT1052)上的移植。

## 一、环境配置

#### 1.1 克隆(clone)  AWTK

```
cd E:\zlgopen
git clone https://github.com/zlgopen/awtk.git
```

#### 1.2 修改 copy\_files.py

  主要有如下3项：

- 设置AWTK_ROOT_DIR（即AWTK根目录）；

  ```
  AWTK_ROOT_DIR = 'E:/zlgopen/awtk';
  ```


- 设置DST_ROOT_DIR（即输出目录）。一般该目录为AWorks工程目录（即.project文件的目录）

  ```
  DST_ROOT_DIR = 'E:/zlgopen/awtk-aworks-rt1052/output';
  ```

  注意路径中不能包含中文字符。

- 设置NANOVG_BACKEND（即nanovg后端类型），目前有AGG、AGGE两种选项。

  ```
  NANOVG_BACKEND = 'AGGE'
  ```

#### 1.3 执行 copy\_files.py

```
python copy_files.py
```

  注意脚本目前只适配了Python2。

#### 1.4 添加预定义宏HAS\_AWTK\_CONFIG

1. 进入eclipse, 导入并选中AWorks工程，在如下两个地方添加宏：

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C Compiler" -> "Preprocessor"；

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C++ Compiler" -> "Preprocessor"


#### 1.5 添加AWTK相关的头文件路径

1. 进入eclipse, 导入并选中AWorks工程，在如下两个地方添加路径：

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C Compiler" -> "Includes"；

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C++ Compiler" -> "Includes"

   AWTK相关的包含路径如下：

   ```
   "${workspace_loc:/${ProjName}/awtk/src}"
   "${workspace_loc:/${ProjName}/awtk/src/ext_widgets}"
   "${workspace_loc:/${ProjName}/awtk/3rd}"
   "${workspace_loc:/${ProjName}/awtk/3rd/agge}"
   "${workspace_loc:/${ProjName}/awtk/3rd/agg/include}"
   "${workspace_loc:/${ProjName}/awtk/3rd/nanovg}"
   "${workspace_loc:/${ProjName}/awtk/3rd/nanovg/base}"
   "${workspace_loc:/${ProjName}/awtk/3rd/gpinyin/include}"
   "${workspace_loc:/${ProjName}/awtk/3rd/libunibreak}"
   "${workspace_loc:/${ProjName}/awtk-port}"
   ```


## 二、注意事项

1、awtk-port/platfrom.c中的 TK_MEM_SIZE 须小于 AWorks 工程的 *.ld 文件中定义的 heap 大小。比如 rt1050_sdram.ld 中定义了 heap 为11M，则 TK_MEM_SIZE 须小于11M。

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

2、AWorks工程的*.ld文件切忌随意修改，这可能会导致内存读写异常（比如读写速度严重变慢）。

3、关于Pxp硬件加速

| 函数             | 是否支持 | 备注                                 |
| ---------------- | -------- | ------------------------------------ |
| g2d_fill_rect    | 不支持   | 比直接memcpy慢                       |
| g2d_copy_image   | 不支持   | 比直接memcpy慢                       |
| g2d_blend_image  | 部分支持 | 不支持带透明通道的位图进行“缩放”贴图 |
| g2d_rotate_image | 支持     |                                      |

4、关于awtk_config.h的常用配置

| 宏               | 说明                                                         |
| ---------------- | ------------------------------------------------------------ |
| WITH_PXP_G2D     | 开启2D加速，可以加快贴图速度                                 |
| WITH_THREE_FB    | 开启三缓冲机制，提高刷新帧率，但如果使用 tk_set_lcd_orientation 旋转屏幕，则应该注释该行，使用默认的双缓冲机制 |
| WITH_NANOVG_AGGE | 使用 AGGE 作为渲染后端，要使用 AGG 则可以改为 WITH_NANOVG_AGG |

