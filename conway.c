#include "conway.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <raylib.h>

#define CELL_SIZE 16
#define CELL_MARGIN 1

int main()
{
  bool paused = true;
  int generation = 0;
  int width = 128, height = 72;
  struct Field *field = init(width, height);
  bool next_state[width][height];

  // bool pattern[13][13] = {
  //   {0, 1, 0},
  //   {1, 1, 1},
  //   {0, 0, 1},
  // };
  // set(field, 3, 3, pattern, 9, 9);
  // spawn(field, "block", 9, 9);
  // spawn(field, "beehive", 8, 9);
  // spawn(field, "loaf", 8, 8);
  // spawn(field, "boat", 9, 9);
  // spawn(field, "tub", 9, 9);
  // spawn(field, "blinker", 9, 1);
  // spawn(field, "toad", 8, 9);
  // spawn(field, "beacon", 8, 8);
  // spawn(field, "pulsar", 4, 4);
  // spawn(field, "pentadecathlon", 9, 5);
  // spawn(field, "glider", 9, 9);
  // spawn(field, "lightweight-spaceship", 8, 8);
  // spawn(field, "middleweight-spaceship", 7, 8);
  // spawn(field, "heavyweight-spaceship", 7, 8);

  float delta_time = 0.0f;
  float update_interval = 0.05f;
  const int windowWidth = width * CELL_SIZE + CELL_MARGIN;
  const int windowHeight = height * CELL_SIZE + CELL_MARGIN;
  InitWindow(windowWidth, windowHeight, "Conway's Game of Life - WASM");
  SetTargetFPS(60);

  while (! WindowShouldClose()) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      int x = clamp(GetMouseX() / CELL_SIZE, 0, width - 1);
      int y = clamp(GetMouseY() / CELL_SIZE, 0, height - 1);
      field->state[x * field->height + y] = ! field->state[x * field->height + y];
    }

    if (IsKeyPressed(KEY_SPACE)) {
      paused = ! paused;
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    render(field, generation);
    if (! paused && delta_time > update_interval) {
      simulate(field, next_state);
      memcpy(field->state, next_state, width * height * sizeof(bool));
      generation++;
      delta_time = 0.0f;
    } else {
      delta_time += GetFrameTime();
    }

    EndDrawing();
  }

  CloseWindow();
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
      int x = wrap(i + offset_x, field->width);
      int y = wrap(j + offset_y, field->height);
      field->state[x * field->height + y] = pattern[i][j];
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
      "beehive", 4, 3, {
        {0, 1, 0},
        {1, 0, 1},
        {1, 0, 1},
        {0, 1, 0},
      },
    },
    {
      "loaf", 4, 4, {
        {0, 1, 0, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 1},
        {0, 1, 1, 0},
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
      "toad", 4, 2, {
        {0, 1},
        {1, 1},
        {1, 1},
        {1, 0},
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
      "pentadecathlon", 3, 10, {
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
        {1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
      },
    },
    {
      "glider", 3, 3, {
        {1, 0, 1},
        {0, 1, 1},
        {0, 1, 0},
      },
    },
    {
      "lightweight-spaceship", 5, 4, {
        {1, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1},
        {0, 1, 1, 1},
      },
    },
    {
      "middleweight-spaceship", 6, 5, {
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 1},
        {0, 0, 1, 1, 1},
      },
    },
    {
      "heavyweight-spaceship", 7, 5, {
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 1},
        {0, 0, 1, 1, 1},
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
      if (field->state[i * field->height + j] == true && (count < 2 || count > 3)) {
        next_state[i][j] = false;
      } else if (field->state[i * field->height + j] == false && count == 3) {
        next_state[i][j] = true;
      } else {
        next_state[i][j] = field->state[i * field->height + j];
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

void render(struct Field *field, int generation)
{
  int offset_x, offset_y;
  int size = CELL_SIZE - CELL_MARGIN;

  int population = 0;
  for (int i = 0; i < field->width; i++) {
    for (int j = 0; j < field->height; j++) {
      if (field->state[i * field->height + j]) {
        population++;
      }

      offset_x = i * CELL_SIZE + CELL_MARGIN;
      offset_y = j * CELL_SIZE + CELL_MARGIN;
      DrawRectangle(offset_x, offset_y, size, size, field->state[i * field->height + j] ? RAYWHITE : BLACK);
    }
  }

  DrawText(TextFormat("Generation: %d\nPopulation: %d", generation, population), 10, 10, 26, RAYWHITE);
}
