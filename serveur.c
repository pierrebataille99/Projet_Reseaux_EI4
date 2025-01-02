#include "compte.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void traiter_commande(int client_fd, char *commande) {
    int id_client, id_compte;
    char password[20];
    char action[20];
    float montant;

    // Réinitialiser les variables pour éviter des données résiduelles
    memset(action, 0, sizeof(action));
    memset(password, 0, sizeof(password));
    montant = 0.0;

    // Extraire les informations de la commande
    sscanf(commande, "%s %d %d %s %f", action, &id_client, &id_compte, password, &montant);

    Compte *compte = trouver_compte(id_client, id_compte, password);

    if (compte == NULL) {
        send(client_fd, "KO : Compte ou mot de passe incorrect\n", 38, 0);
        return;
    }

    if (strcmp(action, "SOLDE") == 0) {
        char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), "RES_SOLDE %.2f\n", compte->solde);
        send(client_fd, response, strlen(response), 0);
    } else if (strcmp(action, "AJOUT") == 0) {
        compte->solde += montant;
        ajouter_operation(compte, "AJOUT");
        send(client_fd, "OK\n", 3, 0);
    } else if (strcmp(action, "RETRAIT") == 0) {
        if (compte->solde >= montant) {
            compte->solde -= montant;
            ajouter_operation(compte, "RETRAIT");
            send(client_fd, "OK\n", 3, 0);
        } else {
            send(client_fd, "KO : Solde insuffisant\n", 23, 0);
        }
    } else if (strcmp(action, "OPERATIONS") == 0) {
        char response[BUFFER_SIZE] = "RES_OPERATIONS\n";
        for (int i = 0; i < compte->operations_count; i++) {
            strcat(response, compte->operations[i]);
            strcat(response, "\n");
        }
        send(client_fd, response, strlen(response), 0);
    } else {
        send(client_fd, "KO : Commande inconnue\n", 23, 0);
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Serveur en attente de connexions sur le port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_fd < 0) {
            perror("Erreur lors de l'acceptation");
            continue;
        }

        printf("Client connecté.\n");
        while (1) {
            // Lire les données envoyées par le client
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) {
                printf("Client déconnecté.\n");
                break;
            }

            // Traiter la commande et envoyer la réponse
            traiter_commande(client_fd, buffer);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
