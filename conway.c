#include "conway.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  int opt;
  int width = 21, height = 21;
  static struct option long_options[] = {
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
  };

  while ((opt = getopt_long(argc, argv, "w:h:", long_options, NULL)) != -1) {
    switch (opt) {
      case 'w':
        width = strtol(optarg, NULL, 10);
        break;
      case 'h':
        height = strtol(optarg, NULL, 10);
        break;
    }
  }

  struct Field *field = init(width, height);
  bool next_state[width][height];
  bool pattern[13][13] = {
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 1},
  };

  set(field, 3, 3, pattern, 9, 9);
  // spawn(field, "block", 9, 9);
  // spawn(field, "beehive", 9, 8);
  // spawn(field, "loaf", 8, 8);
  // spawn(field, "boat", 9, 9);
  // spawn(field, "tub", 9, 9);
  // spawn(field, "blinker", 10, 9);
  // spawn(field, "toad", 9, 8);
  // spawn(field, "beacon", 8, 8);
  // spawn(field, "pulsar", 4, 4);
  // spawn(field, "pentadecathlon", 5, 9);
  // spawn(field, "glider", 9, 9);
  // spawn(field, "lightweight-spaceship", 8, 8);
  // spawn(field, "middleweight-spaceship", 8, 7);
  // spawn(field, "heavyweight-spaceship", 8, 7);
  while (true) {
    simulate(field, next_state);
    render(field);
    memcpy(field->state, next_state, width * height * sizeof(bool));
    usleep(100000);
  }

  free(field);
  return 0;
}

struct Field *init(int width, int height)
{
  struct Field *field = malloc(sizeof(struct Field*) + width * height * sizeof(bool));
  if (field == NULL) {
    printf("Failed to allocate memory for field!");
    exit(EXIT_FAILURE);
  }

  field->width = width;
  field->height = height;
  memset(field->state, false, width * height * sizeof(bool));
  return field;
}

void set(struct Field *field, int width, int height, bool pattern[13][13], int offset_x, int offset_y)
{
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int index = wrap(i + offset_x, field->width);
      int indey = wrap(j + offset_y, field->height);
      field->state[index * field->width + indey] = pattern[i][j];
    }
  }
}

void spawn(struct Field *field, char* pattern_name, int offset_x, int offset_y)
{
  struct Pattern patterns[] = {
    {
      "block", 2, 2, {
        {1, 1},
        {1, 1},
      },
    },
    {
      "beehive", 3, 5, {
        {0, 1, 1, 0},
        {1, 0, 0, 1},
        {0, 1, 1, 0},
      },
    },
    {
      "loaf", 4, 4, {
        {0, 1, 1, 0},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 0},
      },
    },
    {
      "boat", 3, 3, {
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 0},
      },
    },
    {
      "tub", 3, 3, {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0},
      },
    },
    {
      "blinker", 1, 3, {
        {1, 1, 1},
      },
    },
    {
      "toad", 2, 4, {
        {0, 1, 1, 1},
        {1, 1, 1, 0},
      },
    },
    {
      "beacon", 4, 4, {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 0, 1, 1},
      },
    },
    {
      "pulsar", 13, 13, {
        {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
      },
    },
    {
      "pentadecathlon", 10, 3, {
        {0, 1, 0},
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0},
        {0, 1, 0},
      },
    },
    {
      "glider", 3, 3, {
        {1, 0, 0},
        {0, 1, 1},
        {1, 1, 0},
      },
    },
    {
      "lightweight-spaceship", 4, 5, {
        {1, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
      },
    },
    {
      "middleweight-spaceship", 5, 6, {
        {0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 1},
      },
    },
    {
      "heavyweight-spaceship", 5, 7, {
        {0, 0, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 1},
      },
    },
  };

  struct Pattern *pattern;
  for (int i = 0; i < sizeof(patterns) / sizeof(struct Pattern); i++) {
    if (strcmp(pattern_name, patterns[i].name) == 0) {
      pattern = &patterns[i];
      break;
    }
  }

  if (! pattern) {
    return;
  }

  set(field, pattern->width, pattern->height, pattern->state, offset_x, offset_y);
}

void simulate(struct Field *field, bool next_state[field->width][field->height])
{
  int count = 0;
  for (int i = 0; i < field->width; i++) {
    for (int j = 0; j < field->height; j++) {
      count = survey(field, i, j);
      if (field->state[i * field->width + j] == true && (count < 2 || count > 3)) {
        next_state[i][j] = false;
      } else if (field->state[i * field->width + j] == false && count == 3) {
        next_state[i][j] = true;
      } else {
        next_state[i][j] = field->state[i * field->width + j];
      }
    }
  }
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
      if (field->state[index * field->width + indey]) {
        count++;
      }
    }
  }

  return count;
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

void render(struct Field *field)
{
  clear();
  for (int i = 0; i < field->width; i++) {
    for (int j = 0; j < field->height; j++) {
      printf("%s ", field->state[i * field->width + j] ? "#" : "-");
    }

    printf("\n");
  }
}

void clear()
{
  printf("\e[1;1H\e[2J");
}
