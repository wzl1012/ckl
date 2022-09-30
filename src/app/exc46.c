#include "headfile.h"
uint8_t  excaucmd[10]={0x60,0x00,0x41,0xAA,0x5e,0x66,0xf4,0xb0,0x2b,0x48};
static void exc46_reset(void)
{
	pcf7991_init( );
  pcf7991_inactive_driver();
  delay_us(20000);
  pcf7991_active_driver();
  delay_us(100);	
}
static uint8_t exc46_id(pcf7936_tag_t *p_tag)
{
		uint8_t a_read[5]={0},cmd;
	  cmd = EXCIDCMD;
	uint8_t result = FALSE;
	pcf7991_write_tag_n(0x07);
	delay_us(400);
	pcf7936_wr_msbits(&cmd,8, TRUE);
	pcf7991_tagrd_fsetproc();
	pcf7991_read_tag();
#if SYSTEM_SUPPORT_OS	
 taskENTER_CRITICAL();  
#endif
	if (pcf7991_decode_miss_low()) {
		if (p_tag->tmcf_dcs == e_modu_mc)
			result = pcf7991_decode_mc(32, a_read, 37, e_dir_msbit);
		else
			result = pcf7991_decode_bp(32, a_read, 37, e_dir_msbit);
	}
#if SYSTEM_SUPPORT_OS	
 taskEXIT_CRITICAL();
#endif	
	/* check read correct */
	if (!result)
		   ;
		//delay_us(20);
	/* check header '11111' */
	else if ((a_read[0]&0xF8) == 0xf8/*0xF8*/) {
		//copy_msbits(p_tag->a_page[0], 0, a_read, 4, 32);
		//if(*((uint32_t*)(p_tag->a_page[0]))==EXCGIDC){
			    p_tag->now_state=e_idcfm;
		//}
	}
	else
		result = FALSE;
  pcf7991_exit_tag_mode( );
	return result;	
}
static uint8_t exc46_decide_modu(pcf7936_tag_t *p_tag)
{
	uint8_t result;
	result=FALSE;
	p_tag->tmcf_dcs = e_modu_mc;
	pcf7991_confg();
	 result =exc46_id(p_tag);
	pcf7936_to_off();
	if (!result) {
		p_tag->tmcf_dcs = e_modu_bp;
		//pcf7936_off_to_wait();
		pcf7991_confg();
		result = exc46_id(p_tag);
		pcf7936_to_off();
	}
	return result;
}
static uint8_t exc46_auth(pcf7936_tag_t *p_tag)
{
	uint8_t a_read[10]={0},result=FALSE;
	if (exc46_decide_modu(p_tag)==TRUE){
		 	pcf7991_write_tag_n(0x07);
	   delay_us(400);
	   pcf7936_wr_msbits(excaucmd,80, TRUE);
	   pcf7991_read_tag();
		#if SYSTEM_SUPPORT_OS	
 taskENTER_CRITICAL();  
#endif
	if (pcf7991_decode_miss_low()) {
		if (p_tag->tmcf_dcs == e_modu_mc)
			result = pcf7991_decode_mc(32, a_read, 71, e_dir_msbit);
		else
			result = pcf7991_decode_bp(32, a_read, 71, e_dir_msbit);
	}
#if SYSTEM_SUPPORT_OS	
 taskEXIT_CRITICAL();
#endif
	  if (!result)
		   ;
	 else if ((a_read[0]&0xFF) == 0xFF&&(a_read[1]&0xFF) == 0xFE/*0xF8*/) {
			    p_tag->now_state=e_authorized;
	      }
	   else
		   result = FALSE;
  pcf7991_exit_tag_mode( );
	}else{
		result = FALSE;
	}
	return result;
}
uint8_t shift_fun2(uint8_t data)
{
data=(data<<4)|(data>>4);
data=((data<<2)&0xcc)|((data>>2)&0x33);
data=((data<<1)&0xaa)|((data>>1)&0x55);
return data;
}
static uint8_t exc46_write_page(const uint8_t page, pcf7936_tag_t *p_tag, const uint8_t *p_wr)
{
		uint8_t cmd[2], a_read[10]={0}, a_write[4]={0};
		uint8_t result =FALSE;
	switch (p_tag->tmcf_dcs) {
		case e_modu_mc:
		case e_modu_bp:
			break;
		default:
			return FALSE;
	}
	if (page > 7)
		return FALSE;
  if(page%2==0)	
	cmd[0] = shift_fun2(((6-page)<<5)|(EXCPGRWCMD2<<3) |page);		//see pcf7936 command
	else
	cmd[0] = shift_fun2(((8-page)<<5)|(EXCPGRWCMD2<<3) |page);	
	if (page<0x04)
	cmd[1] = EXCPGRWCMD2|(1<<7);
	else 
	cmd[1] = EXCPGRWCMD2;	
	/* send write command, 10 bits */
	if (p_tag->tmcf_enc == e_pcf7936_cipher)
		hitag2_oneway2(cmd, 10);
	//pcf7991_write_tag_n(0);
	pcf7991_write_tag_n(0x07);
	pcf7936_wr_msbits(&cmd[0], 10, TRUE);
	pcf7991_read_tag();
	//delay_us(PCF7936_T_WAIT_TR);
#if SYSTEM_SUPPORT_OS	
 taskENTER_CRITICAL();  
#endif
	if (pcf7991_decode_miss_low()) {
		if (p_tag->tmcf_dcs == e_modu_mc)
			result = pcf7991_decode_mc(32, a_read, 15, e_dir_msbit);
		else
			result = pcf7991_decode_bp(32, a_read, 15, e_dir_msbit);
	}
#if SYSTEM_SUPPORT_OS	
 taskEXIT_CRITICAL();
#endif
	  if (!result){
     if ((a_read[0]&0xF8) != 0xF8/*0xF8*/) {
			    result =FALSE;
	      }
			}
	 if(result){
	/* send write data, 32 bits */
	//cmd[0] = 0x00;
	//cmd[1] = 0x00;
	//copy_msbits(cmd, 0, a_read, 5, 10);
	copy_bytes(a_write, p_wr, 4);		 
		 a_write[0]^=EXCPGDXRP&0xff;
		 a_write[1]^=(EXCPGDXRP>>8&0xff);
		 a_write[2]^=(EXCPGDXRP>>16&0xff);
		 a_write[3]^=(EXCPGDXRP>>24&0xff);
	if (p_tag->tmcf_enc == e_pcf7936_cipher) {
		//hitag2_oneway2(cmd, 10);
		hitag2_oneway2(a_write, 32);
	}
	delay_us(PCF7936_T_WAIT_BS);	
	shift_fun2(a_write[0]);
	shift_fun2(a_write[1]);
	shift_fun2(a_write[2]);
	shift_fun2(a_write[3]);
	pcf7991_write_tag_n(0x07);
	pcf7936_wr_msbits(a_write, 32, TRUE);
	delay_us(PCF7936_T_PROG*1000);
		//if (p_tag->tmcf_enc == e_pcf7936_cipher) //read
		//hitag2_oneway2(cmd, 10);
	pcf7991_write_tag_n(0x07);
	pcf7936_wr_msbits(&cmd[0], 10, TRUE);
	pcf7991_read_tag();
	#if SYSTEM_SUPPORT_OS	
 taskENTER_CRITICAL();  
#endif
	if (pcf7991_decode_miss_low()) {
		if (p_tag->tmcf_dcs == e_modu_mc)
			result = pcf7991_decode_mc(32, a_read, 71, e_dir_msbit);
		else
			result = pcf7991_decode_bp(32, a_read, 71, e_dir_msbit);
	}
#if SYSTEM_SUPPORT_OS	
 taskEXIT_CRITICAL();
#endif
		  if (!result){
     if ((a_read[9]&0xEF) != (a_write[3]&0xEF)/*0xF8*/) {
			    result =FALSE;
	      }
			}
 }
	 
	 pcf7991_exit_tag_mode( );
	return result;
}
uint8_t exc46_wr(pcf7936_tag_t *p_tag,uint8_t pgstart,uint8_t pged,uint8_t *p_wr)
{
	uint8_t i=0,trys=0,result=FALSE;
	exc46_reset( );
	for (trys = 0; (trys<3) && (!result); trys++) {
	if(exc46_auth(p_tag)==TRUE){
		for(i=pgstart;i<=pged;i++){
			delay_us(PCF7936_T_WAIT_BS);
		 if(exc46_write_page(i,p_tag,p_wr)!=TRUE)
			   break;
       p_wr+=4;			
		}
			if (i >pged)
				result = TRUE;
			else
				result = FALSE;		
	}else
	  result=FALSE;
	exc46_reset( );
 }
 return result;	
}


