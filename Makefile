.PHONY: all clean

# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB
PLATFORM ?= PLATFORM_DESKTOP

# Directories
SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
LIB_DIR := ./lib
OUT_DIR := ./out
INCLUDE_DIR := ./include

# Define default C compiler: CC
CC := gcc
CFLAGS := -I $(INCLUDE_DIR)
OBJS := conway.o field.o pattern.o

ifeq ($(PLATFORM),$(filter $(PLATFORM),PLATFORM_DESKTOP))
	# Libraries for Debian GNU/Linux desktop compiling
	# NOTE: Required packages: libegl1-mesa-dev
	LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(PLATFORM),$(filter $(PLATFORM),PLATFORM_WEB))
	# HTML5 emscriptien compiler
	# WARNING: To compile to HTML5, raylib must be recompiled
	# to use emscripten.h and emscripten_set_main_loop()
	CC = emcc

	CFLAGS += -I $(LIB_DIR)/raylib/src

	LDLIBS = -I $(LIB_DIR)/raylib/src -L $(LIB_DIR)/raylib/src -lraylib.web -s USE_GLFW=3 -s ASYNCIFY --shell-file src/shell.html
endif

all: conway

setup:
	mkdir -p $(BUILD_DIR)

conway: setup $(OBJS)
ifeq ($(PLATFORM), PLATFORM_DESKTOP)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway $(foreach file,$(OBJS),$(BUILD_DIR)/$(file)) $(LDLIBS)
endif
ifeq ($(PLATFORM), PLATFORM_WEB)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/index.html $(foreach file,$(OBJS),$(BUILD_DIR)/$(file)) $(LDLIBS)
endif

%.o: setup $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(OUT_DIR)
