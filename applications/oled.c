/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     HuangTing       the first version
 */
#include "oled.h"
#include "pid.h"
#include "timer.h"
#define OLED_THREAD_PRIORITY         25
#define OLED_THREAD_STACK_SIZE       1024
#define OLED_THREAD_TIMESLICE        10

u8g2_t u8g2;

static void oled_thread_entry(void *parameter)
{
    char tmp[4][4];
    while (1) {
        u8g2_ClearBuffer(&u8g2);
        rt_mutex_take(pid_mutex, RT_WAITING_FOREVER);
        itoa(left.setpoint, tmp[0], 10);
        itoa(left.input, tmp[1], 10);
        itoa(right.setpoint, tmp[2], 10);
        itoa(right.input, tmp[3], 10);
        rt_mutex_release(pid_mutex);
        u8g2_SetFont(&u8g2, u8g2_font_ncenB10_tr);
        //显示设定值
        u8g2_DrawStr(&u8g2, 0, 16, "L-Tar:");
        u8g2_DrawStr(&u8g2, 64, 16, tmp[0]);
        u8g2_DrawStr(&u8g2, 100, 16, "r/m");
        //显示测量值
        u8g2_DrawStr(&u8g2, 0, 32, "L-Mea:");
        u8g2_DrawStr(&u8g2, 64, 32, tmp[1]);
        u8g2_DrawStr(&u8g2, 100, 32, "r/m");

        u8g2_DrawStr(&u8g2, 0, 48, "R-Tar:");
        u8g2_DrawStr(&u8g2, 64, 48, tmp[2]);
        u8g2_DrawStr(&u8g2, 100, 48, "r/m");

        u8g2_DrawStr(&u8g2, 0, 64, "R-Mea:");
        u8g2_DrawStr(&u8g2, 64, 64, tmp[3]);
        u8g2_DrawStr(&u8g2, 100, 64, "r/m");
        u8g2_SendBuffer(&u8g2);
        rt_thread_mdelay(1000);
    }
}

int oled_init(void)
{
    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_rt_hw_i2c, u8x8_rt_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);

    struct rt_thread *thread_oled = rt_thread_create("oled", oled_thread_entry, RT_NULL,
                OLED_THREAD_STACK_SIZE, OLED_THREAD_PRIORITY, OLED_THREAD_TIMESLICE);
    if (thread_oled != RT_NULL) {
        rt_thread_startup(thread_oled);
        //rt_kprintf("oled success\n");
    } else {
        //rt_kprintf("oled fail\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}
