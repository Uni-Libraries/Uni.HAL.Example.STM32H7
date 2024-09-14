#pragma once

//
// Includes
//

// stdlib
#include <stdint.h>



//
// Functions
//

bool ox_rng_init();
bool ox_rng_is_inited();
uint32_t ox_rng_get_u32();
