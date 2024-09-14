//
// Includes
//

// stdlib
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


// uni.hal
#include <uni_common.h>
#include <uni_hal.h>

// ox
#include "crypto/ox_hmac.h"
#include "server/ox_server_updater.h"
#include "fwinfo/ox_fwinfo.h"
#include "ox_asset_updater.h"



//
// Defines
//

#define FLASH_WRITE_ALIGN (32)
#define FLASH_START_ADDR (FLASH_BASE + FLASH_BANK_SIZE)
#define FLASH_END_ADDR (FLASH_BASE + FLASH_BANK_SIZE)


//
// Private
//

static uint32_t g_app_flash_write_off = 0;
static uint8_t g_app_flash_buf[FLASH_WRITE_ALIGN];
static size_t g_app_flash_buf_fill = 0;

static size_t _ox_server_updater_handler_upload(void* userdata, uint8_t* buf_out, size_t buf_out_size, const uint8_t* buf_in, size_t buf_in_len)
{
    size_t result = 0U;

    (void)userdata;

    if (buf_in != NULL && g_app_flash_write_off == 0) {
        uni_hal_flash_erase_bank();
    }

    // In case buf_out != nullptr -> we are finished -> write total count of bytes
    if (buf_out != NULL && buf_out_size > 0)
    {
        int true_size =  g_app_flash_write_off + g_app_flash_buf_fill;

        // check if we have any data to write
        if (g_app_flash_buf_fill)
        {
            // pad
            memset(&g_app_flash_buf[g_app_flash_buf_fill], 0xFF, FLASH_WRITE_ALIGN - g_app_flash_buf_fill);  // up to 32
            if (uni_hal_flash_write(FLASH_START_ADDR + g_app_flash_write_off, FLASH_WRITE_ALIGN, g_app_flash_buf) != FLASH_WRITE_ALIGN)
            {
                 //printf("Flash write failed at offset %d\r\n", g_app_flash_write_off);
            }
        }
        
        g_app_flash_buf_fill  = 0;
        g_app_flash_write_off = 0;

        if (true_size != (1 * 1024 * 1024)) {
            result = uni_hal_io_stdio_snprintf((char*)buf_out, buf_out_size, "FAIL: Invalid firmware size");
        } else {
            result = uni_hal_io_stdio_snprintf((char*)buf_out, buf_out_size, "%d", true_size);
        }
    }
    // Buf_in != nullptr -> in progress -> write to flash
    else if (buf_in != NULL && buf_in_len > 0)
    {
        size_t in_idx = 0;

        // finish block from previous call
        if (g_app_flash_buf_fill)
        {
            // copy needed data
            size_t need = uni_common_math_min(FLASH_WRITE_ALIGN - g_app_flash_buf_fill, buf_in_len);
            memcpy(&g_app_flash_buf[g_app_flash_buf_fill], &buf_in[in_idx], need);
            g_app_flash_buf_fill += need;
            in_idx               += need;

            if (uni_hal_flash_write(FLASH_START_ADDR + g_app_flash_write_off, FLASH_WRITE_ALIGN, g_app_flash_buf) != FLASH_WRITE_ALIGN)
            {
                //printf("Flash write failed at offset %d\r\n", g_app_flash_write_off);
                return 0;
            }
            g_app_flash_write_off += FLASH_WRITE_ALIGN;
            g_app_flash_buf_fill   = 0;
        }

        portYIELD();

        // program whole blocks
        while (in_idx + FLASH_WRITE_ALIGN <= buf_in_len)
        {
            if (uni_hal_flash_write(FLASH_START_ADDR + g_app_flash_write_off, FLASH_WRITE_ALIGN, (uint8_t *)&buf_in[in_idx]) != FLASH_WRITE_ALIGN) {
                //printf("Flash write failed at offset %d\r\n", g_app_flash_write_off);
                return 0;
            }
            in_idx               += FLASH_WRITE_ALIGN;
            g_app_flash_write_off += FLASH_WRITE_ALIGN;
        }


        portYIELD();

        // cache remaining part
        g_app_flash_buf_fill = buf_in_len - in_idx;
        if (g_app_flash_buf_fill) {
            memcpy(g_app_flash_buf, &buf_in[in_idx], g_app_flash_buf_fill);
        }
    }
    return result;
}

static size_t _ox_server_updater_handler_apply(void* userdata, uint8_t* buf_out, size_t buf_out_size, const uint8_t* buf_in, size_t buf_in_len)
{
    (void)userdata;
    (void)buf_in;
    (void)buf_in_len;

    portYIELD();
    if (ox_fwinfo_is_valid(OX_FWINFO_NEXT)) {
        uni_hal_flash_swap_banks();
        return uni_hal_io_stdio_snprintf((char*)buf_out, buf_out_size, "OK");
    } else {
        return uni_hal_io_stdio_snprintf((char*)buf_out, buf_out_size, "FAIL: HMAC is not valid");
    }
}

static size_t _ox_server_updater_handler_status(void* userdata, uint8_t* buf_out, size_t buf_out_size, const uint8_t* buf_in, size_t buf_in_len)
{
    (void)userdata;
    (void)buf_in;
    (void)buf_in_len;

    // current
    char current_ver_str[64 + 1] = {0};
    bool is_current_valid = ox_fwinfo_is_valid(OX_FWINFO_CURRENT);
    if (is_current_valid) {
        const ox_fwinfo_t* fw_info_current = ox_fwinfo_get_info(OX_FWINFO_CURRENT);
        strcpy(current_ver_str, fw_info_current->git_version);
    }

    // next
    char next_ver_str[64 + 1] = {0};
    bool is_next_valid = ox_fwinfo_is_valid(OX_FWINFO_NEXT);
    if (is_next_valid) {
        const ox_fwinfo_t* fw_info_next = ox_fwinfo_get_info(OX_FWINFO_NEXT);
        strcpy(next_ver_str, fw_info_next->git_version);
    }

    return uni_hal_io_stdio_snprintf((char*)buf_out, buf_out_size,
                    "{\"current_firmware\": {\"version\": \"%s\", \"hmac_valid\": %s}, "
                    "\"next_firmware\": {\"version\": \"%s\", \"hmac_valid\": %s}}",
                    current_ver_str, is_current_valid ? "true" : "false",
                    next_ver_str, is_next_valid ? "true" : "false");
}


//
// Implementation
//

bool ox_server_updater_init(ox_server_updater_context_t* ctx, uni_net_http_server_context_t* server) {
    bool result = 1;

    result &= uni_net_http_server_register_file_ex(server, "/updater.html", g_ox_asset_updater, g_ox_asset_updater_size);
    result &= uni_net_http_server_register_handler_ex(server, UNI_NET_HTTP_COMMAND_GET, "/updater/status", _ox_server_updater_handler_status, ctx);
    result &= uni_net_http_server_register_handler_ex(server, UNI_NET_HTTP_COMMAND_GET, "/updater/apply", _ox_server_updater_handler_apply, ctx);
    result &= uni_net_http_server_register_handler_ex(server, UNI_NET_HTTP_COMMAND_POST, "/updater/upload", _ox_server_updater_handler_upload, ctx);

    return result;
}
