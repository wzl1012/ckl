#ifndef ICWR_H
#define ICWR_H
#include "headfile.h"
typedef enum
{
	e_modu_mc = 0x10,
	e_modu_bp
} modulation_t;
typedef enum
{
	e_power_off,
	e_idcfm,
	e_auth_half,
	e_authorized,
	e_read_all
} pcf7936_stage_t;
typedef enum
{
	e_pcf7936_not,
	e_pcf7936_yes,
	e_pcf7936_ts46_blank,
	e_pcf7936_cracked
} pcf7936_mode_t;
typedef struct
{
	pcf7936_stage_t now_state;
	pcf7936_mode_t mode;
	uint8_t tmcf_enc;
	modulation_t tmcf_dcs;
	uint8_t a_page[8][4];
	uint8_t random[4];			// MSbit:  :LSBit ,hitag2:32bit,hitag2+:64bit
	uint8_t secret_key[6];		// MSbit:  :LSBit,hitag2:48bit,hitag2+:96bit
	uint8_t cartype;
	//uint8_t hitagmode;
} pcf7936_tag_t;
typedef enum
{
	e_no_em4170,
	e_yes_em4170
} em4170_mode_t;
typedef struct
{
	em4170_mode_t mode;
	uint8_t crypt_ori;
	uint8_t a_id[4];
	uint8_t a_um1[4];
	uint8_t a_um2[8];
	uint8_t key[12];
} em4170_tag_t;
typedef enum
{
	e_tms37145_not,
	e_tms37145_pwm,
	e_tms37145_ppm,
	e_tms37145_b9wk,
	e_b9wk			// 4d tag
} tms37145_mode_t;

typedef struct
{
	uint8_t read_len;
	uint8_t crack_flag;
	uint8_t crack_type;
	tms37145_mode_t mode;
	uint8_t page1[2];
	uint8_t page2[2];
	uint8_t page3[5];		// [0]-->[3] == MSByte-->LSByte
	uint8_t page4[6];		// [0]-->[4] == MSByte-->LSByte
	uint8_t signature[4][3];
	uint8_t page8[5];
	uint8_t page18;
	uint8_t page30[2];
} tms37145_tag_t;
extern em4170_tag_t em4170_tag1;
extern tms37145_tag_t tms37145_tag1;
extern pcf7936_tag_t pcf7936_tag1;

#endif

