#ifndef RFCK_H
#define RFCK_H
//#include "headfile.h"
#define   FC_RATE_IN     GPIO_PIN_6
#define   FC_CODE_IN     GPIO_PIN_1
#define  FC_CODE_IN_PIN_LEVEL  gpio_input_bit_get(GPIOB,FC_CODE_IN)
#define  FC_RATE_IN_PIN_LEVEL  gpio_input_bit_get(GPIOC,FC_RATE_IN)
#define T7CNT_V                  TIMER_CNT(TIMER7)
void rf_chk_init(void);

#endif

