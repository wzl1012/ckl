#include "debug.h"
//#include "gd32f10x_usart.h"
#include <stdarg.h>


void USART0_Config(void)
{
  //rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);
	rcu_periph_clock_enable(RCU_USART0);
	/* ?? USART1 ??*/
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 
	/* USART1 ??IO???? */  
 //???GPIOA
	usart_baudrate_set(USART0, 115200);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_enable(USART0);
}
void void_printf(void*f,...)
{
}
void __no_operation(void)
{
	;
}
 /* ??  :???c???printf?USART1*/ 
int fputc(int ch, FILE *f)
{
/* ?Printf?????? */
	usart_data_transmit(USART0, (unsigned char) ch);
  while (!usart_flag_get(USART0, USART_FLAG_TBE)); 
  return (ch);
}
