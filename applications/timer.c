/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       			Notes
 * 2022-09-29     Huang Ting      the first version
 */
#include <rtdevice.h>
#include <rtthread.h>
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "pid.h"

void motor_timeout(void* parameter)
{
    float rotate = get_motor_rotate_speed();
    int speed = rotate;
    PID_Input_Renew(&left, rotate);
    PID_Compute(&left);
//    PID_Input_Renew(&right, rotate);
//    PID_Compute(&right);
    int ouput = PID_Output(&left);
    rt_device_control(motor_dev, SET_MOTOR_SPEED_LEFT, (void*)(&ouput));
//    rt_device_control(motor_dev, SET_MOTOR_SPEED_RIGHT, (void*)(&speed));
    rt_kprintf("speed %d.\n", speed);
}

int timer_init(void)
{
    rt_err_t ret;
    //创建硬件定时器，周期计时，OSTick = 100hz，100ms计算一次
    rt_timer_t motor_timer = rt_timer_create("motor timer", motor_timeout, NULL,
            PID_TIMER_PERIOD, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    if (motor_timer == RT_NULL) {
       rt_kprintf("motor timer error.\n");
       return -RT_ERROR;
    }
    ret = rt_timer_start(motor_timer);
    if (ret != RT_EOK) {
       rt_kprintf("motor timer start error.\n");
       return ret;
    }
    return RT_EOK;
}
