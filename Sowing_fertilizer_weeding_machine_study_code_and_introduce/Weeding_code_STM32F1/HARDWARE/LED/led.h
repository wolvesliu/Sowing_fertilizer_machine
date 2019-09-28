#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


/************* I/O�ڶ���************************/
//LED�˿ڶ���
#define LED_Power PBout(10)	// DS0
#define LED_Act 	PBout(11)	// DS1	

// ֱ������˿ڶ���
#define ENA PBout(5)		// PB5
#define IN1 PBout(6)		// PB6
#define IN2	PBout(7)		// PB7

// ��������˿ڶ���
#define Dir_Step PEout(2)
#define Pulse_Step PEout(3)
// PLC�˿ڶ���
#define PLC	PCin(4)		// PC4	


/************* �������� ************************/
void LED_Init(void);			// LED��ʼ��		 
void Motor_Reset(void);		// ���������λ����
void DC_Motor(void);			// ֱ�������ת
void Motor_Circle(void);	// �����������ת
		 				    
#endif
