#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


/************* I/O口定义************************/
//LED端口定义
#define LED_Power PBout(10)	// DS0
#define LED_Act 	PBout(11)	// DS1	

// 直流电机端口定义
#define ENA PBout(5)		// PB5
#define IN1 PBout(6)		// PB6
#define IN2	PBout(7)		// PB7

// 步进电机端口定义
#define Dir_Step PEout(2)
#define Pulse_Step PEout(3)
// PLC端口定义
#define PLC	PCin(4)		// PC4	


/************* 函数声明 ************************/
void LED_Init(void);			// LED初始化		 
void Motor_Reset(void);		// 步进电机复位函数
void DC_Motor(void);			// 直流电机正转
void Motor_Circle(void);	// 步进电机正反转
		 				    
#endif
