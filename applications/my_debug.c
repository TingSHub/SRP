/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author                Notes
 * 2022-09-22     Administrator       the first version
 */
#include <rtthread.h>
#include <stdlib.h>
#include "encoder.h"
#include "main.h"
#include "timer.h"
#include "pid.h"

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

void set_left_speed(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("arguments error.\n");
    }
    PID_Change_Setpoint(&left, atoi(argv[1]));
}
MSH_CMD_EXPORT(set_left_speed, set motor left speed control range from -1000 to 1000.);

void set_right_speed(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("arguments error.\n");
    }
    PID_Change_Setpoint(&right, atoi(argv[1]));
}
MSH_CMD_EXPORT(set_right_speed, set motor right speed control range from -1000 to 1000.);

void set_angle(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("arguments error.\n");
    }
    angle = atoi(argv[1]);
}
MSH_CMD_EXPORT(set_angle, set servo angle range from 0 to 120.);


void get_speed(int argc, char *argv[])
{
    int i = 0;
    for (;i < 5; i++) {
        rt_kprintf("leftSpeed %d.\n", leftSpeed);
        rt_kprintf("rightSpeed %d.\n\n", rightSpeed);
        rt_thread_mdelay(2000);
    }
}
MSH_CMD_EXPORT(get_speed, get speed);
