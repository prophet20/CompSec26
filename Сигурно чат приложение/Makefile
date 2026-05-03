CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lssl -lcrypto -lpthread

all: main client

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o client client.c $(LIBS)

clean:
	rm -f main client
