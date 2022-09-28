#include "gd32f10x.h"
#include "gd32f10x_libopt.h"
//#include "systick.h"
#include "gpio.h"
#include "API.h"
#include "debug.h"
//#include "fal-common.h"

int main(void)
{
	SystemInit ( );
	user_gpio_init();
	USART0_Config();
	if(Rom_parameter_cofg(GC_DEV,PCF7945,BIN_EFLSBR_START_ADDR,BIN_EEFLSBR_START_ADDR)==FALSE){
		while(1);
	}
  Erase( );
	Program();
	while(1);
  return 0;
}

