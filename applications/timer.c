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
#include "oled.h"

volatile int leftSpeed;
volatile int rightSpeed;
rt_mutex_t pid_mutex = RT_NULL;

void motor_timeout(void* parameter)
{
    float leftRotate = get_motor_rotate_speed(left_encoder);
    float rightRotate = get_motor_rotate_speed(right_encoder);
    leftSpeed = leftRotate;
    rightSpeed = rightRotate;
    //更新反馈速度 PID计算
    rt_mutex_take(pid_mutex, RT_WAITING_FOREVER); //获取互斥量 防止设定值在pid计算时发生改变
    PID_Input_Renew(&left, leftRotate);
    PID_Compute(&left);
    PID_Input_Renew(&right, rightRotate);
    PID_Compute(&right);
    rt_mutex_release(pid_mutex);  //释放互斥量
    int leftOuput = PID_Output(&left);
    int rightOuput = PID_Output(&right);
    //计算输出值不变不调用驱动程序
    rt_device_control(motor_left, SET_MOTOR_SPEED, (void*)(&leftOuput));
    rt_device_control(motor_right, SET_MOTOR_SPEED, (void*)(&rightOuput));
}

void oled_timeout(void* parameter)
{
    char tmp[2][3];
    u8g2_ClearBuffer(&u8g2);
    itoa(left.setpoint, tmp[0], 10);
    itoa(left.input, tmp[1], 10);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB10_tr);
    //显示设定值
    u8g2_DrawStr(&u8g2, 0, 16, "target:");
    u8g2_DrawStr(&u8g2, 72, 16, tmp[0]);
    u8g2_DrawStr(&u8g2, 100, 16, "r/m");
    //显示测量值
    u8g2_DrawStr(&u8g2, 0, 32, "measure:");
    u8g2_DrawStr(&u8g2, 72, 32, tmp[1]);
    u8g2_DrawStr(&u8g2, 100, 32, "r/m");
    //显示占空比
    u8g2_DrawStr(&u8g2, 0, 48, "duty:");
    u8g2_DrawStr(&u8g2, 0, 64, "battery:");
    u8g2_DrawStr(&u8g2, 100, 48, "%");
    u8g2_DrawStr(&u8g2, 100, 64, "V");
    u8g2_SendBuffer(&u8g2);
}

int timer_init(void)
{
    rt_err_t ret;
    pid_mutex = rt_mutex_create("pid-mutex",RT_IPC_FLAG_PRIO);
    if (pid_mutex == RT_NULL) {
        rt_kprintf("pid_mutex create fail.\n");
    }
    //创建硬件定时器 周期计时 OSTick = 100hz 100ms计算一次
    rt_timer_t motor_timer = rt_timer_create("motor timer", motor_timeout, NULL,
            PID_TIMER_PERIOD, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    if (motor_timer == RT_NULL) {
       rt_kprintf("motor timer error.\n");
       return -RT_ERROR;
    }
    ret = rt_timer_start(motor_timer);
    if (ret != RT_EOK) {
       rt_kprintf("motor timer start error.\n");
       return ret;
    }
    rt_timer_t oled_timer = rt_timer_create("oled timer", oled_timeout, NULL,
            OLED_TIMER_PERIOD, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    if (oled_timer == RT_NULL) {
       rt_kprintf("oled timer error.\n");
       return -RT_ERROR;
    }
    //ret = rt_timer_start(oled_timer);
    if (ret != RT_EOK) {
       rt_kprintf("oled timer start error.\n");
       return ret;
    }
    return RT_EOK;
}
