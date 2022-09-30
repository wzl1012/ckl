#ifndef EXC46_H
#define EXC46_H
#include "headfile.h"

#define EXCIDCMD   0x0F//(0xF0)
//uint8_t  excaucmd[10]={0x60,0x00,0x41,0xAA,0x5e,0x66,0xf4,0xb0,0x2b,0x48};
//#define EXCAUDCMD  uint8_t[10]={0x12,0x34,0x0b,0x2f,0x66,0x7a,0x55,0x82,0x00,0x06}//0001,0010,0011,0100,0000,1011,0010,1111,0110,0110,0111,1010,0101,0101,1000,0010,0000,0000,0000,0110b
#define EXCIDXRP   (0xC9129DA7) 
#define EXCPGDXRP  (0xA8914F48)
#define EXCPGDXR    (0xA7E03B1C)
#define EXCIDEPTGC  1111110100001111110110000001111110
#define EXCIDNEPTGC 1111110101011110001101101010111101

#define PCF7936_T_WAIT_BS (6000)
#define PCF7936_T_PROG				(40)
#define EXCPGRWCMD1  0x60
#define EXCPGRWCMD2  0x02
enum
{
	e_pcf7936_psw = 0x10,
	e_pcf7936_cipher,
};
/*enum{
	exc_pg0rwcmd=0x7a,  //1111010b,
	exc_pg1rwcmd=0x7e,//1111110b,
	exc_pg2rwcmd=0x72,//1110010
	exc_pg3rwcmd=0x76,//1110110
	exc_pg4rwcmd=0x6a,//1101010
	exc_pg5rwcmd=0x6e,//1101110
	exc_pg6rwcmd=0x62,//1100010
	exc_pg7rwcmd=0x66 //1100110
}exc_pgrwcmd_t;
typedef enum
{
	e_pwoff,
	e_idcfm,
	e_authed,
} exc46_stage_t;
typedef enum
{
	exc46_not,
	exc46_yes,
	exc46_cracked
}exc46_mode_t;
typedef struct
{
	exc46_stage_t exc46state;
	exc46_mode_t mode;
	uint8_t tmcf_enc;
	modulation_t tmcf_dcs;
	uint8_t a_page[8][4];
	uint8_t rgidc[4];			// MSbit:  :LSBit 
	uint8_t rid[4];		// MSbit:  :LSBit,hitag2:48bit,hitag2+:96bit
} exc46_tag_t;*/
typedef enum
{
	page0=0,
	page1,
	page2,
	page3,
	page4,
	page5,
	page6,
	page7,
	pages,
} pgnum_t;
extern void pcf7936_wr_msbits(const uint8_t *p_data, uint8_t bits, uint8_t stop_flag);
extern void copy_msbits(uint8_t *p_to, uint8_t offset_to, const uint8_t *p_from, uint8_t offset_from, uint8_t bits);
extern void pcf7936_to_off(void);
extern void hitag2_oneway2(unsigned char * addr, unsigned char bits);
extern void copy_bytes(uint8_t *p_to, const uint8_t *p_from, uint8_t bytes);
uint8_t exc46_wr(pcf7936_tag_t *p_tag,uint8_t pgstart,uint8_t pged,uint8_t *p_wr);
#endif



