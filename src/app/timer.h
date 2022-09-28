#ifndef _TIMER_H_
#define _TIMER_H_
#include "gd32f10x.h"

extern void timer_start(uint32_t timer_periph,uint16_t Prescaler, uint32_t TIM_PSCReloadMode);
extern volatile uint32_t t3;
//extern volatile uint32_t t4cnt,t4cnt1;
//extern volatile uint8_t ckovtimflg;
//extern volatile uint8_t t7dffpulstmp;
#define  TM4PNUM    9
#define  CKOVTIM    (680/(TM4PNUM+1)) //680us
#define  RFCKTIM     40000//(500*100*10/(TM4PNUM+1))  //18ms
#define  RECPLNUM   2
#define  PLNMIN     21//300*(TM4PNUM-0.05)/128
#define timer1_stop()					    timer_disable(TIMER2)
#define timer1_start_8()				timer_start(TIMER2,54,TIMER_PSC_RELOAD_NOW) //125us
#define timer1_start_64()				timer_start(TIMER2,432,TIMER_PSC_RELOAD_NOW)//1000us
#define timer1_clear_ov()				timer_flag_clear(TIMER2,TIMER_FLAG_UP) 
#define timer1_ov_flag          timer_flag_get(TIMER2, TIMER_FLAG_UP)
#define TCNT1                   TIMER_CAR(TIMER2)
#define TCNT_V                  TIMER_CNT(TIMER2)
#define T3CNT_V                  TIMER_CNT(TIMER3)
#define T4CNT_V                  TIMER_CNT(TIMER4)
#define T7CNT_V                  TIMER_CNT(TIMER7)
#define timer3_ov_interrupt_dis()		timer_interrupt_disable(TIMER3,TIMER_INT_UP)
#define timer3_stop()					   timer_disable(TIMER3)
//void timer_start(uint32_t timer_periph,uint16_t Prescaler, uint32_t TIM_PSCReloadMode);
//void user_timer_init(void);
void  timer3_start(void);
//extern  uint16_t  t7dffpuls[RECPLNUM-1];
#endif

