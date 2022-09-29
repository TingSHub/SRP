/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-22     Administrator       the first version
 */
#include <rtthread.h>
#include "main.h"
#include <stdlib.h>

extern volatile int speed;
extern volatile int angle;

void clock_show(void)
{
    rt_kprintf("System Clock information\n");
    rt_kprintf("SYSCLK_Frequency = %d\n", HAL_RCC_GetSysClockFreq()); //168M
    rt_kprintf("HCLK_Frequency   = %d\n", HAL_RCC_GetHCLKFreq());     //168M
    rt_kprintf("PCLK1_Frequency  = %d\n", HAL_RCC_GetPCLK1Freq());    //42M
    rt_kprintf("PCLK2_Frequency  = %d\n", HAL_RCC_GetPCLK2Freq());    //84M
}
MSH_CMD_EXPORT(clock_show, show system clock.);

void set_speed(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("arguments error.\n");
    }
    speed = atoi(argv[1]);
}
MSH_CMD_EXPORT(set_speed, set motor speed control range from -1000 to 1000.);

void set_angle(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("arguments error.\n");
    }
    angle = atoi(argv[1]);
}
MSH_CMD_EXPORT(set_angle, set servo angle range from 0 to 120.);
