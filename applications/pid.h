#ifndef __PID_H_
#define __PID_H_

struct PID
{
	float setpoint;			//设定值
	float input;			//控制器输入
	float output;			//控制器输出
	float last_error;		//上上一次误差
	float next_error;		//上一次误差
	float current_error;	//当前误差
	struct PID_constant *constant;	//PID固定参数
};

struct PID_constant
{
	float time;				//PID计算间隔时间
	float kp;				//kp
	float ki;				//ki
	float kd;				//kd
};
struct PID_constant constant_left, constant_right;
struct PID left, right;
//PID固定参数初始化函数
void PID_Constant_Init(struct PID_constant *p_constant, 
float set_time, float set_kp, float set_ki, float set_kd);
//PID算法初始化函数
void PID_Init(struct PID *pid, struct PID_constant *p_constant, float set_setpoint);
//PID控制器输入更新函数
void PID_Input_Renew(struct PID* pid, float measure_data);
//PID算法主体
void PID_Compute(struct PID* pid);
//PID算法复位函数
void PID_Reset(struct PID* pid, float set_setpoint);
//PID输出获取函数
float PID_Output(struct PID* pid);
//PID设定值获取函数
float PID_Setpoint(struct PID* pid);
//PID给定值改变函数
void PID_Change_Setpoint(struct PID* pid, float set_setpoint);

void Motor_PID_Init(void);
#endif

