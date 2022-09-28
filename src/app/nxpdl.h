#ifndef  _NXPDL_H
#define  _NXPDL_H
#include "gd32f10x.h"
#include "Appdef.h"

bool nxpCmd_send(uint8_t cmdcode,uint8_t cdflg);
bool nxp_rspdata_read(C_CODE_ENUM cmd,uint8_t* dbuff,uint8_t bits);
#endif


