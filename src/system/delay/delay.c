#include "delay.h"
#include "gd32f10x_it.h"

#include "FreeRTOS.h"					//FreeRTOSʹ��		  
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 

//********************************************************************************
//�޸�˵��
//////////////////////////////////////////////////////////////////////////////////  
	   
//static uint16_t fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
	
	
		   
void delay_init(void)
{
	uint32_t reload;
	systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);//ѡ���ⲿʱ��  HCLK
	//fac_us=SystemCoreClock/1000000;				//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	//reload=0xffffff;				//ÿ���ӵļ������� ��λΪM 
  reload=SystemCoreClock/1000000;	
  reload*=1000000/configTICK_RATE_HZ;			//����configTICK_RATE_HZ�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
	//fac_ms=1000/configTICK_RATE_HZ;				//����OS������ʱ�����ٵ�λ	   

	//SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 						//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK   
}								    
/*
void delay_ms(uint32_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{		
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((uint32_t)(nms*1000));				//��ͨ��ʽ��ʱ
}
*/







































