#pragma once

//
// Includes
//

// unimcu
#include <uni_hal.h>
#include <segger/uni_hal_segger_rtt_stdio.h>


//
// Globals
//

UNI_HAL_IO_DECLARATION(g_ox_segger_stdio_io);
extern uni_hal_segger_context_t g_ox_segger_stdio_ctx;
