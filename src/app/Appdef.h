#ifndef APPDEF_H
#define APPDEF_H
#include "gd32f10x.h"
#include "API.h"
#define   MSCL   GPIO_PIN_6
#define   MSDA   GPIO_PIN_7
#define   POCTRL  GPIO_PIN_8
//#define   EEROMSZ   1024
#define   EROM_PGSIZE   32
#define   EEROM_PGSIZE  4
#define   EEROM_B0LPGSIZE 2
#define   EEROM_BKSIZE  512
#define   GEROM_PGSIZE   64
#define   GEEROM_PGSIZE  4
#define   ERDBUGBY   0xA5
#define   C1   0x48
#define   C2   0x36
#define   C3   0x06
#define   C4   0x27
#define   C5   0x06
#define   C6   0x03
#define   C7   0x37
#define   C8   0xE8
#define   C9   0x99
#define   C10   0x66
#define   C11   0x55
#define   ERCRSP  0x11  
#define   RRDRSP  0x02
#define   TEELL    0
#define   PGWSZ  192
//#define   TECADR   0x7FF
#define   TEECADR  0x
//#define   ERCKADR  0x1FFF
#define   EERBCKADR  0x01
#define   ERBCKADR  0x0000
//#define   ERCCKADR  ERBCKADR+TECADR
#define   ERCKVAL  0xA5
//#define   DEFAULTCMD  0x00
#define   ERBINADR   0x2000
#define   ID1  0x08
#define   ID2  0x02
#define   ID3  0x38
#define   IDEEEPGADR  (ID3>>2)
typedef enum{
	DEFAULTCMD=0x00,
	c_trace=0x02,
	//c_setpc=0x05,  //0x05
	c_reset=0x06,	
	c_er_erom=0x08,  //0x08
	c_wr_erom=0x09,
	c_wr_eeprom=0x0A,
	c_ee_dump=0x0E,
	c_er_dump=0x0F,
	c__prog_config=0x14,
	c_protect=0x12,
  c_wr_erom64=0x18,	
	g_trace=0x4E,
	g_setpc=0x05,  //0x05
	g_reset=0x06,	
	g_er_erom=0x54,  //0x08
	g_wr_erom=0x64,  //0x09
	g_wr_eeprom=0x56, //0x0A
	g_crty_wr=0x55,
	g_ee_dump=0x0E,
	g_er_dump=0x0F,
	g_protect=0x12,	
	g_EROMBR=0x87,
	g_EROMVCK=0x51,
	g_EEROMBR=0x52,
	g_EBL=0x55,
	g_ct=0x71,
	g_cry=0x6E,
	g_b0pglst=0x68 //0x0A
}C_CODE_ENUM;

typedef enum{
	ERASE_CMD=0x0E,
	EPRGM_CMD=0x0B
}DEV_PRGCMD;
typedef enum{
	entmonit_mode_rsp=0xAA,
	EEROM_WFail_rsp=0xFF
}dev_respdat;

void  Download_intface_init(void);
static bool  Moni_int_modset_PPS(void);
static bool EROM_32Write(uint8_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wsize);
void  EEROM_4Write(uint8_t pgaddr,uint8_t *wbuff,uint8_t wsize);
static bool Cmd_send(uint8_t cmdcode);
static bool GCdata_send(uint8_t data);
bool rspdata_read(C_CODE_ENUM cmd,uint8_t* dbuff,uint8_t bits);
bool ROM_PRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize);
bool GCROM_PRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize);
bool GCROM_LBPRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize);
//bool ROM_ER(C_CODE_ENUM cmd);
bool ROM_ER2(C_CODE_ENUM cmd);
static bool Gsopr(void);
static bool Gce8(void);
static uint8_t Moni_int_modset_PU(DEV_FTCODE fcode);
static bool Moni_modset_SPDPU(void);
uint8_t GCEROM_RD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint16_t raddr);
bool GCEROM_CKRD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint8_t bsz);
//bool GCEEROM_RD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint8_t *wbuff,uint16_t bytes,uint16_t rlbytes);
//bool GCEEROM_RD1(C_CODE_ENUM cmd,uint16_t bytes);
bool Gcgetchk_devtype(uint8_t *wbuff);
bool GCCryt(C_CODE_ENUM cmd);
bool GCdecryt(void);
bool GCEEROMPG0_PRO(uint8_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize);
//bool Rom_parameter_cofg(DEV_FTCODE fcode,NXP_DEVTPE_ENUM devtype,uint32_t ERdlflsadr,uint32_t EERdlflsadr);
bool GCERBIN_RDTOOUT(C_CODE_ENUM cmd,uint16_t eraddr,uint16_t bsz,uint8_t gsz);
static bool  NXPWP(void);
#endif
