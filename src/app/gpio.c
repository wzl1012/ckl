#include "headfile.h"
//#include "systick.h"
void user_kdl_init(void)
{
	//rcu_periph_clock_enable(RCU_GPIOC);
   gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSCL); 
   gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
   gpio_bit_reset(GPIOA, MSDA);	
	/*
	gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSCL);
	gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	gpio_bit_reset(GPIOA, MSCL);
	gpio_bit_reset(GPIOA, MSDA);
	*/
#if defined(MONI_INTMODE_PU)
 // rcu_periph_clock_enable(RCU_GPIOB);
	gpio_bit_set(GPIOC, POCTRL);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, POCTRL);
	//gpio_bit_set(GPIOC, POCTRL);
#endif
}


