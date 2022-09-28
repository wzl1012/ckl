#ifndef  _DEBUG_H
#define  _DEBUG_H
#include "gd32f10x.h"
#include <stdio.h>
#define  DEBUG  0
void void_printf(void*f,...);
void USART0_Config(void);
int fputc(int ch, FILE *f);

#if DEBUG
//#define  debug_printf   printf(const char *fmt, ...)
#define  debug_printf   printf
#else
#define  debug_printf   void_printf 
#endif // (DEBUG || _DEBUG) && !DEBUG_PRINT_DISABLE
#define  info_printf   printf

#endif
