/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       			Notes
 * 2022-09-29     Huang Ting      the first version
 */
#include <rtdevice.h>
#include <rtthread.h>
#include "timer.h"
#include "encoder.h"

#define ENCODER_RESOLUTION 13    //编码器一圈的物理脉冲数
#define ENCODER_MULTIPLE 4       //编码器倍频，通过定时器的编码器模式设置
#define MOTOR_REDUCTION_RATIO 30 //电机的减速比
// 电机1圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比= 1560
#define TOTAL_RESOLUTION \
 (ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO)

struct rt_device *left_encoder;
struct rt_device *right_encoder;

int encoder_init(void)
{
    rt_err_t ret = -1;
    left_encoder = rt_device_find(LEFT_ENCODER_NAME);
    right_encoder = rt_device_find(RIGHT_ENCODER_NAME);
    if (left_encoder == RT_NULL|| right_encoder == RT_NULL) {
        rt_kprintf("find Encoder error.\n");
        return -RT_ERROR;
    }
    ret = rt_device_open(left_encoder, RT_DEVICE_OFLAG_RDONLY);
    ret = rt_device_open(right_encoder, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK) {
        rt_kprintf("open Encoder error.\n");
        return ret;
    }
    return RT_EOK;
}

float get_motor_rotate_speed(struct rt_device *encoder)
{
    rt_int32_t count;
    float rotateSpeed;
    rt_device_read(encoder, 0, &count, 1);
    /* 清空脉冲编码器计数值 */
    rt_device_control(encoder, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
    //转速 = 单位时间内的计数值 / 总分辨率 * 时间系数 单位 r / min
    rotateSpeed = 60 * count * (float)(1000 / PID_TIMER_PERIOD) / (float)TOTAL_RESOLUTION;
    return rotateSpeed;
}
