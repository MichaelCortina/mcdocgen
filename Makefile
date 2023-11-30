CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -fstack-protector-all -Werror -Wno-switch -g

all: main

main: main.o mcdocgen.o
	$(CC) main.o mcdocgen.o -o main

main.o: main.c mcdocgen.h
	$(CC) $(CFLAGS) main.c -c

mcdocgen.o: mcdocgen.c mcdocgen.h
	$(CC) $(CFLAGS) mcdocgen.c -c

clean:
	rm -f *.o main
