CC = gcc
CFLAGS = -Wall -Wextra -g

all: serveur_TCP client_TCP

serveur_TCP: serveur_TCP.c compte.c
	$(CC) $(CFLAGS) -o serveur_TCP serveur_TCP.c compte.c

client_TCP: client_TCP.c
	$(CC) $(CFLAGS) -o client_TCP client_TCP.c

clean:
	rm -f serveur_TCP client_TCP
