#pragma once

//
// Includes
//

// ox
#include "server/ox_server_dashboard.h"
#include "server/ox_server_updater.h"


void app_config_server_init(void);

//
// Structs
//

extern uni_net_http_server_context_t g_ox_server_ctx;
extern ox_server_dashboard_context_t g_ox_server_dashboard_ctx;
extern ox_server_updater_context_t g_ox_server_updater_ctx;
