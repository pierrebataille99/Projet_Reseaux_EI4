CC = gcc
CFLAGS = -Wall -g

all: serveur_UDP client_UDP

serveur_UDP: serveur_UDP.c compte.c
	$(CC) $(CFLAGS) -o serveur_UDP serveur_UDP.c compte.c

client_UDP: client_UDP.c
	$(CC) $(CFLAGS) -o client_UDP client_UDP.c

clean:
	rm -f serveur_UDP client_UDP
