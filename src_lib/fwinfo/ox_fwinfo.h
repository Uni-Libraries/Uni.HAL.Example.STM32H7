#pragma once

//
// Includes
//

// stdlib
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



//
// Structs
//

typedef enum {
    OX_FWINFO_CURRENT,
    OX_FWINFO_NEXT
} ox_fwinfo_type;

typedef struct {
    char target_device[64];
    char build_type[64];
    char build_timestamp[64];
    char compiler_info[64];
    char git_version[64];
    char git_commit_sha[64];
    char git_branch[64];
} ox_fwinfo_t;



//
// Functions
//

const uint8_t* ox_fwinfo_get_address(ox_fwinfo_type fw_type);
const uint8_t* ox_fwinfo_get_hash(ox_fwinfo_type fw_type);
const ox_fwinfo_t* ox_fwinfo_get_info(ox_fwinfo_type fw_type);
size_t ox_fwinfo_get_size(ox_fwinfo_type fw_type);

bool ox_fwinfo_is_valid(ox_fwinfo_type fw_type);

