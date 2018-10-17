# AWTK针对AWorks平台的移植。

[AWTK](https://github.com/zlgopen/awtk)是为嵌入式系统开发的GUI引擎库。

[awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)是AWTK在AWorks(RT1052)上的移植。

## 一、环境配置

#### 1.1 克隆(clone)  awtk

```
git clone https://github.com/zlgopen/awtk.git
```

#### 1.2 修改 copy\_files.py

  主要有如下两项：

- 设置AWTK_ROOT_DIR（即awtk所在目录）；


- 设置DST_ROOT_DIR（即输出目录）。一般该目录为AWorks工程目录, 不支持中文。

#### 1.3 执行 copy\_files.py

```
python copy_files.py
```

#### 1.4 添加预定义宏HAS\_AWTK\_CONFIG

  有两种方式：

1. 进入eclipse, 导入并选中AWorks工程，在如下两个地方添加宏：

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C Compiler" -> "Preprocessor"；

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C++ Compiler" -> "Preprocessor"

2. 直接修改AWorks工程的.cproject文件

   找到所有 name="Cross ARM C Compiler"或者name="Cross ARM C++ Compiler"的tool元素，在name="Defined symbols (-D)"的option子元素中加入：

   ```
   <listOptionValue builtIn="false" value="HAS_AWTK_CONFIG"/>
   ```

#### 1.5 添加awtk相关的头文件路径

  有两种方式：

1. 进入eclipse, 导入并选中AWorks工程，在如下两个地方添加路径：

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C Compiler" -> "Includes"；

   - "Project"菜单项 -> "C/C++ Build" -> "Settings" -> "Tool Settings"标签页 -> "Cross ARM C++ Compiler" -> "Includes"

   awtk相关的包含路径如下：

   ```
   "${workspace_loc:/${ProjName}/awtk/}"
   "${workspace_loc:/${ProjName}/awtk/src}"
   "${workspace_loc:/${ProjName}/awtk/src/ext_widgets}"
   "${workspace_loc:/${ProjName}/awtk/3rd}"
   "${workspace_loc:/${ProjName}/awtk/3rd/agge}"
   "${workspace_loc:/${ProjName}/awtk/3rd/nanovg}"
   "${workspace_loc:/${ProjName}/awtk/3rd/nanovg/base}"
   "${workspace_loc:/${ProjName}/awtk/3rd/gpinyin/include}"
   "${workspace_loc:/${ProjName}/awtk/3rd/libunibreak}"
   "${workspace_loc:/${ProjName}/awtk-port}"
   "${workspace_loc:/${ProjName}/pxp}"
   ```

2. 直接修改AWorks工程的.cproject文件，找到所有 name="Cross ARM C Compiler"或者name="Cross ARM C++ Compiler"的tool元素，在name="Include paths (-I)"的option子元素中加入：

```
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/src}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/src/ext_widgets}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/agge}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/nanovg}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/nanovg/base}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/gpinyin/include}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/libunibreak}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk-port}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/pxp}&quot;"/>
```

## 二、注意事项

  AWorks工程的*.ld文件切忌随意修改，这可能会导致内存读写异常（比如读写速度严重变慢）。