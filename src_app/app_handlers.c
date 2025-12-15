//
// Includes
//

// stdlib
#include <stdint.h>



//
// Hooks
//

void HardFault_Handler(void)
{
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}

void MemManage_Handler(void)
{
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}

void BusFault_Handler(void)
{
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}

void UsageFault_Handler(void)
{
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}

void DebugMon_Handler(void)
{
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}

void __assert_func(const char *, int, const char *, const char *) {
    volatile uint32_t c = 0;
    while(c == 0U){

    }
}
