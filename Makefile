CC=gcc

CFLAGS=-Wall -Wextra -Werror -std=c99  -O0 -g
LDFLAGS=-lm

all: grade

sim: sim.c tests.c
	$(CC) $(CFLAGS) -o sim sim.c tests.c $(LDFLAGS)

mytests: sim.c mytests.c
	$(CC) $(CFLAGS) -o mytests sim.c mytests.c $(LDFLAGS)
	./mytests

grade: sim
	./sim

clean:
	rm -rf *.o sim
