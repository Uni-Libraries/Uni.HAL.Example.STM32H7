#pragma once

//
// Includes
//

// uni.net
#include <uni_net.h>

// ox
#include "net/ox_net.h"



//
// Typedefs
//

#define OX_NET_ICMP_COUNT (2U)



//
// Globals
//

extern const uint32_t g_ox_net_icmp_addrs[OX_NET_ICMP_COUNT];
extern ox_net_context_t g_ox_net_ctx;
extern uni_net_ftp_client_context_t g_ox_ftp_ctx;

void app_config_net_init(void);
