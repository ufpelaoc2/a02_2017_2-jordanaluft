CC=gcc

CFLAGS=-Wall -Wextra -Werror -std=c99  -O0 -g
LDFLAGS=-lm

all: grade

sim: sim.c mytests.c
	$(CC) $(CFLAGS) -o sim sim.c mytests.c $(LDFLAGS)

grade: sim
	./sim

clean:
	rm -rf *.o sim
