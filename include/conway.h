#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

void render(struct Field *field);
void spawn(struct Field *field, char* pattern_name, int offset_x, int offset_y);

#endif /* CONWAY_H */
