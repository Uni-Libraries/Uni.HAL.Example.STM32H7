#pragma once

//
// Includes
//

// ox
#include "server_http/ox_server_http_dashboard.h"
#include "server_http/ox_server_http_updater.h"


void app_config_server_init(void);

//
// Structs
//

extern uni_net_http_server_context_t g_ox_server_ctx;
extern ox_server_dashboard_context_t g_ox_server_http_dashboard_ctx;
extern ox_server_updater_context_t g_ox_server_http_updater_ctx;
