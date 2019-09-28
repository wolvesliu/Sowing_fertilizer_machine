#include "led.h"
#include "usart.h"
#include "delay.h"

u16 flag = 1;				// flag��һ����־,�ø�λ����ִֻ��һ��

extern u16 rev;

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,  GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11);	//GPIOF9,F10���øߣ�����
 GPIO_ResetBits(GPIOB, GPIO_Pin_5);											// �õͣ�ֱ�������ʹ��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  							//������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3); 					//PE.5 ����� 	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 			//LED0-->PB.13 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//�����趨������ʼ��GPIOB.13
 GPIO_ResetBits(GPIOB, GPIO_Pin_13);										//GPIOB13���õ�
 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 			//LED0-->PC.4 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 				//�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOC, GPIO_Pin_5);											//GPIOC4���õ�
}
 
// ֱ�������ת
void DC_Motor(void)
{
	IN1 = 0;	//��ת1s
	IN2 = 1;
}	

//	���������λ����������
//	�����λ��δ�������while��һ��if���������whileѭ����ִֻ��һ�Σ���Ϊ��flag��־
void Motor_Reset(void)
{		
		u16 j = 0;

		if(flag == 1)
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1)
			{
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1)
				{
					Dir_Step = 0;						// ��ת
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
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)	//���PF13�ǵ͵�ƽ�����谭����ת
				{
					Dir_Step = 1;						// ��ת
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
		
		// ����ת�����£������ת25��-> С����ת242��-> ��Ӧ1075������
		if(flag == 2)
		{
			Dir_Step = 0;						// ��ת
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
		
// �����������ת
void Motor_Circle(void)
{
	int k = 0;
	Dir_Step = 1;						// ��ת
	for(k=0; k<2700; k++)		//2800
	{
			Pulse_Step = 0;
			delay_us(rev);
			Pulse_Step = 1;
			delay_us(rev);
	}
	
	Dir_Step = 0;						// ��ת
	for(k=0; k<2700; k++)
	{
			Pulse_Step = 0;
			delay_us(rev);
			Pulse_Step = 1;
			delay_us(rev);
	}
}














