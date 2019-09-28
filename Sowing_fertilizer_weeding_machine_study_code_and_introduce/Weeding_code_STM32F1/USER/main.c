/*ʹ�õ����Լ��ķ�ԭװ�Ĵ������ĳ���
 *
 *	������	���ݼ����ƴ���
 *	���̣�	���ؿ��� -> ֱ�����������ת -> ���������λ -> ���㲽�����ת��-> �����������ת
 *	��λ��	������ҵ��ѧ
 *	����:	wolves_liu
 *	ʱ��:	2018.3.22
 *
 *	����˵����
 *	ֱ�����L298P����˵����PB5-ʹ�ܣ�PB6��IN1��PB7��IN2��
 *	�����������˵��		  ��PE2 �����źţ�PE3 �����źţ�PEout(2)=0,�����ת��˳ʱ�롣
 *	���������λ������		��PB13���������ţ����谭�Ǹߵ�ƽ�����谭�ǵ͵�ƽ
 *	ԭװ������						�����谭�ǵ͵�ƽ�����谭�Ǹߵ�ƽ
 *
 *	LED��							  ��PB10��PB11;�����ƣ���Դ�ơ������ơ���ͨ��Դʱ����Դ�������������ת��ʱ����������
 *	PLC�ź�							��PC5�����Ƶ�Ƭ���Ƿ��������乤��ʱΪ�ߵ�ƽ����֮��Ϊ�͵�ƽ
 *	ADC								  :	PA1 ��Ϊģ��ͨ����������  
 *
 *
 */

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "timer.h"

u16 rev = 0;

int main(void)
{	 
  u16 adcx;								// rev�ǲ������ת��					
	float voltage;					// voltage�ǵ�ѹ
	delay_init();	    	 		//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     		//LED�˿ڳ�ʼ��		 	
 	Adc_Init();		  				//ADC��ʼ��
  

	TIM3_PWM_Init(899,0);	 	//ֱ�����PWM���١�arr���Զ���װֵ = 899;//psc��ʱ��Ԥ��Ƶ�� = 0; ����Ƶ��
   
	LED_Power = 0;					// ��Դ����	
	
	// ֱ�������PWM����
	TIM_SetCompare2(TIM3,500); //PWM���١�
	
	while(1)					// PLCΪ�ߵ�ƽʱ��Ч		
	{ 
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
			{
				LED_Act = 0;				// �������������LED_Act����	
				DC_Motor();					// ֱ�������ת
				Motor_Reset();			// ���������λ����������
				
				// �õ���ǰ��ѹvoltage
				adcx=Get_Adc_Average(ADC_Channel_1,10);		//��ȡͨ��5��ת��ֵ��10��ȡƽ��
				voltage=(float)adcx*(3.3/4096);          	//��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111		
				printf("voltage = %f\n", voltage);				// ��ʾ���ǵ�ѹֵ
				
				// ���㲽�������ת��
				rev = -182 * voltage + 1000;	
				
				Motor_Circle();			//	�����������ת		
			}			
		}
		else
		{
			LED_Act = 1;
			IN1 = 0;	//ֱ�����ֹͣ
			IN2 = 0;
		}
		
	}	
}

