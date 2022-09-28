#include"flash.h"


#include "gd32f10x.h" 
#include <stdio.h>


uint32_t *ptrd;



void flash_rd(uint32_t flsadr)
{
    ptrd = (uint32_t *)flsadr;
}

