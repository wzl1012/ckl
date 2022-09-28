#include "headfile.h"

//////////////////////////////////////////////////////////////////////////////////	 
extern SemaphoreHandle_t KeyPSPhore;
extern  TaskHandle_t BTR_T_Task_Handler;
#define USART_REC_LEN  			224  	//定义最大接收字节数 200  	
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

void u2_sendhex(uint8_t *f,uint8_t bytes)
{  
   for(uint8_t i=0;i<bytes;i++){
		 	usart_data_transmit(USART1, f[i]);
      while (!usart_flag_get(USART1, USART_FLAG_TBE)); 
	}		
} 
 
  	
uint8_t USART_RX_BUF[USART_REC_LEN];     
uint16_t USART_RX_STA=0;        
//extern TaskHandle_t BTR_T_Task_Handler;
void USART1_IRQHandler(void)                	//串口2中断服务程序
{
	static uint16_t i=0;
//  uint8_t tmpd=0;
	BaseType_t xHigherPriorityTaskWoken1,xHigherPriorityTaskWoken2;
 //static enum {
		//chkpkg_head,
	//	bgin_recv,
	//}state;
	uint32_t  retur;
	  retur=taskENTER_CRITICAL_FROM_ISR();	
	if(usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		    usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
		   // Res[i]=usart_data_receive(USART2);	//读取接收到的数据
			//debug_printf("\r\nuartd");
			 if((USART_RX_STA&0x8000)==0){
				// switch(state){
					 /*case chkpkg_head:{
						 tmp[i]=usart_data_receive(USART1);	
						 	 if(i==0&&tmp[i]==0xfd){
								   i+=1;
				         }
               else if(i==1&&tmp[i]==0xdf){
								    i=0;
								 		tmp[0]=0;
								    tmp[1]=0;
                  	state=bgin_recv;							 
					    }else{
								  i=0;
								tmp[0]=0;
								tmp[1]=0;
							}
						}
					   break;*/
				 // case bgin_recv:{
						       USART_RX_BUF[USART_RX_STA&0X1FFF]=(uint8_t)usart_data_receive(USART1);
						 	/* if((USART_RX_STA&0X3FFF)==0&&USART_RX_BUF[USART_RX_STA&0X3FFF]<9){
								    USART_RX_BUF[0]=0;
                    USART_RX_BUF[1]=0;
                    USART_RX_STA=0;
                    state=chkpkg_head;
                    break;
				         }*/
								
               if(USART_RX_STA&0x4000){
								 		if(((USART_RX_STA&0X1FFF)==i+1)&&USART_RX_BUF[USART_RX_STA&0X1FFF]==0x55){
                        USART_RX_STA|=0x8000;
											  i=0;
									      //state=chkpkg_head;
									     // break;
								      }else{
												if(USART_RX_BUF[USART_RX_STA&0X1FFF]!=0xAA){
												  USART_RX_STA&=~0x4000;
												   i=0;
												 }
												 else
													 i++;
											}
							 }else {
                   if(USART_RX_BUF[USART_RX_STA&0X1FFF]==0xaa){
								    i=USART_RX_STA&0X1FFF;
                    USART_RX_STA|=0x4000;								 
					         }
								 }
							USART_RX_STA++;
										   if((USART_RX_STA&0X1FFF)>(USART_REC_LEN-1)){USART_RX_STA=0;
								        // state=chkpkg_head;
												 USART_RX_STA|=0x2000;
							        }							
				//	 }
					// break;
		  }
				 
	  }	
   taskEXIT_CRITICAL_FROM_ISR( retur );		
 //}
			if((USART_RX_STA&0xA000)&&(BTR_T_Task_Handler!=NULL))
	{
		//for(i=0;i<(USART_RX_STA&0X3FFF);i++)
		//debug_printf("\r\nrev[%d]=%x",i,USART_RX_BUF[i]);
		vTaskNotifyGiveFromISR(BTR_T_Task_Handler,&xHigherPriorityTaskWoken1);
		xSemaphoreGiveFromISR(KeyPSPhore,&xHigherPriorityTaskWoken2);
		portYIELD_FROM_ISR((xHigherPriorityTaskWoken1|xHigherPriorityTaskWoken2));
	}
			//taskEXIT_CRITICAL_FROM_ISR( retur );
}		


