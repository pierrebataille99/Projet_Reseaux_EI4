#include "compte.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void traiter_commande(char *commande, char *response)
{
    int id_client, id_compte;
    char password[20];
    char action[20];
    float montant;

    memset(action, 0, sizeof(action));
    memset(password, 0, sizeof(password));
    montant = 0.0;

    sscanf(commande, "%s %d %d %s %f", action, &id_client, &id_compte, password, &montant);

    Compte *compte = trouver_compte(id_client, id_compte, password);

    if (compte == NULL)
    {
        snprintf(response, BUFFER_SIZE, "KO : Compte ou mot de passe incorrect\n");
        return;
    }

    if (strcmp(action, "SOLDE") == 0)
    {
        snprintf(response, BUFFER_SIZE, "RES_SOLDE %.2f\n", compte->solde);
    }
    else if (strcmp(action, "AJOUT") == 0)
    {
        compte->solde += montant;
        ajouter_operation(compte, "AJOUT");
        snprintf(response, BUFFER_SIZE, "OK\n");
    }
    else if (strcmp(action, "RETRAIT") == 0)
    {
        if (compte->solde >= montant)
        {
            compte->solde -= montant;
            ajouter_operation(compte, "RETRAIT");
            snprintf(response, BUFFER_SIZE, "OK\n");
        }
        else
        {
            snprintf(response, BUFFER_SIZE, "KO : Solde insuffisant\n");
        }
    }
    else if (strcmp(action, "OPERATIONS") == 0)
    {
        snprintf(response, BUFFER_SIZE, "RES_OPERATIONS\n");
        for (int i = 0; i < compte->operations_count; i++)
        {
            strcat(response, compte->operations[i]);
            strcat(response, "\n");
        }
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "KO : Commande inconnue\n");
    }
}

int main()
{
    int server_fd;
    struct sockaddr_in address, client_address;
    int addrlen = sizeof(client_address);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0)
    {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Erreur lors de la liaison");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur UDP en attente sur le port %d...\n", PORT);

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);

        int bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
        if (bytes_received < 0)
        {
            perror("Erreur de réception");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connecté depuis %s:%d\n", client_ip, ntohs(client_address.sin_port));

        printf("Commande reçue : %s\n", buffer);
        traiter_commande(buffer, response);

        sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_address, addrlen);

        printf("Client déconnecté : %s:%d\n", client_ip, ntohs(client_address.sin_port));
    }

    close(server_fd);
    return 0;
}
