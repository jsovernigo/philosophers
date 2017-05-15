CC		=gcc
CFLAGS	=-Wall -ansi -g #-Werror -Wextra
INCLUDE	=./include
OBJECTS	=vector.o eat.o mem.o proc.o
EXEC1	=dine
EXEC2	=holes


all: build dine holes

build:
	mkdir -p bin/

# ----- Programs -----

dine: eat.o
	$(CC) -o $(EXEC1) $(CFLAGS) src/dine.c bin/eat.o -I$(INCLUDE) -lpthread

holes: mem.o vector.o proc.o
	$(CC) -o $(EXEC2) $(CFLAGS) src/holes.c bin/mem.o bin/proc.o bin/vector.o -I$(INCLUDE)

# ----- Objects -----

eat.o: src/eat.c include/eat.h
	$(CC) -o bin/eat.o $(CFLAGS) src/eat.c -c -I$(INCLUDE)

mem.o: src/mem.c include/mem.h
	$(CC) -o bin/mem.o $(CFLAGS) src/mem.c -c -I$(INCLUDE)

proc.o: src/proc.c include/proc.h
	$(CC) -o bin/proc.o $(CFLAGS) src/proc.c -c -I$(INCLUDE)

vector.o: src/vector.c include/vector.h
	$(CC) -o bin/vector.o $(CFLAGS) src/vector.c -c -I$(INCLUDE)

clean:
	rm $(EXEC1) $(EXEC2)

clobber:
	rm $(EXEC1) $(EXEC2) bin/*
