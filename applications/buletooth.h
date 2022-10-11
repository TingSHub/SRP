/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-11     Huang Ting       the first version
 */
#ifndef APPLICATIONS_BULETOOTH_H_
#define APPLICATIONS_BULETOOTH_H_
#include <rtthread.h>
#include <rtdbg.h>
#include <rtdevice.h>
#define BULETOOTH_UART_NAME       "uart2"    //串口设备名称
struct rt_device *buletooth;                 //串口设备句柄
struct rt_semaphore buletooth_rx_sem;        //用于接收消息的信号量

int buletooth_init(void);


#endif /* APPLICATIONS_BULETOOTH_H_ */
