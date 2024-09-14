//
// Includes
//

// ox app
#include "config/app_config_segger.h"



//
// Globals
//

UNI_HAL_IO_DEFINITION(g_ox_segger_stdio_io, 64, 512);

static uni_hal_tim_context_t  g_ox_segger_stdio_tim_ctx = {
    .config = {
        .instance = UNI_HAL_CORE_PERIPH_TIM_15,
        .prescaler = 256,
        .reload_value = 1024,
},
.status = {
        .inited = false,
}
};

uni_hal_segger_context_t g_ox_segger_stdio_ctx = {
    .buffer_index_down = 0,
    .buffer_index_up = 0,
    .initialized = false,
    .timer = &g_ox_segger_stdio_tim_ctx,
    .io = &g_ox_segger_stdio_io_ctx,
};
