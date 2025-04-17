#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>

struct field_ops;

struct Field {
  int width;
  int height;
  int generation;
  const struct field_ops *field_ops;
  bool state[];
};

struct field_ops {
  void (*set)(struct Field *field, int width, int height, bool pattern[width][height], int offset_x, int offset_y);
  void (*simulate)(struct Field *field);
};

struct Field *init_field(int width, int height);
int survey(struct Field *field, int x, int y);

int clamp(int value, int min, int max);
int wrap(int value, int size);

#endif /* FIELD_H */
