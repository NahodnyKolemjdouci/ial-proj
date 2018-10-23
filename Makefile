PROJ = proj
GENERATOR = generator
CFLAGS = -std=c99 -Wall -Wextra -Werror
CC = gcc


hello: $(PROJ).c
	-$(CC) $(CFLAGS) $(PROJ).c -o $(PROJ)
	-$(CC) $(CFLAGS) $(GENERATOR).c -o $(GENERATOR)

clean:
	rm -f $(PROJ)
	rm -f $(GENERATOR)