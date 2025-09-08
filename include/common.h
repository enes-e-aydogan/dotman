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

// Logging macros
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"

#define LOG_ERROR(msg, ...)                                                                \
  fprintf(stderr, COLOR_RED "[ERROR] %s:%d:%s()\n" COLOR_RESET "   ↳ " msg "\n", __FILE__, \
          __LINE__, __func__, ##__VA_ARGS__)

#define LOG_WARN(msg, ...)                                                                   \
  fprintf(stderr, COLOR_YELLOW "[WARN] %s:%d:%s()\n" COLOR_RESET "   ↳ " msg "\n", __FILE__, \
          __LINE__, __func__, ##__VA_ARGS__)

#define LOG_INFO(msg, ...)                                                                 \
  fprintf(stdout, COLOR_BLUE "[INFO] %s:%d:%s()\n" COLOR_RESET "   ↳ " msg "\n", __FILE__, \
          __LINE__, __func__, ##__VA_ARGS__)
#endif  // !COMMON_H
