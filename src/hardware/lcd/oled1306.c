#include "headfile.h"
#include  "oled128X32font.h"
#define  OLED_CMD  0x00
//#define  DIS_V    0x00000002
//uint32_t const vdata __attribute__ ((section(".ARM.__at_0x08016400")))=DIS_V;
uint8_t OLED_GRAM[128][4]={0};	
static void OLED_1306_iic_write(uint8_t data,uint8_t wtype)
{
	uint8_t s_data[2]={0};
	  s_data[0]=wtype;
	  s_data[1]=data;
	i2c_write(I2C1,s_data,2);
}
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_1306_iic_write (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_1306_iic_write (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_1306_iic_write (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_1306_iic_write(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_1306_iic_write (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_1306_iic_write(0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_1306_iic_write(0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_1306_iic_write(0,OLED_DATA); 
	} //������ʾ
}
void hex_to_char(const uint8_t *p_hex, uint8_t *p_ch, uint8_t bytes)
{
	uint8_t temp;
	
	while (bytes) {
		temp = (*p_hex) >> 4;
		if (temp <= 9)
			temp += '0';
		else
			temp += ('A' - 10);
		*p_ch++ = temp;
		
		temp = (*p_hex) & 0x0F;
		if (temp <= 9)
			temp += '0';
		else
			temp += ('A' - 10);
		*p_ch++ = temp;
		
		p_hex++;
		bytes--;
	}
	//p_ch[i*2]='\0';
}
void oled_print_hex(uint8_t x, uint8_t y,uint8_t size, const uint8_t *p_hex, uint8_t bytes)
{
	uint8_t a_ch[Max_Row];
	
	hex_to_char(p_hex, a_ch, bytes);
	OLED_ShowString(x, y,a_ch,size,bytes*2);
}
/*
void oled1306_init(void)
{
	delay_us(100000);
	OLED_1306_iic_write(0xAE,OLED_CMD);//�ر���ʾ
	OLED_1306_iic_write(0x40,OLED_CMD);//---set low column address
	OLED_1306_iic_write(0xB0,OLED_CMD);//---set high column address

	OLED_1306_iic_write(0xC8,OLED_CMD);//-not offset

	OLED_1306_iic_write(0x81,OLED_CMD);//���öԱȶ�
	OLED_1306_iic_write(0xff,OLED_CMD);

	OLED_1306_iic_write(0xa1,OLED_CMD);//���ض�������

	OLED_1306_iic_write(0xa6,OLED_CMD);//
	
	OLED_1306_iic_write(0xa8,OLED_CMD);//��������·��
	OLED_1306_iic_write(0x1f,OLED_CMD);
	
	OLED_1306_iic_write(0xd3,OLED_CMD);
	OLED_1306_iic_write(0x00,OLED_CMD);
	
	OLED_1306_iic_write(0xd5,OLED_CMD);
	OLED_1306_iic_write(0xf0,OLED_CMD);
	
	OLED_1306_iic_write(0xd9,OLED_CMD);
	OLED_1306_iic_write(0x22,OLED_CMD);
	
	OLED_1306_iic_write(0xda,OLED_CMD);
	OLED_1306_iic_write(0x02,OLED_CMD);
	
	OLED_1306_iic_write(0xdb,OLED_CMD);
	OLED_1306_iic_write(0x49,OLED_CMD);
	
	OLED_1306_iic_write(0x8d,OLED_CMD);
	OLED_1306_iic_write(0x14,OLED_CMD);
	
	OLED_1306_iic_write(0xaf,OLED_CMD);
	OLED_Clear();
	OLED_ShowString(0,0,dis_v,16,16);
	OLED_ShowString(0,2,"SYZ",16,3);
	oled_print_hex(24,2,16,temp,4);
	delay_us(1500000);
}
*/
// fill_Picture

void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_1306_iic_write(0xb0+m,0);		//page0-page1
		OLED_1306_iic_write(0x00,0);		//low column start address
		OLED_1306_iic_write(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_1306_iic_write(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/

//��������

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_1306_iic_write(0xb0+y,OLED_CMD);
	OLED_1306_iic_write(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_1306_iic_write((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_1306_iic_write(0X8D,OLED_CMD);  //SET DCDC����
	OLED_1306_iic_write(0X14,OLED_CMD);  //DCDC ON
	OLED_1306_iic_write(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_1306_iic_write(0X8D,OLED_CMD);  //SET DCDC����
	OLED_1306_iic_write(0X10,OLED_CMD);  //DCDC OFF
	OLED_1306_iic_write(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_1306_iic_write (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_1306_iic_write (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_1306_iic_write (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_1306_iic_write(1,OLED_DATA); 
	} //������ʾ
}
/*static void OLED_DrawP(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>31)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}*/
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;
  //uint8_t temp=0,t=0,t1=0,y0=y,mode=1;
	// uint8_t  csize=(Char_Size/8+((Char_Size%8)?1:0))*(Char_Size/2);
	  if(chr!=' ')
		c=chr-'\'';//
    else
      c=0;			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_1306_iic_write(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_1306_iic_write(F8X16[c*16+i+8],OLED_DATA);
			}else if(Char_Size ==24){
         /* for(t=0;t<csize;t++)
         {   
            temp=asc2_2412[c][t];	//����2412����
            for(t1=0;t1<8;t1++)
		        {
			       if(temp&0x80)OLED_DrawP(x,y,mode);
			        else OLED_DrawP(x,y,!mode);
			         temp<<=1;
			          y++;
			          if((y-y0)==Char_Size)
			         {
				          y=y0;
				           x++;
				          break;
			          }
		           }  	 
           }*/
			OLED_Set_Pos(x,y);	
			for(i=0;i<10;i++)				
				OLED_1306_iic_write(asc2_2412[c*30+i],OLED_DATA);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<10;i++)				
				OLED_1306_iic_write(asc2_2412[c*30+i+10],OLED_DATA);
				OLED_Set_Pos(x,y+2);
				for(i=0;i<10;i++)				
				OLED_1306_iic_write(asc2_2412[c*30+i+20],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_1306_iic_write(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=1;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
/*void OLED_ShowStr(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(32-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size);	 
        x+=size/2;
        p++;
    }  
	
}*/
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size,uint8_t len)
{
	unsigned char j=0;
	while (chr[j]!='\0'&&len--)
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
		if(Char_Size!=24)
			x+=8;
		else
			x+=10;
		if(Char_Size!=24&&x>120){x=0;y+=2;}
		if(Char_Size==24&&x>118){x=0;y+=2;}
			j++;
	}
}
void OLED_DrawxsPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>31)return;//������Χ��.
	pos=3-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
OLED_1306_iic_write(OLED_GRAM[x][pos],OLED_DATA);		
}
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>31)return;//������Χ��.
	pos=3-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}

void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
void OLED_drawline(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)
{      	
	uint8_t x,y,pos;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
		for(pos=0;pos<4;pos++){
		OLED_Set_Pos(x,pos);
		OLED_1306_iic_write(OLED_GRAM[x][pos],OLED_DATA);
		}
	}				
}
//��ʾ����
/*
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_1306_iic_write(Hzk[2*no][t],OLED_DATA);
				//adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_1306_iic_write(Hzk[2*no+1][t],OLED_DATA);
				//adder+=1;
      }					
}*/
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
 //if(y1%8==0) y=y1/8;      
  //else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_1306_iic_write(BMP[j++],OLED_DATA);	    	
	    }
	}
} 




