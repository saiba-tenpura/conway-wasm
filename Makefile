.PHONY: all clean

# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB
TARGET_PLATFORM ?= PLATFORM_DESKTOP

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
LIB_DIR := ./lib
INCLUDE_DIR := ./include

OBJS := conway.o field.o pattern.o

# Define default C compiler: CC
CC := gcc

ifeq ($(TARGET_PLATFORM),$(filter $(TARGET_PLATFORM),PLATFORM_DESKTOP))
	# Libraries for Debian GNU/Linux desktop compiling
	# NOTE: Required packages: libegl1-mesa-dev
	LDLIBS = -I $(LIB_DIR)/raylib/src -L $(LIB_DIR)/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(TARGET_PLATFORM),$(filter $(TARGET_PLATFORM),PLATFORM_WEB))
	# HTML5 emscriptien compiler
	# WARNING: To compile to HTML5, raylib must be recompiled
	# to use emscripten.h and emscripten_set_main_loop()
	CC = emcc

	LDLIBS = -I $(LIB_DIR)/raylib/src -L $(LIB_DIR)/raylib/src -lraylib -s USE_GLFW=3 -DPLATFORM_WEB
endif

CFLAGS := -I $(INCLUDE_DIR)

all: conway

setup:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

conway: setup raylib $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway $(foreach file,$(OBJS),$(BUILD_DIR)/$(file)) $(LDLIBS)

%.o: setup $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c -I $(LIB_DIR)/raylib/src

raylib:
	-[ ! -d $(LIB_DIR)/raylib ] && git clone --depth 1 https://github.com/raysan5/raylib.git $(LIB_DIR)/raylib
	cd $(LIB_DIR)/raylib/src && make PLATFORM=$(TARGET_PLATFORM)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	cd $(LIB_DIR)/raylib/src && make clean

# emcc -o game.html game.c -Os -Wall ./path-to/libraylib.a -I. -Ipath-to-raylib-h -L. -Lpath-to-libraylib-a -s USE_GLFW=3 --shell-file path-to/shell.html -DPLATFORM_WEB
