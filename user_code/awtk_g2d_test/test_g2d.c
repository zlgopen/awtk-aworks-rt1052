
/* 接口函数测试 */

#include "aw_vdebug.h"
#include "aw_emwin_fb.h"
#include "base/g2d.h"
#include <Unity/unity.h>
#include "aw_bsp_mem.h"
#include "string.h"

/* 定义全局的 frame buffer 设备 */
static aw_emwin_fb_info_t  *__gp_awtk_fb;

static uint8_t  *__gp_ps_buf;
static uint8_t  *__gp_as_buf;


/* 定义屏幕的高度和宽度 */
#define __APP_IMG_HEIGHT  272
#define __APP_IMG_WIDTH   480
#define __APP_BPP         2

#define __APP_PS_WIDTH    (__APP_IMG_WIDTH )
#define __APP_PS_HEIGHT   (__APP_IMG_HEIGHT)

#define __APP_AS_WIDTH    200
#define __APP_AS_HEIGHT   200

#define __APP_BPP         2

static void __fb_init(void)
{
    uint8_t *p_vaddr = NULL;
    __gp_awtk_fb = aw_emwin_fb_open("imx1050_emwin_fb", 0);
    TEST_ASSERT_NOT_EQUAL(NULL, __gp_awtk_fb);

    p_vaddr = (uint8_t *)aw_no_cache_mem_alloc(__gp_awtk_fb->x_res * __gp_awtk_fb->y_res * __APP_BPP * 2);
    TEST_ASSERT_NOT_EQUAL(NULL, p_vaddr);

    /* frame buffer 初始化 */
    aw_emwin_fb_init(__gp_awtk_fb, (uint32_t *)p_vaddr, 0);

    /* frame buffer 运行 */
    aw_emwin_fb_run(__gp_awtk_fb);

    /* 设置背光亮度 */
    aw_emwin_fb_backlight(__gp_awtk_fb, 99);

    /* 初始化屏幕背景色(白色) */
    memset(p_vaddr, 0xFFFF, __gp_awtk_fb->x_res *  __gp_awtk_fb->y_res * 2);
    aw_emwin_fb_vram_addr_set(__gp_awtk_fb, (uint32_t)p_vaddr);
}


#define __rgb_to_565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
static uint16_t __rgb888_to_rgb565(uint32_t rgb888)
{
    return __rgb_to_565( 0xff & (rgb888 >> 16), 0xff & (rgb888 >> 8),  0xff & rgb888);
}

static void __as_ps_buf_init()
{
    uint32_t i, j;
    uint16_t *ps_buf = (uint16_t *)__gp_ps_buf;
    uint16_t *as_buf = (uint16_t *)__gp_as_buf;

    /* The PS buffer is BLUE rectangle  */
    for (i = 0; i < __APP_PS_HEIGHT; i++) {
        for (j = 0; j < __APP_PS_WIDTH; j++) {
            ps_buf[i * __APP_PS_WIDTH +  j] = 0xF0 ;
        }
    }

    /*
     * The AS input buffer is:
     *
     *  -----------------------------
     *  |             |             |
     *  |             |             |
     *  |   BLUE      |   GREEN     |
     *  |             |             |
     *  |             |             |
     *  |-------------+-------------|
     *  |             |             |
     *  |             |             |
     *  |   WHITE     |    RED      |
     *  |             |             |
     *  |             |             |
     *  -----------------------------
     */
    for (i = 0; i < (__APP_AS_HEIGHT / 2); i++) {

        for (j = 0; j < (__APP_AS_WIDTH / 2); j++) {
            as_buf[i * __APP_AS_WIDTH  +  j] = __rgb888_to_rgb565(0xFFU);
        }

        for (; j < __APP_AS_WIDTH; j++) {
            as_buf[i * __APP_AS_WIDTH  +  j] = __rgb888_to_rgb565(0xFF00U);
        }
    }

    for (; i < __APP_AS_HEIGHT; i++) {

        for (j = 0; j < (__APP_AS_WIDTH / 2); j++) {
            as_buf[i * __APP_AS_WIDTH  +  j] = __rgb888_to_rgb565(0xFFFFFFU);
        }

        for (; j < __APP_AS_WIDTH; j++) {
            as_buf[i * __APP_AS_WIDTH  +  j] = __rgb888_to_rgb565(0xFF0000U);
        }
    }
}


void demo_g2d(void)
{
    int j = 0;
    bitmap_t  bmp_fb;
    bitmap_t  bmp_img;
    rect_t dst;
    rect_t src;

    __fb_init();

    __gp_ps_buf = (uint8_t *) aw_no_cache_mem_alloc(__APP_PS_WIDTH * __APP_PS_HEIGHT * __APP_BPP);
    __gp_as_buf = (uint8_t *) aw_no_cache_mem_alloc(__APP_AS_WIDTH * __APP_AS_HEIGHT * __APP_BPP);
    TEST_ASSERT_NOT_EQUAL(NULL, __gp_ps_buf);
    TEST_ASSERT_NOT_EQUAL(NULL, __gp_as_buf);

    memset(&bmp_fb, 0x00, sizeof(bitmap_t));
    memset(&bmp_img, 0x00, sizeof(bitmap_t));

    while (1) {

        for (j = 10; j <= __APP_PS_HEIGHT; j +=10) {

            __as_ps_buf_init();

            bmp_fb.w = __APP_PS_WIDTH;
            bmp_fb.h = __APP_PS_HEIGHT;
            bmp_fb.flags  = 0;
            bmp_fb.format = BITMAP_FMT_BGR565;
            bmp_fb.data = __gp_ps_buf;

            dst.x = 0;
            dst.y = 0;
            dst.w = j ;
            dst.h = j;

            bmp_img.w = __APP_AS_WIDTH;
            bmp_img.h = __APP_AS_HEIGHT;
            bmp_img.flags  = 0;
            bmp_img.format = BITMAP_FMT_BGR565;
            bmp_img.data = __gp_as_buf;

            src.x = __APP_AS_WIDTH  / 4;
            src.y = __APP_AS_HEIGHT / 4;
            src.w = __APP_AS_WIDTH  / 2;
            src.h = __APP_AS_HEIGHT / 2;

            /* 使用软件 */
            //soft_blend_image(&bmp_fb, &bmp_img, &dst, &src, j);

            /* 使用PXP硬件 */
            //g2d_blend_image(&bmp_fb, &bmp_img, &src, &dst, 200);
            g2d_blend_image(&bmp_fb, &bmp_img, &dst, &src, j);

            memcpy(__gp_awtk_fb->v_addr, __gp_ps_buf, __APP_PS_WIDTH * __APP_PS_HEIGHT * __APP_BPP);
            aw_emwin_fb_vram_addr_set(__gp_awtk_fb, (uint32_t)__gp_awtk_fb->v_addr);
        }
    }

}

void test_g2d(void)
{
    aw_kprintf("\n");
    UnityBegin(__FILE__);
    RUN_TEST(demo_g2d);
    UnityEnd();
}

/* end of file */
