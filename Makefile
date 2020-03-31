cc		 = gcc
CFLAGS	 = -Wall -Wextra -g
LDFLAGS  =
DEPS	 = main.c logs.c logs.h dirs.c dirs.h args_handler.c args_handler.h Makefile
OBJFILES = main.o logs.o dirs.o args_handler.o
TARGET	 = simpledu

all: $(TARGET)

$(TARGET): $(OBJFILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~