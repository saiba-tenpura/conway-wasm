#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

struct Pattern {
  char *name;
  int width;
  int height;
  bool state[13][13];
};

void render(struct Field *field, int generation);

#endif /* CONWAY_H */
