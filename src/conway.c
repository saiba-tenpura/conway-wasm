#include "field.h"
#include "pattern.h"
#include "conway.h"
#include <emscripten/emscripten.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <raylib.h>

#define CELL_SIZE 16
#define CELL_MARGIN 1
#define TARGET_FPS 60

struct Field * field;
bool paused = true;

int main()
{
  float delta_time = 0.0f;
  float update_interval = 0.05f;
  InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Conway's Game of Life - WASM");
  SetTargetFPS(TARGET_FPS);

  field = init_field(GetScreenWidth() / CELL_SIZE, GetScreenHeight() / CELL_SIZE);
  // bool pattern[13][13] = {
  //   {0, 1, 0},
  //   {1, 1, 1},
  //   {0, 0, 1},
  // };
  // field->field_ops->set(field, 3, 3, pattern, 9, 9);
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

  while (! WindowShouldClose()) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      int x = clamp(GetMouseX() / CELL_SIZE, 0, field->width - 1);
      int y = clamp(GetMouseY() / CELL_SIZE, 0, field->height - 1);
      field->state[x * field->height + y] = ! field->state[x * field->height + y];
    }

    if (IsKeyPressed(KEY_N)) {
      simulate();
    }

    if (IsKeyPressed(KEY_P)) {
      togglePause();
    }

    if (IsKeyPressed(KEY_R)) {
      reset();
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    render(field);
    if (! paused && delta_time > update_interval) {
      simulate();
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

void spawn(struct Field *field, char *pattern_name, int offset_x, int offset_y)
{
  struct Pattern *pattern = init_pattern(pattern_name);
  if (! pattern) {
    return;
  }

  field->field_ops->set(field, pattern->width, pattern->height, pattern->state, offset_x, offset_y);
}

void render(struct Field *field)
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

  DrawText(TextFormat("Generation: %d\nPopulation: %d", field->generation, population), 10, 10, 24, RAYWHITE);
}

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE
void simulate() {
  field->field_ops->simulate(field);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void reset() {
  field = init_field(field->width, field->height);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void resizeScreen(int width, int height) {
  const int fieldWidth = width / CELL_SIZE;
  const int fieldHeight = height / CELL_SIZE;
  SetWindowSize(fieldWidth * CELL_SIZE, fieldHeight * CELL_SIZE);
  field = init_field(fieldWidth, fieldHeight);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void togglePause() {
  paused = ! paused;
}

