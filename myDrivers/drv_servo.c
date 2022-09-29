/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-26     Administrator       the first version
 */
#include "board.h"
#include "drv_common.h"
#include <rtdevice.h>
#include <rtservice.h>
#include <string.h>

#define SERVO_PWM_DEV "pwm2"
#define SERVO_PWM_CHANNEL 4
#define SERVO_PERIOD_PWM 20000000 //50HZ
#define SERVO_NAME "servo"

#define SET_SERVO_ANGLE 0x20 //舵机角度设置cmd

struct servo_pwm_drv {
    struct rt_device_pwm* pwm_device;
    rt_uint32_t period;
    char channel;
    char *name;
};

struct servo_dev
{
    struct rt_device parent;
    struct servo_pwm_drv servo_pwm_drv;
    char *name;
} __servo_dev;

static struct servo_dev *servo_dev = &__servo_dev;

static rt_err_t drv_servo_init(rt_device_t dev);
static rt_err_t drv_servo_control(rt_device_t dev, int cmd, void *arg);
static rt_err_t drv_servo_open(rt_device_t dev, rt_uint16_t oflag);
static rt_err_t drv_servo_close(rt_device_t dev);

static struct rt_device_ops servo_drv_ops =
{
    .init = drv_servo_init,
    .control = drv_servo_control,
    .open = drv_servo_open,
    .close = drv_servo_close,
    .read = NULL,
    .write = NULL
};

static void set_servo_angle(struct servo_dev *servo_dev, int duty)
{
    rt_uint32_t pulse = servo_dev->servo_pwm_drv.period / 1000 * duty;
    rt_pwm_set(servo_dev->servo_pwm_drv.pwm_device, servo_dev->servo_pwm_drv.channel, servo_dev->servo_pwm_drv.period, pulse);
}

static rt_err_t drv_servo_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct servo_dev *servo_dev = rt_container_of(dev, struct servo_dev, parent);
    rt_pwm_enable(servo_dev->servo_pwm_drv.pwm_device, servo_dev->servo_pwm_drv.channel);
    return RT_EOK;
}

static rt_err_t drv_servo_close(rt_device_t dev)
{
    struct servo_dev *servo_dev = rt_container_of(dev, struct servo_dev, parent);
    rt_pwm_disable(servo_dev->servo_pwm_drv.pwm_device, servo_dev->servo_pwm_drv.channel);
    return RT_EOK;
}

static rt_err_t drv_servo_control(rt_device_t dev, int cmd, void *arg)
{
    int duty = *((int*)(arg));
    struct servo_dev *servo_dev = rt_container_of(dev, struct servo_dev, parent);
    if (duty < -1000) {
        duty = -1000;
    } else if (duty > 1000) {
        duty = 1000;
    } else {}
    switch (cmd) {
    case SET_SERVO_ANGLE:
        set_servo_angle(servo_dev, duty);
        break;
    default:
        break;
    }
    return RT_EOK;
}

static rt_err_t drv_servo_init(rt_device_t dev)
{
    struct servo_dev *servo_dev = rt_container_of(dev, struct servo_dev, parent);
    servo_dev->servo_pwm_drv.name = SERVO_PWM_DEV;
    servo_dev->servo_pwm_drv.pwm_device =  (struct rt_device_pwm*)rt_device_find(SERVO_PWM_DEV);
    if (servo_dev->servo_pwm_drv.pwm_device == RT_NULL) {
        rt_kprintf("find %s error.\n", servo_dev->servo_pwm_drv.name);
        return -1;
    }
    servo_dev->servo_pwm_drv.period = SERVO_PERIOD_PWM;
    servo_dev->servo_pwm_drv.channel = SERVO_PWM_CHANNEL;
    return RT_EOK;
}

int init_servo(void)
{
    int ret = -1;
    servo_dev->name = SERVO_NAME;
    memset(&servo_dev->parent, 0, sizeof(struct rt_device));

#ifdef RT_USING_DEVICE_OPS
    servo_dev->parent.ops = &servo_drv_ops;
#else
    servo_dev->parent.init = drv_servo_init;
    servo_dev->parent.open = drv_servo_open;
    servo_dev->parent.close = drv_servo_close;
    servo_dev->parent.control = drv_servo_control;
    servo_dev->parent.read  = RT_NULL;
    servo_dev->parent.write = RT_NULL;
#endif /* RT_USING_DEVICE_OPS */

    servo_dev->parent.type = RT_Device_Class_Miscellaneous;
    //设备注册
    ret = rt_device_register(&servo_dev->parent, servo_dev->name, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);
    return RT_EOK;
}
INIT_BOARD_EXPORT(init_servo);
