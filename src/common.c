#include "common.h"
scp_type_t scp_str_to_enum(const char* scp) {
  if (!(strcmp("dot", scp))) {
    return SCP_DOT;
  }
  if (!(strcmp("dep", scp))) {
    return SCP_DEP;
  }
  return SCP_UNKNOWN;
}
