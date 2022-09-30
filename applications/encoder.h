/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-29     Huang Ting       the first version
 */
#ifndef APPLICATIONS_ENCODER_H_
#define APPLICATIONS_ENCODER_H_

#define LEFT_ENCODER_NAME   "pulse3"
#define RIGHT_ENCODER_NAME  "pulse4"

struct rt_device *left_encoder;
struct rt_device *right_encoder;

int encoder_init(void);
float get_motor_rotate_speed(struct rt_device *encoder);


#endif /* APPLICATIONS_ENCODER_H_ */
