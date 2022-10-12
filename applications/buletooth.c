/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author       			Notes
 * 2022-10-11     Huang Ting      the first version
 */
#include "buletooth.h"
#include "pid.h"
#include "timer.h"
#include "servo.h"
struct rt_device *buletooth;                //串口设备句柄
struct rt_semaphore buletooth_rx_sem;       //用于接收消息的信号量

/* 接收数据回调函数 */
static rt_err_t buletooth_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断 调用此回调函数 然后发送接收信号量 */
    rt_sem_release(&buletooth_rx_sem);

    return RT_EOK;
}

static void RemoteControl(uint8_t ch)
{
    switch (ch) {
    case 'A':
        angle = 60;
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&(angle)));
        break;
    case 'B':
        angle = 90;
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&(angle)));
        break;
    case 'C':
        angle = 120;
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&(angle)));
        break;
    case 'D':

        break;
    case 'E':

        break;
    case 'F':

        break;
    case 'G':
        angle = 30;
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&(angle)));
        break;
    case 'H':
        angle = 40;
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&(angle)));
        break;
    case 'X':
        PID_Change_Setpoint(&left, left.setpoint + 50);
        PID_Change_Setpoint(&right, right.setpoint + 50);
        break;
    case 'Y':
        PID_Change_Setpoint(&left, left.setpoint - 50);
        PID_Change_Setpoint(&right, right.setpoint - 50);
        break;
    case 'Z':
        PID_Change_Setpoint(&left, 0);
        PID_Change_Setpoint(&right, 0);
        break;
    default:
        break;
    }
}


static void serial_thread_entry(void *parameter)
{
    static char ch = '\0';
    while (1) {
        char tmp = ch;
        /* 从串口读取一个字节的数据 没有读取到则等待接收信号量 */
        while (rt_device_read(buletooth, -1, &ch, 1) != 1) {
            /* 阻塞等待接收信号量 等到信号量后再次读取数据 */
            rt_sem_take(&buletooth_rx_sem, RT_WAITING_FOREVER);
        }
        if (tmp != ch || ch == 'X' || ch == 'Y') {
            rt_mutex_take(pid_mutex, RT_WAITING_FOREVER);
            RemoteControl(ch); //根据蓝牙发送字符进行动作
            rt_mutex_release(pid_mutex);
            rt_kprintf("%c\n", ch);
        }
    }
}

int buletooth_init(void)
{
    rt_err_t ret = RT_EOK;

    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  //初始化配置参数
    config.baud_rate = BAUD_RATE_9600;                          //修改波特率为 9600

    /* 查找系统中的串口设备 */
    buletooth = rt_device_find(BULETOOTH_UART_NAME);
    if (!buletooth) {
        rt_kprintf("find %s failed!\n", BULETOOTH_UART_NAME);
        return RT_ERROR;
    }
    //修改串口配置
    rt_device_control(buletooth, RT_DEVICE_CTRL_CONFIG, &config);
    /* 初始化信号量 */
    rt_sem_init(&buletooth_rx_sem, "buletooth_rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(buletooth, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(buletooth, buletooth_input);
    /* 创建 蓝牙接收 线程 */
    struct rt_thread *thread_buletooth = rt_thread_create("Buletooth-Thread", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread_buletooth != RT_NULL) {
        rt_thread_startup(thread_buletooth);
    } else {
        ret = RT_ERROR;
    }

    return ret;
}
