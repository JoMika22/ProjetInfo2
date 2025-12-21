CC=gcc
CFLAGS=-Wall -Wextra -O2 -g
SRC=main.c avl.c network.c parser.c utils.c
OBJ=$(SRC:.c=.o)
EXEC=wildwater

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
