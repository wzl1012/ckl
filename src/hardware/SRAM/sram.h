#ifndef __SRAM_H
#define __SRAM_H															    
#include <gd32f10x.h>	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//�ⲿSRAM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/1/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
												  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumHalfwordToRead);


void fsmc_sram_test_write(uint32_t addr,uint8_t data);
uint8_t fsmc_sram_test_read(uint32_t addr);

#endif

