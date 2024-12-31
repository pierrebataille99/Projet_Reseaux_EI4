// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Attacher le socket à l'adresse
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur lors du bind");
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions
    if (listen(server_fd, 3) < 0) {
        perror("Erreur lors de l'écoute");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions sur le port %d...\n", PORT);

    // Accepter une connexion
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Erreur lors de l'acceptation");
        exit(EXIT_FAILURE);
    }

    // Lire le message envoyé par le client
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Message reçu : %s\n", buffer);

    // Répondre au client
    const char *response = "Message bien reçu par le serveur";
    send(client_fd, response, strlen(response), 0);

    // Fermer les connexions
    close(client_fd);
    close(server_fd);
    return 0;
}
