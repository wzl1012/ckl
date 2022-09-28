#include "headfile.h"
//#include <string.h>  
#define  KEYSZ  16
#define  FLS_PGSZ 2048
#define  SKL 24
#define  BINEND  0x996622CC
#define  FLS_SEC_PGSZ 1
//uint8_t kdecryptbinfile(uint32_t binsz,uint32_t EERdlflsadr);

//int main(int argc, char *argv[]) {
  //  char targetFile[60]={0}; 
static uint8_t const secretKey[]={0xA1,0xB3,0xC5,0xD7,0xF9,0xE6,0xDB,0x69,0xE8,0xCC,0xA2,0xA7,0x46,0xB3,0xDC,0xE1};      

   //char a1[]="D:/rspdbs/kencry/kencry/TL_7953-EEPROM_v1.0.bin";
   //char a2[]="D:/rspdbs/kencry/kencry/7953-EEPROM_v1.0.bin"; 
  //  if( decryptFile(argv[1], secretKey, targetFile) ){
      //  printf("%s decrypt ok,save as: %s?\n", argv[1], targetFile);
  //  }
  //  return 0;
//}
//static bool fmc_program_check(uint32_t flswadr,uint32_t* rdadr,uint8_t wsz)
//{
 //   uint32_t i;
 //   uint32_t* ptrd=NULL;
	//  uint32_t* rtrd=NULL; 
  //  ptrd = (uint32_t *)flswadr;
 //   rtrd =rdadr;
    /* check flash whether has been programmed */
  //  for(i = 0; i <wsz; i++){
    //    if((*ptrd) != (*rtrd)){
				//	  return FALSE;
           // break;
     //   }else{
       //     ptrd++;
				//	  rtrd++;
      //  }
   // }
	//	return TRUE;
//}
/*
static void fmc_erase_pages1(uint32_t flsadr)
{
    uint8_t erase_counter;

    fmc_unlock();

    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    for(erase_counter = 0; erase_counter < FLS_SEC_PGSZ; erase_counter++){
        fmc_page_erase(flsadr + (FLS_PGSZ * erase_counter));
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }

    fmc_lock();
}
static bool fmc_prg1(uint32_t staradr, uint32_t *pdata,uint32_t bytesz)
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
		if(fmc_program_check(staradr,pdata,bytesz/4)!=TRUE)
		{
			return FALSE;
		}
    fmc_lock();
   return TRUE;		
}*/
bool sk1_decrty(uint32_t fwflsadr)
{

	 uint8_t *buffer=(uint8_t *)fwflsadr;
	 uint8_t  tmpbuf[FLS_PGSZ]={0};
	 uint32_t *p=(uint32_t *)fwflsadr;
	 uint32_t tmpadr=fwflsadr;
   if(buffer==NULL){
		 return FALSE;
   }
   uint32_t	j=0,m=0;
   uint8_t i,k; 
   uint8_t s1Key[24]={0x31,0x56,0x5d,0xe1,0xe8,0xaa,0xfc,0xfb,0x79,0x90,0x92,0x84,0x85,0x27,0x15,0xa8,
					                            0xc6,0x8c,0x71,0x61,0x9f,0x7a,0xdd,0x68};	 
   uint8_t s1key_bcy[24]={0x59,0x9c,0x16,0x12,0x35,0x82,0x7b,0xb1,0xb8,0xf2,0xf8,0xfa,0x69,0x49,0x88,0x53,
	                                    0xc2,0xd1,0x4d,0x92,0xdb,0x3b,0x92,0x10};
	 uint8_t secretKey2[24]={0};
	 uint8_t rflslenth=4;
	    for(i=0;i<SKL;i++){
			secretKey2[i]=s1Key[i]^s1key_bcy[i];
		}
			while(p[j]!=BINEND){
	       for(i=0; i<rflslenth; i++){
        	if(j%2==0){
						tmpbuf[m]=*buffer;
						for(k=SKL-1;k>0;k--)
            tmpbuf[m] ^= s1Key[k];
        	}
          else{
						tmpbuf[m]=*buffer;
						for(k=SKL-1;k>0;k--)
            tmpbuf[m] ^= secretKey2[k];
						}
             //printf("buff_p[%d]=%02x  ",j,buffer[j]);
			 //if(i==15)
			 //printf("\n");
				m++;
        buffer++;						
        //j++;			 
        }
				 j++;
			}
			//	if(m==FLS_PGSZ){
				//	m=0;
					fwflsadr=tmpadr;
					fmc_erase_pages(fwflsadr,1);
					if(!fmc_prg(fwflsadr, (uint32_t *)tmpbuf,m)){
						 return FALSE;
					}
			//	}
			return TRUE;
}
uint8_t kdecryptbinfile(uint32_t EERdlflsadr){
//uint32_t tmsz=0;
	//if(binsz==0||binsz%16){
		//return FALSE;
	//}
	uint32_t	j=0,m=0,l=0;
   uint8_t *buffer=(uint8_t *)EERdlflsadr;
	 uint8_t  tmpbuf[FLS_PGSZ]={0};
	 uint32_t *p=(uint32_t *)EERdlflsadr;
   if(buffer==NULL){
		 return FALSE;
   }		 
   uint8_t i;  
   uint8_t secretKey_bcy[16]={0x23,0x65,0xF9,0x7D,0xee,0x18,0x5C,0x4A,0xAA,0x7C,0x82,0x8b,0xf3,0x49,0x12,0x53};
	 uint8_t secretKey2[16]={0};
	 
	    for(i=0;i<KEYSZ;i++){
			secretKey2[i]=secretKey[i]^secretKey_bcy[i];
		}
			while(p[j]!=BINEND){
        for(i=0; i<KEYSZ; i++){
					if(p[j]==BINEND)
						break;
        	if((l/16)%2==0){
						tmpbuf[m]=*buffer;
            tmpbuf[m] ^= secretKey[i];
        	}
            else{
						tmpbuf[m]=*buffer;
            tmpbuf[m] ^= secretKey2[i];
						}
						m++;
            // debug_printf("buff_p[%d]=%02x  ",m,tmpbuf[m]);
			 //if(i==15)
			 //debug_printf("\n");
	   	if(m==FLS_PGSZ){
		     	m=0;
					fmc_erase_pages(EERdlflsadr+(l/FLS_PGSZ)*FLS_PGSZ,1);
					if(!fmc_prg(EERdlflsadr+(l/FLS_PGSZ)*FLS_PGSZ, (uint32_t *)tmpbuf,FLS_PGSZ)){
						 return FALSE;
					}
      	}
         l++;
        buffer++;
        j=l/4;				
        }
			}
		if(p[j]==BINEND&&(m>0)){
				//fwflsadr=tmpadr;
				fmc_erase_pages(EERdlflsadr+(l/FLS_PGSZ)*FLS_PGSZ,1);
					if(!fmc_prg(EERdlflsadr+(l/FLS_PGSZ)*FLS_PGSZ, (uint32_t *)tmpbuf,m)){
						 return FALSE;
					}				
			}
    return TRUE;
}
