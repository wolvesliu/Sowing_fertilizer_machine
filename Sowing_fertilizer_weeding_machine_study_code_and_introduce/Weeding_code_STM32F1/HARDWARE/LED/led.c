#include "led.h"
#include "usart.h"
#include "delay.h"

u16 flag = 1;				// flag是一个标志,让复位程序只执行一次

extern u16 rev;

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,  GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11);	//GPIOF9,F10设置高，灯灭
 GPIO_ResetBits(GPIOB, GPIO_Pin_5);											// 置低，直流电机不使能

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  							//推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3); 					//PE.5 输出高 	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 			//LED0-->PB.13 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB.13
 GPIO_ResetBits(GPIOB, GPIO_Pin_13);										//GPIOB13设置低
 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 			//LED0-->PC.4 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOC, GPIO_Pin_5);											//GPIOC4设置低
}
 
// 直流电机正转
void DC_Motor(void)
{
	IN1 = 0;	//正转1s
	IN2 = 1;
}	

//	步进电机复位（传感器）
//	电机复位这段代码两个while和一个if在整个大的while循环中只执行一次，因为有flag标志
void Motor_Reset(void)
{		
		u16 j = 0;

		if(flag == 1)
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1)
			{
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1)
				{
					Dir_Step = 0;						// 正转
					for(j=0; j<5; j++)
					{
						Pulse_Step = 0;
						delay_us(1000);
						Pulse_Step = 1;
						delay_us(1000);
					}	
				}
				flag = 2;
			}
		}
			
		if(flag == 1 )
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)
			{
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)	//如果PF13是低电平，无阻碍，反转
				{
					Dir_Step = 1;						// 反转
					for(j=0; j<5; j++)
					{
						Pulse_Step = 0;
						delay_us(1000);
						Pulse_Step = 1;
						delay_us(1000);
					}			
				}
				flag = 2;
			}				
		}		
		
		// 先正转到极致，大齿轮转25°-> 小齿轮转242°-> 对应1075个脉冲
		if(flag == 2)
		{
			Dir_Step = 0;						// 正转
			for(j=0; j<1200; j++)		//1075
			{
				Pulse_Step = 0;
				delay_us(1000);
				Pulse_Step = 1;
				delay_us(1000);
			}
		}		
		flag = 3;
}
		
// 步进电机正反转
void Motor_Circle(void)
{
	int k = 0;
	Dir_Step = 1;						// 反转
	for(k=0; k<2700; k++)		//2800
	{
			Pulse_Step = 0;
			delay_us(rev);
			Pulse_Step = 1;
			delay_us(rev);
	}
	
	Dir_Step = 0;						// 正转
	for(k=0; k<2700; k++)
	{
			Pulse_Step = 0;
			delay_us(rev);
			Pulse_Step = 1;
			delay_us(rev);
	}
}














