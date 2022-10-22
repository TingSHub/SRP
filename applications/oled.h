/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     HuangTing      the first version
 */
#ifndef APPLICATIONS_OLED_H_
#define APPLICATIONS_OLED_H_
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>

u8g2_t u8g2;
void oled_init(void);

#endif /* APPLICATIONS_OLED_H_ */
