#include "headfile.h"
bool nxpCmd_send(uint8_t cmdcode,uint8_t cdflg)
{
	uint8_t  i=0;
	uint8_t BitData_Cur=0;
	uint8_t BitData_Next=0;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	//gpio_bit_set(GPIOA, MSDA);
	gpio_bit_reset(GPIOA, MSDA);
	//delay_us(5);
	while(!gpio_input_bit_get(GPIOA, MSCL)){	
	}
	for(i=0;i<8;i++){
		if(!cdflg){
		if(/*(*/cmdcode/*>>i)*/&0x01){
			//delay_us(1);
			gpio_bit_set(GPIOA, MSDA);
		}else{
			//delay_us(1);
			gpio_bit_reset(GPIOA, MSDA);
		}
	}else if(cdflg==1){
				if(/*(*/cmdcode/*>>i)*/&0x80){
			//delay_us(1);
			gpio_bit_set(GPIOA, MSDA);
		}else{
			//delay_us(1);
			gpio_bit_reset(GPIOA, MSDA);
		}
	}
		while(gpio_input_bit_get(GPIOA, MSCL)){
		}
		if(!cdflg){
		//BitData_Cur = cmdcode&0x01;
		cmdcode>>=1;
		//BitData_Next= cmdcode&0x01;
	  }else if(cdflg==1){
			//BitData_Cur = cmdcode&0x80;
			cmdcode<<=1;
			//BitData_Next= cmdcode&0x80;
		}
		if(i!=7)
		{
			delay_us(1);
			if(!cdflg){
			switch((BitData_Cur<<1)|(BitData_Next))
			{
			case 0x00:	
			{
				break;
			}
			case 0x01:	
			{
				gpio_bit_set(GPIOA, MSDA);	
				break;
			}
			case 0x02:	
			{
				gpio_bit_reset(GPIOA, MSDA);	
				break;
			}
			case 0x03:	
			{
				break;
			}
			default:break;
			}
		}else if(cdflg==1){
			switch((BitData_Cur)|(BitData_Next>>1))
			{
			case 0x00:	
			{
				break;
			}
			case 0x40:	
			{
				gpio_bit_set(GPIOA, MSDA);	
				break;
			}
			case 0x80:	
			{
				gpio_bit_reset(GPIOA, MSDA);	
				break;
			}
			case 0x0c:	
			{
				break;
			}
			default:break;
			}
		}

			while(!gpio_input_bit_get(GPIOA, MSCL));      
		}
		else	; 
			while(!gpio_input_bit_get(GPIOA, MSCL));     
	    // while(!gpio_input_bit_get(GPIOA, MSCL)&&(i<8)){
	   //}
	//delay_us(2);
	//microseconds_delay(2);	
}
	//while(gpio_input_bit_get(GPIOA, MSCL));
   delay_us(1);
	//microseconds_delay(8);
	gpio_bit_set(GPIOA, MSDA);
  delay_us(80);
  return TRUE;
}

bool nxp_rspdata_read(C_CODE_ENUM cmd,uint8_t* dbuff,uint8_t bits)
{
  uint8_t Loop_Count;
	uint8_t Read_Data=0;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	gpio_bit_reset(GPIOA, MSDA);
	while(!gpio_input_bit_get(GPIOA, MSCL));
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MSDA);
	while(gpio_input_bit_get(GPIOA, MSCL));
	for(Loop_Count=0;Loop_Count<8;Loop_Count++) //????,???????
	{
		if(gpio_input_bit_get(GPIOA, MSDA)==1)
		{
			Read_Data|=(0x01<<Loop_Count);
		}
		while(!gpio_input_bit_get(GPIOA, MSCL));
		while(gpio_input_bit_get(GPIOA, MSCL));
	}
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, MSDA);
	gpio_bit_set(GPIOA, MSDA);
	  *dbuff=Read_Data;
	delay_us(15);
	return TRUE;
}

