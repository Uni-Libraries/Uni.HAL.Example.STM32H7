//
// Includes
//

// stdlib
#include <stdlib.h>

// Uni.HAL
#include "uni_hal.h"

// ox
#include "ox_rng.h"



//
// Globals
//

uni_hal_rng_context_t g_ox_rng_ctx = {
        .instance = UNI_HAL_CORE_PERIPH_RNG,
        .inited = false,
};



//
// Functions
//

bool ox_rng_init() {
    bool result = false;
    if (!ox_rng_is_inited()) {
        //FIXME: sometimes DRDY is not set -> leads to crash
        //bsusat_rng_init(&g_ox_rng_ctx);

        srand(uni_hal_dwt_get_tick());
        g_ox_rng_ctx.inited = true;
    }
    return result;
}


bool ox_rng_is_inited(){
    return g_ox_rng_ctx.inited;
}


uint32_t ox_rng_get_u32() {
    ox_rng_init();
    return rand();
}
