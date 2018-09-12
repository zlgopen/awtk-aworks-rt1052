
/**
 * File:   awtk_config.h
 * Author: AWTK Develop Team
 * Brief:  config
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-09-12 Li XianJing <xianjimli@hotmail.com> created
 *arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=fpv5-d16 -mno-unaligned-access -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DAW_DEBUG -DAW_VDEBUG -DAW_VDEBUG_INFO -DAW_VDEBUG_WARN -DAW_VDEBUG_ERROR -DAW_CORTEX_M7 -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/interface/include" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug/../examples/peripheral" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/rtk/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/rtk/config" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/psp/rtk/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/psp/rtk/config" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/components/awbus_lite/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/components/awbus_lite/config" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/components/base/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/components/base/config" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/bsp" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/cpu" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/GUI" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/lora" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/fs" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/include/config" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/3rdparty/net/lwip_1.4.x/src/include" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/apollo/3rdparty/net/lwip_1.4.x/src/include/ipv4" -I"D:/work/AWorks/sdk/aworks_m105x_sdk_1.0.1-alpha/img_rt1050_debug/../aworks_sdk/ametal/common/interface" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\user_config" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\user_code" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\user_config\awbl_hwconf_usrcfg" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\user_config\emWin_Config" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk-port" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\src" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\src\ext_widgets" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\3rd" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\3rd\pixman\pixman" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\3rd\cairo\cairo" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\3rd\gpinyin\include" -I"D:\work\AWorks\sdk\aworks_m105x_sdk_1.0.1-alpha\img_rt1050_debug\awtk\3rd\libunibreak\src" -std=gnu11 -MMD -MP -MF"awtk/demos/demo_main.d" -MT"awtk/demos/demo_main.o" -c -o "awtk/demos/demo_main.o" "../awtk/demos/demo_main.c"

 */

#ifndef AWTK_CONFIG_H
#define AWTK_CONFIG_H

/**
 * 嵌入式系统有自己的main函数时，请定义本宏。
 *
 */
#define USE_GUI_MAIN 1

/**
 * 如果支持png/jpeg图片，请定义本宏
 *
 */
#define WITH_STB_IMAGE 1

/**
 * 如果支持Truetype字体，请定义本宏
 *
 */
#define WITH_STB_FONT 1

/**
 * 如果定义本宏，使用标准的UNICODE换行算法，除非资源极为有限，请定义本宏。
 *
 */
#define WITH_UNICODE_BREAK 1

/**
 * 如果定义本宏，将图片解码成BGRA8888格式，否则解码成RGBA8888的格式。
 *
 */
#define WITH_BITMAP_BGRA 1

/**
 * 如果定义本宏，将不透明的PNG图片解码成RGB565格式，建议定义。
 *
 */
#define WITH_BITMAP_RGB565 1

/**
 * 如果有优化版本的memcpy函数，请定义本宏
 *
 */
#define HAS_FAST_MEMCPY 1

/**
 * 如果启用NXP PXP硬件加速，请定义本宏
 * 
 */
#define WITH_PXP_G2D 1

/**
 * 如果FLASH空间较小，不足以放大字体文件时，请定义本宏
 *
 */
#define WITH_MINI_FONT 1

#endif/*AWTK_CONFIG_H*/
