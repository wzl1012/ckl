#ifndef __KEY_H
#define __KEY_H	 
#include "gd32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define keypad_read_level   gpio_input_bit_get(GPIOA,GPIO_PIN_15)//��ȡ����0
#define keypad_write_level  gpio_input_bit_get(GPIOC,GPIO_PIN_5)//��ȡ����1
#define keypad_FCHK_level  gpio_input_bit_get(GPIOA,GPIO_PIN_0)//��ȡ����2 
//#define keypad_write_level GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) 
 
typedef enum
{
	not_find=0,
	d7991_find=1,
	d3705_find,
} dev_findrslt_t;

#define KEYPAD_RD_PRES 	1	//KEY0����
#define KEYPAD_WR_PRES	2	//KEY1����
#define KEYPAD_FCHK_PRES	3	//KEY2����
#define KEYPAD_NO_PRES	0	//KEY2����
#define KEYSTART_SIG_PRES 4
//#define WKUP_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)
extern volatile uint8_t rfckstart;
uint8_t KEY_Scan(uint8_t);  	//����ɨ�躯��	
void keypad_event_check(void);
void Key_EXTI_Init(void);
#endif
