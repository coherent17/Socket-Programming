CC = gcc
CFLAGS = -g -Wall
BIN = server client

all: $(BIN)

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(BIN)