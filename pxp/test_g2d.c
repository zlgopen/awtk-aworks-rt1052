
/* 接口函数测试 */

#include "aw_vdebug.h"
#include "aw_emwin_fb.h"
#include "base/g2d.h"
#include "aw_bsp_mem.h"
#include "string.h"
#include "aw_mem.h"

/* 定义全局的 frame buffer 设备 */
static aw_emwin_fb_info_t  *__gp_awtk_fb;

static uint8_t  *__gp_ps_buf;
static uint8_t  *__gp_as_buf;


/* 定义屏幕的高度和宽度 */
//#define __APP_IMG_HEIGHT  272
//#define __APP_IMG_WIDTH   480

#define __APP_IMG_HEIGHT  480
#define __APP_IMG_WIDTH   800

#define __APP_BPP         2

#define __APP_PS_WIDTH    (__APP_IMG_WIDTH )
#define __APP_PS_HEIGHT   (__APP_IMG_HEIGHT)

#define __APP_AS_WIDTH    __APP_PS_HEIGHT
#define __APP_AS_HEIGHT   __APP_PS_WIDTH

#define __APP_BPP         2


#define DWT_CYCCNT  *(volatile uint32_t *)0xE0001004


static void __fb_init(void)
{
    uint8_t *p_vaddr = NULL;
    __gp_awtk_fb = aw_emwin_fb_open("imx1050_emwin_fb", 0);
    if (NULL == __gp_awtk_fb) {
        while(1);
    }

    p_vaddr = (uint8_t *)aw_mem_align(__gp_awtk_fb->x_res * __gp_awtk_fb->y_res * __APP_BPP * 2, 64);
    if (NULL == p_vaddr) {
        while(1);
    }
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

void test_g2d_blend(void)
{
    int j = 0;
    bitmap_t  bmp_fb;
    bitmap_t  bmp_img;
    rect_t dst;
    rect_t src;

    uint32_t test_t1 = 0;
    uint32_t test_t2 = 0;
    float test_time = 0.0;

    __fb_init();

    __gp_ps_buf = (uint8_t *) aw_mem_align(__APP_PS_WIDTH * __APP_PS_HEIGHT * __APP_BPP, 64);
    __gp_as_buf = (uint8_t *) aw_mem_align(__APP_AS_WIDTH * __APP_AS_HEIGHT * __APP_BPP, 64);
    TEST_ASSERT_NOT_EQUAL(NULL, __gp_ps_buf);
    TEST_ASSERT_NOT_EQUAL(NULL, __gp_as_buf);

    memset(&bmp_fb, 0x00, sizeof(bitmap_t));
    memset(&bmp_img, 0x00, sizeof(bitmap_t));

    while (1) {

        for (j = 10; j <= __APP_PS_HEIGHT; j +=10)
        {

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

#if 0
            src.x = __APP_AS_WIDTH  / 4;
            src.y = __APP_AS_HEIGHT / 4;
            src.w = __APP_AS_WIDTH  / 2;
            src.h = __APP_AS_HEIGHT / 2;

#else

            src.x = 0 ;
            src.y = 0;
            src.w = __APP_AS_WIDTH ;
            src.h = __APP_AS_HEIGHT;

#endif
//            //不需要缩放：
//            src.w = dst.w;
//            src.h = dst.h;

#if 0
            t1 = DWT_CYCCNT;
            soft_blend_image(&bmp_fb, &bmp_img, &dst, &src, 200);
            t2 = DWT_CYCCNT - t1;
            time = t2 / 528000.0;
#endif

#if 1
            test_t1 = DWT_CYCCNT;
            g2d_blend_image(&bmp_fb, &bmp_img, &dst, &src, 200);
            test_t2 = DWT_CYCCNT - test_t1;
            test_time = test_t2 / 528000.0;

            aw_kprintf("blend time is: %f\r\n", test_time);
#endif

            test_t1 = DWT_CYCCNT;
            memcpy(__gp_awtk_fb->v_addr, __gp_ps_buf, __APP_PS_WIDTH * __APP_PS_HEIGHT * __APP_BPP);
            test_t2 = DWT_CYCCNT - test_t1;
            test_time = test_t2 / 528000.0;
        }
    }
}


/******************************************************************************/
extern ret_t soft_fill_rect(bitmap_t* fb, rect_t* dst, color_t c);

/* 测试硬件的矩形填充 */
void  test_g2d_fill_rect(void )
{
    bitmap_t  bmp_fb = {0};
    rect_t    dst = {0};
    color_t   color = {0};
    int i = 0;
    int j = 0;

    uint32_t fill_t1, fill_t2;
    float    fill_time;

    /* frame buffer初始化 */
    __fb_init();

    bmp_fb.w = __APP_PS_WIDTH;
    bmp_fb.h = __APP_PS_HEIGHT;
    bmp_fb.flags  = 0;
    bmp_fb.format = BITMAP_FMT_RGB565;
    bmp_fb.data = (uint8_t *)__gp_awtk_fb->v_addr;
    bmp_fb.line_length = bmp_fb.w * __APP_BPP;


    dst.x = 0 ;
    dst.y = 0;
    dst.w = __APP_PS_WIDTH ;
    dst.h = __APP_PS_HEIGHT;

    color.rgba.a = 255;
    color.rgba.r = 0xFF;
    color.rgba.g = 0xAA;
    color.rgba.b = 0x55;

    while(1) {

        for (i = 10; i < 480; i+= 10) {

            if (i >= 480 - 10) {
                memset((void *)bmp_fb.data, 0xFFFF, __gp_awtk_fb->x_res *  __gp_awtk_fb->y_res * 2);
            }
            dst.w = i;
            dst.h = i;

#if 1  //软件
            fill_t1 = DWT_CYCCNT;
            soft_fill_rect(&bmp_fb, &dst, color);
            fill_t2 = DWT_CYCCNT - fill_t1;
            fill_time = fill_t2 / 528000.0;
#endif

#if 0  //硬件
            fill_t1 = DWT_CYCCNT;
            g2d_fill_rect(&bmp_fb, &dst, color);
            fill_t2 = DWT_CYCCNT - fill_t1;
            fill_time = fill_t2 / 528000.0;
#endif
            j++;
        }
    }
}


/******************************************************************************/

#define TEST_SIZE  (__APP_IMG_HEIGHT * __APP_IMG_WIDTH * 2)
extern ret_t soft_copy_image(bitmap_t* dst, bitmap_t* src, rect_t* src_r, xy_t dx, xy_t dy);

/* 测试硬件的矩形填充 */
void  test_g2d_copy_image(void )
{
    bitmap_t  bmp_fb = {0};
    bitmap_t  img = {0};
    rect_t    src = {0};
    uint32_t  cp_t1, cp_t2;
    float     cp_time;

    /* frame buffer初始化 */
    __fb_init();

    uint8_t *test_buf = NULL;

    test_buf = (uint8_t *)aw_mem_align(TEST_SIZE, 128);
    if (test_buf == NULL) {
        while(1);
    }
    memset(test_buf,  0x12, TEST_SIZE);

    bmp_fb.w = __APP_PS_WIDTH;
    bmp_fb.h = __APP_PS_HEIGHT;
    bmp_fb.flags  = 0;
    bmp_fb.format = BITMAP_FMT_BGR565;
    bmp_fb.data = (uint8_t *)__gp_awtk_fb->v_addr;
    bmp_fb.line_length = bmp_fb.w * __APP_BPP;

    img.w = bmp_fb.w;
    img.h = bmp_fb.h;
    img.flags  = 0;
    img.format = BITMAP_FMT_BGR565;
    img.data = (uint8_t *)test_buf;
    img.line_length = img.w * __APP_BPP;

    src.x = 0 ;
    src.y = 0;
    //src.w = __APP_PS_WIDTH / 2  ;
    //src.h = __APP_PS_HEIGHT / 2 ;
    src.w = __APP_PS_WIDTH;
    src.h = __APP_PS_HEIGHT;

    while(1) {

        cp_t1 = DWT_CYCCNT;
        soft_copy_image(&bmp_fb, &img, &src, 0, 0);
        //g2d_copy_image(&bmp_fb, &img, &src, 0, 0);
        cp_t2 = DWT_CYCCNT - cp_t1;
        cp_time = cp_t2 / 528000.0;
    }
}


void test_g2d_rotate(void)
{
    bitmap_t  bmp_fb;
    bitmap_t  bmp_img;
    rect_t dst;

    uint32_t rotate_t1, rotate_t2;
    float rotate_time = 0.0;

    __fb_init();

    __gp_ps_buf = (uint8_t *) aw_mem_align(__APP_PS_WIDTH * __APP_PS_HEIGHT * __APP_BPP, 64);
    __gp_as_buf = (uint8_t *) aw_mem_align(__APP_AS_WIDTH * __APP_AS_HEIGHT * __APP_BPP, 64);
    if ((NULL == __gp_ps_buf) || (NULL == __gp_as_buf)) {
        while(1);
    }

    memset(&bmp_fb, 0x00, sizeof(bitmap_t));
    memset(&bmp_img, 0x00, sizeof(bitmap_t));

    while (1) {

            __as_ps_buf_init();

            bmp_fb.w = __APP_PS_WIDTH;
            bmp_fb.h = __APP_PS_HEIGHT;
            bmp_fb.flags  = 0;
            bmp_fb.format = BITMAP_FMT_BGR565;
            bmp_fb.data = (uint8_t *)__gp_awtk_fb->v_addr;

            dst.x = 0;
            dst.y = 0;
            dst.w = __APP_AS_WIDTH - 20;
            dst.h = __APP_AS_HEIGHT - 20 ;

            bmp_img.w = __APP_AS_WIDTH;
            bmp_img.h = __APP_AS_HEIGHT;
            bmp_img.flags  = 0;
            bmp_img.format = BITMAP_FMT_BGR565;
            bmp_img.data = __gp_as_buf;

#if 0
            rotate_t1 = DWT_CYCCNT;
            soft_rotate_image(&bmp_fb, &bmp_img, &dst, LCD_ORIENTATION_90);
            rotate_t2 = DWT_CYCCNT - rotate_t1;
            rotate_time = rotate_t2 / 528000.0;
            aw_kprintf("rotate_time:%d\r\n", (uint32_t)rotate_time);
#else
            rotate_t1 = DWT_CYCCNT;
            g2d_rotate_image(&bmp_fb, &bmp_img, &dst, LCD_ORIENTATION_90);
            rotate_t2 = DWT_CYCCNT - rotate_t1;
            rotate_time = rotate_t2 / 528000.0;
            aw_kprintf("rotate_time:%d\r\n", (uint32_t)rotate_time);
#endif

    }
}

/* end of file */
