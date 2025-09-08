#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#include "common.h"

typedef struct {
    scp_type_t scp;
    str_vec_t fields;
    str_vec_t array;
} config_line_t;

#endif
