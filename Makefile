CC = gcc
CFLAGS = -Wall -Wextra -g

all: serveur client

serveur: serveur.c compte.c
	$(CC) $(CFLAGS) -o serveur serveur.c compte.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f serveur client
