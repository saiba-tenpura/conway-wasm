.PHONY: all clean

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
INCLUDE_DIR := ./include

OBJS := conway.o field.o pattern.o

CC := gcc
CFLAGS := -I$(INCLUDE_DIR)
# Libraries for Debian GNU/Linux desktop compiling
# NOTE: Required packages: libegl1-mesa-dev
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: conway

setup:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

conway: setup $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway $(LDLIBS) $(foreach file,$(OBJS),$(BUILD_DIR)/$(file))

%.o: setup $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$*.o -c $(SRC_DIR)/$*.c

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
