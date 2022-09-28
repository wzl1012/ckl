#include "headfile.h"


extern  TaskHandle_t CardrdTask_Handler;
extern  TaskHandle_t SLEEP_Task_Handler;
extern  void OLED_Display_Off(void);
//extern  bool tmcnts(uint8_t v,uint32_t tmpd);
//volatile uint8_t  cls_kbrdflg=0;
//volatile uint8_t  cls_lcddisflg=0;


static void timer3_init(void)
{
	    timer_parameter_struct timer_initpara;
	  rcu_periph_clock_enable(RCU_TIMER3);
	  timer_deinit(TIMER3);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 6912-1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 250-1;        //16ms 
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	  timer_auto_reload_shadow_enable(TIMER3);
    timer_init(TIMER3, &timer_initpara);
}		 
static void de_gpio_init(void)
{
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
	rcu_periph_clock_disable(RCU_GPIOA);
}
bool tmcnts(uint8_t v,uint32_t tmpd)
{
		 uint32_t tmdata[9]={0};
		 for(uint8_t i=0;i<9;i++)
		 tmdata[i]=*(uint32_t*)(FLS_CBY_ADR+i*4);
		 switch(v){
			 case 1:
		 tmdata[4]+=t3;
		 tmdata[5]+=1;
			 break;
			 case 2:
			tmdata[6]=tmpd;	 
			 break;
		 }
		 fmc_erase_pages(FLS_CBY_ADR,FLS_SEC_PGSZ2);
	   if(fmc_prg(FLS_CBY_ADR,tmdata,36)!=TRUE){
			   return FALSE;
		 }else{
			   return TRUE;
		 }		 
}
void fls_infs(void)
{
	  timer3_ov_interrupt_dis();
		timer3_stop();
		tmcnts(1,0);	
}
static bool dosm(uint32_t* tmid,uint32_t tmp)
{
	 int32_t timdif=0;
   int32_t tdif=0;
   uint32_t errcod=0;	
	 if(tmid==NULL)
		return FALSE;
	 uint32_t* tp=(uint32_t*)tmp;
	 if(*tp!=TMD||(*(tp+1)>>24)>31||(*(tp+2)>>24)>12||(((*(tp+3)>>8)&0xFF00)|(*(tp+3)>>24))>2023||(*(tp+2)>>24)==0||
		 (((*(tp+3)>>8)&0xFF00)|(*(tp+3)>>24))<2022){
		  							vTaskDelete(CardrdTask_Handler);
		                vTaskDelete(SLEEP_Task_Handler);
                     OLED_Display_Off( );
                     de_gpio_init(); 										
									errcod=(errcod&0xFFFFFFF7)|0x8;
	 }
		 timdif=(*(tp+4)-TMO);	
	   tdif=(*(tp+5)-CNTO);	 
	 if(timdif>0||tdif>0){
        if(timdif<=OTLMT0||tdif<=OCNTLMT0){ 				
				if(100<(tmid[0]&0xff)&&(tmid[0]&0xff)<200){
					       //vTaskDelete(SLEEP_Task_Handler);
					       //nvic_irq_disable(EXTI10_15_IRQn);
					       errcod=(errcod&0xFFFFFFFe)|0x1;
				 }
			 }
        else if((timdif<OTLMT1_B&&timdif>OTLMT1_S)||(OCNTLMT1_B>tdif&&tdif>OCNTLMT1_S)){
         				if(100<(tmid[0]&0xff)&&(tmid[0]&0xff)<255){
					       nvic_irq_disable(EXTI5_9_IRQn);
									 //vTaskDelete(CardrdTask_Handler);
									//vTaskDelete(SLEEP_Task_Handler);
								errcod=(errcod&0xFFFFFFFd)|0x2;
									
				     }
        }					
				else if((timdif<OTLMT2_B&&timdif>OTLMT2_S)||(OCNTLMT2_B>tdif&&tdif>OCNTLMT2_S)){	
									if((0<(tmid[0]&0xff00)&&(tmid[0]&0xff00)<180)||(100<(tmid[0]&0xff)&&(tmid[0]&0xff)<255)){										
										 //vTaskDelete(CardrdTask_Handler);
										//vTaskDelete(SLEEP_Task_Handler);
                     OLED_Display_Off( );
                     //de_gpio_init(); 										
									errcod=(errcod&0xFFFFFFFb)|0x4;
				 }
				}else if((timdif>OTLMT3_S)||(tdif>OCNTLMT3_S)){
					if((60<(tmid[0]&0xff0000)&&(tmid[0]&0xff0000)<255)||(0<(tmid[0]&0xff00)&&(tmid[0]&0xff00)<180)||(100<(tmid[0]&0xff)&&(tmid[0]&0xff)<255)){	
										//vTaskDelete(CardrdTask_Handler);
					           //vTaskDelete(SLEEP_Task_Handler);
                     OLED_Display_Off( );
                     //de_gpio_init(); 										
									errcod=(errcod&0xFFFFFFF7)|0x8;
					}
				}
			if(!tmcnts(2,errcod))
				return FALSE;
		}	
	 return TRUE;
}
 static void errchk(void)
 {	
  uint32_t tempid[1]={0};
  tempid[0]= *(__IO uint32_t*)(0x1FFFF7E8);
	//tempid[1]= *(__IO uint32_t*)(0x1FFFF7EC);	 
	 uint8_t tmp=*(uint32_t*)(FLS_CBY_ADR+6*4)&0xff,i=4;
	  timer3_init();
		timer3_start();
	 	dosm(tempid,FLS_CBY_ADR);	
	 while(i>0){
		 i--;
	 switch(tmp&(0x1<<i)){
		 case 1:
			 gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,INTPIN15);	   
		   exti_interrupt_disable(EXTI_15);
			return;
		 case 2:
			 gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,INTPIN5);
		   rcu_periph_clock_disable(RCU_TIMER7);
		   exti_deinit();
			return;
		 case 4:
			 gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
	     rcu_periph_clock_disable(RCU_I2C1);
       //exti_deinit();		 
			return;
		 case 8:
			 gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
	     rcu_periph_clock_disable(RCU_GPIOB);
       exti_deinit();
			return;		 
	 }
  }
 }

 void vApplicationIdleHook( void )
 {
	static  uint8_t chkflg=0;
	 if(!chkflg){
	  errchk( ); 
	  chkflg=1;
	 }
 }
