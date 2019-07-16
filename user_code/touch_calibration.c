#include "aworks.h"
#include "aw_fb.h"
#include "aw_ts.h"
#include "aw_vdebug.h"
#include "aw_delay.h"
#include "string.h"
#include "aw_mem.h"
#include "aw_prj_params.h"

/* 颜色数组*/
uint16_t color_group[5] = {0xFF80, 0x051D,0xE8C4,0xC618,0x2589};


/******************************************************************************/
/* 画点函数*/
aw_err_t app_fbuf_pixel (int      x,
                         int      y,
                         uint8_t *screen_buf,
                         uint16_t color,
                         void*    p_fb)
{
    int                 idx;

    aw_fb_fix_info_t fix_info;
    aw_fb_ioctl(p_fb, AW_FB_CMD_GET_FINFO, &fix_info);

    if ((x > fix_info.x_res) || (y > fix_info.y_res)) {
        return -AW_EINVAL;
    }

    idx = (y * fix_info.x_res + x) * 2;
    memcpy(&screen_buf[idx], &color, sizeof(color));

    return AW_OK;
}

/* 画线函数*/
aw_err_t app_fbuf_line (int      x,
                        int      y,
                        uint8_t *screen_buf,
                        uint16_t color,
                        void*    p_fb)
{
    int  i;

    aw_fb_fix_info_t fix_info;
    aw_fb_ioctl(p_fb, AW_FB_CMD_GET_FINFO, &fix_info);

    if ((x - 10 < 0) || (x + 10 >= fix_info.x_res) ||
        (y - 10 < 0) || (y + 10 > fix_info.y_res)) {
        return -AW_EINVAL;
    }

    for (i = 0; i < 20; i++) {

    	app_fbuf_pixel( x - 10 + i, y,screen_buf, color, p_fb);
    }

    for (i = 0; i < 20; i++) {
        app_fbuf_pixel( x, y - 10 + i, screen_buf,color, p_fb);
    }

    return AW_OK;
}

/********************************************************************************/
/* 校准函数*/
int ts_calibrate (uint8_t                 *screen_buf,
                  aw_ts_id                 id,
                  aw_ts_lib_calibration_t *p_cal,
                  void*                    p_fb)
{
    int                 i;
    struct aw_ts_state  sta;

    aw_fb_fix_info_t fix_info;
    aw_fb_ioctl(p_fb, AW_FB_CMD_GET_FINFO, &fix_info);

    /* 初始化五点触摸数据 */
    p_cal->log[0].x = 60 - 1;
    p_cal->log[0].y = 60 - 1;
    p_cal->log[1].x = fix_info.x_res  - 60 - 1;
    p_cal->log[1].y = 60 - 1;
    p_cal->log[2].x = fix_info.x_res - 60 - 1;
    p_cal->log[2].y = fix_info.y_res - 60 - 1;
    p_cal->log[3].x = 60 - 1;
    p_cal->log[3].y = fix_info.y_res - 60 - 1;
    p_cal->log[4].x = fix_info.x_res / 2 - 1;
    p_cal->log[4].y = fix_info.y_res / 2 - 1;

    p_cal->cal_res_x = fix_info.x_res;
    p_cal->cal_res_y = fix_info.y_res;

    /* 五点触摸 */
    for (i = 0; i < 5; i++) {
        /* 画出点图案*/
        app_fbuf_line (p_cal->log[i].x, p_cal->log[i].y, screen_buf, 0xFFFF, p_fb);

        while (1) {
            /* 等待获取光标所在位置触摸数据 */
            if ((aw_ts_get_phys(id, &sta, 1) > 0) &&
                (sta.pressed == AW_TRUE)) {
                p_cal->phy[i].x = sta.x;
                p_cal->phy[i].y = sta.y;

                aw_kprintf("\n x=%d, y=%d \r\n", (uint32_t)sta.x, (uint32_t)sta.y);
                while(1) {
                    aw_mdelay(500);
                    if ((aw_ts_get_phys(id, &sta, 1) == AW_OK) &&
                        (sta.pressed == AW_FALSE)) {
                        break;
                    }
                }
                break;
            }
            aw_mdelay(10);
        }
        /* 清除上个点图案*/
        app_fbuf_line (p_cal->log[i].x, p_cal->log[i].y, screen_buf, 0, p_fb);
    }
    return 0;
}


void ts_init (void* p_fb)
{
    aw_ts_id                sys_ts;
    aw_ts_lib_calibration_t cal;
    uint8_t                *p_buf = NULL;

    aw_fb_fix_info_t fix_info;
    aw_fb_ioctl(p_fb, AW_FB_CMD_GET_FINFO, &fix_info);

    /* 获取触摸设备 */
    sys_ts = aw_ts_serv_id_get(SYS_TS_ID, 0, 0);
    if (sys_ts == NULL) {
        return;
    }

    /* 填充摄像头帧结构体*/
    p_buf = (uint8_t *)fix_info.vram_addr;   /*emwin帧缓存地址*/
    /* 设置背景为黑色*/
    memset(p_buf, 0, fix_info.x_res * fix_info.y_res *2);

    /* 判断是否设备支持触摸校准 */
    if (aw_ts_calc_flag_get(sys_ts)) {
        /* 加载系统触摸数据 */
        if (aw_ts_calc_data_read(sys_ts) != AW_OK) {
            /* 没有有效数据，进行校准 */
            do {
                if (ts_calibrate(p_buf, sys_ts, &cal, p_fb) < 0) {
                    //todo
                }
            } while (aw_ts_calibrate(sys_ts, &cal) != AW_OK);
            /* 校准成功，保存触摸数据 */
            aw_ts_calc_data_write(sys_ts);
        }
    } else {
      if (strcmp(SYS_TS_ID, "ft5x06") == 0) {
        /* 评估板配套4.3电容屏需要进行XY转换 */
        aw_ts_set_orientation(sys_ts, AW_TS_SWAP_XY);
      }
    }
}

