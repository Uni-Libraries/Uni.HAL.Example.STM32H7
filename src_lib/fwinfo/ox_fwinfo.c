//
// Includes
//

// stdlib
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// uni.hal
#include <uni_hal.h>
#include "stm32h7xx.h"

// ox
#include "crypto/ox_hmac.h"
#include "fwinfo/ox_fwinfo.h"

//
// Defines
//

#define OX_FWINFO_OFFSET (1024U)

//
// Globals
//

extern uint8_t g_ox_crypto_key[OX_HMAC_HASH_BYTES];

//
// Private
//

const uint8_t *_ox_fwinfo_get_key()
{
    return g_ox_crypto_key;
}

//
// Functions
//

const uint8_t *ox_fwinfo_get_address(ox_fwinfo_type fw_type)
{
    const uint8_t *result = NULL;

    switch (fw_type)
    {
    case OX_FWINFO_CURRENT:
        result = (const uint8_t *)(FLASH_BASE);
        break;
    case OX_FWINFO_NEXT:
        result = (const uint8_t *)(FLASH_BASE + 1 * FLASH_BANK_SIZE);
        break;
    default:
        break;
    }

    return result;
}

const uint8_t *ox_fwinfo_get_hash(ox_fwinfo_type fw_type)
{
    const uint8_t *result = NULL;

    switch (fw_type)
    {
    case OX_FWINFO_CURRENT:
        result = (const uint8_t *)(FLASH_BASE + 1 * FLASH_BANK_SIZE - OX_HMAC_HASH_BYTES);
        break;
    case OX_FWINFO_NEXT:
        result = (const uint8_t *)(FLASH_BASE + 2 * FLASH_BANK_SIZE - OX_HMAC_HASH_BYTES);
        break;
    default:
        break;
    }

    return result;
}

const ox_fwinfo_t *ox_fwinfo_get_info(ox_fwinfo_type fw_type)
{
    const ox_fwinfo_t *result = NULL;

    switch (fw_type)
    {
    case OX_FWINFO_CURRENT:
        result = (const ox_fwinfo_t *)(FLASH_BASE + 1 * FLASH_BANK_SIZE - OX_FWINFO_OFFSET);
        break;
    case OX_FWINFO_NEXT:
        result = (const ox_fwinfo_t *)(FLASH_BASE + 2 * FLASH_BANK_SIZE - OX_FWINFO_OFFSET);
        break;
    default:
        break;
    }

    return result;
}

size_t ox_fwinfo_get_size(ox_fwinfo_type fw_type)
{
    size_t result = 0U;

    switch (fw_type)
    {
    case OX_FWINFO_CURRENT:
    case OX_FWINFO_NEXT:
        result = FLASH_BANK_SIZE;
        break;
    default:
        break;
    }

    return result;
}

bool ox_fwinfo_is_valid(ox_fwinfo_type fw_type)
{
    ox_hmac_context_t hmac_ctx;
    uint8_t calculated_hash[OX_HMAC_HASH_BYTES];

    if (!ox_hmac_init(&hmac_ctx, g_ox_crypto_key, sizeof(g_ox_crypto_key)))
    {
        return false;
    }

    const uint8_t* addr_start = ox_fwinfo_get_address(fw_type);
    const uint8_t* addr_end = addr_start + ox_fwinfo_get_size(fw_type) - OX_HMAC_HASH_BYTES;
    for (const uint8_t* addr = addr_start; addr < addr_end ; addr += 64*1024)
    {
        portYIELD();
        if (!ox_hmac_append(&hmac_ctx, (const void*)addr, uni_common_math_min(64*1024, addr_end-addr)))
        {
            ox_hmac_deinit(&hmac_ctx);
            return false;
        }
    }

    if (!ox_hmac_finish(&hmac_ctx, calculated_hash))
    {
        ox_hmac_deinit(&hmac_ctx);
        return false;
    }

    ox_hmac_deinit(&hmac_ctx);

    return memcmp(calculated_hash, ox_fwinfo_get_hash(fw_type), OX_HMAC_HASH_BYTES) == 0;
}
