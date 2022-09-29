/**
  ******************************************************************************
  * @file    pid.c
  * @Author: 夏末终年
  * @date    2022-3-25
  * @brief   增量式PID算法
  ******************************************************************************
  */ 
#include "pid.h"

struct PID_constant constant_left, constant_right;
struct PID left, right;

void PID_Constant_Init(struct PID_constant *p_constant, 
float set_time, float set_kp, float set_ki, float set_kd)
{
	p_constant->time = set_time;			//PID计算间隔时间
	p_constant->kp = set_kp;				//kp
	p_constant->ki = set_ki;				//ki
	p_constant->kd = set_kd;				//kd
}

void PID_Init(struct PID *pid, struct PID_constant *p_constant, float set_setpoint)
{
	pid->setpoint = set_setpoint;
	pid->input = 0;
	pid->output = 0;
	pid->last_error = 0;
	pid->next_error = 0;				
	pid->current_error = set_setpoint;	//当前误差值也等于设定值
	pid->constant = p_constant;
}

//速度计时时间到了进入此函数进行速度计算
void PID_Input_Renew(struct PID* pid, float measure_data)
{
	pid->input = measure_data;
}

//时间到了进入此函数进行PID算法
void PID_Compute(struct PID* pid)
{
	// 更新误差值
	pid->last_error = pid->next_error;
	pid->next_error = pid->current_error;
	pid->current_error = pid->setpoint - pid->input;
	
	// 计算输出值
	// output += Kp[e(k) - e(k-1)] + Ki*e(k) + Kd[e(k)- 2e(k-1) + e(k-2)]
	pid->output += 
	pid->constant->kp * (pid->current_error - pid->next_error) + 
	pid->constant->ki * (pid->current_error) + 
	pid->constant->kd * (pid->current_error - 2 * pid->next_error + pid->last_error);

	if (pid->output > 1000) pid->output = 1000;
	if (pid->output < -1000) pid->output = -1000;
}

//PID算法复位，将所有误差清零，重新开始计算
void PID_Reset(struct PID* pid, float set_setpoint)
{
	pid->setpoint = set_setpoint;
	pid->input = 0;
	pid->output = 0;
	pid->last_error = 0;
	pid->next_error = 0;				
	pid->current_error = set_setpoint;	//当前误差值也等于设定值
}

float PID_Output(struct PID* pid)
{
	return pid->output;
}

float PID_Setpoint(struct PID* pid)
{
	return pid->setpoint;
}

void PID_Change_Setpoint(struct PID* pid, float set_setpoint)
{	
	if (set_setpoint > 320) 	
		set_setpoint = 320;
	else if (set_setpoint < -320) 	
		set_setpoint = -320;
	else
		pid->setpoint = set_setpoint;	
}

void Motor_PID_Init()
{
	PID_Constant_Init(&constant_left, 0.1, 3.5, 1.5, 0.08);
	PID_Constant_Init(&constant_right, 0.1, 3.5, 1.5, 0.08);
	PID_Init(&left, &constant_left, 200);
	PID_Init(&right, &constant_right, 200);
}
