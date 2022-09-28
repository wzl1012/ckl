#include "headfile.h"

//uint8_t t1_tick_16ms; //t1_tick_2s;
volatile uint8_t t3_tick_16ms=0;
volatile uint32_t t3=0;
//static uint16_t  t7cvpuls[RECPLNUM]={0};
 void TIMER3_IRQHandler(void)
{
	//g_sreg = SREG;
    if(SET == timer_interrupt_flag_get(TIMER3,TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER3,TIMER_INT_FLAG_UP);
    }
	++t3_tick_16ms;
	if (t3_tick_16ms >= 62) {
		++t3;
	}
}
/*
 void TIMER4_IRQHandler(void)
{
	  if(pulcnt<=(RECPLNUM-1)){
	   t7cvpuls[pulcnt]=T7CNT_V;			
	   pulcnt++;
			if(pulcnt>1){
			t7dffpulstmp=t7cvpuls[pulcnt-1]- t7cvpuls[pulcnt-2];
      if(t7dffpulstmp<=PLNMIN){
				pulcnt=0;
			}				
		 }
		}
    //if(SET == timer_interrupt_flag_get(TIMER4,TIMER_INT_FLAG_UP)){
        timer_interrupt_flag_clear(TIMER4,TIMER_INT_FLAG_UP);			
  //  }  
		t4cnt++;
		t4cnt1++;
		if(t4cnt1>=CKOVTIM){
			  ckovtimflg=1;
		}
    if(frvavbflg||t4cnt>=RFCKTIM){
			timer_disable(TIMER4);
			timer_interrupt_disable(TIMER4,TIMER_INT_UP);
			timer_disable(TIMER7);
			memset(t7cvpuls,0,sizeof(t7cvpuls));
			 pulcnt=0;
      //t4cnt=0;		
		}
}*/
void  timer3_start(void)
{
	t3_tick_16ms = 0;
	t3 = 0;
	T3CNT_V=0;
	timer_flag_clear(TIMER3,TIMER_FLAG_UP); 
	nvic_irq_enable(TIMER3_IRQn,11U, 2U);
	timer_interrupt_enable(TIMER3,TIMER_INT_UP);
	timer_enable(TIMER3);
}
