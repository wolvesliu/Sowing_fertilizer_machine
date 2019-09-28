/*使用的是自己的非原装的传感器的程序
 *
 *	描述：	除草剂控制代码
 *	流程：	开关开启 -> 直流电机持续正转 -> 步进电机复位 -> 计算步进电机转速-> 步进电机正反转
 *	单位：	湖北工业大学
 *	作者:	wolves_liu
 *	时间:	2018.3.22
 *
 *	引脚说明：
 *	直流电机L298P引脚说明：PB5-使能，PB6是IN1，PB7是IN2；
 *	步进电机引脚说明		  ：PE2 方向信号，PE3 脉冲信号；PEout(2)=0,电机正转，顺时针。
 *	步进电机复位传感器		：PB13，输入引脚，有阻碍是高电平，无阻碍是低电平
 *	原装传感器						：有阻碍是低电平，无阻碍是高电平
 *
 *	LED灯							  ：PB10、PB11;两个灯：电源灯、动作灯。接通电源时，电源灯亮；步进电机转动时，动作灯亮
 *	PLC信号							：PC5，控制单片机是否工作，让其工作时为高电平，反之则为低电平
 *	ADC								  :	PA1 作为模拟通道输入引脚  
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
  u16 adcx;								// rev是步进电机转速					
	float voltage;					// voltage是电压
	delay_init();	    	 		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
 	LED_Init();			     		//LED端口初始化		 	
 	Adc_Init();		  				//ADC初始化
  

	TIM3_PWM_Init(899,0);	 	//直流电机PWM调速。arr：自动重装值 = 899;//psc：时钟预分频数 = 0; 不分频。
   
	LED_Power = 0;					// 电源灯亮	
	
	// 直流电机，PWM调速
	TIM_SetCompare2(TIM3,500); //PWM调速。
	
	while(1)					// PLC为高电平时有效		
	{ 
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
			{
				LED_Act = 0;				// 步进电机动作，LED_Act才亮	
				DC_Motor();					// 直流电机正转
				Motor_Reset();			// 步进电机复位（传感器）
				
				// 得到当前电压voltage
				adcx=Get_Adc_Average(ADC_Channel_1,10);		//获取通道5的转换值，10次取平均
				voltage=(float)adcx*(3.3/4096);          	//获取计算后的带小数的实际电压值，比如3.1111		
				printf("voltage = %f\n", voltage);				// 显示的是电压值
				
				// 计算步进电机的转速
				rev = -182 * voltage + 1000;	
				
				Motor_Circle();			//	步进电机正反转		
			}			
		}
		else
		{
			LED_Act = 1;
			IN1 = 0;	//直流电机停止
			IN2 = 0;
		}
		
	}	
}

