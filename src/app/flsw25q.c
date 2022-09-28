#include "headfile.h"
static void SPI1_Init(void)
{
	    rcu_periph_clock_enable(RCU_SPI0);
	    /* SPI0 GPIO config:SCK/PA5, MISO/PA6, MOSI/PA7 */
	  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, POCTRL);
    gpio_bit_reset(GPIOC, POCTRL);
	  delay_us(1000000);
	  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	  W25QXX_CSH;
	  delay_us(5);
	  //W25QXX_CSH;
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	   spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI0);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);    
    spi_nss_internal_high(SPI0);
		//spi_nss_output_enable(SPI0);
	    spi_enable(SPI0);
}
static bool spi_trxfer_overtime(uint32_t spi_periph)
{
    uint32_t overtimes = CYCLE_CNTS;

    while (spi_i2s_flag_get(spi_periph,SPI_FLAG_TBE) == 0) {
        if (overtimes-- == 0) {
            //set_errornum_to_statword(STDBY_CFG_ERROR_CODE, flash_read_OT );
            debug_printf("warning:write flash over time\r\n");
            return FALSE;
        }
    }
    return TRUE;
}
static bool spi_rxfer_overtime(uint32_t spi_periph)
{
    uint32_t overtimes = CYCLE_CNTS;

    while (spi_i2s_flag_get(spi_periph,SPI_FLAG_RBNE) == 0) {
        if (overtimes-- == 0) {
            //set_errornum_to_statword(STDBY_CFG_ERROR_CODE, flash_read_OT );
            debug_printf("warning:read flash over time\r\n");
            return FALSE;
        }
    }
    return TRUE;
}
int spifls_rw(uint8_t data)
{
	 // uint8_t byte=0;
		if(spi_trxfer_overtime(SPI0)!=TRUE)
        return -1;
		spi_i2s_data_transmit(SPI0,data);
		if(spi_rxfer_overtime(SPI0))		 
	   return(spi_i2s_data_receive(SPI0));          //读取一个字节  
	 else 
		  return -1;     
	//return byte;  		
}
int8_t spifls_w2(uint8_t data)
{
	  //uint8_t byte=0;
		if(spi_trxfer_overtime(SPI0)!=TRUE)
        return -1;
		spi_i2s_data_transmit(SPI0,data);
        return TRUE;		
}
static uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=RDSR;    //读状态寄存器1指令
            break;
        case 2:
            command=RDSR2;    //读状态寄存器2指令
            break;
        case 3:
            command=RDSR3;    //读状态寄存器3指令
            break;
        default:
            command=RDSR;    
            break;
    }    
	W25QXX_CSL;
	delay_us(5);
  byte=spifls_rw(command);
	byte=spifls_rw(0xFF);	
	W25QXX_CSH;                              
	return byte;   
}
static bool W25QXX_Wait_Busy(void)   
{   
	uint32_t overtimes=CYCLE_CNTS;
	while((W25QXX_ReadSR(1)&0x01)==0x01){
		        if (overtimes-- == 0) {
            //set_errornum_to_statword(STDBY_CFG_ERROR_CODE, flash_read_OT );
            debug_printf("warning:fls rdsr over time\r\n");
            return FALSE;
        }
	} 
  return TRUE;  
} 
static uint16_t W25QXX_ReadID(void)
{
  uint16_t tmdata=0;	
 if (!W25QXX_Wait_Busy())
      return FALSE;
 	W25QXX_CSL;
 delay_us(5);
 if(spifls_rw(ID)==-1)
	  return FALSE;
 if(spifls_rw(0x00)==-1)
	  return FALSE;
 if(spifls_rw(0x00)==-1)
	  return FALSE;
 if(spifls_rw(0x00)==-1)
	  return FALSE;
 tmdata|=spifls_rw(0xFF)<<8;
 tmdata|=spifls_rw(0xFF);
	W25QXX_CSH;				    
	return tmdata;
}

static uint16_t W25QXX_TYPE=W25Q80;
bool W25QXX_Init(void)
{ 
    uint8_t temp=0; 		
	SPI1_Init();
	W25QXX_TYPE=W25QXX_ReadID();
   if(W25QXX_TYPE==FALSE)
		    return FALSE;
    if(W25QXX_TYPE==W25Q256)               
    {
        temp=W25QXX_ReadSR(RDSR3);            
        if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25QXX_CSL; 			        //选中
			if(spi_trxfer_overtime(SPI0)!=TRUE)
         return FALSE;
			spi_i2s_data_transmit(SPI0,W25X_Enable4ByteAddr);//发送进入4字节地址模式指令		
			W25QXX_CSH;       		          
		}
    }
		return TRUE;
}

static bool W25QXX_Write_Enable(void)   
{
	W25QXX_CSL;                            //使能器件
	delay_us(5);
    //if(spi_trxfer_overtime(SPI0)!=TRUE)
       //return FALSE;	
 // spi_i2s_data_transmit(SPI0,WE);   //发送写使能
   if(spifls_rw(WE)==-1)
		  return FALSE;  
	W25QXX_CSH;                            //取消片选 
   return TRUE;		
} 
//W25QXX写禁止	
//将WEL清零  
static bool W25QXX_Write_Disable(void)   
{  
	W25QXX_CSL;                            //使能器件 
delay_us(5);	
    //if(spi_trxfer_overtime(SPI0)!=TRUE)
     //  return FALSE;	
  //spi_i2s_data_transmit(SPI0,WD);  //发送写禁止指令 
  if(spifls_rw(WD)==-1)
		 return FALSE; 	
	W25QXX_CSH;                            //取消片选 
   return TRUE;		
} 
/*
static bool remov_pt(uint8_t reg,uint16_t srv)
{
	if(!W25QXX_Write_Enable())
		return FALSE;
		W25QXX_CSL;                           
    delay_us(5);	
	  if(spifls_rw(reg)==-1)
		 return FALSE; 
	 	if(spifls_rw(srv)==-1)
		 return FALSE;
   W25QXX_CSH;
		   if (!W25QXX_Wait_Busy())
      return FALSE;
	W25QXX_Write_Disable();
		 return TRUE;			 
}*/
bool W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
// if (!W25QXX_Wait_Busy())
       //return FALSE;
 W25QXX_CSL;
	delay_us(5);
 //delay_us(2);
    if(spifls_rw(RD)==-1)
			  return FALSE;		  
    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
			   //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;
				 if(spifls_rw((uint8_t)((ReadAddr)>>24))==-1)
			        return FALSE;	
        //spi_i2s_data_transmit(SPI0,(uint8_t)((ReadAddr)>>24));    
    }
					  // if(!spi_trxfer_overtime(SPI0))
          //return FALSE;
    //spi_i2s_data_transmit(SPI0,(uint8_t)((ReadAddr)>>16));   //发送24bit地址 
		if(spifls_rw((uint8_t)((ReadAddr)>>16))==-1)
			  return FALSE;	
			   //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;						 
    //spi_i2s_data_transmit(SPI0,(uint8_t)((ReadAddr)>>8));
		if(spifls_rw((uint8_t)((ReadAddr)>>8))==-1)
			  return FALSE;	
			   //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;				 
    //spi_i2s_data_transmit(SPI0,(uint8_t)ReadAddr);
     if(spifls_rw((uint8_t)ReadAddr)==-1)
			  return FALSE;	
    for(i=0;i<NumByteToRead;i++)
	{ 
		   /* if(!spi_trxfer_overtime(SPI0))
          return FALSE;
				spi_i2s_data_transmit(SPI0,0xff);
		    if(spi_rxfer_overtime(SPI0))
        pBuffer[i]=spi_i2s_data_receive(SPI0);    //循环读数
				else 
					return FALSE;*/
		   pBuffer[i]=spifls_rw(0xff);
    }
	W25QXX_CSH;
  return TRUE;		
}
static bool W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;
    if(!W25QXX_Write_Enable())                 //SET WEL 
			  return FALSE;
	W25QXX_CSL;                            //使能器件 
		delay_us(5);
		    //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;		
    //spi_i2s_data_transmit(SPI0,PP);   //发送写页命令
	  if(spifls_rw(PP)==-1)
			  return FALSE;	
    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
					    //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;
       // spi_i2s_data_transmit(SPI0,(uint8_t)((WriteAddr)>>24));
       if(spifls_rw((uint8_t)((WriteAddr)>>24))==-1)
			  return FALSE;				
    }
				    //if(!spi_trxfer_overtime(SPI0))
         // return FALSE;
     //spi_i2s_data_transmit(SPI0,(uint8_t)((WriteAddr)>>16)); //发送24bit地址 
        if(spifls_rw((uint8_t)((WriteAddr)>>16))==-1)
			  return FALSE;			
		    //if(!spi_trxfer_overtime(SPI0))
         // return FALSE;						
     //spi_i2s_data_transmit(SPI0,(uint8_t)((WriteAddr)>>8));
		if(spifls_rw((uint8_t)((WriteAddr)>>8))==-1)
			  return FALSE;	
		    //if(!spi_trxfer_overtime(SPI0))
          //return FALSE;				
    // spi_i2s_data_transmit(SPI0,(uint8_t)WriteAddr);
     if(spifls_rw((uint8_t)(WriteAddr))==-1)
			  return FALSE;			
    for(i=0;i<NumByteToWrite;i++){
			 //if(spi_trxfer_overtime(SPI0))
			   //spi_i2s_data_transmit(SPI0,pBuffer[i]);//循环写数			 
			 //else 
				  //return FALSE;
			if(spifls_rw(pBuffer[i])==-1)
			  return FALSE;	
		}			
	W25QXX_CSH;                            //取消片选 
	   if (!W25QXX_Wait_Busy())
      return FALSE; 
	W25QXX_Write_Disable();
		 return TRUE;
}
static bool W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=FLSPGSZ-WriteAddr%FLSPGSZ; 	 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		if(!W25QXX_Write_Page(pBuffer,WriteAddr,pageremain))
			    return FALSE;
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>FLSPGSZ)pageremain=FLSPGSZ; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};
  return TRUE;	
} 
static bool W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=FLSSCSZ;
	 if(!W25QXX_Wait_Busy())
		 return FALSE;
    if(!W25QXX_Write_Enable())                 //SET WEL
       	return FALSE;		
  	W25QXX_CSL;                            //使能器件
		delay_us(10);
    //if(spi_trxfer_overtime(SPI0)!=TRUE)
      // return FALSE;			
    //spi_i2s_data_transmit(SPI0,SE);   //发送扇区擦除指令 
    	if(spifls_rw(SE)==-1)
			  return FALSE;	
		if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
			// if(spi_trxfer_overtime(SPI0)!=TRUE)
       //return FALSE;
       //spi_i2s_data_transmit(SPI0,(uint8_t)((Dst_Addr)>>24));
    	if(spifls_rw((uint8_t)((Dst_Addr)>>24))==-1)
			  return FALSE;				
    }
		    //if(!spi_trxfer_overtime(SPI0))
       //return FALSE;
    //spi_i2s_data_transmit(SPI0,(uint8_t)((Dst_Addr)>>16));  //发送24bit地址 
		    	if(spifls_rw((uint8_t)((Dst_Addr)>>16))==-1)
			  return FALSE;	
    //if(!spi_trxfer_overtime(SPI0))
       //return FALSE;				
   //spi_i2s_data_transmit(SPI0,(uint8_t)((Dst_Addr)>>8));
					    	if(spifls_rw((uint8_t)((Dst_Addr)>>8))==-1)
			  return FALSE;	
   // if(!spi_trxfer_overtime(SPI0))
      // return FALSE;		
   //spi_i2s_data_transmit(SPI0,(uint8_t)Dst_Addr); 
    	if(spifls_rw((uint8_t)(Dst_Addr))==-1)
			  return FALSE;									
	W25QXX_CSH;                            //取消片选     	      
    if (!W25QXX_Wait_Busy())
      return FALSE;   				    //等待擦除完成
		W25QXX_Write_Disable();
	return TRUE;
}
//static uint8_t W25QXX_BUFFER[FLSSCSZ];
bool W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i,j;
	uint8_t W25QXX_BUFFER[FLSSCSZ]={0};
  uint8_t * W25QXX_BUF;
  W25QXX_BUF=W25QXX_BUFFER;	
 	secpos=WriteAddr/FLSSCSZ;  
	secoff=WriteAddr%FLSSCSZ;
	secremain=FLSSCSZ-secoff;
 if (!W25QXX_Wait_Busy()){
	    	W25QXX_BUF=NULL;
      return FALSE;
    }
   // if(!remov_pt(WRSR,0x0000))
			//  return FALSE; 
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);
		//spifls_rw(0xFF);	
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		if(!W25QXX_Read(W25QXX_BUF,secpos*FLSSCSZ,FLSSCSZ)){
				W25QXX_BUF=NULL;
			return FALSE;
		}
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
					for(j=0;j<secremain;j++)	   //复制
			{				
				W25QXX_BUF[j+secoff]=*pBuffer;
          pBuffer++;				
			}
		if(i<secremain)//需要擦除
		{
			if(!W25QXX_Erase_Sector(secpos)){
					W25QXX_BUF=NULL;
				return FALSE;
			}
			//for(i=0;i<secremain;i++)	   //复制
			//{				
			//	W25QXX_BUF[i+secoff]=*pBuffer;
         // pBuffer++;				
			//}
			//if(!W25QXX_Write_NoCheck(W25QXX_BUF,secpos*FLSSCSZ,FLSSCSZ)){
					//W25QXX_BUF=NULL;
				//return FALSE;
			//}
		}
		if(!W25QXX_Write_NoCheck(W25QXX_BUF,WriteAddr,secremain)){
		
		          W25QXX_BUF=NULL;
		          return FALSE;
		         }			
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	//pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>FLSSCSZ)secremain=FLSSCSZ;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};
	//W25QXX_BUF=NULL;
	  //free(W25QXX_BUF);
	 //free(W25QXX_BUFFER);
  return TRUE;	
}
