### 基于rt-thread的遥控调速小车项目

**学 校： 华南理工大学**

**指导教师：**

**参与组员：**

**仓库地址：[TingSHub (TingHuang) (github.com)](https://github.com/TingSHub)**

#### §<u>01</u> 引言

##### 1、背景介绍：

#### [§<u>02</u> RT-thread]([https://www.rt-thread.org](https://www.rt-thread.org/))

##### 2.2.1 嵌入式实时操作系统

嵌入式实时操作系统（Embedded Real-time Operation System，一般称作RTOS），是指当外界事件或数据产生时，能够接受并以足够快的速度予以处理，其处理的结果又能在规定的时间之内来控制生产过程或对处理系统作出快速响应，并控制所有实时任务协调一致运行的嵌入式操作系统 。其主要特点是提供及时响应和高可靠性，核心是任务调度，任务调度的核心是调度算法。主流的RTOS国外的有μClinux、μC/OS-II、eCos、FreeRTOS等，国内的有RT-Thread、Huawei LiteOS等。

##### [2.3.2 RT-Thread 概述]([RT-Thread 简介](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/README))

RT-Thread，全称是 Real Time-Thread，是一款我国具有完全自主知识产权的开源嵌入式实时多线程操作系统，3.1.0 以后的版本遵循 Apache License 2.0 开源许可协议。它的基本属性之一是支持多任务，事实上，多个任务同时执行只是一种错觉，一个处理器核心在某一时刻只能运行一个任务。由于每次对一个任务的执行时间很短、任务与任务之间通过任务调度器进行非常快速地切换（调度器根据优先级决定此刻该执行的任务），就造成了这样一种错觉。在 RT-Thread 系统中，任务是通过线程实现的，RT-Thread 中的线程调度器也就是以上提到的任务调度器。经过了15年的迭代和丰富，伴随着物联网的兴起，现已经成为市面上装机量最大（超6亿台）、开发者数量最多、软硬件生态最好的物联网操作系统之一。

##### 2.3.3 RT-Thread 架构

RT-Thread主要采用C语言编写，容易理解，移植方便，同时具有良好的可剪裁性，其架构图如图1.3中所示。Nano 版本（极简内核）仅需要 3KB Flash、1.2KB RAM 内存资源，适用于资源受限的微控制器（MCU）系统。完整版可以实现直观快速的模块化裁剪，无缝地导入丰富的软件功能包，实现类似 Android 的图形界面及触摸滑动效果、智能语音交互效果等复杂功能，适用于资源丰富的物联网设备。相较于 Linux 操作系统，RT-Thread 体积小，成本低，功耗低、启动快速。除此以外 RT-Thread 还具有实时性高、占用资源小等特点，非常适用于各种资源受限（如成本、功耗限制等）的场合。

<img src="https://www.rt-thread.org/document/site/rt-thread-version/rt-thread-standard/figures/02Software_framework_diagram.png" title="" alt="RT-Thread 软件框架图" data-align="center">

其中RT-Thread 内核，是 RT-Thread 的核心部分，包括了多线程及其调度、信号量、邮箱、消息队列、内存管理、定时器等。

##### 2.3.4 前后台系统

在嵌入式编程发展的早期，是用裸机大while()+中断模式编程的。优点是上手容易，处理简单任务绰绰有余。而随着计算性能的提高，嵌入式编程的发展是从简单到复杂、从单任务到多任务，加上物联网的兴起，要处理的任务也是越来越复杂，这种模式渐渐显露出弊端。

1. 函数可能变得非常复杂，并且需要很长执行时间。且中断嵌套可能产生不可预测的执行时间和堆栈需求。
2. 超级循环和 ISR 之间的数据交换是通过全局共享变量进行的,应用程序的程序员必须确保数据一致性。
3. 超级循环可以与系统计时器(硬件计时器)轻松同步，但如果系统需要多种不同的周期时间，则会很难实现。
4. 超过超级循环周期的耗时函数需要做拆分，增加软件开销，应用程序难以理解，超级循环使得应用程序变得非常复杂，因此难以扩展。

##### 2.3.5 RT-Thread 优劣势

1. 更灵活的任务处理和更好的实时性。线程数量不受限制，优先级最大256个。首先RT-Thread系统先天就有着处理复杂任务、多任务并发的属性。可以把不同的任务拆分成不同的线程，根据优先级让系统自动调度，更好地可以对多任务进行区别对待。如果优先级配置得当，不同任务之间相互的影响可以降到最低。显著的优势在于，delay()时会将线程挂起，把cpu使用权交出去，这时候cpu可以处理其他任务，显著提高cpu的使用率。

2. 更方便的模块化开发和团队合作。如果是团队协作开发，那么可以各自写各自的线程，最后汇总、配置优先级启动即可。模块化开发也是用了面向对象的观点，屏蔽了一些底层的实现细节，可以更专注于所要解决的任务上，代码逻辑更加清晰，后续的拓展和维护也相对省力。

3. 可重用性。这个是比较显著的优势。不同的平台编程逻辑可能有很大不同，就智能车而言，不同的组别平台就各有不同，同一个组别每一届的平台也可能会有变化。所以对于许多打算做两年或想换组别的同学来说，就免去了痛苦的从头开始的过程，直接一键无痛移植。对于其他的比赛或项目而言，如果RT-Thread系统对该平台有适配，则熟悉的编程逻辑和风格可以让同学更加游刃有余。

4. 丰富的软件生态。这一优点可能在智能车竞赛中不那么突出，但是如果做物联网的一些比赛，丰富的第三方库会让人拍手称快。也许目前智能车面对的任务还不够复杂，但任务越来越复杂是大趋势，一些复杂的项目也是用RT-Thread系统处理的。通过做智能车熟悉了RT-Thread操作系统，也有利于未来自身嵌入式编程的发展。

5. 劣势则是使用有一定门槛，上手不如大while()+中断模式那么容易。

#### §<u>03</u> 硬件

##### 3.1 STM32407单片机

STM32系列单片机为32位单片机，功能更强大，同时配置过程更加复杂，项目选用ST（意法半导体）推出的STM32F407系列芯片。它是ST推出了以基于ARM Cortex™-M4为内核的高性能微控制器 。STM32F407提供了工作频率为168 MHz的Cortex™-M4内核（具有浮点单元）的性能。从Flash存储器执行时，STM32F407/417能够提供210 DMIPS/566 CoreMark性能，并且利用意法半导体的ART加速器实现了FLASH零等待状态。DSP指令和浮点单元扩大了产品的应用范围。该单片机片上外设丰富，熟悉使用之后便能体会到其强大之处。

##### 3.2 驱动电路

###### 3.2.1 H桥

电路得名于“H桥式驱动电路”是因为它的形状酷似字母H。4个三极管组成H的4条垂直腿，而电机就是H中的横杠（注意：图1及随后的两个图都只是示意图，而不是完整的电路图，其中三极管的驱动电路没有画出来）。如图所示，H桥式电机驱动电路包括4个三极管和一个电机。要使电机运转，必须导通对角线上的一对三极管。根据不同三极管对的导通情况，电流可能会从左至右或从右至左流过电机，从而控制电机的转向。

![H桥1.png](https://github.com/TingSHub/SRP/blob/master/assets/H%E6%A1%A51.png?raw=true)

要使电机运转，必须使对角线上的一对三极管导通。例如，如图2所示，当Q1管和Q4管导通时，电流就从电源正极经Q1从左至右穿过电机，然后再经Q4回到电源负极。按图中电流箭头所示，该流向的电流将驱动电机顺时针转动。当三极管Q1和Q4导通时，电流将从左至右流过电机，从而驱动电机按特定方向转动（电机周围的箭头指示为顺时针方向）。

![H桥2.png](https://github.com/TingSHub/SRP/blob/master/assets/H%E6%A1%A52.png?raw=true)

由图所示为另一对三极管Q2和Q3导通的情况，电流将从右至左流过电机。当三极管Q2和Q3导通时，电流将从右至左流过电机，从而驱动电机沿另一方向转动（电机周围的箭头表示为逆时针方向）。

![H桥3.png](https://github.com/TingSHub/SRP/blob/master/assets/H%E6%A1%A53.png?raw=true)

###### 3.2.2 驱动器

L298N，是一款接受高电压的电机驱动器，直流电机和步进电机都可以驱动。一片驱动芯片可同时控制两个直流减速电机做不同动作，在6V到46V的电压范围内，提供2安培的电流，并且具有过热自断和反馈检测功能。L298N可对电机进行直接控制，通过主控芯片的I/O输入对其控制电平进行设定，就可为电机进行正转反转驱动，操作简单、稳定性好，可以满足直流电机的大电流驱动条件。

![L298N.jpg](https://github.com/TingSHub/SRP/blob/master/assets/L298N.jpg?raw=true)

##### 3.3 编码器电机

###### 3.3.1 电机

要想实现PID速度控制，必须对电机进行测速，这里采用自带测速装置的电机，编码器为霍尔编码器，具体如下图。

![电机.png](https://github.com/TingSHub/SRP/blob/master/assets/%E7%94%B5%E6%9C%BA.png?raw=true)

###### 3.3.2 测速原理及方法如下

霍尔编码器是有霍尔马盘和霍尔元件组成。霍尔马盘是在一定直径的圆板上等分的布置有不同的磁极。霍尔马盘与电动机同轴，电动机旋转时，霍尔元件检测输出若干脉冲信号，为判断转向，一般输出两组存在一定相位差的方波信号。示意图如下：

![编码器.png](https://github.com/TingSHub/SRP/blob/master/assets/%E7%BC%96%E7%A0%81%E5%99%A8.png?raw=true)

通常有两种方式，第一种软件技术直接采用外部中断进行采集，根据AB相位差的不同可以判断正负。第二种硬件技术直接使用定时器的编码器模式，这里采用第二种。也是常说的四倍频，提高测量精度的方法。其实就是把AB相的上升沿和下降沿都采集而已，所以是四倍频。

#### §<u>04</u> PID

##### 4.1 PID算法

![PID.png](https://github.com/TingSHub/SRP/blob/master/assets/PID.png?raw=true)

###### 4.1.1 位置式PID

![位置式PID.png](https://github.com/TingSHub/SRP/blob/master/assets/%E4%BD%8D%E7%BD%AE%E5%BC%8FPID.png?raw=true)

###### 4.1.2 增量式PID

![增量式PID.png](https://github.com/TingSHub/SRP/blob/master/assets/%E5%A2%9E%E9%87%8F%E5%BC%8FPID.png?raw=true)

##### 4.2 基本编程实现

```c
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
void PID_Compute(struct PID* pid)
{
	// 更新误差值
	pid->last_error = pid->next_error;
	pid->next_error = pid->current_error;
	pid->current_error = pid->setpoint - pid->input;
	
	// 计算输出值
	// output += Kp[e(k) - e(k-1)] + Ki*e(k) + Kd[e(k)- 2e(k-1) + e(k-2)]
	pid->output +=  pid->constant->kp * (pid->current_error - pid->next_error) + 
					pid->constant->ki * (pid->current_error) + 
					pid->constant->kd * (pid->current_error - 2 * pid->next_error + pid->last_error);
    
	// 限幅
	if (pid->output > 1000) pid->output = 1000;
	if (pid->output < -1000) pid->output = -1000;
}
```

#### §<u>5</u> 开发工具

##### [5.1 STM32CubeMX](https://www.st.com/zh/development-tools/stm32cubemx.html)

STM32CubeMX是一种图形工具，通过分步过程可以非常轻松地配置STM32微控制器和微处理器，以及为Arm® Cortex®-M内核或面向Arm® Cortex®-A内核的特定Linux®设备树生成相应的初始化C代码。

![image-20221022193237215.png](https://github.com/TingSHub/SRP/blob/master/assets/image-20221022193237215.png?raw=true)

##### [5.2 RT-Thread Studio](https://www.rt-thread.org/studio.html)

一站式的 RT-Thread 开发工具，通过简单易用的图形化配置系统以及丰富的软件包和组件资源，让物联网开发变得简单和高效。

RT-Thread Studio 主要包括工程创建和管理，代码编辑，SDK管理，RT-Thread配置，构建配置，调试配置，程序下载和调试等功能，结合图形化配置系统以及软件包和组件资源，减少重复工作，提高开发效率。

![image-20221022191426278.png](https://github.com/TingSHub/SRP/blob/master/assets/image-20221022191426278.png?raw=true)

##### 5.3 开发环境搭建 

结合这两种开发工具能够有效提高RT-Thread工程的效率，由于软件的兼容性，依然需要进行一些必要的设置，以保证正确的配置。仅仅使用RT-Thread Studio能够完成开发，RT-Thread框架中本身支持STM32 HAL库，结合STM32CubeMX只是为了一些端口配置的简便。

###### 5.3.1 创建RT-Thread项目

![image-20221022192551411.png](https://github.com/TingSHub/SRP/blob/master/assets/image-20221022192551411.png?raw=true)

###### 5.3.2 关联RT_Thread Studio与STM32CubeMX

右键STM32CubeMX Settings，更新STM32CubeMX安装路径。

![image-20221022192624899.png](https://github.com/TingSHub/SRP/blob/master/assets/image-20221022192624899.png?raw=true)

#### §<u>06</u> FinSH

##### 6.1 FinSH 

FinSH 是 RT-Thread 的命令行组件，提供一套供用户在命令行调用的操作接口，主要用于调试或查看系统信息。它可以使用串口 / 以太网 / USB 等与 PC 机进行通信。
用户在控制终端输入命令，控制终端通过串口、USB、网络等方式将命令传给设备里的 FinSH，FinSH 会读取设备输入命令，解析并自动扫描内部函数表，寻找对应函数名，执行函数后输出回应，回应通过原路返回，将结果显示在控制终端上。当使用串口连接设备与控制终端时，FinSH 命令的执行流程，如下图所示：

![finsh.png](https://github.com/TingSHub/SRP/blob/master/assets/finsh.png?raw=true)

##### 6.2 FinSH初探

通过FinSH，我们可以很方便地进行程序的调试以及动态查看RT-Thread运行情况，通过help命令查看FinSH中支持的命令，也可以通过list_thread命令查看操作系统中运行的进程。

![FinSH初探.png](https://github.com/TingSHub/SRP/blob/master/assets/FinSH%E5%88%9D%E6%8E%A2.png?raw=true)

##### 6.2 导出命令到FinSH

FinSH不仅支持本身自带命令，也支持自定义的命令，以下代码便是自定义查看系统时钟频率的命令导出函数：

```c
void clock_show(void)
{
    rt_kprintf("System Clock information\n");
    rt_kprintf("SYSCLK_Frequency = %d\n", HAL_RCC_GetSysClockFreq()); //168M
    rt_kprintf("HCLK_Frequency   = %d\n", HAL_RCC_GetHCLKFreq());     //168M
    rt_kprintf("PCLK1_Frequency  = %d\n", HAL_RCC_GetPCLK1Freq());    //42M
    rt_kprintf("PCLK2_Frequency  = %d\n", HAL_RCC_GetPCLK2Freq());    //84M
}
MSH_CMD_EXPORT(clock_show, show system clock.);
```

测试如下：

![时钟频率测试.png](https://github.com/TingSHub/SRP/blob/master/assets/%E6%97%B6%E9%92%9F%E9%A2%91%E7%8E%87%E6%B5%8B%E8%AF%95.png?raw=true)

#### §<u>07</u> 串口

#### §<u>08</u> 电机

#### §<u>09</u> 编码器

#### §<u>10</u> 速度控制

#### §<u>11</u> 舵机

#### §<u>12</u> OLED

##### 12.1 [u8g2](https://github.com/olikraus/u8g2)软件包移植

###### 12.1.1 U8g2 是用于嵌入式设备的单色图形库

支持显示控制器：SSD1305、SSD1306、SSD1309、SSD1316、SSD1322、SSD1325、SSD1327、SSD1329、SSD1606、 SSD1607、 SH1106、SH1107、SH1108、SH1122、T6963、RA8835、LC7981、PCD8544、PCF8812、HX1230、UC1601、UC1604、UC1608、UC1610、 UC1611， UC1617， UC1701， ST7511， ST7528， ST7565， ST7567， ST7571， ST7586， ST7588， ST75256， ST75320， NT7 534， ST7920， IST3020， IST7920， LD7032， KS0108， KS0713， SED1520， SBN1661， IL3820， MAX7219 等。

###### 12.1.2 U8g2 还包括 U8x8 库

**U8g2 和 U8x8 的功能包括**

**U8g2：**

- 包括所有图形程序（线/框/圆画）。
- 支持很丰富的字体库。
- 需要微控制器中的一些内存来渲染显示屏（需要消耗较多的ram空间资源）。

**U8x8：**

- 仅文本输出（字符）设备。
- 仅允许使用每个字符固定大小（8x8 像素）的字体。
- 直接写到显示屏上，无需微控制器中的缓冲（需要消耗较少的ram空间资源）。
- U8g2图形库使用技巧（硬件驱动接口部分的分析和选择）：

U8g2图形库的驱动接口主要取决于所选用的lcd屏幕的驱动芯片方案，目前常用的驱动接口多为spi和i2c两种串行总线，如果需要较高的刷新帧率，spi的驱动方式是比较好的选择，spi的驱动时钟频率一般可以达到8Mbit，而i2c的方式一般只能达到400Kbit，但是使用spi方式驱动的时候，需要比较多的io管脚资源，一般最少需要3个io（三线spi方式），而i2c方式一般只需要2个io就可以满足。这里OLED用来显示速度等信息，对传输速率要求不高，采用IIC方式进行通信。

##### 12.2 RT-Thread移植u8g2软件包

###### 12.2.1 将u8g2软件包加入项目

在软件包页面搜索u8g2软件包，选择使用硬件IIC，版本选择C-lastest。

![添加U8g2软件包.png](https://github.com/TingSHub/SRP/blob/master/assets/%E6%B7%BB%E5%8A%A0U8g2%E8%BD%AF%E4%BB%B6%E5%8C%85.png?raw=true)

###### 12.2.2 使用STM32硬件IIC

在board.h文件中添加如下代码。

```c
#define BSP_USING_I2C1
#ifdef BSP_USING_I2C1
#define BSP_I2C1_SCL_PIN    GET_PIN(B, 6)
#define BSP_I2C1_SDA_PIN    GET_PIN(B, 7)
#endif
```

同时硬件上连接好对应的SDA和SCL，测试如下。

![OLED测试.jpg](https://github.com/TingSHub/SRP/blob/master/assets/OLED%E6%B5%8B%E8%AF%95.jpg?raw=true)