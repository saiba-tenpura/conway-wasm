#include "field.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void field_set(struct Field *field, int width, int height, bool pattern[13][13], int offset_x, int offset_y)
{
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int x = wrap(i + offset_x, field->width);
      int y = wrap(j + offset_y, field->height);
      field->state[x * field->height + y] = pattern[i][j];
    }
  }
}

void field_simulate(struct Field *field, bool next_state[field->width][field->height])
{
  int count = 0;
  for (int i = 0; i < field->width; i++) {
    for (int j = 0; j < field->height; j++) {
      count = survey(field, i, j);
      if (field->state[i * field->height + j] == true && (count < 2 || count > 3)) {
        next_state[i][j] = false;
      } else if (field->state[i * field->height + j] == false && count == 3) {
        next_state[i][j] = true;
      } else {
        next_state[i][j] = field->state[i * field->height + j];
      }
    }
  }

  field->generation++;
}

static const struct field_ops field_ops = {
  .set = field_set,
  .simulate = field_simulate,
};

struct Field *init_field(int width, int height)
{
  struct Field *field = malloc(sizeof(struct Field) + width * height * sizeof(bool));
  if (field == NULL) {
    printf("Failed to allocate memory for field!");
    exit(EXIT_FAILURE);
  }

  field->generation = 0;
  field->field_ops = &field_ops;
  field->width = width;
  field->height = height;
  memset(field->state, false, width * height * sizeof(bool));
  return field;
}

int survey(struct Field *field, int x, int y)
{
  int count = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i == x && j == y) {
         continue;
      }

      int index = wrap(i, field->width);
      int indey = wrap(j, field->height);
      if (field->state[index * field->height + indey]) {
        count++;
      }
    }
  }

  return count;
}

int clamp(int value, int min, int max)
{
  const int temp = value < min ? min : value;
  return temp > max ? max : temp;
}

int wrap(int value, int size)
{
  if (value < 0) {
    return value + size;
  }

  if (value > size - 1) {
    return value - size;
  }

  return value;
}
