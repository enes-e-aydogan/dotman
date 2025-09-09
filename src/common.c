#include "common.h"
scope_type_t scope_str_to_enum(const char* scp) {
  if (!(strcmp("dot", scp))) {
    return SCOPE_DOT;
  }
  if (!(strcmp("dep", scp))) {
    return SCOPE_DEP;
  }
  return SCOPE_UNKNOWN;
}
