#include "config.h"

#include <ctype.h>
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

int parse_file(const char* filename, conf_line_vec_t* conf_lines) {
int parse_file(const char* filename, conf_line_vec_t* conf_line_vec) {
  /*
   * read file,
   * pass to parse_line function with strtok
   * */

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

    if (parse_line(conf_line_vec, token)) {
      LOG_ERROR("Failed to parse line");
      free(buffer);
      return EXIT_FAILURE;
    }

    token = strtok(NULL, "\n");
  }

  free(buffer);

  return EXIT_SUCCESS;
}
