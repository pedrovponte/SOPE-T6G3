CFLAGS = -Wall -Wextra -Werror

simpledu: main.c
	gcc $(CFLAGS) -o simpledu main.c

clean:
	rm simpledu
