#ifndef FLSCFGCK_H
#define FLSCFGCK_H
#include "headfile.h"


#define  MSDA               GPIO_PIN_7
#define  INTPIN5            GPIO_PIN_5 
#define  INTPIN15             GPIO_PIN_15 
#define  FLS_CBY_ADR       (0x08000000+0x3f800)
#define  FLS_SEC_PGSZ2     1
#define  SEC_H            3600
#define  TMOD             90
#define  HSPERD          (2*SEC_H)    
#define  TMO             (TMOD*HSPERD) 
#define  TMD             0xdd440000
#define  CNTO            400

#define  OTLMT0          1800
#define  OCNTLMT0         4

#define  OTLMT1_S         (SEC_H*3)
#define  OTLMT1_B         (SEC_H*5)
#define  OCNTLMT1_S        25
#define  OCNTLMT1_B        30

#define  OTLMT2_S         (SEC_H*9)
#define  OTLMT2_B         (SEC_H*11)
#define  OCNTLMT2_S        42
#define  OCNTLMT2_B       47


#define  OTLMT3_S         (SEC_H*12)
//#define  OTLMT4_B         (SEC_H*5)
#define  OCNTLMT3_S        52
//#define  OCNTLMT4_B        30
static void errchk(void);
 

#endif
