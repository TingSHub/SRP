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

volatile int leftSpeed;
volatile int rightSpeed;

void motor_timeout(void* parameter)
{
    float leftRotate = get_motor_rotate_speed(left_encoder);
    float rightRotate = get_motor_rotate_speed(right_encoder);
    leftSpeed = leftRotate;
    rightSpeed = rightRotate;
    //更新反馈速度 PID计算
    PID_Input_Renew(&left, leftRotate);
    PID_Compute(&left);
    PID_Input_Renew(&right, rightRotate);
    PID_Compute(&right);
    int leftOuput = PID_Output(&left);
    int rightOuput = PID_Output(&right);
    //计算输出值不变不调用驱动程序
    rt_device_control(motor_left, SET_MOTOR_SPEED, (void*)(&leftOuput));
    rt_device_control(motor_right, SET_MOTOR_SPEED, (void*)(&rightOuput));
}

int timer_init(void)
{
    rt_err_t ret;
    //创建硬件定时器 周期计时 OSTick = 100hz 100ms计算一次
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
