#include "headfile.h"
#define  RF317MX     348000000//360000000
#define  RF313MI     300000000//303000000
#define  RF458MX     477400000//480000000
#define  RF431MI     400000000//400000000
#define  RF902MX     960000000//930000000
#define  RF902MI     850000000
#define  T7CNTS   3
//#define  T7OVCNTS  13
#define  STAROT 13
#define  F315DEVMIN   42//39
#define  F315DEVSPE   44
#define  F315DEVMX   46//48
#define  F433DEVMIN   29
#define  F433DEVSPE   31
#define  F433DEVMX    33
#define  F903DEVMIN   13
#define  F903DEVSPE   15
#define  F903DEVMX   17
#define  F315SPE   314181760
#define  F433SPE   432000000
#define  F903SPE   921600000
#define  DEVF  0.65
#define  DIFF   4
#define  FDIFFV 3
#define  DIFFMX  100
#define  F315DEV2MIN   85//39
#define  F315DEV2MX   91//48
#define  F433DEV2MIN   59
#define  F433DEV2MX    65

#define  F315DEV3MIN   129//39
#define  F315DEV3MX   135//48
#define  F433DEV3MIN   93
#define  F433DEV3MX    99
#define  FSRANGL       13
#define  FSRANGH       46
#define  SCANTMS       32
/*static  uint8_t find_dffmin(uint8_t* diffmin,uint8_t sz)
{
	 uint8_t * tmp=NULL;
	 uint8_t tmpmx=6;
	 tmp=diffmin;
	 for(uint8_t i=0;i<sz;i++){
		   if(tmp[i]<tmpmx)
				 tmpmx=tmp[i];
	 }
	return tmpmx; 
}*/
static uint16_t  fct7cnt[T7CNTS]={0};
/*
static bool rfdata_proc(uint16_t *ic1value1,float *frev,uint8_t *flg)
{
	//float frevtmp=0;
	uint16_t* tp=NULL;
	tp=ic1value1;
	uint8_t i=0,j=0,k=0,diffsz=0;
	uint8_t diff[T7CNTS-1]={0};
	uint16_t sum=0;
	float dffmin=0;
	*frev=0;
	*flg=0;
 for(i=0;i<(T7CNTS-1);i++){
      diff[i]=tp[i+1]-tp[i];		
	}
 diffsz=sizeof(diff);
 for(i=0;i<diffsz;i++){
			if(diff[i]==0||diff[i]>=DIFFMX){
				  	return FALSE;
			}*//*else{
			if(j>0)
				j--;
			}
		if(j>=2)
			return FALSE;*/
	/*}
	for(j=0;j<2;j++){
 	for(i=0;i<diffsz;i++){
		   if(j==0){
			    if((diff[i]%F315DEVSPE)<FDIFFV||(F315DEVSPE-(diff[i]%F315DEVSPE)<FDIFFV)){
						 //k++;
						  if(F315DEV3MIN<=diff[i]&&diff[i]<=F315DEV3MX)
								        *flg=2;
							else
								  *flg=1; 
							if(F315DEVMIN<=diff[i]&&diff[i]<=F315DEVMX){
						     sum+=diff[i];
								  k++;
							}
						//if(k>=3){
						//dffmin=F315DEVSPE;
							//break;
						//}else{
							//continue;
							//}
					}else{
						//if(k>0)
	           //k--;
						sum=0;
						k=0;
            break;
				   }					
					}
			 if(j==1){
			   if((diff[i]%F433DEVSPE)<FDIFFV||(F433DEVSPE-(diff[i]%F433DEVSPE)<FDIFFV)){
						 //k++;
						  if(F433DEV3MIN<=diff[i]&&diff[i]<=F433DEV3MX)
								        *flg=2;
							else
								  *flg=1; 
							if(F433DEVMIN<=diff[i]&&diff[i]<=F433DEVMX){
						     sum+=diff[i];
								  k++;
							}
						//if(k>=3){
						//dffmin=F315DEVSPE;
							//break;
						//}else{
							//continue;
							//}
					}else{
						//if(k>0)
	           //k--;
						sum=0;
						k=0;
            break;
				   }
				}*/
			/* if(j==2){
			    if((diff[i]%F903DEVSPE)<FDIFFV||(F903DEVSPE-(diff[i]%F903DEVSPE)<FDIFFV)){
						 k++;
						if(k>=3){
						dffmin=F903DEVSPE;
							break;
						}else{
							continue;
							}
					}else{
						if(k>0)
	           k--;
            continue;
				   }
        }									
	//	 }
				if(sum!=0){
						break;
					}else{
						k=0;
					}
 }
if(sum==0){
  return FALSE;
}	
j=0;
k=0;
for(i=0;i<(diffsz-1);i++){
		if(diff[i]>=diff[i+1]){
    if((diff[i]-diff[i+1])<FDIFFV){
			     j++;
				 }else{
					 j=0;
				 }
		}else {if(diff[i+1]-diff[i]<FDIFFV){
          j++;
		     }else{
			    j=0;
         }
       }
	 if(j>=2){
				 k++;
		    if(k==1){
				 *flg=1;
				 break;
				}
			}		
}
 if(k!=1){
	 *flg=2; 
 }*/
 /*if(sum!=0){
	 dffmin=sum/k;
	*frev=108000000/dffmin*128;
 }
 return TRUE;*/
	//debug_printf("frevtmp=%f\r\n",frevtmp);
	    /*if(frevtmp>=315000000*(0.85)&&frevtmp<315000000*(0.9)){                 //267.75
		  frevtmp1=frevtmp*1.18;
	  }else if(frevtmp>=315000000*(0.9)&&frevtmp<315000000*(0.95)){
		  frevtmp1=frevtmp*1.08;
	  }else if(frevtmp>=315000000*(0.95)&&frevtmp<314000000){
		  frevtmp1=frevtmp*1.05;
	  }else	 if(frevtmp>=314000000&&frevtmp<316000000){
			frevtmp1=frevtmp;
	  }else if(frevtmp>=316000000&&frevtmp<=315000000*1.05){
		  frevtmp1=frevtmp*0.98;
	  }else if(frevtmp>315000000*1.05&&frevtmp<=315000000*1.1){
		  frevtmp1=frevtmp*0.94;
	  }else if(frevtmp>315000000*1.1&&frevtmp<=315000000*1.15){    //362
		  frevtmp1=frevtmp*0.88;
	  }else if(frevtmp>=434000000*(0.86)&&frevtmp<434000000*(0.9)){    //373.24
		  frevtmp1=frevtmp*1.18;
	  }else if(frevtmp>=434000000*(0.9)&&frevtmp<434000000*(0.95)){
		  frevtmp1=frevtmp*1.08;
	  }else if(frevtmp>=434000000*(0.95)&&frevtmp<433000000){
		  frevtmp1=frevtmp*1.05;
	  }else if(frevtmp>=433000000&&frevtmp<435000000){
			frevtmp1=frevtmp;
	  }else if(frevtmp>=435000000&&frevtmp<=434000000*1.05){
		frevtmp1=frevtmp*0.98;
	  }else if(frevtmp>434000000*1.05&&frevtmp<=434000000*1.1){
		  frevtmp1=frevtmp*0.94;
	  }else if(frevtmp>434000000*1.1&&frevtmp<=434000000*1.15){     //499
		  frevtmp1=frevtmp*0.88;
	  }else if(frevtmp>=903000000*(0.85)&&frevtmp<903000000*(0.9)){   //767
		  frevtmp1=frevtmp*1.15;
	  }else if(frevtmp>=903000000*(0.9)&&frevtmp<903000000*(0.95)){
		  frevtmp1=frevtmp*1.08;
	  }else if(frevtmp>=903000000*(0.95)&&frevtmp<901000000){
		  frevtmp1=frevtmp*1.05;
	  }else if(frevtmp>=901000000&&frevtmp<905000000){
			frevtmp1=frevtmp;
	 }else if(frevtmp>=905000000&&frevtmp<=903000000*1.05){
		  frevtmp1=frevtmp*0.98;
	  }else if(frevtmp>903000000*1.05&&frevtmp<=903000000*1.1){
		  frevtmp1=frevtmp*0.94;
	  }else if(frevtmp>903000000*1.1&&frevtmp<=903000000*1.15){   //1038
		  frevtmp1=frevtmp*0.88;
	  }
		else{
			return FALSE;
		}*/
   /*if(frevtmp==F315SPE){
		    *frev=315000000+(frevtmp-(frevtmp/1000000)*1000000);
		   return TRUE;
	   }
   else if(frevtmp==F433SPE){
		 *frev=434000000+(frevtmp-(frevtmp/1000000)*1000000);
		 return TRUE;
	 }
   else if(frevtmp==F903SPE){ 
		 *frev=903000000+(frevtmp-frevtmp/1000000*1000000);
     return TRUE;
	 }
   else{
     	return FALSE;
	 }*/		 
//}
static bool rfdata_proc2(uint8_t ic1value1,float *frev)
{
	if(ic1value1>=FSRANGL&&ic1value1<=FSRANGH){
		*frev=108000000/ic1value1*128;
		 return TRUE;
	}
  return FALSE;	
}
//#define FWMIN 97
//uint8_t timeout=0;
 //uint8_t i=0;
	float frequency=0;
	uint16_t tpxs=0,tmpint=0,tmpdffmin=0,tmv=0;
	uint8_t fskflgtmp=0,i=0;
 //uint16_t tmov=0,tmpv=0;
static float frequencyls=0;
static uint8_t fskflgls=0;
static uint16_t tmpxsls=0,tmpintls=0;
//uint8_t finalv[]={0};
void EXTI1_IRQHandler(void)
{
	uint32_t  retur;
	retur=taskENTER_CRITICAL_FROM_ISR();
		exti_interrupt_flag_clear(EXTI_1);
	//while(FC_CODE_IN_PIN_LEVEL){		
					 //timeout++;
        // if(timeout>STAROT){
					 TCNT1=40000;
					 TCNT_V=0;
           timer1_start_64();
           T7CNT_V=0;					 
           timer_flag_clear(TIMER7, TIMER_FLAG_CH0);
					while(timer1_ov_flag==0){
					   while(timer_flag_get(TIMER7, TIMER_FLAG_CH0)){
					     fct7cnt[i]=TIMER_CH0CV(TIMER7);
						   timer_flag_clear(TIMER7, TIMER_FLAG_CH0);
							 i++;
							 if(i==2){
								   tmpdffmin=fct7cnt[i-1]-fct7cnt[i-2];
								 if(tmpdffmin>FSRANGH){
									  i=0;
								    continue;
								   }  
							  }
							 if(i==3){
                    tmv=fct7cnt[i-1]-fct7cnt[i-2];
								   if((tmv>=((tmpdffmin<<1)-1))&&tmv<=((tmpdffmin<<1)+1)){
									  i=0;
									 continue;
								  } 
							  }else{
									  tmv=fct7cnt[i-1]-fct7cnt[i-2];
										if(tmv>=(((tmpdffmin<<1)+tmpdffmin)-1)&&tmv<=(((tmpdffmin<<1)+tmpdffmin)+1)){
										        fskflgtmp=2;
											       goto out;
										        }else if(i>=SCANTMS){
												     fskflgtmp=1;
															goto out;
											      }
								}
							 
						 }						 
			 	  }
out:       if(timer1_ov_flag){
           timer1_stop();
					 timer1_clear_ov();
				 }					 
          if(rfdata_proc2(tmpdffmin,&frequency)){				
		        if(frequency!=frequencyls){
				      frequencyls=frequency; 
             tmpint=frequency/1000000;
				     tpxs=(frequency-tmpint*1000000/*+(fct7cnt[4]%10)*10000*/)/1000;
				     if(tpxs%10>=5)
		           tpxs=tpxs/10+1;
				     else
					     tpxs=tpxs/10; 
		          }	             						           							
		  if(tmpint!=tmpintls){
				 tmpintls=tmpint; 
	    OLED_ShowString(40,0,"   ",24,3);
	    OLED_ShowNum(40,0,tmpint,3,24);
			}
			if(tpxs!=tmpxsls){
			tmpxsls=tpxs;
			OLED_ShowString(78,0,"  ",24,2);
	    OLED_ShowNum(78,0,tpxs,2,24);
			}
		}
		 if(fskflgtmp!=fskflgls){
			 fskflgls=fskflgtmp;
			 OLED_ShowString(100,2,"   ",8,3);
      if(fskflgtmp==1){
			OLED_ShowString(100,2,"FSK",8,3);
      }else if(fskflgtmp==2){
				OLED_ShowString(100,2,"ASK",8,3);
			}else{
				OLED_ShowString(100,2,"   ",8,3);
			 } 
		  }	
		 tmpdffmin=0;
      i=0;
    exti_interrupt_flag_clear(EXTI_1);		
		exti_interrupt_disable(EXTI_1);
		timer_disable(TIMER7);
		memset(fct7cnt,0,sizeof(fct7cnt)*2);	
 			taskEXIT_CRITICAL_FROM_ISR( retur );	
}

static void timer_configuration(void)
{
	  timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER7);
    //rcu_periph_clock_enable(RCU_TIMER4);
    timer_deinit(TIMER7);
    //timer_deinit(TIMER4);
    /* TIMER7 configuration */
   // timer_initpara.prescaler         = 1080-1;;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    //timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    //timer_init(TIMER4,&timer_initpara);
	  timer_initpara.prescaler         = 0;
		timer_initpara.period            = 65535;
		timer_init(TIMER7,&timer_initpara);
	
		timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING ;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
		timer_icinitpara.icfilter    = 0x0;
    timer_input_capture_config(TIMER7,TIMER_CH_0,&timer_icinitpara);
		timer_input_trigger_source_select(TIMER7,TIMER_SMCFG_TRGSEL_CI0F_ED);
		
		timer_auto_reload_shadow_enable(TIMER7);
		//timer_auto_reload_shadow_enable(TIMER4);
		//timer_flag_clear(TIMER4,TIMER_FLAG_UP);	
		timer_flag_clear(TIMER7,TIMER_FLAG_UP|TIMER_FLAG_CH0);
		//timer_interrupt_enable(TIMER4,TIMER_INT_UP);

		//nvic_irq_enable(TIMER4_IRQn,6, 2);
    //nvic_irq_enable(TIMER0_Channel_IRQn,7, 3);
		//timer_enable(TIMER4);
    //timer_enable(TIMER7);
}
void rf_chk_init(void)
{
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,FC_CODE_IN);
	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,FC_RATE_IN);
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_1);
  nvic_irq_enable(EXTI1_IRQn,10U, 2U);
	exti_init(EXTI_1, EXTI_INTERRUPT,EXTI_TRIG_RISING);
	exti_interrupt_flag_clear(EXTI_1);
  exti_interrupt_disable(EXTI_1);	
	timer_configuration();
}




