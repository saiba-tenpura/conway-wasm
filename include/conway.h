#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

void render(struct Field *field);
void spawn(struct Field *field, char* pattern_name, int offset_x, int offset_y);

void simulate();
void reset();
void resizeScreen(int width, int height);
void togglePause();

#endif /* CONWAY_H */
