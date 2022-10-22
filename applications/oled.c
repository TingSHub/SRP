/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     HuangTing       the first version
 */
#include "oled.h"

u8g2_t u8g2;

void oled_init(void)
{
    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_rt_hw_i2c, u8x8_rt_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);
}
