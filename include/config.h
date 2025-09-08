#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#include "common.h"
#include "cvector.h"

typedef struct {
  scp_type_t scp;
  str_vec_t  fields;
  str_vec_t  array;
} config_line_t;

DEFINE_VECTOR(config_line_t, conf_line)

char* trim(char* str);
int   parse_line(conf_line_vec_t* conf_line, const char* line);
int   parse_file(const char* file, conf_line_vec_t** conf_lines);

#endif
