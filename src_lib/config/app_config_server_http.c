//
// Includes
//

// ox
#include "config/app_config_server_http.h"
#include "uni_common_array.h"
#include "uni_net_http_common.h"



//
// Structs
//
UNI_COMMON_ARRAY_DEFINITION_EX_CLEAR(g_ox_server_arr_files, uni_net_http_file_t, 32);
UNI_COMMON_ARRAY_DEFINITION_EX_CLEAR(g_ox_server_arr_handlers, uni_net_http_handler_t, 32);


uni_net_http_server_context_t g_ox_server_ctx;
ox_server_dashboard_context_t g_ox_server_dashboard_ctx;
ox_server_updater_context_t g_ox_server_updater_ctx;



void app_config_server_init(void) {
    g_ox_server_ctx.config.files = g_ox_server_arr_files_ctx;
    g_ox_server_ctx.config.handlers = g_ox_server_arr_handlers_ctx;
    g_ox_server_ctx.config.max_clients = 10;

    uni_net_http_server_init(&g_ox_server_ctx);

    ox_server_dashboard_init(&g_ox_server_dashboard_ctx, &g_ox_server_ctx);
    ox_server_updater_init(&g_ox_server_updater_ctx, &g_ox_server_ctx);
}
