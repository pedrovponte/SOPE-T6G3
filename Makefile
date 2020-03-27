cc		 = gcc
CFLAGS	 = -Wall -Wextra -g
LDFLAGS  =
DEPS	 = main.c logs.c logs.h Makefile
OBJFILES = main.o logs.o
TARGET	 = simgrep

all: $(TARGET)

$(TARGET): $(OBJFILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~