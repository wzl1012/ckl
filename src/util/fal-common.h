#ifndef  _FALCON_COMMON_H
#define  _FALCON_COMMON_H
#include <stdint.h>
#include <stdlib.h>
#include "debug.h"
#define  flssections      64
#define  sectsize          262144
#define  flslastsectadr  (uint32_t)(flssections*(sectsize-1))
#define  flserronumsavadr    (uint32_t *)((flslastsectadr+sectsize)-1)
#define  errornumsavadr      flslastsect*sectsize
#define  Frequency_1MHz   1000000u
#define    CYCLE_CNTS     0x0FFFFFFF
/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))
/*! @brief boot version 1.0.0. */
#define FAL_BOOT_MAJOR_VERSION  1
#define FAL_BOOT_MINOR_VERSION  0
#define FAL_BOOT_BUGFIX_VERSION 3
/*@}*/
#define CHECKRESULT(result)                                            \
  if (result == 0) {                                                        \
    debug_printf("[%s %d] function exec failed\r\n", __FILE__, __LINE__);   \
    return (0);                                                             \
  } else{ \
      if (g_DEBUG){\
      debug_printf("(%s %d) function exec pass\r\n", __FILE__, __LINE__);\
      }\
  }
//extern volatile uint8_t  g_dfxfercompltflg;
//extern  volatile uint32_t g_mciramloadaddr;
/*! @brief Status group numbers. */
enum _status_groups {
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_MSHC = 1,                    /*!< Group number for SDMMC status code */
};
enum _processstatus {
    failstauts  = 0,                 /*!< process fail. */
    OKstatus = 1                   /*!<process succeed. */
};
enum _enablesw {
    disable  = 0,                 /*!< disable sw. */
    enable = 1                   /*!<enable sw. */
};

void *aligned_malloc(uint32_t required_bytes, int alignment);
void aligned_free(void *p2);
uint32_t get_aligned_address(uint32_t srcaddr, uint32_t size, int alignment);
#define CHECKALIGN(srcaddr, alignment)                                         \
  if ((0 != alignment) && (0 != srcaddr % alignment)) {                        \
      set_errornum_to_statword(STDBY_CFG_ERROR_CODE, srcaddr_alig_NG);  \
      debug_printf("error %d: address[0x%x] Not aligned [%d]!!\n",srcaddr_alig_NG,srcaddr, alignment);      \
    while(1);                                                                  \
  }
void set_errornum_to_statword(volatile uint32_t *addr, uint32_t errornum );
void microseconds_init(void);
void microseconds_shutdown(void);
uint32_t microseconds_get_ticks(void);
uint32_t miliseconds_convert_to_ticks(uint32_t miliseconds);
void microseconds_delay(uint32_t us);
uint32_t checksum(uint8_t* databuff,uint16_t sizes);

#endif
