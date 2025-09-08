#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
