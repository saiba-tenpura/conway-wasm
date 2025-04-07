#ifndef PATTERN_H
#define PATTERN_H

#include <stdbool.h>

struct Pattern {
  char *name;
  int width;
  int height;
  bool state[13][13];
};

struct Pattern *init_pattern(char *pattern_name);

#endif /* PATTERN_H */
