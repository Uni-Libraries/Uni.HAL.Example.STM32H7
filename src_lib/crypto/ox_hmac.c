//
// Includes
//

// ox
#include "crypto/ox_hmac.h"



//
// Statics
//

static const mbedtls_md_type_t g_md_type = MBEDTLS_MD_SHA256;

//
// Functions
//

bool ox_hmac_init(ox_hmac_context_t *ctx, const uint8_t *key, size_t key_len) {
    if(!ctx) {
        return false;
    }

    ctx->state.initialized = false;
    mbedtls_md_init(&ctx->state.md_ctx);

    if (mbedtls_md_setup(&ctx->state.md_ctx, mbedtls_md_info_from_type(g_md_type), 1) != 0) {
        return false;
    }

    if (mbedtls_md_hmac_starts(&ctx->state.md_ctx, key, key_len) != 0) {
        return false;
    }

    ctx->state.initialized = true;
    return true;
}

void ox_hmac_deinit(ox_hmac_context_t *ctx) {
    if (ctx && ctx->state.initialized) {
        mbedtls_md_free(&ctx->state.md_ctx);
        ctx->state.initialized = false;
    }
}

bool ox_hmac_append(ox_hmac_context_t *ctx, const void *data, size_t data_len) {
    if (!ctx || !ctx->state.initialized) {
        return false;
    }
    return mbedtls_md_hmac_update(&ctx->state.md_ctx, (const uint8_t *)data, data_len) == 0;
}

bool ox_hmac_reset(ox_hmac_context_t *ctx) {
    if (!ctx || !ctx->state.initialized) {
        return false;
    }
    return mbedtls_md_hmac_reset(&ctx->state.md_ctx) == 0;
}

bool ox_hmac_finish(ox_hmac_context_t *ctx, uint8_t *out_hash) {
    if (!ctx || !ctx->state.initialized || !out_hash) {
        return false;
    }
    return mbedtls_md_hmac_finish(&ctx->state.md_ctx, out_hash) == 0;
}
