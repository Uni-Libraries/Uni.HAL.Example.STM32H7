//
// Includes
//

// FreeRTOS
#include <FreeRTOS_IP.h>

// ox
#include "config/app_config_telemetry.h"
#include "io/uni_hal_io_stdio.h"
#include "rng/ox_rng.h"
#include "telemetry/ox_telemetry.h"


//
// Hooks/IP
//

void vApplicationIPNetworkEventHook_Multi( eIPCallbackEvent_t eNetworkEvent,
                                           struct xNetworkEndPoint * pxEndPoint ) {
    (void) pxEndPoint;

    switch (eNetworkEvent) {
        case eNetworkUp:
            uni_hal_io_stdio_printf("LINK UP \r\n");
            ox_telemetry_set_u32(&g_ox_telemetry_ctx, OX_TELEMETRY_ID_NET_LINK_UP, 1);
            break;
        case eNetworkDown:
            uni_hal_io_stdio_printf("LINK DOWN \r\n");
            ox_telemetry_set_u32(&g_ox_telemetry_ctx, OX_TELEMETRY_ID_NET_LINK_UP, 0);
            break;
        default:
            break;
    }
}


uint32_t ulApplicationGetNextSequenceNumber( uint32_t , uint16_t , uint32_t , uint16_t  ) {
    uint32_t result = 0U;
    xApplicationGetRandomNumber(&result);
    return result;
}



//
// Hooks/DHCP
//

const char * pcApplicationHostnameHook( void ){
    return "STM32 H7";
}

eDHCPCallbackAnswer_t xApplicationDHCPHook_Multi( eDHCPCallbackPhase_t phase,struct xNetworkEndPoint * endpoint, IP_Address_t *  address) {
    (void)phase;
    (void)endpoint;
    (void)address;
    return eDHCPContinue;
}



//
// Hooks/Random
//

BaseType_t xApplicationGetRandomNumber( uint32_t *pulValue ) {
    BaseType_t xReturn = pdFAIL;
    if (pulValue != nullptr) {
        *pulValue = ox_rng_get_u32();
        xReturn = pdTRUE;
    }
    return xReturn;
}
