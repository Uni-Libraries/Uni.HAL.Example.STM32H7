//
// Includes
//

// ox
#include "config/app_config_server.h"
#include "uni_common_array.h"
#include "uni_net_http_common.h"



//
// Structs
//
UNI_COMMON_ARRAY_DEFINITION_EX_CLEAR(g_ox_server_arr_files, uni_net_http_file_t, 16);
UNI_COMMON_ARRAY_DEFINITION_EX_CLEAR(g_ox_server_arr_handlers, uni_net_http_handler_t, 16);
UNI_COMMON_ARRAY_DEFINITION_EX_CLEAR(g_ox_server_arr_stream_handlers, uni_net_http_stream_handler_t, 16);


uni_net_http_server_context_t g_ox_server_ctx;
ox_server_dashboard_context_t g_ox_server_dashboard_ctx;
ox_server_updater_context_t g_ox_server_updater_ctx;

void app_config_server_init(void) {
    g_ox_server_ctx.config.files = g_ox_server_arr_files_ctx;
    g_ox_server_ctx.config.handlers = g_ox_server_arr_handlers_ctx;
    g_ox_server_ctx.config.stream_handlers = g_ox_server_arr_stream_handlers_ctx;
    g_ox_server_ctx.config.max_clients = 5;

}
