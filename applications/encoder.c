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
#include "main.h"
#include "encoder.h"

int measurment;
struct rt_device *left_encoder;
struct rt_device *right_encoder;

int encoder_init(void)
{
    rt_err_t ret = -1;
    left_encoder = rt_device_find(LEFT_ENCODER_NAME);
    right_encoder = rt_device_find(RIGHT_ENCODER_NAME);
    if (left_encoder == RT_NULL|| right_encoder == RT_NULL) {
        rt_kprintf("Encoder find error.\n");
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

static int pulse_encoder_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t index;
    rt_int32_t count;

    /* 查找脉冲编码器设备 */
    left_encoder = rt_device_find(LEFT_ENCODER_NAME);
    if (left_encoder == RT_NULL) {
        rt_kprintf("pulse encoder sample run failed! can't find %s device!\n", LEFT_ENCODER_NAME);
        return RT_ERROR;
    }

    /* 以只读方式打开设备 */
    ret = rt_device_open(left_encoder, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK) {
        rt_kprintf("open %s device failed!\n", LEFT_ENCODER_NAME);
        return ret;
    }

    for (index = 0; index <= 10; index ++) {
        rt_thread_mdelay(100);
        /* 读取脉冲编码器计数值 */
        rt_device_read(left_encoder, 0, &count, 1);
        /* 清空脉冲编码器计数值 */
        rt_device_control(left_encoder, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
        rt_kprintf("get count %d\n",count);
    }

    rt_device_close(left_encoder);
    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pulse_encoder_sample, pulse encoder sample);
