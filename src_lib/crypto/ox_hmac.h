#pragma once

#if defined(__cplusplus)
extern "C" {

#endif

//
// Includes
//

// stdlib
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// mbedtls
#include <mbedtls/md.h>


//
// Defines
//

#define OX_HMAC_HASH_BYTES (32)

//
// Typedefs
//

typedef struct {
    bool initialized;
    mbedtls_md_context_t md_ctx;
} ox_hmac_state_t;

typedef struct {
    ox_hmac_state_t state;
} ox_hmac_context_t;


//
// Functions
//

bool ox_hmac_init(ox_hmac_context_t *ctx, const uint8_t *key, size_t key_len);

void ox_hmac_deinit(ox_hmac_context_t *ctx);

bool ox_hmac_append(ox_hmac_context_t *ctx, const void *data, size_t data_len);

bool ox_hmac_reset(ox_hmac_context_t *ctx);

bool ox_hmac_finish(ox_hmac_context_t *ctx, uint8_t *out_hash);


#if defined(__cplusplus)
}
#endif
