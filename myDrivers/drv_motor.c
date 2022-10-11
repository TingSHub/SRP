/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author           Notes
 * 2022-09-24     Huang Ting       the first version
 */
#include "board.h"
#include "drv_common.h"
#include <rtdevice.h>
#include <rtservice.h>
#include <string.h>

#define IN1 81
#define IN2 82
#define IN3 83
#define IN4 84
#define MOTOR_PWM_DEV           "pwm2"
#define MOTOR_LEFT_NAME         "motor-left"
#define MOTOR_RIGHT_NAME        "motor-right"
#define MOTOR_LEFT_PWM_CHANNEL  2
#define MOTOR_RIGHT_PWM_CHANNEL 3
#define MOTOR_PERIOD_PWM        1000000

#define SET_MOTOR_SPEED        0x21   /* 速度设置，参数：占空比 */

struct motor_dev
{
    struct rt_device parent;
    struct rt_device_pwm* pwm_device;
    rt_uint32_t period;
    char channel;
    unsigned char pin1;
    unsigned char pin2;
    char *name;
    char *pwm_name;
} __motor_dev_left, __motor_dev_right;

static rt_err_t drv_motor_init(rt_device_t dev);
static rt_err_t drv_motor_control(rt_device_t dev, int cmd, void *arg);
static rt_err_t drv_motor_open(rt_device_t dev, rt_uint16_t oflag);
static rt_err_t drv_motor_close(rt_device_t dev);

static struct motor_dev *motor_left = &__motor_dev_left;
static struct motor_dev *motor_right = &__motor_dev_right;

static struct rt_device_ops motor_drv_ops =
{
    .init = drv_motor_init,
    .control = drv_motor_control,
    .open = drv_motor_open,
    .close = drv_motor_close,
    .read = NULL,
    .write = NULL
};

static inline void set_forward(struct motor_dev *parent)
{
    rt_pin_write(parent->pin1, PIN_HIGH);
    rt_pin_write(parent->pin2, PIN_LOW);
}

static inline void set_backward(struct motor_dev *parent)
{
    rt_pin_write(parent->pin1, PIN_LOW);
    rt_pin_write(parent->pin2, PIN_HIGH);
}

static inline void set_stop(struct motor_dev *parent)
{
    rt_pin_write(parent->pin1, PIN_LOW);
    rt_pin_write(parent->pin2, PIN_LOW);
}

static inline void set_pwm(struct motor_dev *motor_dev, int duty)
{
    rt_uint32_t pulse = motor_dev->period / 1000 * duty;
    rt_pwm_set(motor_dev->pwm_device, motor_dev->channel, motor_dev->period, pulse);
}

static void set_motor_rotate(struct motor_dev *motor_dev, int duty)
{
    if(duty > 0) {
        set_pwm(motor_dev, duty);
        set_forward(motor_dev);
    } else if(duty < 0) {
        set_pwm(motor_dev, -duty);
        set_backward(motor_dev);
    } else {
        set_pwm(motor_dev, 0);
        set_stop(motor_dev);
    }
}

static void motor_pin_init(struct motor_dev *motor_dev)
{
    rt_pin_mode(motor_dev->pin1, PIN_MODE_OUTPUT);
    rt_pin_mode(motor_dev->pin2, PIN_MODE_OUTPUT);
    set_stop(motor_dev);
}

static rt_err_t drv_motor_control(rt_device_t dev, int cmd, void *arg)
{
    int duty = *((int*)(arg));
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    if (duty < -1000) {
        duty = -1000;
    } else if (duty > 1000) {
        duty = 1000;
    } else {}
    switch (cmd) {
    case SET_MOTOR_SPEED:
        //rt_kprintf("left:%d.\n", duty);
        set_motor_rotate(motor_dev, duty);
        break;
    default:
        break;
    }
    return RT_EOK;
}
static rt_err_t drv_motor_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    rt_pwm_enable(motor_dev->pwm_device, motor_dev->channel);
    return RT_EOK;
}

static rt_err_t drv_motor_close(rt_device_t dev)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    rt_pwm_disable(motor_dev->pwm_device, motor_dev->channel);
    return RT_EOK;
}

static rt_err_t drv_motor_init(rt_device_t dev)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    motor_dev->pwm_name = MOTOR_PWM_DEV;
    motor_dev->pwm_device =  (struct rt_device_pwm*)rt_device_find(MOTOR_PWM_DEV);
    if (motor_dev->pwm_device == RT_NULL) {
        rt_kprintf("find %s error.\n", motor_dev->pwm_name);
        return -1;
    }
    motor_pin_init(motor_dev);
    set_motor_rotate(motor_dev, 0);
    return RT_EOK;
}

int init_motor(void)
{
    int ret = -1;
    motor_left->name = MOTOR_LEFT_NAME;
    motor_right->name = MOTOR_RIGHT_NAME;
    memset(&motor_left->parent, 0, sizeof(struct rt_device));
    memset(&motor_right->parent, 0, sizeof(struct rt_device));
#ifdef RT_USING_DEVICE_OPS
    motor_left->parent.ops = &motor_drv_ops;
    motor_right->parent.ops = &motor_drv_ops;
#else
    motor_left->parent.init = drv_motor_init;
    motor_left->parent.open = drv_motor_open;
    motor_left->parent.close = drv_motor_close;
    motor_left->parent.control = drv_motor_control;
    motor_left->parent.read  = RT_NULL;
    motor_left->parent.write = RT_NULL;
    motor_right->parent.init = drv_motor_init;
    motor_right->parent.open = drv_motor_open;
    motor_right->parent.close = drv_motor_close;
    motor_right->parent.control = drv_motor_control;
    motor_right->parent.read  = RT_NULL;
    motor_right->parent.write = RT_NULL;
#endif /* RT_USING_DEVICE_OPS */

    motor_left->parent.type = RT_Device_Class_Miscellaneous;
    motor_right->parent.type = RT_Device_Class_Miscellaneous;
    //设备注册
    ret = rt_device_register(&motor_left->parent, motor_left->name, RT_DEVICE_FLAG_RDWR);
    ret = rt_device_register(&motor_right->parent, motor_right->name, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    motor_left->period = MOTOR_PERIOD_PWM;
    motor_left->channel = MOTOR_LEFT_PWM_CHANNEL;
    motor_left->pin1 = IN1;
    motor_left->pin2 = IN2;

    motor_right->period = MOTOR_PERIOD_PWM;
    motor_right->channel = MOTOR_RIGHT_PWM_CHANNEL;
    motor_right->pin1 = IN3;
    motor_right->pin2 = IN4;

    return RT_EOK;
}
INIT_BOARD_EXPORT(init_motor);
