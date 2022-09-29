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

int encoder_init(void);
float get_motor_rotate_speed(void);


#endif /* APPLICATIONS_ENCODER_H_ */
