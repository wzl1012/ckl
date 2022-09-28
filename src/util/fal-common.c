#include "fal-common.h"
//#include "falcon-clk.h"
#include "gd32f10x.h"
volatile uint8_t  g_dfxfercompltflg = 0;
volatile uint32_t g_mciramloadaddr = 0;
/*void set_errornum_to_statword(volatile uint32_t *addr, uint32_t errornum )
{
    *addr = (*addr & 0xff) | (errornum << 16);
}
void *aligned_malloc(uint32_t required_bytes, int alignment)
{
    uint32_t offset = alignment - 1 + sizeof(void *);
    void *p1 = (void *)malloc(required_bytes + offset);

    if (p1 == NULL) {
        return NULL;
    }

    void **p2 = (void **)(((size_t)p1 + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void *p2)
{
    void *p1 = ((void **)p2)[-1];
    free(p1);
}*/
/*******************************************************************************
*   Name: get_aligned_address
*  Brief: DMA mode encryt &decryt,AES/hmac keyaddr ,inputdata_addr,output_addr必须对齐，否则DMA 会出错
*  keyaddr alignment 32Byte,
*  inputdata_addr alignment 64Byte,
*  output_addr alignment 32Byte
*  Input: 1: srcaddr & size
*         2: alignment
*
* Output: new address
* Return: new address
*******************************************************************************/
/*
uint32_t get_aligned_address(uint32_t srcaddr, uint32_t size,
                             int alignment)
{
    uint32_t dstaddr = 0;
    uint32_t totalsize = size;

    if (0 == size) {
        set_errornum_to_statword(STDBY_CFG_ERROR_CODE, in_fsize_fail);
        debug_printf("error %d:input data length is 0 for address alignment \r\n", in_fsize_fail);

        while (1);
    }

    // 1:alignment为0 或者 地址刚好是对齐状态 且不需要做padding 不需要重新分配地址
    if ((0 == alignment) || (0 == srcaddr % alignment)) {
        dstaddr = srcaddr;
        return dstaddr;
    }

    void *p = aligned_malloc(totalsize, alignment);

    if (p) {
        memcpy(p, (void *)srcaddr, totalsize);
        dstaddr = (uint32_t)p;
        return dstaddr;
    }
    else {
        return 0;
    }
}
*/
uint32_t checksum(uint8_t* databuff,uint16_t sizes)
{
	  uint32_t chksum=0;
	  while(sizes--){
			chksum+=*databuff;
			databuff++;
		}
}
static volatile uint32_t s_highCounter = 0;
static volatile uint32_t s_tickPerMicrosecondMul8 = 0; //!< This value equal to 8 times ticks per microseconds
void microseconds_init(void)
{
    s_highCounter = 0;
    SysTick->LOAD = SysTick_LOAD_RELOAD_Msk; // Set reload register to max value
    SysTick->VAL = 0;                        // As per ARM reference initialization, set initial value to 0
    //  interrupts are only triggered when going from 1 to 0
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // Set timer to core clock frequency
                    SysTick_CTRL_TICKINT_Msk |   // Enable interrupts on timeout
                    SysTick_CTRL_ENABLE_Msk;     // Enable SysTick IRQ and SysTick Timer
    s_tickPerMicrosecondMul8 = (SystemCoreClock * 8) / Frequency_1MHz;

    // Make sure this value is greater than 0
    if (!s_tickPerMicrosecondMul8) {
        s_tickPerMicrosecondMul8 = 1;
    }
}
//! @brief Shutdown the microsecond timer
void microseconds_shutdown(void)
{
    // Disable the timer and interrupts from it
    SysTick->CTRL = SysTick->CTRL & ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
    // Clear the current value register
    SysTick->VAL = 0;
}
//! @brief Read back the running tick count
uint32_t microseconds_get_ticks(void)
{
    uint32_t retVal;
    uint32_t high;
    uint32_t low;

    // Check for an overflow condition between the two reads above
    do {
        high = s_highCounter;
        low = (~SysTick->VAL) & SysTick_LOAD_RELOAD_Msk;
    }
    while (high != s_highCounter);

    retVal = (high << 24) + low;
    return retVal;
}

//! @brief Delay specified time
//!
//! @param us Delay time in microseconds unit
void microseconds_delay(uint32_t us)
{
    uint32_t currentTicks = microseconds_get_ticks();
    //! The clock value in Mhz = ticks/microsecond
    uint32_t ticksNeeded = (us * s_tickPerMicrosecondMul8 / 8) + currentTicks;

    while (microseconds_get_ticks() < ticksNeeded) {
        ;
    }
}
//! @brief Returns the conversion of miliseconds to ticks
uint32_t miliseconds_convert_to_ticks(uint32_t miliseconds)
{
    return (miliseconds * 1000 * s_tickPerMicrosecondMul8 / 8);
}
//! @brief Interrupt handler for the SysTick timer, this will just increment
//         the rollover counter for extended time keeping
/*void SysTick_Handler(void)
{
    s_highCounter++;
}*/


