#include "headfile.h"
#define  STEPNUM   1
//#define  FLS_CBY_ADR 0x0803f800
//#define  FLS_SEC_PGSZ2  1
#define  KBSNFLSADR  (FLS_CBY_ADR+32)
void  Download_intface_init(void)
{
	
}
void fmc_erase_pages(uint32_t flsadr,uint8_t pgsz)
{
    uint8_t erase_counter;

    fmc_unlock();

    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    for(erase_counter = 0; erase_counter < pgsz; erase_counter++){
        fmc_page_erase(flsadr + (FLS_PGSZ * erase_counter));
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }

    fmc_lock();
}
 bool fmc_program_check(uint32_t flswadr,uint32_t rdadr,uint8_t wsz)
{
    uint32_t i;
    uint32_t* ptrd=NULL;
	  uint32_t* rtrd=NULL; 
    ptrd = (uint32_t *)flswadr;
    rtrd =(uint32_t *)rdadr;
    for(i = 0; i <wsz; i++){
        if((*ptrd) != (*rtrd)){
					  return FALSE;
           // break;
        }else{
            ptrd++;
					  rtrd++;
        }
    }
		return TRUE;
}
 bool fmc_prg(uint32_t staradr, uint32_t *pdata,uint32_t bytesz)
{
	  fmc_unlock();
	  uint32_t adrtar=staradr,adrend=0;
    uint32_t* dtmp=NULL;	
	   if(bytesz%4){
			   return FALSE;
		 }
		   adrend=adrtar+bytesz;
		   dtmp= pdata;
    while(adrtar < adrend){
        if(fmc_word_program(adrtar, *dtmp)!=FMC_READY)return FALSE;
        adrtar += 4;
			  dtmp+=1;
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR); 
    }
		if(fmc_program_check(staradr,(uint32_t)pdata,bytesz/4)!=TRUE)
		{
			return FALSE;
		}
    fmc_lock();
   return TRUE;		
}
static bool bdcmp_update(void)
{
		 uint32_t tmdata[9]={0};
		 for(uint8_t i=0;i<9;i++)
		 tmdata[i]=*(uint32_t*)(FLS_CBY_ADR+i*4);
		 tmdata[8]+=STEPNUM;
		 fmc_erase_pages(FLS_CBY_ADR,FLS_SEC_PGSZ2);
	   if(fmc_prg(FLS_CBY_ADR,tmdata,36)!=TRUE){
			   return FALSE;
		 }else{
			   return TRUE;
		 }
}
static Rom_parameter_struct Rom_partmp={0};
static bool Cmd_send(uint8_t cmdcode)
{
	uint8_t  i=0;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	//gpio_bit_set(GPIOA, MSDA);
	gpio_bit_reset(GPIOA, MSDA);
	//delay_us(5);
  timer1_stop();
	timer1_clear_ov();
	TCNT1 = 40000;				// ptime is 1ms
	timer1_start_8();	
	while(!gpio_input_bit_get(GPIOA, MSCL)){
		if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		}			
	}
			if((cmdcode>>0)&0x01){		
			gpio_bit_set(GPIOA, MSDA);
		}else{
			gpio_bit_reset(GPIOA, MSDA);
		}
	 timer1_stop();
	timer1_clear_ov();
	TCNT1 = 40000;				// ptime is 100
	timer1_start_8();	
	for(i=1;i<9;i++){	
		while(gpio_input_bit_get(GPIOA, MSCL)){
			if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		}
		}
		if(i==8)break;
		delay_us(1);
		if(Rom_partmp.devtype==PCF7922){
			delay_us(16);
		}
	if((cmdcode>>i)&0x01){
			//delay_us(1);
			gpio_bit_set(GPIOA, MSDA);
		}else{
			//delay_us(1);
			gpio_bit_reset(GPIOA, MSDA);
		}
		//i++;
		//cmdcode=cmdcode>>i;
		if(Rom_partmp.devtype==PCF7922){
			delay_us(6);
		}
			 TCNT_V=0;
	     timer1_clear_ov();
	     while(!gpio_input_bit_get(GPIOA, MSCL)&&(i<8)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		}				
	   }
		if(Rom_partmp.devtype==PCF7922){
			delay_us(8);
		}
	//microseconds_delay(2);	
}
	//while(gpio_input_bit_get(GPIOA, MSCL));
   //delay_us(1);
	//microseconds_delay(8);
   timer1_stop();
	gpio_bit_set(GPIOA, MSDA);
  delay_us(20);
  return TRUE;
}
/*static bool Moni_modset_SPDPU(void)
{
	  uint8_t  rspdbuff;
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	 // gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, POCTRL);
	  gpio_bit_set(GPIOA, POCTRL);
    delay_us(700);
 	  gpio_bit_reset(GPIOA, MSDA);
	  gpio_bit_reset(GPIOA, POCTRL);
	 // microseconds_delay(300);
	  while(!gpio_input_bit_get(GPIOA, MSCL));
	  delay_us(120);
	  gpio_bit_set(GPIOA, MSDA);
	  while(gpio_input_bit_get(GPIOA, MSCL));
	  gpio_bit_reset(GPIOA, MSDA);
	  rspdata_read(DEFAULTCMD,&rspdbuff,8);
	  if(rspdbuff==entmonit_mode_rsp)
		{
			//Cmd_send(c_trace);
			delay_us(200);
			return TRUE;
		}else{
			return FALSE;
		}
}*/
#if defined(MONI_INTMODE_PU)
static uint8_t Moni_int_PUmod(DEV_FTCODE fcode)
{
	  uint8_t  rspdbuff=0;
		gpio_bit_set(GPIOC, POCTRL);
    delay_us(20000);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
		gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, MSCL);
	  gpio_bit_reset(GPIOA, MSDA);
	  delay_us(200000);
	  gpio_bit_reset(GPIOC, POCTRL);
	  timer1_stop();
	timer1_clear_ov();
	TCNT1 =50000;				// ptime is 2ms
	timer1_start_64();
	  while(!gpio_input_bit_get(GPIOA, MSCL)){
			// debug_printf("\r\nt1cntv=%04x\r\n",TCNT_V);
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
				return FALSE;
		  }
		}	
		timer1_stop();
	  timer1_clear_ov();
	  //TCNT1 =60000;				// ptime is 2ms
	  timer1_start_64();
	  while(gpio_input_bit_get(GPIOA, MSCL)){
			//debug_printf("\r\nt1cntv=%04x\r\n",TCNT_V);
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			 return FALSE;
		  }
		}
		if(fcode==NXP_DEV){
			gpio_bit_set(GPIOA, MSDA);
		   delay_us(200);
		}
		 timer1_stop();
		timer1_clear_ov();
	  rspdata_read(DEFAULTCMD,&rspdbuff,8);
	  if(rspdbuff==entmonit_mode_rsp)
		{
			//Cmd_send(c_trace);
			delay_us(200);
			return TRUE;
		}else{
      return FALSE;
    }			
}
static uint8_t Moni_int_modset_PU(DEV_FTCODE fcode)
{
	  uint8_t  rspdbuff=0,t=2;
	  uint8_t  result=TRUE;
while(t--){
	  gpio_bit_set(GPIOC, POCTRL);
	  delay_us(20);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	if(fcode==NXP_DEV/*&&&Rom_partmp.devtype==PCF7952*/){
		 if(Rom_partmp.devtype==PCF7952||Rom_partmp.devtype==PCF7945||Rom_partmp.devtype==PCF7953||Rom_partmp.devtype==PCF7941)
		   gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSCL);
		 if(Rom_partmp.devtype==PCF7922){
			 gpio_init(GPIOA,GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, MSCL);
		 }
	}
	else{
	  gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSCL);
	  gpio_bit_reset(GPIOA, MSCL);
	 }
	  gpio_bit_reset(GPIOA, MSDA);
	  result=TRUE;
	 if(fcode==NXP_DEV&&(Rom_partmp.devtype!=PCF7945&&Rom_partmp.devtype!=PCF7953))
		 delay_us(270000);
		 else if(fcode==GC_DEV||(fcode==NXP_DEV&&(Rom_partmp.devtype==PCF7945||Rom_partmp.devtype==PCF7953)))
    delay_us(3000);
 	  //gpio_bit_reset(GPIOA, MSDA);
	  gpio_bit_reset(GPIOC, POCTRL);
	 if(fcode==GC_DEV/*Rom_partmp.devtype!=PCF7952*/)
	  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSCL);
	 // microseconds_delay(300);
	  timer1_stop();
	timer1_clear_ov();
	TCNT1 =50000;				// ptime is 2ms
	timer1_start_64();
	  while(!gpio_input_bit_get(GPIOA, MSCL)){
			// debug_printf("\r\nt1cntv=%04x\r\n",TCNT_V);
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
				result=FALSE;
			break;
		  }
		}
		if(result==FALSE)continue;
		//if(Rom_partmp.devtype!=PCF7952){
		if(fcode==GC_DEV){
	  delay_us(120);
		}else
		if(fcode==NXP_DEV&&(Rom_partmp.devtype==PCF7945||Rom_partmp.devtype==PCF7953||Rom_partmp.devtype==PCF7952)){
		  delay_us(500);
     gpio_bit_set(GPIOA, MSDA);			
		 }
	  //}		
		//debug_printf("\r\nt1cntv=%04x\r\n",TCNT_V);
		timer1_stop();
	  timer1_clear_ov();
	  //TCNT1 =60000;				// ptime is 2ms
	  timer1_start_64();
	  while(gpio_input_bit_get(GPIOA, MSCL)){
			//debug_printf("\r\nt1cntv=%04x\r\n",TCNT_V);
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  result=FALSE;
					break;
		  }
		}
		if(result==FALSE)continue;
		if(fcode==NXP_DEV&&(Rom_partmp.devtype!=PCF7945&&Rom_partmp.devtype!=PCF7953&&Rom_partmp.devtype!=PCF7952)){
			gpio_bit_set(GPIOA, MSDA);
		   delay_us(200);
		}else if(fcode==GC_DEV||(fcode==NXP_DEV&&(Rom_partmp.devtype==PCF7945||Rom_partmp.devtype==PCF7953||Rom_partmp.devtype==PCF7952))){
		gpio_bit_reset(GPIOA, MSDA);
		}
		 timer1_stop();
	  rspdata_read(DEFAULTCMD,&rspdbuff,8);
	  if(rspdbuff==entmonit_mode_rsp)
		{
			//Cmd_send(c_trace);
			delay_us(200);
			
			result=TRUE;
			break;
		}else{
		result=FALSE;
			continue;
		}		
	}
 return result;
}
#endif
/*static bool Moni_int_modset_PPS(void)
{
	  uint8_t  rspdbuff;
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
    //gpio_bit_set(GPIOA, MSDA);
   // microseconds_delay(5);
	 // delay_us(5);
 	  gpio_bit_reset(GPIOA, MSDA);
	 // microseconds_delay(300);
	  delay_us(300);
	  gpio_bit_set(GPIOA, MSDA);
	  while(gpio_input_bit_get(GPIOA, MSCL));
	  gpio_bit_reset(GPIOA, MSDA);
	  rspdata_read(DEFAULTCMD,&rspdbuff,8);
	  if(rspdbuff==entmonit_mode_rsp)
		{
		//	Cmd_send(c_trace);
			//delay_us(100);
			return TRUE;
		}else{
			return FALSE;
		}
}*/

bool rspdata_read(C_CODE_ENUM cmd,uint8_t* dbuff,uint8_t bits)
{
  uint8_t tdata=0,i;
  uint8_t btmp=bits;
	*dbuff=0;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	gpio_bit_reset(GPIOA, MSDA);
	 // timer1_stop();
	timer1_clear_ov();
	TCNT1 =10000;				// ptime is 1ms
	timer1_start_1080();
	while(!gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
  //while(!gpio_input_bit_get(GPIOA, MSCL));
	timer1_stop();
	timer1_clear_ov();
	TCNT1 = 4000;				// ptime is 50
	timer1_start_8();			
	while(bits--){
		while(gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
		delay_us(1);
		tdata=tdata<<1;
		tdata|=gpio_input_bit_get(GPIOA, MSDA);
		TCNT_V=0;
    while(!gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
		//tdata|=gpio_input_bit_get(GPIOA, MSDA);		
	}
	//bits=btmp;
	TCNT_V=0;
	timer1_clear_ov();
	while(gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
	timer1_stop();
	gpio_bit_set(GPIOA, MSDA);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	//debug_printf("\r\nrspdata=%2x\r\n",tdata);
	//microseconds_delay(8);
	//delay_us(5);
	//gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	//gpio_bit_set(GPIOA, MSDA);
	if(cmd==g_EROMBR){
			bits=btmp;
	  for(i=0;i<bits;i++){
		*dbuff=*dbuff<<1;
		*dbuff|=(tdata>>i)&0x01;
	  }
  }else{
	  *dbuff=tdata;
  }
	delay_us(150);
	return TRUE;
	//debug_printf("\r\nrspdata1=%2x\r\n",*dbuff);
}
static bool GCdata_send(uint8_t data)
{
	uint8_t  i=0;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	//gpio_bit_set(GPIOA, MSDA);
	gpio_bit_reset(GPIOA, MSDA);
	timer1_clear_ov();
	TCNT1 = 4000;				// ptime is 1ms
	timer1_start_8();	
	while(!gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
  	if((data<<0)&0x80){		
			gpio_bit_set(GPIOA, MSDA);
		}else{
			gpio_bit_reset(GPIOA, MSDA);
		}
	timer1_stop();
	timer1_clear_ov();
	TCNT1 = 4000;				// ptime is 50
	timer1_start_8();	
	for(i=1;i<9;i++){
		while(gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
		if(i==8)break;
		delay_us(1);
		if((data<<i)&0x80){
			//delay_us(1);
			gpio_bit_set(GPIOA, MSDA);
		}else{
			//delay_us(1);
			gpio_bit_reset(GPIOA, MSDA);
		}
		TCNT_V=0;
	     while(!gpio_input_bit_get(GPIOA, MSCL)&&(i<8)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
	//delay_us(2);
	//microseconds_delay(2);	
}
	//while(gpio_input_bit_get(GPIOA, MSCL));
   //delay_us(1);
	//microseconds_delay(8);
  timer1_stop();
	gpio_bit_set(GPIOA, MSDA);
  delay_us(160);
  return TRUE;
}
/*static uint32_t chbtsx(uint32_t tmp)
{
	uint32_t tmpd=0;	
	uint8_t *p=NULL;
	p=(uint8_t *)&tmp;
	for(uint8_t i=0;i<4;i++)
	      tmpd=(tmpd<<i*8)|p[i];
  return tmpd;	
}*/
//static Rom_parameter_struct Rom_partmp={0};
static uint32_t randv=0;
static bool EROM_32Write(uint8_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wsize)
{
	uint8_t  rspd=0;
	uint8_t  tmpdata=0;
	if(Cmd_send(cmd)!=TRUE)
		return FALSE;
	if(cmd!=g_b0pglst){
	 if(Cmd_send(pgaddr)!=TRUE)
		return FALSE;
	}
	if((cmd==g_wr_eeprom||cmd==c_wr_eeprom)&&pgaddr==*(Rom_partmp.EEProbuff+511)){
		//uint32_t tmpdata1=0;
		//tmpdata1=*(uint32_t*)wbuff;
		//tmpdata1=((*(uint32_t*)KBSNFLSADR)<<8)+tmpdata1;
		//tmpdata1=chbtsx(tmpdata1);
		switch(Rom_partmp.devtype){
			case(PCF7945):
			randv=((randv&0x0FFFFFFF)|((uint32_t)9<<28));
			break;
			case(PCF7952):
			randv=((randv&0x0FFFFFFF)|(7<<28));
			break;
			case(PCF7953):
			randv=((randv&0x0FFFFFFF)|(3<<28));
			break;
			case(PCF7941):
			randv=((randv&0x0FFFFFFF)|(6<<28));
			break;
			case(PCF7922):
			randv=((randv&0x0FFFFFFF)|((uint32_t)8<<28));
			break;
			default:
      break;
		}			
		wbuff=(uint8_t*)(&randv);
	}
	while(wsize--){
	if(cmd==g_wr_eeprom||cmd==c_wr_eeprom||cmd==g_b0pglst){
		  tmpdata=*wbuff;
	  if(GCdata_send(tmpdata)!=TRUE)
		return FALSE;	
	}else{
		if(wbuff==Rom_partmp.EProbuff+Rom_partmp.EROMsize-1){
			tmpdata=ERDBUGBY;
		}else{
		  tmpdata=*wbuff;
		}
		if(Cmd_send(tmpdata)!=TRUE)
		 return FALSE;
	}
  	wbuff++;	
	}
  if(!rspdata_read(cmd,&rspd,8))
		 return FALSE;
	if(cmd!=g_b0pglst&&(rspd&RRDRSP)){
		return FALSE;
	}
		return TRUE;		
}
static bool rdwddis(void)
{
		timer1_stop();
	timer1_clear_ov();
	TCNT1 = 40000;				// ptime is 50
	timer1_start_8();			
		while(!gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
		while(gpio_input_bit_get(GPIOA, MSCL)){
				if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		  }
		}
		return TRUE;
}
/*static bool setreg(C_CODE_ENUM cmd ,uint8_t regadr,uint8_t data)
{
	 if(Cmd_send(cmd)!=TRUE)
		   return FALSE;
	 if(Cmd_send(regadr)!=TRUE)
		   return FALSE;
	 if(Cmd_send(data)!=TRUE)
		   return FALSE;
	 return TRUE;
}*/
static bool nxpopr(C_CODE_ENUM cmd)
{
	  uint8_t  rspd=0,i;
	  uint8_t C_BYTE[3]={0};
	  if(cmd==c_ee_dump){
	     C_BYTE[0]=C2;
       C_BYTE[1]=C8;
       C_BYTE[2]=C9;}
		if(cmd==c_wr_erom){
	     C_BYTE[0]=C7;
       C_BYTE[1]=C8;
       C_BYTE[2]=C10;}
		if(cmd==c_wr_erom64){
	     C_BYTE[0]=C7;
       C_BYTE[1]=C8;
       C_BYTE[2]=C12;}
     if(cmd==c_ee_dump){		
			if(Cmd_send(c_trace)!=TRUE)
		return FALSE;
			// if(Rom_partmp.devtype==PCF7922){
   if(rdwddis()!=TRUE){
		  return FALSE;
   }
 //}
    rspdata_read(DEFAULTCMD,&rspd,8);
		 if(rspd!=0)
	   return FALSE;
		delay_us(10);
   rspdata_read(DEFAULTCMD,&rspd,8);
				 if(rspd!=0)
	   return FALSE;
			 }				 
		 delay_us(100);
	  for(i=0;i<3;i++){
	   if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
		}
		return TRUE;
}
bool ROM_PRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize,uint8_t pn)
{
	uint16_t tpgsize=0;
	uint8_t  rspd=0;
	uint8_t* tmpadrcnt=wbuff;
	//static uint8_t bcnt=0;
	if(wsize%wpsize){
		tpgsize=wsize/wpsize+1;
	}else{
		tpgsize=wsize/wpsize;
	}
#if defined(MONI_INTMODE_PP)
		//debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
if(cmd==c_wr_erom||cmd==c_wr_erom64){
#if defined(MONI_INTMODE_PU)
		 //debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(NXP_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
 if(Cmd_send(c_trace)!=TRUE)
	  return FALSE;
// if(Rom_partmp.devtype==PCF7922||Rom_partmp.devtype==PCF7941){
   if(rdwddis()!=TRUE){
		  return FALSE;
   }
 //}
  if(rspdata_read(cmd,&rspd,8)!=TRUE)
		  return FALSE;
 if(rspd!=0)
	 return FALSE;
   if(rspdata_read(cmd,&rspd,8)!=TRUE)
		  return FALSE;
  if(rspd!=0)
	 return FALSE;
  if(nxpopr(cmd)!=TRUE)
		  return FALSE;
  //if(Rom_partmp.devtype==PCF7922&&!setreg(c_setreg ,0x27,0x04))
       //	return FALSE;	
}
if(cmd==c_wr_eeprom){
  //bcnt++;
	if(pn==0){
		pgaddr=SPGADR1;
		tpgsize=EEROM_BKSIZE/wpsize-3;
	}else if(pn>0){
		//pgaddr=pgaddr+(bcnt-1)*EEROM_BKSIZE/wpsize;
    tpgsize=pgaddr+tpgsize;
         //tmpgadr=pgaddr;
    //if(bcnt==2)	
	     // bcnt=0;		
	 }
 }	
	  //debug_printf("\r\nbegin program");
	for(;pgaddr<tpgsize;pgaddr++)
	{
		 wbuff=tmpadrcnt;
		 wbuff+=pgaddr*wpsize;
		if(EROM_32Write(pgaddr,cmd,wbuff,wpsize)==FALSE){
			return FALSE;
		}
	}
 if(pn==0&&cmd==c_wr_eeprom){
	   //cmd=c__prog_config;
	    //bcnt=0;
	    pgaddr=127;
	   //wbuff=tmpadrcnt;
	   wbuff=tmpadrcnt+pgaddr*wpsize+2;	 
		  tpgsize=*wbuff;
	  if(Cmd_send(c__prog_config)!=TRUE)
		return FALSE; 
	  if(GCdata_send(tpgsize)!=TRUE)
		return FALSE;	
     tpgsize=*(wbuff+1);
    if(GCdata_send(tpgsize)!=TRUE)
		return FALSE;	
    rspdata_read(cmd,&rspd,8);
	if(rspd&RRDRSP){
		return FALSE;
	} 
 }	 
	return TRUE;
}
static void zdsend(uint8_t*d,uint8_t bsz)
{
	gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSCL|MSDA);
	 for(uint8_t i=0;i<bsz;i++){
		 for(uint8_t j=0;j<8;j++){
	 gpio_bit_set(GPIOA, MSCL);
	 if(d[i]&0x01){
			 gpio_bit_set(GPIOA, MSDA); 
	   }else{
			 gpio_bit_reset(GPIOA, MSDA);
		 }
		 delay_us(6);
		gpio_bit_reset(GPIOA, MSCL);
		  d[i]=d[i]>>1;
     delay_us(6);
	 }
		if(bsz==1){
			gpio_bit_set(GPIOA, MSCL);
			delay_us(6);
			gpio_bit_reset(GPIOA, MSCL);
			delay_us(6);
		}
    delay_us(20);		 
	}
  gpio_bit_reset(GPIOA, MSCL);
	delay_us(2);
  gpio_bit_reset(GPIOA, MSDA);	
}
uint8_t ROM_ER(NXP_DEVTPE_ENUM devtype,C_CODE_ENUM cmd)
{
	uint8_t  rspd=0;
	uint8_t  i;
	uint8_t ER_BYTE[16]={0x55,0x45,0xE8,0x92,
                       0xD6,0xB1,0x62,0x59,
                       0xFC,0x8A,0xC8,0xF2,
                       0xD6,0xE1,0x4A,0x35  };

	//	if(gpio_input_bit_get(GPIOA, MSCL)){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
				if((cmd==c_er_erom&&Moni_int_modset_PU(NXP_DEV)!=TRUE)||
					(cmd==g_er_erom&&Moni_int_modset_PU(GC_DEV)!=TRUE)){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return 2;
		}
#endif		
//	 }
	// debug_printf("\r\nEnter downld mode\r\n");
	if((cmd==g_er_erom&&Cmd_send(cmd)!=TRUE)||(cmd==c_er_erom&&Cmd_send(cmd)!=TRUE))
		return FALSE;
	if(devtype==0xA1||devtype==0xA5||devtype==0xA3){
    //if(cmd==g_er_erom)	
	    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		  return FALSE;
		//if(cmd==c_er_erom)
		//nxp_rspdata_read(cmd,&rspd,8);	
	if(rspd!=ERCRSP){
		return FALSE;
	  }
 }
	//Cmd_send(c_trace);
	if(devtype==0xA1||devtype==0xA5||devtype==0xA3){
	delay_us(80);
	for(i=0;i<16;i++){
	   if(Cmd_send(ER_BYTE[i])!=TRUE)
		  return FALSE;
   }
 }
	   if(rspdata_read(cmd,&rspd,8)!=TRUE)
		  return FALSE;
		if(rspd&RRDRSP){
		return FALSE;
	}
	gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSCL);
	gpio_bit_reset(GPIOA, MSCL);		
	gpio_bit_set(GPIOC, POCTRL);
	;
	gpio_bit_reset(GPIOA, MSDA);
if(cmd==g_er_erom)	
	delay_us(4600);
  if(cmd==c_er_erom){
		delay_us(2600);
		ER_BYTE[0]=0x8A;
		ER_BYTE[1]=0xC8;
		ER_BYTE[2]=0x56;
		zdsend(ER_BYTE,3);
		delay_us(200);
		ER_BYTE[0]=0x2F;
		zdsend(ER_BYTE,1);
  }		
		return TRUE; 
}
static bool Gce8(void)
{

	uint8_t  i;
	uint8_t C_BYTE[3]={C1,C2,C3};
	uint8_t C2_BYTE[3]={C1,C7,C8};
	 	for(i=0;i<3;i++){
	if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
   }
	 	for(i=0;i<3;i++){
	if(Cmd_send(C2_BYTE[i])!=TRUE)
		return FALSE;
  }
		return TRUE;
}
bool ROM_ER2(C_CODE_ENUM cmd)
{
	uint8_t  rspd=0;
	uint8_t  i;
	uint8_t ER_BYTE[16]={0x55,0x45,0xE8,0x92,
                       0xD6,0xB1,0x62,0x59,
                       0xFC,0x8A,0xC8,0xF2,
                       0xD6,0xE1,0x4A,0x35  };
	//uint8_t C_BYTE[3]={C1,C2,C3};
	//uint8_t C2_BYTE[3]={C1,C7,C8};
	//	if(gpio_input_bit_get(GPIOA, MSCL)){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
	// debug_printf("\r\nEnter downld mode\r\n");
	if(Gce8()!=TRUE)
		return FALSE;	
	if(Cmd_send(cmd)!=TRUE)
		return FALSE;		
	rspdata_read(cmd,&rspd,8);
	if(rspd!=ERCRSP){
		return FALSE;
	}
	//Cmd_send(c_trace);
		delay_us(80);
	for(i=0;i<16;i++){
	if(Cmd_send(ER_BYTE[i])!=TRUE)
		return FALSE;
  }
	rspdata_read(cmd,&rspd,8);
		if(rspd&RRDRSP){
		return FALSE;
	}
		return TRUE; 
}
bool GCROM_PRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize)
{
	uint16_t tpgsize=0,i,j,ptimes=0,tmpgadr=0;
	uint8_t  rspd=0;
	uint8_t C_BYTE[3]={C1,C4,C6};
   uint8_t* tmpadrcnt=wbuff;
  static uint8_t bcnt=0;
if(cmd==g_wr_eeprom){
  bcnt++;
}
	if(wsize%wpsize){
		tpgsize=wsize/wpsize+1;
	}else{
		tpgsize=wsize/wpsize;
	}
		if(wsize%(PGWSZ)){
		ptimes=wsize/(PGWSZ)+1;
	}else{
		ptimes=wsize/(PGWSZ);
	}
	if(bcnt==1){
		pgaddr=SPGADR1;
		if(wsize/EEROM_BKSIZE<1){
			tpgsize=tpgsize-3;
		}else{
		  tpgsize=EEROM_BKSIZE/wpsize-3;
		}
	}else if(bcnt>1){
		//pgaddr=pgaddr+(bcnt-1)*EEROM_BKSIZE/wpsize;
    tpgsize=pgaddr+tpgsize;
         tmpgadr=pgaddr;
    if(cmd==g_wr_eeprom&&bcnt==2)	
	      bcnt=0;		
	}
	//	if(gpio_input_bit_get(GPIOA, MSCL)){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
		if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;			
	  debug_printf("\r\nbegin program");
		
		//delay_us(20000);
		for(j=0;j<ptimes;j++){
	  for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
	for(;pgaddr<tpgsize;pgaddr++)
	{
			wbuff=tmpadrcnt;
		  wbuff+=pgaddr*wpsize;
		if(EROM_32Write(pgaddr,cmd,wbuff,wpsize)==FALSE){
			return FALSE;
		}else{
			//if(pgaddr%2==0){
				//debug_printf(".");
			//}
			if(cmd==g_wr_eeprom&&((pgaddr+1-tmpgadr)%48==0)){
				 pgaddr++;
				break;
			}
			 if(cmd==g_wr_erom&&((pgaddr+1)%3==0)){
				 pgaddr++;
				break;
			}
			if(cmd==g_crty_wr&&(pgaddr+1)%6==0){
				pgaddr++;
				break;
			}
		}			
	 }
 }
	return TRUE;
}
bool GCROM_LBPRO(uint16_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize,uint16_t wsize)
{
	uint16_t tpgsize=0,i,j,ptimes=0,tmpgadr=0;
	uint8_t  rspd=0;
	uint8_t C_BYTE[3]={C1,C4,C6};
   uint8_t* tmpadrcnt=wbuff;
	if(wsize%wpsize){
		tpgsize=wsize/wpsize+1;
	}else{
		tpgsize=wsize/wpsize;
	}
		if(wsize%(PGWSZ)){
		ptimes=wsize/(PGWSZ)+1;
	}else{
		ptimes=wsize/(PGWSZ);
	}	
	//	if(gpio_input_bit_get(GPIOA, MSCL)){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
		if(Gce8()!=TRUE)
		return FALSE;	
		delay_us(100);
		if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;			
	  debug_printf("\r\nbegin program");
		
		delay_us(20000);
		for(j=0;j<ptimes;j++){
	  for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
	for(;pgaddr<tpgsize;pgaddr++)
	{
		  wbuff=tmpadrcnt;
		  wbuff+=pgaddr*wpsize;
		if(EROM_32Write(pgaddr,cmd,wbuff,wpsize)==FALSE){
			return FALSE;
		}else{
			//if(pgaddr%2==0){
				//debug_printf(".");
			//}
			if(cmd==g_wr_eeprom&&((pgaddr+1-tmpgadr)%48==0)){
				 pgaddr++;
				break;
			}
			 if(cmd==g_wr_erom&&((pgaddr+1)%3==0)){
				 pgaddr++;
				break;
			}
			if(cmd==g_crty_wr&&(pgaddr+1)%6==0){
				pgaddr++;
				break;
			}
		}			
	 }
 }
	return TRUE;
}
static bool Gsopr(void)
{
	  uint8_t C_BYTE[3]={C1,C4,C6};
    uint8_t  rspd=0,i;
			if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
    if(rspdata_read(DEFAULTCMD,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(DEFAULTCMD,&rspd,8)!=TRUE)
		return FALSE;		
		delay_us(10000);
	  for(i=0;i<3;i++){
	   if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
		}
		return TRUE;
}
static  bool Gcsetprc(void)
{
	uint8_t  rspd=0;
	#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
		if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
		if(rspdata_read(DEFAULTCMD,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(DEFAULTCMD,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(10000);
		if(Cmd_send(g_cry)!=TRUE)
		return FALSE;
		rspdata_read(g_cry,&rspd,8);
		if(rspd==0xF4){
			return TRUE;
		}else{
			return FALSE;
		}
}
bool GCERBIN_RDTOOUT(C_CODE_ENUM cmd,uint16_t eraddr,uint16_t bsz,uint8_t gsz)
{
	uint8_t /*tdata=0,i=0,*/tcn=0;
	uint8_t dbuff[3072+207]={0};
  uint8_t btmp=8;
  uint16_t k=0;
	uint8_t  mg=16;
	uint8_t  lg=6;
	uint8_t  bytes=37;
	 tcn=3;
	while(!gpio_input_bit_get(GPIOA, MSDA)||tcn--);
	 gpio_bit_reset(GPIOC, POCTRL);
	 ;
	 gpio_bit_set(GPIOC, POCTRL);
	 delay_us(6854809);  //6873440 
	//while(gpio_input_bit_get(GPIOA, MSDA));
	 gpio_bit_reset(GPIOC, POCTRL);
	 ;
	 gpio_bit_set(GPIOC, POCTRL);
	 tcn=2;
	 while(!gpio_input_bit_get(GPIOA, MSCL)||tcn--);
	while(mg--){
	while(lg--){
		//while(!gpio_input_bit_get(GPIOA, MSCL));
     //delay_us(194);
	while(bytes--){
		//while(!gpio_input_bit_get(GPIOA, MSCL));
	while(btmp--){
		//tcn=1;
		while(!gpio_input_bit_get(GPIOA, MSCL));
     delay_us(2);
		dbuff[k]=(gpio_input_bit_get(GPIOA, MSDA)<<7)|(dbuff[k]>>1);
		//dbuff[k]|=gpio_input_bit_get(GPIOA, MSDA);
    while(gpio_input_bit_get(GPIOA, MSCL));
		//delay_us(1);
		//tdata|=gpio_input_bit_get(GPIOA, MSDA);		
	   }
	/*if(TRUE){
			btmp=8;
	  for(i=0;i<btmp;i++){
		dbuff[k]=dbuff[k]<<1;
		dbuff[k]|=(tdata>>i)&0x01;
	  }
  }else{
	  dbuff[k]=tdata;
  }*/
	k++;
	  //debug_printf("%2x ",dbuff);
	   	btmp=8;
   }
	// debug_printf("\n");
	 bytes=34;
	 while(!gpio_input_bit_get(GPIOA, MSCL));
    delay_us(194);
  }
	 lg=6;
	 bytes=37;
	//while(!gpio_input_bit_get(GPIOA, MSCL));
		 	 gpio_bit_reset(GPIOC, POCTRL);
	 ;
	 gpio_bit_set(GPIOC, POCTRL);
	 delay_us(17929);
  }
 return TRUE;
}
/*
static bool GCERBIN_RDTOFLS(C_CODE_ENUM cmd,uint16_t eraddr,uint32_t flsaddr,uint16_t bsz)
{
	uint8_t  rspd=0,i;
	uint8_t C_BYTE[3]={C1,C4,C6};
	uint32_t tmpdata=0;
	#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	Gce8( );
	Cmd_send(g_trace);
	while(j--){		
		  for(i=0;i<3;i++)
	  Cmd_send(C_BYTE[i]);
	//Cmd_send(g_trace);	
	delay_us(100);
	while(i--){	
	for(i=0;i++)
	Cmd_send(cmd);
  Cmd_send(eraddr%256);
	Cmd_send(eraddr/256);
  rspdata_read(cmd,&rspd,8);
  *((uint8_t*)(&tmpdata))=rspd;
	}	
 }	
	*(uint8_t*)flsaddr=rspd;	*/
	/*fmc_word_program(uint32_t address, uint32_t data)	
  		
}
*/
uint8_t GCEROM_RD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint16_t raddr)
{
	uint8_t  rspd=0,i;
	uint8_t C_BYTE[3]={C1,C4,C6};
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
		if(cmd0==g_wr_erom){
    if(Gsopr()!=TRUE)
		return FALSE;	
		}else if(cmd0==g_crty_wr){
					if(Gce8()!=TRUE)
		return FALSE;	
		delay_us(100);
		if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;					
		delay_us(20000);
			  for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
		}
		if(Cmd_send(cmd)!=TRUE)
		return FALSE;
		if(Cmd_send(raddr%256)!=TRUE)
		return FALSE;
		if(Cmd_send(raddr/256)!=TRUE)
		return FALSE;
		if(rspdata_read(g_EROMBR,&rspd,8)!=TRUE)
		return FALSE;	
	  return(rspd);
}
bool GCEROM_CKRD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint8_t bsz)
{
	#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif	
	  uint8_t  rspd=0,i;
	  uint8_t C_BYTE[3]={C1,C4,C6};
    if(cmd0==g_wr_erom){
    for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
		}else if(cmd0==g_er_erom){
			/*if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}*/
		if(Cmd_send(g_trace)!=TRUE)
		return FALSE;
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;	
		delay_us(80);
    if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;					
		//delay_us(20000);		
		}
		if(Cmd_send(cmd)!=TRUE)
		return FALSE;
    while(bsz--){
		if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;	
		}			
	  return(TRUE);
}
uint8_t fxorder(uint8_t tmd,uint8_t bits)
{
	 uint8_t fxv=0,i=0;
	 while(bits--){ 
   fxv|=((tmd>>i)&0x01)<<bits;
	 i++;
	 }
   return fxv;
}
bool static EROMB_RD(C_CODE_ENUM cmd,uint8_t *wbuff)
{
  uint16_t adr[3]={0x00,0x04,0x09};	
	uint8_t i=0,rspd=0;
	for(i=0;i<3;i++)
	{
		if(!Cmd_send(cmd))
			 return FALSE;
		if(!Cmd_send((adr[i])&0xFF))
			 return FALSE;
		if(!Cmd_send((adr[i]>>8)&0xFF))
			 return FALSE;
		if(!rspdata_read(cmd,&rspd,8))
			 return FALSE;
		if(rspd!=fxorder(*(wbuff+adr[i]),8))
			return FALSE;
	}
 return TRUE;	
}
bool GCEEROM_RD(C_CODE_ENUM cmd0,C_CODE_ENUM cmd,uint8_t *wbuff,uint16_t bytes,uint16_t rlbytes)
{
	uint8_t  rspd=0,i;
	uint16_t  tmpsz=rlbytes;
	uint16_t  tmpEERsz=bytes;
	uint8_t C_BYTE[3]={C1,C4,C6};
if(cmd0==g_wr_eeprom){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
   if(Gsopr()!=TRUE)
		return FALSE;	
	}else if(cmd0==g_er_erom){
		    for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
	}
	if(Rom_partmp.devtype==PCF7922&&cmd==c_er_dump){
		if(!EROMB_RD(c_erm_rdb,wbuff))
			  return FALSE;
	}else{
		if(Cmd_send(cmd)!=TRUE)
		return FALSE;
		while(bytes--){
		  rspdata_read(cmd,&rspd,8);
			if(cmd0==g_wr_eeprom){				
      if(((bytes==(tmpEERsz-tmpsz))&&rspd!=*(wbuff+tmpsz-1))||
				(bytes==(tmpEERsz-tmpsz/2)&&rspd!=*(wbuff+tmpsz/2-1))||
			  (bytes==(tmpEERsz-1)&&rspd!=*(wbuff))){
					 return FALSE;
			}
		}else if(cmd0==c_wr_eeprom){
			      if(((bytes==(tmpEERsz-tmpsz))&&rspd!=*(wbuff+tmpsz-1))||
				(bytes==(tmpEERsz-tmpsz/2)&&rspd!=*(wbuff+tmpsz/2-1))||
			  (bytes==(tmpEERsz-8)&&rspd!=*(wbuff+7))){
					 return FALSE;
				}
		}else if(cmd0==g_er_erom){
			      if(((bytes==(tmpEERsz-tmpsz))&&rspd!=0xFF)||
				(bytes==(tmpEERsz-tmpsz/2)&&rspd!=0xFF)){
					 return FALSE;
			}
		 }else if(cmd0==c_wr_erom){					 
        if(((bytes==(tmpEERsz-tmpsz))&&rspd!=fxorder(*(wbuff+tmpsz-1),8))||
				(bytes==(tmpEERsz-tmpsz/2)&&rspd!=fxorder(*(wbuff+tmpsz/2-1),8))||
			  (bytes==(tmpEERsz-1)&&rspd!=fxorder(*(wbuff),8))){
					 return FALSE;
			  } 
		 }			 
	}
 }		
	  return TRUE;
}
bool GCEEROM_RD1(C_CODE_ENUM cmd,uint16_t bytes)
{
	uint8_t  rspd=0;
	//uint8_t C_BYTE[3]={C1,C4,C6};
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
    if(Gsopr()!=TRUE)
		return FALSE;	
		if(Cmd_send(cmd)!=TRUE)
		return FALSE;
		while(bytes--){
		  if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;					
		}		
	  return TRUE;
}
/*
bool Gcgetchk_devtype(uint8_t *wbuff)
{
	  uint8_t tmsn=0;
	  tmsn=*(wbuff+EEROM_BKSIZE-1);
}*/
bool GCCryt(C_CODE_ENUM cmd)
{
	uint8_t  rspd=0;
	uint8_t  i;
	//	if(gpio_input_bit_get(GPIOA, MSCL)){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
	// debug_printf("\r\nEnter downld mode\r\n");
   if(Gce8()!=TRUE)
		return FALSE;	
	 Gsopr();
	if(Cmd_send(cmd)!=TRUE)
		return FALSE;		
	for(i=0;i<3;i++){
	if(rspdata_read(cmd,&rspd,8)!=TRUE)
		return FALSE;
}	
  if(Gcsetprc()!=TRUE){
		return FALSE;
	}else{
		return TRUE;
  }		
}
bool GCEEROMPG0_PRO(uint8_t pgaddr,C_CODE_ENUM cmd,uint8_t *wbuff,uint8_t wpsize)
{
	uint8_t i;
	//uint8_t  rspd=0;
	//uint8_t *tmp=wbuff;
	uint8_t C_BYTE[3]={C1,C4,C6};	
	  for(i=0;i<3;i++){
	  if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	}
    if(cmd==g_b0pglst){
    wbuff=wbuff+pgaddr*wpsize+2;
			//*tmp=ID2;
			//*(tmp+1)=0x00;
			wpsize=wpsize-2;
		}else{
			wbuff=wbuff+pgaddr*wpsize;
			//*(tmp+3)=(*(tmp+3)&0xF0)|ID1;			
		}
    delay_us(150);
		if(EROM_32Write(pgaddr,cmd,wbuff,wpsize)==FALSE){
			return FALSE;
		}else{
		delay_us(5000);
	   return TRUE;
		}			
}
bool GCdecryt(void)
{
	uint8_t  /*rspd=0,*/i;
	uint8_t C_BYTE[3]={C9,C10,C11};
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if(Moni_int_modset_PU(GC_DEV)!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
	  for(i=0;i<3;i++){
	   if(Cmd_send(C_BYTE[i])!=TRUE)
		return FALSE;
	 }		 
    delay_us(2000);
		//gpio_bit_reset(GPIOA, MSDA);
		//gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
   	return TRUE;	
}
static bool  NXPWP(void)
{
	uint8_t rspd=0;
	if(Cmd_send(c_protect)!=TRUE)
		return FALSE;
	if(!rspdata_read(c_protect,&rspd,8))
		      return FALSE;
	//if(rspd==0xff)
		//return FALSE;
	 if(rspd&RRDRSP)
		return FALSE;
	return TRUE;	
}
/*
static uint8_t devmod=0xA1;
static NXP_DEVTPE_ENUM get_dev_type(NXP_DEVTPE_ENUM devtype)
{
	if(devmod==0xA1){
	devtype=PCF7945;	
	}else if(devmod==0xA2){
	devtype=PCF7952;	
	}else if(devmod==0xA3){
	devtype=PCF7953;	
	}else{
		debug_printf("\r\n dev type error!\r\n");
	}
	return devtype;
}*/
/*
uint8_t * Pro_dl_recv(uint16_t recbytes){
	uint8_t* tmpp=NULL;
	//uint16_t tmpbytes=0;
	//uint8_t  i=0;
	uint8_t dataarry[]={0x1B,0x80, 0x00, 0xED, 0x00, 0xED, 0x65, 0x81, 0x80 ,0x84 ,0x00, 0xED, 0x70, 0x84, 0x00, 0xED,
                    0xF5, 0x8F, 0xA6, 0xC5, 0xA6, 0xC5, 0xA1, 0x81, 0xE5, 0x81, 0xA3, 0x81, 0xDF, 0x81, 0xDC, 0x81,
                    0xA0, 0x81, 0xA6, 0xC5, 0x00, 0x26, 0xF7, 0x46, 0x00, 0x08, 0x4B, 0xC0, 0x41, 0xC0, 0x45, 0x08,
                    0x47, 0x00, 0x07, 0x40, 0x20, 0x20, 0x13, 0xE6, 0x00, 0xED, 0x05, 0x78, 0xA7, 0x70, 0x00, 0x78};
	//tmpbytes=recbytes+4;
	 tmpp=(uint8_t*)malloc(64);								
   memcpy(tmpp,dataarry,64);
	debug_printf("\r\n%02x ",tmpp[0]);
  debug_printf(" %02x ",tmpp[1]);
  debug_printf(" %02x ",tmpp[63]);										
	*tmpp=dlrecvbuff;*/
	 /*  while(tmpbytes--){
		 while(RESET == usart_flag_get(USART0, USART_FLAG_RBNE));//{
    *tmpp=usart_data_receive(USART0);
			 tmpp++;
       i++;			 
			 debug_printf("%x ",*tmpp);
			 if(i%16)
			debug_printf("\n"); 
	 //}
 }
 tmpp=dlrecvbuff;
 if(checksum(tmpp,recbytes)==(uint32_t)tmpp[recbytes]){
  return TRUE;
 }else{
	debug_printf("\r\ndl recvdata vertify error!\r\n"); 
	 //free(dlrecvbuff);
	  return FALSE;
 }*/
	 //return(tmpp);
//}	
fmc_state_enum GD32FMC_OB_Toggle(uint8_t ob_spc)
{
	fmc_state_enum res = FMC_BUSY;
	fmc_unlock();	
	ob_unlock();	
	
	ob_erase();		
	res = ob_security_protection_config(ob_spc);	
	ob_lock();		
	fmc_lock();			
	return res;
}
void swd_cls(void)
{
	//rcu_periph_clock_enable(RCU_GPIOA);
	//rcu_periph_clock_enable(RCU_AF);
	//gpio_init(GPIOA,GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	//gpio_init(GPIOA,GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
	//gpio_pin_remap_config(GPIO_SWJ_DISABLE_REMAP, ENABLE);
	GD32FMC_OB_Toggle(FMC_USPC);
}
bool Rom_parameter_cofg(DEV_FTCODE fcode,NXP_DEVTPE_ENUM devtype,uint32_t ERdlflsadr,uint32_t EERdlflsadr)
{
	  //uint8_t tmdata0=0,tmdata1=0;
      //swd_cls(); 
	/*	if(fcode!=GC_DEV&&fcode!=NXP_DEV){
			info_printf("\r\ndev factcode error!\r\n");
			return FALSE;
		}
		if(devtype!=PCF7945&&devtype!=PCF7952&&devtype!=PCF7953){
			info_printf("\r\ndev type error!\r\n");
			return FALSE;
		}*/
	if(devtype!=OTHERDEV){
		Rom_partmp.fctcode=fcode;
		Rom_partmp.EEProbuff=(uint8_t*)EERdlflsadr;
		Rom_partmp.EProbuff=(uint8_t*)ERdlflsadr;
	}
	Rom_partmp.devtype=devtype;
		//Rom_partmp.EProbuff=(uint8_t*)ERdlflsadr;		
	switch(Rom_partmp.devtype){
	  case(PCF7945):
		case(PCF7953):{
				 Rom_partmp.Epgsize=GEROM_PGSIZE;
         Rom_partmp.EEpgsize=GEEROM_PGSIZE;
			if(Rom_partmp.fctcode==NXP_DEV){
		     Rom_partmp.Epgsize=EROM_PGSIZE;
         Rom_partmp.EEpgsize=EEROM_PGSIZE;
		    }			
         Rom_partmp.EROMsize=PCF7945_E_ROM_SZ;
         Rom_partmp.EEROMsize=PCF7945_EE_ROM_SZ;			
        }		  
		  break;
		case(PCF7941):
	  case(PCF7952):{
		  	 Rom_partmp.Epgsize=GEROM_PGSIZE;
         Rom_partmp.EEpgsize=GEEROM_PGSIZE;
			  if(Rom_partmp.fctcode==NXP_DEV){
		       Rom_partmp.Epgsize=EROM_PGSIZE;
           Rom_partmp.EEpgsize=EEROM_PGSIZE;
		    }				
         Rom_partmp.EROMsize=PCF7952_E_ROM_SZ;
         Rom_partmp.EEROMsize=PCF7952_EE_ROM_SZ;	
	     }
		  break;
			case(PCF7922):{
		       Rom_partmp.Epgsize=GEROM_PGSIZE;
           Rom_partmp.EEpgsize=EEROM_PGSIZE;			
           Rom_partmp.EROMsize=PCF7945_E_ROM_SZ;
           Rom_partmp.EEROMsize=PCF7952_EE_ROM_SZ;	
	     }
		  break;
		case(OTHERDEV):{
					Rom_partmp.EROMsize=ERdlflsadr;
			    Rom_partmp.EProbuff=(uint8_t*)EERdlflsadr;
         // Rom_partmp.EROMsize=EERdlflsadr;			
				 }
		break;
	  default:{
			  info_printf("\r\ndev type error!\r\n");
		    return FALSE;
	     }
		}
        /* capture error */
		
			//			if(tmdata0=GCEROM_RD(g_crty_wr,g_EROMBR,KEY_CRTY_FLSADR)!=0x00&&GCEROM_RD(tmdata1=g_crty_wr,g_EROMBR,KEY_CRTY_EDFLSADR)!=0x00
	 /*
	 	   tp=(uint8_t*)(FLSSTCKSADR+1);    					
	  if((Rom_partmp.fctcode==GC_DEV&&GCROM_LBPRO(KEY_STCKSADR+16,g_crty_wr,tp,4,4)!=TRUE)){
			debug_printf("\r\ncrty_wr fail!\r\n");
				//free(Rom_partmp.EProbuff);
       return FALSE;					
		}
					if((erstics=GCEROM_RD(g_crty_wr,g_EROMBR,KEY_STCKSADR,4))!=0x00
		   ){
         debug_printf("\r\nerrdtick=%04x,\r\n",erstics);
      }	
if((Rom_partmp.desticstat=stic_decry(erstics))!=TRUE){		
	while(i--){
		 microseconds_init( );
	tmpticks=microseconds_get_ticks();
		   if(i==4)tmp=tmpticks<<8;
		   if(i==3)tmp|=(tmpticks&0xff00)>>8;
		   if(i==2)tmp|=(tmpticks&0xff00)<<8;
		   if(i==1)tmp|=(tmpticks&0xff00)<<16;
	  if(i==4) microseconds_delay(100);//delay_us(100); 
	  if(i==3) microseconds_delay(115);//delay_us(108);
	  if(i==2) microseconds_delay(121);//delay_us(116);
 }
 */
 //if (SysTick_Config(SystemCoreClock /Frequency_1MHz)){
        /* capture error */
	 //info_printf("\r\nsystics set error!\r\n");
       // while (1){
      // }
   //} 
	/*
debug_printf("\r\nstics=%04x\r\n",tmp);
 tmp=(tmp)^(CRYK);
 while(fmc_word_program(FLSSTCKSADR, tmp)!=FMC_READY&&timeout--){
	 if(timeout==0){
		 return FALSE;
	 }
 }
 while(fmc_word_program(FLSSTCKSADR+1,MD)!=FMC_READY&&timeout--){
		 if(timeout==0){
		 return FALSE;
	 }
 }
 }*/
		return TRUE;
}
/*static bool clk_rd(void)
{
	uint8_t  i=0,j=0;
  timer1_stop();
	timer1_clear_ov();
	TCNT1 = 40000;				// ptime is 1ms
	timer1_start_8();
	for(j=0;j<3;j++){
	for(i=0;i<8;i++){
	  while(!gpio_input_bit_get(GPIOA, MSCL)){
		   if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		    }			
	    }
    delay_us(15);		
	  while(gpio_input_bit_get(GPIOA, MSCL)){
		   if(timer1_ov_flag == 1){
			  timer1_stop();
	      timer1_clear_ov();
			  return FALSE;
		    }			
	    }
    delay_us(15);			
	  }
	}
	if(j==3){
		 gpio_bit_reset(GPIOA, MSCL);
		 gpio_init(GPIOA,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSCL);
	}
	return TRUE;
}*/
static  uint32_t randnum(void)
{
	 uint32_t stics=0;
	 stics=SysTick->VAL;
   srand(stics);
   return(rand());		
}
bool Rom_Prgram_OP(DEV_PRGCMD cmd)
{
  uint8_t  Pts=0,i=0;
	uint8_t* crtyfls=NULL;
	uint8_t* crtyflsed=NULL;
  uint16_t  tmppsz=0,pgaddr=0;
    if(Rom_partmp.EEProbuff==NULL||Rom_partmp.EProbuff==NULL){
			  return FALSE;
		}
		
  switch(cmd){
	  case(0xE):{
			        // info_printf("\r\nBegin erase...\r\n");
					if(ROM_ER(Rom_partmp.devtype,g_er_erom)!=TRUE){
								debug_printf("\r\n1Erase fail!\r\n");
						    return FALSE;
						}
		      if(Rom_partmp.fctcode==GC_DEV&&ROM_ER2(g_er_erom)!=TRUE){
		          debug_printf("\r\nErase fail!\r\n");
                  return FALSE;						
		     }
				if(GCEROM_CKRD(g_er_erom,g_EROMVCK,3)!=TRUE){
					return FALSE;
				 }else{
				if(GCEEROM_RD(g_er_erom,g_EEROMBR,Rom_partmp.EEProbuff,Rom_partmp.EEROMsize,10)==TRUE){
			    debug_printf("\r\nEEROM Erase OK!\r\n");
			  }else{
					debug_printf("\r\nEEROM Erase fail!\r\n");
					 return FALSE;
				}
				if(GCEROM_RD(g_crty_wr,g_EROMBR,KEY_CRTY_FLSADR)==0x00&&GCEROM_RD(g_crty_wr,g_EROMBR,KEY_CRTY_EDFLSADR)==0x00
				){
			  debug_printf("\r\nfls erase OK!\r\n");
			}else{debug_printf("\r\nfls erase fail!\r\n");;
				   return FALSE;
        }
				 }
				 //return TRUE;
		      //debug_printf("\r\nErase done!\r\n");

	      }
			break;
		case(0xB):{
					// info_printf("\r\nBegin programming...\r\n");
			     if(Rom_partmp.fctcode==GC_DEV){
			      if(ROM_ER(Rom_partmp.devtype,g_er_erom)!=TRUE){
								debug_printf("\r\nbErase fail!\r\n");
							  return FALSE;
						}
						if(GCdecryt( )!=TRUE){
							debug_printf("\r\ndecryt fail!\r\n");
							return FALSE;
						}
						if(GCEEROM_RD1(g_EEROMBR,Rom_partmp.EEROMsize)!=TRUE){
							debug_printf("\r\nEEROM read fail!\r\n");
							return FALSE;
						}
				  if(GCEEROMPG0_PRO(EEPRADR,g_wr_eeprom,Rom_partmp.EEProbuff,EEROM_PGSIZE)!=TRUE){
						 debug_printf("\r\nEEROM PG0_w fail!\r\n");
						return FALSE;
					}
					if(GCEEROMPG0_PRO(EE0PGl,g_b0pglst,Rom_partmp.EEProbuff,EEROM_PGSIZE)!=TRUE){
						 debug_printf("\r\nEEROM PG127_w fail!\r\n");
						return FALSE;
					}	
						if(ROM_ER2(g_er_erom)!=TRUE){
								debug_printf("\r\neErase fail!\r\n");
						     return FALSE;
						}
					}else if(Rom_partmp.fctcode==NXP_DEV){
						  if(Rom_partmp.devtype==PCF7922){
						   if(Moni_int_PUmod(Rom_partmp.fctcode)!=TRUE){
								    return FALSE;
								 }else{
									 //if(Cmd_send(0x00)!=TRUE)
									  gpio_bit_reset(GPIOA, MSDA);
										//if(clk_rd( )!=TRUE)
											//return FALSE;
									    delay_us(50000);
										//else
											//return FALSE;
								 }
							 }
							if(ROM_ER(Rom_partmp.devtype,c_er_erom)!=TRUE){
								debug_printf("\r\nnxErase fail!\r\n");
							  return FALSE;
						}
					}					 
					/* if((Rom_partmp.fctcode==NXP_DEV&&ROM_ER(c_er_erom)!=TRUE)||(Rom_partmp.fctcode==GC_DEV&&ROM_ER2(g_er_erom)!=TRUE)){
		       debug_printf("\r\nfErase fail!\r\n");
           return FALSE;				 
		     }else{
		      debug_printf("\r\nfErase done!\r\n");
				 }*/
			   debug_printf("\r\nEROM burn begin\r\n");
			   //debug_printf("\r\n%02x!\r\n",*(Rom_partmp.EProbuff+Rom_partmp.EROMsize-1));
				 if(Rom_partmp.fctcode==GC_DEV&&Rom_partmp.devtype==PCF7953){
				 crtyfls=(uint8_t*)BIN_EFL_START_ADDR;
				 crtyflsed=(uint8_t*)(BIN_CRTY_MDFLSADR);
				 }
	  if((Rom_partmp.fctcode==NXP_DEV&&((Rom_partmp.devtype==PCF7922&&ROM_PRO(EPRADR,c_wr_erom64,Rom_partmp.EProbuff,Rom_partmp.Epgsize,Rom_partmp.EROMsize,0)!=TRUE)
			||(Rom_partmp.devtype!=PCF7922&&ROM_PRO(EPRADR,c_wr_erom,Rom_partmp.EProbuff,Rom_partmp.Epgsize,Rom_partmp.EROMsize,0)!=TRUE)))||
			(Rom_partmp.fctcode==GC_DEV&&GCROM_PRO(EPRADR,g_wr_erom,Rom_partmp.EProbuff,Rom_partmp.Epgsize,Rom_partmp.EROMsize)!=TRUE)){
			debug_printf("\r\nEROM burn fail!\r\n");
				//free(Rom_partmp.EProbuff);
       return FALSE;					
		}else{
			if(Rom_partmp.fctcode==GC_DEV){
			if(GCEROM_RD(g_wr_erom,g_EROMBR,Rom_partmp.EROMsize-1)==ERCKVAL&&GCEROM_RD(g_wr_erom,g_EROMBR,ERBCKADR)==*Rom_partmp.EProbuff
				&&GCEROM_RD(g_wr_erom,g_EROMBR,Rom_partmp.EROMsize/2-1)==*(Rom_partmp.EProbuff+Rom_partmp.EROMsize/2-1)/*&&
			  (GCEROM_CKRD(g_wr_erom,g_EROMVCK,3)==TRUE)*/){
			  debug_printf("\r\nEROM burn OK!\r\n");
			}else{debug_printf("\r\nEPro_vertify error!\r\n");
				   return FALSE;
        }
			}else if(Rom_partmp.fctcode==NXP_DEV){
				  if(GCEEROM_RD(c_wr_erom,c_er_dump,Rom_partmp.EProbuff,Rom_partmp.EROMsize,10)!=TRUE){
						return FALSE;
					}
			}
			
			//free(Rom_partmp.EProbuff);
		}
		 //debug_printf("\r\n%02x!\r\n",*crtyflsed);
		//debug_printf("\r\n%02x!\r\n",*crtyfls);
    debug_printf("\r\nEEROM burn begin\r\n");
			 if(Rom_partmp.EEROMsize%EEROM_BKSIZE){
				 Pts=Rom_partmp.EEROMsize/EEROM_BKSIZE+1;
			 }else{
				 Pts=Rom_partmp.EEROMsize/EEROM_BKSIZE;
			 }
       tmppsz=Rom_partmp.EEROMsize;
       pgaddr=EEPRADR;
       randv=randnum();			 
			 while(Pts--){
				 if(tmppsz/EEROM_BKSIZE){
					 tmppsz=EEROM_BKSIZE;
				 }
		   if((Rom_partmp.fctcode==NXP_DEV&&ROM_PRO(pgaddr,c_wr_eeprom,Rom_partmp.EEProbuff,Rom_partmp.EEpgsize,tmppsz,i)!=TRUE)||
			      (Rom_partmp.fctcode==GC_DEV&&GCROM_PRO(pgaddr,g_wr_eeprom,Rom_partmp.EEProbuff,Rom_partmp.EEpgsize,tmppsz)!=TRUE) ){
					   debug_printf("\r\nEEROM burn fail!\r\n");
            return FALSE;			
		      }
					 i++;
					pgaddr=EEPRADR+i*(EEROM_BKSIZE/Rom_partmp.EEpgsize);
					tmppsz=Rom_partmp.EEROMsize-i*EEROM_BKSIZE;
	     }
			if((Rom_partmp.fctcode==GC_DEV&&GCEEROM_RD(g_wr_eeprom,g_EEROMBR,Rom_partmp.EEProbuff,Rom_partmp.EEROMsize,10)==TRUE)||
				(Rom_partmp.fctcode==NXP_DEV&&GCEEROM_RD(c_wr_eeprom,c_ee_dump,Rom_partmp.EEProbuff,Rom_partmp.EEROMsize,10)==TRUE)){
			    debug_printf("\r\nEEROM burn OK!\r\n");
			  }else{
					debug_printf("\r\nEEPro_vertify error!\r\n");
					 return FALSE;
				}				
			     //free(Rom_partmp.EEProbuff);
				if(Rom_partmp.fctcode==GC_DEV){
						if((GCROM_LBPRO(EPRADR,g_crty_wr,crtyfls,CRTYPPSZ,FLSRDBSZ)!=TRUE)){
			debug_printf("\r\ncrty_wr fail!\r\n");
				//free(Rom_partmp.EProbuff);
       return FALSE;					
		}else{
			if(GCEROM_RD(g_crty_wr,g_EROMBR,KEY_CRTY_FLSADR)==*crtyfls&&GCEROM_RD(g_crty_wr,g_EROMBR,KEY_CRTY_EDFLSADR)==*crtyflsed
				){
			  debug_printf("\r\ncrty_wr OK!\r\n");
			}else{debug_printf("\r\ncrty_wr_vertify error!\r\n");
				   return FALSE;
        }
			//free(Rom_partmp.EProbuff);
		}
	}
						  if((Rom_partmp.fctcode==GC_DEV&&GCCryt(g_ct)!=TRUE)||
								 (Rom_partmp.fctcode==NXP_DEV&&NXPWP()!=TRUE)){
						debug_printf("\r\ncrypt fail!\r\n");
						 return FALSE;
					}else{
						debug_printf("\r\ncrypt ok!\r\n");
					}
					
					//if(Moni_modset_SPDPU()!=TRUE){
			//debug_printf("\r\nEnter downld mode fail!\r\n");	
			//return FALSE;
		//}
				 gpio_bit_set(GPIOC, POCTRL);	
			  if(Rom_partmp.fctcode==NXP_DEV){
					 uint8_t ER_BYTE[3]={0x38,0x27,0x4e};
		       delay_us(20);
		       zdsend(ER_BYTE,3);
		       delay_us(200);
		       ER_BYTE[0]=0x2F;
		      zdsend(ER_BYTE,1);
        }
			if(Rom_partmp.fctcode==GC_DEV)
			  if(bdcmp_update()==FALSE)
           return FALSE;  				
     }
		break;
	 default:{
		 	  debug_printf("\r\nprg op_cmd error!\r\n");
		    return FALSE;
	 }
	 //break;
	}
	
	return TRUE;
}
/*static bool rddata_send(uint8_t data)
{
	uint8_t	bits=7;
	  
		while(bits--){
		gpio_bit_set(GPIOA, MSCL);
			delay_us(1);
			if(data&0x01){
				gpio_bit_set(GPIOA, MSDA);
			}else{
				gpio_bit_reset(GPIOA, MSDA);
			}
		delay_us(3);
		gpio_bit_reset(GPIOA, MSCL);
		delay_us(1);
    	data=data>>1;		
		delay_us(3);
		}
}*/
static bool fls_pro_op(uint32_t flssz,uint8_t* flsadr)
{	
	if(!W25QXX_Write(flsadr,0,flssz))
		  return FALSE;	
	else 
		  return TRUE;
}
static void de_gpio_init(void)
{
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA|MSCL);
	gpio_bit_reset(GPIOA, MSCL);
	gpio_bit_reset(GPIOA, MSDA);
	delay_us(1000);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA|MSCL);
	//rcu_periph_clock_disable(RCU_GPIOA);
}
static void de_spi_init(void)
{
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6| GPIO_PIN_7);
	//rcu_periph_clock_disable(RCU_GPIOA);
	spi_i2s_deinit(SPI0);
	rcu_periph_clock_disable(RCU_SPI0);
}
bool  Erase(void)
{
	if(Rom_Prgram_OP(ERASE_CMD)==FALSE){
	  //info_printf("\r\nErase fail!\r\n");
		gpio_bit_set(GPIOC, POCTRL);
		delay_us(3000);
		de_gpio_init( );
		return FALSE; 
   // while(1);		
	}else{
		//info_printf("\r\nErase done!\r\n");
		return TRUE;
	}
}
bool Program(void)
{
	uint8_t result=FALSE;
	if(Rom_partmp.devtype!=OTHERDEV){
	result=Rom_Prgram_OP(EPRGM_CMD);
		gpio_bit_set(GPIOC, POCTRL);
		de_gpio_init( );
	}else{
		 //gpio_bit_reset(GPIOC, POCTRL);
	 if((result=W25QXX_Init())==TRUE)
	 result=fls_pro_op(Rom_partmp.EROMsize,Rom_partmp.EProbuff);
    gpio_bit_set(GPIOC, POCTRL);
		delay_us(1000);	
    de_spi_init();		
	}
	if(result==FALSE){
		return FALSE; 
		//info_printf("\r\nProgram fail!\r\n");
		//while(1);
	}else{
		//info_printf("\r\nProgram OK!\r\n");
		//gpio_afio_deinit();
		//rcu_periph_clock_disable(RCU_AF);
		return TRUE;
	 }	
}
static bool GCEEROMID_RD(C_CODE_ENUM cmd,uint16_t bytes,uint8_t* p)
{
	uint8_t  rspd=0;
	//uint16_t  tmpsz=rlbytes;
	uint16_t  tmpEERsz=bytes;
	//uint8_t C_BYTE[3]={C1,C4,C6};
//if(cmd0==g_wr_eeprom){
#if defined(MONI_INTMODE_PP)
		debug_printf("\r\nEnter PPM!\r\n");
		if(Moni_int_modset_PPS()!=TRUE){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif
	// }else{
#if defined(MONI_INTMODE_PU)
		 debug_printf("\r\nEnter PUM!\r\n");
				if((cmd==g_EEROMBR&&Moni_int_modset_PU(GC_DEV)!=TRUE)||
					(cmd==c_ee_dump&&Moni_int_modset_PU(NXP_DEV)!=TRUE)){
			debug_printf("\r\nEnter downld mode fail!\r\n");	
			return FALSE;
		}
#endif		
//	 }
		if(cmd==g_EEROMBR)
      if(Gsopr()!=TRUE)
		return FALSE;				
//	}
		if(cmd==c_ee_dump)
      if(nxpopr(c_ee_dump)!=TRUE)
		   return FALSE;
		if(Cmd_send(cmd)!=TRUE)
		return FALSE;
		while(bytes--){
		  rspdata_read(cmd,&rspd,8);
			if(cmd==g_EEROMBR){
         if((bytes==(tmpEERsz-516)&&((*p=rspd)&0x0F)!=ID1)||((bytes==tmpEERsz-511)&&rspd!=ID2)){
					 return FALSE;
			 }
		 }else if(cmd==c_ee_dump){
			       if(tmpEERsz==PCF7945_EE_ROM_SZ){
				      if((bytes==(tmpEERsz-512)&&rspd!=ID3))
					     return FALSE;
						 //if(bytes==(tmpEERsz-516))
							  // *p=rspd;
					   }else if(tmpEERsz==PCF7952_EE_ROM_SZ){
							  if(Rom_partmp.devtype==PCF7922){
									if((bytes==(tmpEERsz-512)&&rspd!=ID4))
					          return FALSE;
								}else{
							   if((bytes==(tmpEERsz-512)&&rspd!=ID3))
					          return FALSE;
							 }
						    //if(bytes==(tmpEERsz-507))
							     //*p=rspd;
						 }
    	 }
		 }				
	  return TRUE;
}
uint8_t key_id(DEV_FTCODE devcd,NXP_DEVTPE_ENUM devtype,uint8_t* rddtype)
{
	             uint8_t tmdevtype=0;
	              uint16_t eeromsz=0;
	              uint8_t  result=0;
	  // user_kdl_init();
    if(devtype==PCF7945||devtype==PCF7953)
         eeromsz=PCF7945_EE_ROM_SZ;			
    else if(devtype==PCF7952||devtype==PCF7941||devtype==PCF7922){
         eeromsz=PCF7952_EE_ROM_SZ;	
	     }
		 Rom_partmp.devtype=devtype;
		       if(devcd==GC_DEV){
		  			      if((result=ROM_ER(devtype,g_er_erom))==FALSE){
								debug_printf("\r\nbgErase fail!\r\n");									
							  //return result;
						   }/*else{
								 if(result==2)
									 return result;									
							 }*/
						if(result==TRUE&&GCEEROMID_RD(g_EEROMBR,eeromsz,&tmdevtype)!=TRUE){
									 debug_printf("\r\ndev error!\r\n");
							     result=FALSE;
								}else{
                   if((tmdevtype&0xf0)==0x60){
										   *rddtype=PCF7953; 
									 }else if((tmdevtype&0xf0)==0x20){
										   *rddtype=PCF7952;
									 }else if((tmdevtype&0xf0)==0x40){
										   *rddtype=PCF7945;
									 }else{
										   *rddtype=0; 
									 }										 
								}
						 }
					 if(devcd==NXP_DEV){
						 //if(devtype==PCF7922)
		  			      if((result=ROM_ER(devtype,c_er_erom))==FALSE){
								debug_printf("\r\nbcErase fail!\r\n");
							  //return FALSE;
						   }/*else{
								 if(result==2)
									 return 2;									
							 }*/
						if(result==TRUE&&GCEEROMID_RD(c_ee_dump,eeromsz,&tmdevtype)!=TRUE){
									 debug_printf("\r\ndev error!\r\n");
							     result=FALSE;
								}/*else{
             if((tmdevtype&0xf0)==0x90){
										   *rddtype=PCF7953; 
									 }else{
										   *rddtype=0; 
									 }										 
								}*/
							}
		     gpio_bit_set(GPIOC, POCTRL);
         delay_us(120000);
         gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	       gpio_bit_reset(GPIOA, MSDA);
						 									
	     return result;
}
bool kfw_pkgszchk(NXP_DEVTPE_ENUM devtype,uint16_t fmpkgsz)
{
	 if(devtype==PCF7945||devtype==PCF7953||devtype==PCF7922){
		 if(fmpkgsz!=0x2414){
			 return FALSE;
		 }
	 }/*else if(devtype==PCF7952){
		 if(fmpkgsz!=0x1214){
			 return FALSE;
		 }
	 }*/		 
   else if(devtype==PCF7941||devtype==PCF7952){
		 if(fmpkgsz!=0x1814){
			 return FALSE;
		 } 		 
	 }else if(devtype==OTHERDEV){
		 if(fmpkgsz==0){
			 return FALSE;
		 } 		 
	 }else{
		 return FALSE;
	 }
	 return TRUE;
	
}
extern uint8_t kdecryptbinfile(uint32_t EERdlflsadr);
bool fm_burd(DEV_FTCODE fcode,NXP_DEVTPE_ENUM devtype,/*uint32_t binsz,*/uint32_t ERdlflsadr,uint32_t EERdlflsadr)
{
	if(Rom_parameter_cofg(fcode,devtype,ERdlflsadr,EERdlflsadr)==FALSE){
		info_printf("\r\ncfg fail!\r\n");
		return FALSE;
	}
		if(devtype!=OTHERDEV){
	if(kdecryptbinfile(EERdlflsadr)==FALSE){
		debug_printf("\r\ndecrty fail!\r\n");
		return FALSE;
	}
  if(fcode==GC_DEV){	
	debug_printf("\r\nBegin erase...\r\n");
	if(Erase()){
		debug_printf("\r\nErase done!\r\n");
	}else{
		info_printf("\r\nErase fail!\r\n");
		return FALSE;
	 }
  }
 }	
	 debug_printf("\r\nBegin programming...\r\n");
  if(Program()){
		//gpio_bit_reset(GPIOB,  LEDG);
		debug_printf("\r\nProgram OK!\r\n");
	}else{
		//gpio_bit_reset(GPIOB,  LEDR);
		info_printf("\r\nProgram fail!\r\n");
		return FALSE;
	}
	return TRUE;
	
}
