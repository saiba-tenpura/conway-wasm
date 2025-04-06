#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

struct Pattern {
  char *name;
  int width;
  int height;
  bool state[13][13];
};

void render(struct Field *field);
void spawn(struct Field *field, char* pattern, int offset_x, int offset_y);

#endif /* CONWAY_H */
