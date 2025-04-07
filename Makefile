.PHONY: all clean

CC = gcc

# Libraries for Debian GNU/Linux desktop compiling
# NOTE: Required packages: libegl1-mesa-dev
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: conway

conway: conway.o field.o pattern.o
	$(CC) conway.o field.o pattern.o -o conway $(LDLIBS)

conway.o: conway.c conway.h
	$(CC) -c conway.c

field.o: field.c field.h
	$(CC) -c field.c

pattern.o: pattern.c pattern.h
	$(CC) -c pattern.c

clean:
	rm conway conway.o field.o pattern.o
