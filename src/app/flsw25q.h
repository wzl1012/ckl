#ifndef FLSW25Q_H
#define FLSW25Q_H
#include "gd32f10x.h"
#define W25QXX_CSH  gpio_bit_set(GPIOA,GPIO_PIN_4)
#define W25QXX_CSL  gpio_bit_reset(GPIOA,GPIO_PIN_4)
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define NM25Q80 	0X5213 
#define NM25Q16 	0X5214
#define NM25Q32 	0X5215
#define NM25Q64 	0X5216
#define NM25Q128	0X5217
#define NM25Q256 	0X5218

#define FLSPGSZ   256
#define FLSSCSZ   4096
typedef enum {RSTEN = 0x66, RST = 0x99, EQPI = 0x38, RSTQPI = 0xff,
              WE = 0x06, //write enable
              VSRWE = 0x50, //volatile status regs write enable
              WD = 0x04, //write disable/exit OTP mode
              RDSR = 0x05, //read status reg
              WRSR = 0x01, //write status reg
              RDSR2 = 0x35, //read status reg2
              WINRDSR2 = 0x35, //winbond read status reg2
              RDSR3 = 0x15, //read status reg3
              WRSR3 = 0xC0, //write status reg3
              WRSUS = 0XB0, //write suspend
              WRRES = 0X30, //write resume
              DPD = 0xb9, //deep  power_down
              RES = 0xab, //release from deep power down
              ID = 0x90, //device ID
              RDID = 0x9F, //read ID
              OTP = 0x3A, //enter OTP mode
              RDSFDP = 0x5A, //read SFDP
              //read cmd
              RD = 0x03, //read data
              FRD = 0x0B, //fast read data
              QIOFRD = 0xEB, //Quad I/O fast read
              QOFRD = 0x6b, //Quad output fast read
              //program cmd
              PP = 0x02, //page program
              QPP = 0x32, //Quad page program
              //erase cmd
              SE = 0x20, //sector erase
              HBE = 0x52, //half block erase
              BE = 0xD8, //block erase
              CE = 0xc7, //chip erase
							 W25X_Enable4ByteAddr=0xB7
             } XIP_XMC_CMD_E;
bool W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
bool W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
bool W25QXX_Init(void);
#endif


