#pragma once

//
// Includes
//

// uni.net
#include "uni_net.h"



//
// Structs
//

typedef struct {
    bool empty;
} ox_server_updater_config_t;


typedef struct {
    bool initialized;
} ox_server_updater_state_t;


typedef struct {
    ox_server_updater_config_t config;
    ox_server_updater_state_t state;
} ox_server_updater_context_t;



//
// Functions
//

bool ox_server_updater_init(ox_server_updater_context_t* ctx, uni_net_http_server_context_t* server);
