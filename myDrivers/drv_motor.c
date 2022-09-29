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
#define MOTOR_NAME              "motor"
#define MOTOR_LEFT_PWM_CHANNEL  2
#define MOTOR_RIGHT_PWM_CHANNEL 3
#define MOTOR_PERIOD_PWM        1000000

#define SET_MOTOR_SPEED_LEFT        0x21   /* 速度设置，参数：占空比 */
#define SET_MOTOR_SPEED_RIGHT       0x22   /* 速度设置，参数：占空比 */

struct motor_pwm_drv {
    struct rt_device_pwm* pwm_device;
    rt_uint32_t period;
    char channel_left;
    char channel_right;
    char *name;
};

struct motor_dev
{
    struct rt_device parent;
    struct motor_pwm_drv motor_pwm_drv;
    unsigned char left_pin1;
    unsigned char left_pin2;
    unsigned char right_pin1;
    unsigned char right_pin2;
    char *name;
} __motor_dev;

static rt_err_t drv_motor_init(rt_device_t dev);
static rt_err_t drv_motor_control(rt_device_t dev, int cmd, void *arg);
static rt_err_t drv_motor_open(rt_device_t dev, rt_uint16_t oflag);
static rt_err_t drv_motor_close(rt_device_t dev);

static struct motor_dev *motor_dev = &__motor_dev;

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
    rt_pin_write(parent->left_pin1, PIN_HIGH);
    rt_pin_write(parent->left_pin2, PIN_LOW);
    rt_pin_write(parent->right_pin1, PIN_HIGH);
    rt_pin_write(parent->right_pin2, PIN_LOW);
}

static inline void set_backward(struct motor_dev *parent)
{
    rt_pin_write(parent->left_pin1, PIN_LOW);
    rt_pin_write(parent->left_pin2, PIN_HIGH);
    rt_pin_write(parent->right_pin1, PIN_LOW);
    rt_pin_write(parent->right_pin2, PIN_HIGH);
}

static inline void set_stop(struct motor_dev *parent)
{
    rt_pin_write(parent->left_pin1, PIN_LOW);
    rt_pin_write(parent->left_pin2, PIN_LOW);
    rt_pin_write(parent->right_pin1, PIN_LOW);
    rt_pin_write(parent->right_pin2, PIN_LOW);
}

static inline void set_pwm_left(struct motor_dev *motor_dev, int duty)
{
    rt_uint32_t pulse = motor_dev->motor_pwm_drv.period / 1000 * duty;
    rt_pwm_set(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_left, motor_dev->motor_pwm_drv.period, pulse);
}

static inline void set_pwm_right(struct motor_dev *motor_dev, int duty)
{
    rt_uint32_t pulse = motor_dev->motor_pwm_drv.period / 1000 * duty;
    rt_pwm_set(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_right, motor_dev->motor_pwm_drv.period, pulse);
}

static void set_motor_left_rotate(struct motor_dev *motor_dev, int duty)
{
    if(duty > 0) {
        set_pwm_left(motor_dev, duty);
        set_forward(motor_dev);
    } else if(duty < 0) {
        set_pwm_left(motor_dev, -duty);
        set_backward(motor_dev);
    } else {
        set_pwm_left(motor_dev, 0);
        set_stop(motor_dev);
    }
}

static void set_motor_right_rotate(struct motor_dev *motor_dev, int duty)
{
    if(duty > 0) {
        set_pwm_right(motor_dev, duty);
        set_forward(motor_dev);
    } else if(duty < 0) {
        set_pwm_right(motor_dev, -duty);
        set_backward(motor_dev);
    } else {
        set_pwm_right(motor_dev, 0);
        set_stop(motor_dev);
    }
}

static void motor_pin_init(struct motor_dev *motor_dev)
{
    rt_pin_mode(motor_dev->left_pin1, PIN_MODE_OUTPUT);
    rt_pin_mode(motor_dev->left_pin2, PIN_MODE_OUTPUT);
    rt_pin_mode(motor_dev->right_pin1, PIN_MODE_OUTPUT);
    rt_pin_mode(motor_dev->right_pin2, PIN_MODE_OUTPUT);
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
    case SET_MOTOR_SPEED_LEFT:
        //rt_kprintf("left:%d.\n", duty);
        set_motor_left_rotate(motor_dev, duty);
        break;
    case SET_MOTOR_SPEED_RIGHT:
        //rt_kprintf("right:%d.\n", duty);
        set_motor_right_rotate(motor_dev, duty);
        break;
    default:
        break;
    }
    return RT_EOK;
}
static rt_err_t drv_motor_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    rt_pwm_enable(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_left);
    rt_pwm_enable(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_right);
    return RT_EOK;
}

static rt_err_t drv_motor_close(rt_device_t dev)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    rt_pwm_disable(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_left);
    rt_pwm_disable(motor_dev->motor_pwm_drv.pwm_device, motor_dev->motor_pwm_drv.channel_right);
    return RT_EOK;
}

static rt_err_t drv_motor_init(rt_device_t dev)
{
    struct motor_dev *motor_dev = rt_container_of(dev, struct motor_dev, parent);
    motor_dev->motor_pwm_drv.name = MOTOR_PWM_DEV;
    motor_dev->motor_pwm_drv.pwm_device =  (struct rt_device_pwm*)rt_device_find(MOTOR_PWM_DEV);
    if (motor_dev->motor_pwm_drv.pwm_device == RT_NULL) {
        rt_kprintf("find %s error.\n", motor_dev->motor_pwm_drv.name);
        return -1;
    }
    motor_dev->motor_pwm_drv.period = MOTOR_PERIOD_PWM;
    motor_dev->motor_pwm_drv.channel_left = MOTOR_LEFT_PWM_CHANNEL;
    motor_dev->motor_pwm_drv.channel_right = MOTOR_RIGHT_PWM_CHANNEL;
    motor_dev->left_pin1 = IN1;
    motor_dev->left_pin2 = IN2;
    motor_dev->right_pin1 = IN3;
    motor_dev->right_pin1 = IN4;

    motor_pin_init(motor_dev);
    set_motor_left_rotate(motor_dev, 0);
    set_motor_left_rotate(motor_dev, 0);
    return RT_EOK;
}

int init_motor(void)
{
    int ret = -1;
    motor_dev->name = MOTOR_NAME;
    memset(&motor_dev->parent, 0, sizeof(struct rt_device));

#ifdef RT_USING_DEVICE_OPS
    motor_dev->parent.ops = &motor_drv_ops;
#else
    motor_dev->parent.init = drv_motor_init;
    motor_dev->parent.open = drv_motor_open;
    motor_dev->parent.close = drv_motor_close;
    motor_dev->parent.control = drv_motor_control;
    motor_dev->parent.read  = RT_NULL;
    motor_dev->parent.write = RT_NULL;
#endif /* RT_USING_DEVICE_OPS */

    motor_dev->parent.type = RT_Device_Class_Miscellaneous;
    //设备注册
    ret = rt_device_register(&motor_dev->parent, motor_dev->name, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);
    return RT_EOK;
}
INIT_BOARD_EXPORT(init_motor);
