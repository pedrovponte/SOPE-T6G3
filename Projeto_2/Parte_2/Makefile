CC = gcc
CFLAGS = -Wall -pthread
DEPS = utils.h args_handler.h U2.c Q2.c queue.h
OBJ = utils.o args_handler.o queue.o
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

U2: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -o $@

Q2: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -o $@

clean:
	rm *.o $(TARGETS)
