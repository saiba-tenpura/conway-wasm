.PHONY: all clean

CC = gcc

# Libraries for Debian GNU/Linux desktop compiling
# NOTE: Required packages: libegl1-mesa-dev
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: conway

conway: conway.c conway.h field.c field.h
	$(CC) conway.c field.c -o conway $(LDLIBS)

clean:
	rm conway
