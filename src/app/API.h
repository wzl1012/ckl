#ifndef  _API_H
#define  _API_H
#include "gd32f10x.h"
#define BIN_EFLSBR_START_ADDR    ((uint32_t)0x0800D400U)
#define BIN_EFLSBR_END_ADDR      ((uint32_t)0x0800F3FFU)
#define BIN_EEFLSBR_START_ADDR    ((uint32_t)0x0800F800U)
#define BIN_EEFLSBR_END_ADDR      ((uint32_t)0x0800FBFFU)


typedef enum{
	GC_DEV=0xD1,
	NXP_DEV=0xD2
}DEV_FTCODE;
typedef enum{
	NUNDEV=0,
	PCF7945=0xA1,
	PCF7952=0xA2,
	PCF7953=0xA3,
	PCF7941=0xA4,
	OTHERDEV=0xB0
}NXP_DEVTPE_ENUM;
bool  Erase(void);
bool Program(void);
//bool Rom_parameter_cofg(DEV_FTCODE fcode,NXP_DEVTPE_ENUM devtype,uint32_t ERdlflsadr,uint32_t EERdlflsadr);
void fmc_erase_pages(uint32_t flsadr,uint8_t pgsz);
 bool fmc_prg(uint32_t staradr, uint32_t *pdata,uint32_t bytesz);
 bool fmc_program_check(uint32_t flswadr,uint32_t rdadr,uint8_t wsz);
#endif
