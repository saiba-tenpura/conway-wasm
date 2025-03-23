#ifndef CONWAY_H
#define CONWAY_H

#include <stdbool.h>

struct Field {
  int width;
  int height;
  bool state[];
};

struct Pattern {
  char *name;
  int width;
  int height;
  bool state[13][13];
};

struct Field *init(int width, int height);

void set(struct Field *field, int width, int height, bool pattern[width][height], int offset_x, int offset_y);
void spawn(struct Field *field, char* pattern, int offset_x, int offset_y);
void simulate(struct Field *field, bool next_state[field->width][field->height]);
void render(struct Field *field, int generation);

int survey(struct Field *field, int x, int y);
int clamp(int value, int min, int max);
int wrap(int value, int size);

#endif /* CONWAY_H */
