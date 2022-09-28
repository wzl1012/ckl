#include "headfile.h"
//#include "sys.h" 
//#include "delay.h"
								    

#define  KEY_EXTI_LINE1    (EXTI_15)
#define  KEY_EXTI_LINE2    (EXTI_5)
#define  KEY_EXTI_LINE3    (EXTI_0)
#define  KEY_EXTI_LINE4    (EXTI_14)
static uint8_t  keypre=0;
volatile uint8_t rfckstart=0;
void Key_EXTI_Init(void)
{
	
  gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_5);  
  gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_15);
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_14);
 // gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_14);
    
	//中断线5-PC5
	  nvic_irq_enable(EXTI0_IRQn,6U, 2U);
    nvic_irq_enable(EXTI5_9_IRQn,6U, 2U); 
	
    //中断线15-PA15  14--PB14
     nvic_irq_enable(EXTI10_15_IRQn,6U, 2U);  
  exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);	
	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_init(EXTI_15, EXTI_INTERRUPT,EXTI_TRIG_FALLING);
	exti_init(EXTI_14, EXTI_INTERRUPT,EXTI_TRIG_FALLING);
	exti_interrupt_flag_clear(EXTI_0);
	exti_interrupt_flag_clear(EXTI_5);
	exti_interrupt_flag_clear(EXTI_15);
	exti_interrupt_flag_clear(EXTI_14);
}
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(keypad_read_level==0||keypad_write_level==0|| keystart_sigal_level==0||keypad_FCHK_level==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(keypad_read_level==0)return KEYPAD_RD_PRES;
		else if(keypad_write_level==0)return KEYPAD_WR_PRES;
		else if(keypad_FCHK_level==0)return KEYPAD_FCHK_PRES;
		else if(keystart_sigal_level==0)return KEYSTART_SIG_PRES;
	}else if(keypad_read_level==1&&keypad_write_level==1&&keystart_sigal_level==1)key_up=1; 	    
 	return 0;// 无按键按下
}
void EXTI10_15_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken1=pdFALSE,xHigherPriorityTaskWoken2=pdFALSE;
		uint32_t  retur;
	  //uint8_t  kpre_tmp=0;
	  retur=taskENTER_CRITICAL_FROM_ISR();
	if (RESET != exti_interrupt_flag_get(KEY_EXTI_LINE1)) {
		  exti_interrupt_flag_clear(KEY_EXTI_LINE1);
		  debug_printf("\r\nrkeyd");
		xSemaphoreGiveFromISR(KeyprePhore,&xHigherPriorityTaskWoken1);
	  }
		if (RESET != exti_interrupt_flag_get(KEY_EXTI_LINE4)) {
		  exti_interrupt_flag_clear(KEY_EXTI_LINE4);
			// OLED_ShowString(0,1,"K3 Genie",16,8);
	  }
		 //  delay_ms(10);
		 // if(keypad_read_level){
		  //keypre=KEYPAD_RD_PRES;
		//kpre_tmp=KEY_Scan(0);
		  xSemaphoreGiveFromISR(KeyPSPhore,&xHigherPriorityTaskWoken2);
		  portYIELD_FROM_ISR((xHigherPriorityTaskWoken1|xHigherPriorityTaskWoken2));
			//}
		taskEXIT_CRITICAL_FROM_ISR( retur );
}
void EXTI0_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken1=pdFALSE,xHigherPriorityTaskWoken2=pdFALSE;
	uint32_t  retur;
	  retur=taskENTER_CRITICAL_FROM_ISR();
		if (RESET != exti_interrupt_flag_get(KEY_EXTI_LINE3)) {
					   exti_interrupt_flag_clear(KEY_EXTI_LINE3);	
			debug_printf("\r\nfckeyd");
			 //keypre=KEYPAD_WR_PRES;
       xSemaphoreGiveFromISR(KeyprePhore,&xHigherPriorityTaskWoken1);
       xSemaphoreGiveFromISR(KeyPSPhore,&xHigherPriorityTaskWoken2);			
       portYIELD_FROM_ISR((xHigherPriorityTaskWoken1|xHigherPriorityTaskWoken2));			
	}
		taskEXIT_CRITICAL_FROM_ISR( retur );
	
}
void EXTI5_9_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken1=pdFALSE,xHigherPriorityTaskWoken2=pdFALSE;
	uint32_t  retur;
	  retur=taskENTER_CRITICAL_FROM_ISR();
		if (RESET != exti_interrupt_flag_get(KEY_EXTI_LINE2)) {
					   exti_interrupt_flag_clear(KEY_EXTI_LINE2);	
			debug_printf("\r\nwkeyd");
			 //keypre=KEYPAD_WR_PRES;
       xSemaphoreGiveFromISR(KeyprePhore,&xHigherPriorityTaskWoken1);
       xSemaphoreGiveFromISR(KeyPSPhore,&xHigherPriorityTaskWoken2);			
       portYIELD_FROM_ISR((xHigherPriorityTaskWoken1|xHigherPriorityTaskWoken2));			
	}
		taskEXIT_CRITICAL_FROM_ISR( retur );
	
}
void keypad_event_check(void)
{
#define TRY_TIME	(2)	// in order to read stable
uint8_t i,d7991_find_ok = FALSE,d3705_find_OK = FALSE;
uint8_t ttf,scantimes=0; 
//BaseType_t xHigherPriorityTaskWoken;
//uint8_t  keypre=0;
while(1){
//keypre=KEY_Scan(0);
	//debug_printf("\r\ntask1");
if(KeyprePhore!=NULL&&xSemaphoreTake(KeyprePhore,portMAX_DELAY)==pdTRUE){
	keypre=KEY_Scan(0);
	d7991_find_ok = FALSE;
	d3705_find_OK = FALSE;
	//	vTaskSuspend(RFCK_Task_Handler);
switch(keypre){
  case KEYPAD_RD_PRES:{
	//	vTaskSuspend(RFCK_Task_Handler);
		  ICD_5V_ON;
		RFCHK_5V_OFF;
     delay_us(20);
	pcf7991_init();	 
	tms3705_init();		
		  rfckstart=0;		
			  oled_print_reading(); 
				 //DEV_SEL_7991;
		     //delay_us(1000);
  			/* check ASK tag talk frist */
		//debug_printf("\r\ntask1dbg");
		scantimes=2;		
while(scantimes--){
		for (i = 0; (i<TRY_TIME); i++){ 
	    ttf = pcf7991_turn_dout(50, PCF7991_PAGE0_NORMAL, 20, 10);
		}     
	/* ASK tag talk first */
	if (ttf) {
		/* em4170(48) */
			if(!DEV_SELPIN_level){
		for (i = 0; (i<TRY_TIME)&&(!d7991_find_ok); i++) {
			em4170_find_tag(&em4170_tag1);
			d7991_find_ok = em4170_keypad_interaction(&em4170_tag1);
		}
		if(d7991_find_ok){break;}
	}
				/* new(48) */
		if(!DEV_SELPIN_level){
		for (i = 0; (i<TRY_TIME)&&(!d7991_find_ok); i++) {
			new48_find_tag(&em4170_tag1);
			d7991_find_ok = em4170_keypad_interaction(&em4170_tag1);
		}
			if(d7991_find_ok){break;}
	}
		/* nova(T5) */
		//for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
		//	nova_find_tag(&nova_tag1);
	//		find_ok = nova_keypad_interaction(&nova_tag1);
	//	}
		
		/* pcf7935(44) */
	//	for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
		//	pcf7935_find_tag(&pcf7935_tag1);
	//		find_ok = pcf7935_keypad_interaction(&pcf7935_tag1);
	//	}
		
		/* e5561(8C) */
	//	for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
	//		e5561_find_tag(&e5561_tag1);
		//	find_ok = e5561_keypad_interaction(&e5561_tag1);
	//	}
		
		/* em4100(13) */
	//	for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
		//	em4100_find_tag(&em4100_tag1, PCF7991_PAGE0_EM4100);
	//		find_ok = em4100_keypad_interaction(&em4100_tag1);
	//	}
	}

	/* ASK reader talk first or TI FSK */
	else {
		/* r9wk(4C) */
		//for (i = 0; (i<1)&&(!find_ok); i++) {
			//r9wk_find_tag(&r9wk_tag_ori);
			//find_ok = r9wk_keypad_interaction(&r9wk_tag_ori);
	//	}
		
		/* ts06 */
	//	for (i = 0; (i<1)&&(!find_ok); i++) {
		//	ts06_find_tag(&ts06_tag1);
		//	find_ok = ts06_keypad_interaction(&ts06_tag1);
		//}
		
		/* ts06new */
	//	g_pcf7991_p0_pcf7936 = PCF7991_PAGE0_PCF7936_NEW_BOARD;
	//	for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
	//		ts06new_find_tag(&ts06new_tag1, &pcf7936_tag1);
	//		find_ok = ts06new_keypad_interaction(&ts06new_tag1);
	//	}		
				/* ts06new */
	//	g_pcf7991_p0_pcf7936 = PCF7991_PAGE0_PCF7936_OLD_BOARD;
	//	for (i = 0; (i<TRY_TIME)&&(!find_ok); i++) {
		//	ts06new_find_tag(&ts06new_tag1, &pcf7936_tag1);
		//	find_ok = ts06new_keypad_interaction(&ts06new_tag1);
		//}
		/* tms37145 */
		if(DEV_SELPIN_level){
  	for (i = 0; (i<TRY_TIME)&&(!d3705_find_OK); i++) {
			tms37145_find_tag(&tms37145_tag1);
			d3705_find_OK = tms37145_keypad_interaction(&tms37145_tag1);
		}
			if(d3705_find_OK){break;}
	}
		/* tms37206 */
	//	for (i = 0; (i<1)&&(!find_ok); i++) {
		//	tms37206_find_tag(&tms37206_tag1);
		//	find_ok = tms37206_keypad_interaction(&tms37206_tag1);
	//	}
		
                
		/* pcf7936(46) */
	//	g_pcf7991_p0_pcf7936 = PCF7991_PAGE0_PCF7936_NEW_BOARD;
	if(!DEV_SELPIN_level){
   ts06_pcf7936typeinit_tag(&pcf7936_tag1/*,pcf7936_yes,nocrty*/);
		for (i = 0; (i<TRY_TIME)&&(!d7991_find_ok); i++) {
			if(pcf7936_find_tag(&pcf7936_tag1)){
			d7991_find_ok = pcf7936_keypad_interaction(&pcf7936_tag1);
			}
		}
		if(d7991_find_ok){break;}
	}
		/* pcf7936(46) */
		//g_pcf7991_p0_pcf7936 = PCF7991_PAGE0_PCF7936_OLD_BOARD;
	//	if(!DEV_SELPIN_level){
			// ts06_pcf7936typeinit_tag(&pcf7936_tag1/*,pcf7936_not,hitag2*/);
		//for (i = 0; (i<TRY_TIME)&&(!d7991_find_ok); i++) {
			//pcf7936_clear_tag_flag(&pcf7936_tag1);
		//	pcf7936_find_tag(&pcf7936_tag1);
     //d7991_find_ok = pcf7936_keypad_interaction(&pcf7936_tag1);
           //     }
		 //  if(d7991_find_ok){break;}
		//	}
		
				/* pcf7953(47) */
		/*if(!DEV_SELPIN_level){
			pcf7953_init_tag(&pcf7953_tag);
		   for (i = 0; (i<TRY_TIME)&&(!d7991_find_ok); i++) {
		       pcf7953_find_tag(&pcf7953_tag);
          // d7991_find_ok = pcf7953_keypad_interaction(&pcf7953_tag);
         }
		   if(d7991_find_ok){break;}
			}*/
		}
	  if(DEV_SELPIN_level)
			   DEV_SEL_7991;
	  else
			  DEV_SEL_3705;
        delay_us(1000);		
	}
	   if (!d7991_find_ok&&!d3705_find_OK)
		    oled_print_ng_press();
  }
	break;
	case KEYPAD_WR_PRES:		
	  {
			//vTaskSuspend(RFCK_Task_Handler);
			  rfckstart=0;	
         ICD_5V_ON;
			  RFCHK_5V_OFF;
			delay_us(20);
	     pcf7991_init();	 
	    tms3705_init();		
			oled_print_writing();
		  break;
	  }
	case KEYPAD_FCHK_PRES:
	  {
			RFCHK_5V_ON;
			delay_us(20);
			oled_print_RFrate();			
			//rf_chk_init();
		  //if(eTaskGetState(RFCK_Task_Handler)==eSuspended)
			//vTaskResume(RFCK_Task_Handler);
		   rfckstart=1;
			//xTaskNotifyGive(RFCK_Task_Handler);
			//rf_ratepinchk_rd();
		  break;
	  }
	default:
		//if(d7991_find_ok)d7991_find_ok=FALSE;
		//if(d3705_find_OK)d3705_find_OK=FALSE;
	//debug_printf("\r\ntask2end");
		break;
  }
 if(!ICD_5V_CTRL_PIN_LEVEL)
	   ICD_5V_OFF;
 // if(!RFCHK_5V_CTRL_PIN_LEVEL)
	  // RFCHK_5V_OFF;
 }
 //portBASE_TYPE uxHighWaterMark;
 //uxHighWaterMark=uxTaskGetStackHighWaterMark( CardrdTask_Handler );
 //debug_printf("\r\ntasksz=%d",(uint16_t)uxHighWaterMark);
//debug_printf("\r\ntask1end");
 // if(keypre&&(KeyPSPhore!=NULL))
	//{
	//	xSemaphoreGive(KeyPSPhore);
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//	}
// vTaskDelay(10);
 }
	/* exit keypad state, enable uart communication */
//	uart1_receive_en();
}
