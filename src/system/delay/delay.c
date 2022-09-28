#include "delay.h"
#include "gd32f10x_it.h"

#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 

//********************************************************************************
//修改说明
//////////////////////////////////////////////////////////////////////////////////  
	   
//static uint16_t fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
	
	
		   
void delay_init(void)
{
	uint32_t reload;
	systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);//选择外部时钟  HCLK
	//fac_us=SystemCoreClock/1000000;				//不论是否使用OS,fac_us都需要使用
	//reload=0xffffff;				//每秒钟的计数次数 单位为M 
  reload=SystemCoreClock/1000000;	
  reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	
	//fac_ms=1000/configTICK_RATE_HZ;				//代表OS可以延时的最少单位	   

	//SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK   
}								    
/*
void delay_ms(uint32_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((uint32_t)(nms*1000));				//普通方式延时
}
*/







































