CC = gcc
CFLAGS = -Wall -pthread
DEPS = utils.h args_handler.h U2.c Q2.c
OBJ = utils.o args_handler.o
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

U2: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

Q2: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)
