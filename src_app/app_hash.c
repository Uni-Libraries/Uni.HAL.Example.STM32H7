//
// Includes
//

// ox
#include "config/app_config_crypto.h"
#include "crypto/ox_hmac.h"


//
// Globals
//

__attribute__((section(".FLASH_HMAC_SECTION")))
uint8_t g_app_hmac_hash[OX_HMAC_HASH_BYTES] = {0};
