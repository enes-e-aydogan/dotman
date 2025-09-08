#ifndef COMMON_H
#define COMMON_H

#include "cvector.h"

#define DOTMAN_VERSION "v0.1.0"

typedef enum {
    SCP_NONE = 0,
    SCP_DOT,
    SCP_DEP,
    SCP_UNKNOWN,
} scp_type_t;


// Vectors
DEFINE_STR_VECTOR


#endif // !COMMON_H
