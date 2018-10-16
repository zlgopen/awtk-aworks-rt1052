# AWTK针对AWorks平台的移植。

[AWTK](https://github.com/zlgopen/awtk)是为嵌入式系统开发的GUI引擎库。

[awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)是AWTK在AWorks(RT1052)上的移植。

## 编译

1. 将awtk取到当前目录

```
git clone https://github.com/zlgopen/awtk.git
```

2. 拷贝源文件到Aworks SDK中(请修改copy\_files.py指明Aworks SDK路径, SDK路不支持中文)。

```
python copy_files.py
```

3. 在eclipse中定义宏: HAS\_AWTK\_CONFIG

4. 在eclipse中增加下列路径：

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
"${workspace_loc:/${ProjName}/awtk-demos}"
"${workspace_loc:/${ProjName}/pxp}"
"${workspace_loc:/${ProjName}/app}"
```

或者直接修改.cproject文件，在相应位置加入：

```
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk-port}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk-demos}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/src}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/src/ext_widgets}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/nanovg}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/nanovg/base}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/agge}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/gpinyin/include}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/awtk/3rd/libunibreak}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/pxp}&quot;"/>
<listOptionValue builtIn="false" value="&quot;${workspace_loc:/${ProjName}/app}&quot;"/>
```
