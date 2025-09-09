#include "config.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

char* trim(char* str) {
  while (*str && isspace((unsigned char) *str)) {
    str++;
  }
  char* end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char) *end)) {
    *end-- = '\0';
  }
  return str;
}

int extract_array(config_line_t* config_line, char* data) {
  return EXIT_SUCCESS;
}

int parse_line(config_line_vec_t* config_line_vec, char* line) {
  config_line_t config_line;
  bool          scope_extracted = false;
  char*         data            = strtok(line, ":");
  while (data != NULL) {
    data = trim(data);
    if (*data == '\0') {
      LOG_ERROR("Empty field in the config file.");
      return EXIT_FAILURE;
    }

    if (!scope_extracted) {
      config_line.scope = scope_str_to_enum(data);
      if (config_line.scope == SCOPE_UNKNOWN) {
        LOG_ERROR("Unknown scope name!");
        return EXIT_FAILURE;
      }
      scope_extracted = true;
      data            = strtok(NULL, ":");
      continue;
    }

    if ((strchr(data, ',')) != NULL) {
      if (extract_array(&config_line, data)) {
        LOG_ERROR("Failed to extract array");
        return EXIT_FAILURE;
      }
      data = strtok(NULL, ":");
      continue;
    }

    str_vec_push(&config_line.fields, data);
    data = strtok(NULL, ":");
  }

  config_line_vec_push(config_line_vec, config_line);

  return EXIT_SUCCESS;
}

int parse_file(const char* filename, config_line_vec_t* config_line_vec) {
  FILE* file_p = fopen(filename, "rb");
  if (!file_p) {
    LOG_ERROR("Failed to open file");
    return EXIT_FAILURE;
  }
  fseek(file_p, 0, SEEK_END);
  size_t file_size = ftell(file_p);
  fseek(file_p, 0, SEEK_SET);

  char* buffer = malloc(file_size);
  if (!buffer) {
    LOG_ERROR("Failed to allocate buffer");
    fclose(file_p);
    return EXIT_FAILURE;
  }

  size_t read = fread(buffer, 1, file_size, file_p);
  fclose(file_p);

  if (read != file_size) {
    LOG_ERROR("Failed to read file");
    free(buffer);
    return EXIT_FAILURE;
  }

  char* token = strtok(buffer, "\n");

  while (token != NULL) {
    if (*token == '\0') {
      token = strtok(NULL, "\n");
      continue;
    }

    token = trim(token);

    if (parse_line(config_line_vec, token)) {
      LOG_ERROR("Failed to parse line");
      free(buffer);
      return EXIT_FAILURE;
    }

    token = strtok(NULL, "\n");
  }

  free(buffer);

  return EXIT_SUCCESS;
}
