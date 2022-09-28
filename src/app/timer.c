#include"timer.h"
void timer_start(uint32_t timer_periph,uint16_t Prescaler, uint32_t TIM_PSCReloadMode)
{
	timer_prescaler_config(timer_periph,Prescaler,TIM_PSCReloadMode);
	//TIMER_CAR(timer_periph)=tscnt;
	timer1_clear_ov();
	//debug_printf("\r\n t2cnt=0x%x",TCNT_V);
	timer_enable(timer_periph);
}



