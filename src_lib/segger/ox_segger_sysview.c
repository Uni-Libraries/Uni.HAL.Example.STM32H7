//
// Includes
//

// stdlib
#include <stdint.h>

// segger
//#include "SEGGER_SYSVIEW.h"

// ox
#include "segger/ox_segger_sysview.h"



//
// Globals
//

extern uint32_t SystemCoreClock;

//extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;



//
// Hooks
//

//static void _cbSendSystemDesc(void) {
//    SEGGER_SYSVIEW_SendSysDesc("N=OS-XBAND-FWv1,D=OS-XBAND-1,O=FreeRTOS");
//    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#30=IRQ_DMA1_S3,I#77=IRQ_ETH,I#100=IRQ_SPI6");
//}



//
// Functions
//

void ox_segger_sysview_init(void) {
//    SEGGER_SYSVIEW_Init(SystemCoreClock, SystemCoreClock, &SYSVIEW_X_OS_TraceAPI, _cbSendSystemDesc);
//    SEGGER_SYSVIEW_SetRAMBase(0x20000000);
}
