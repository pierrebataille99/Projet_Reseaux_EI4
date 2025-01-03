#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char commande[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Adresse invalide ou non supportée");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur UDP. Tapez 'exit' pour quitter.\n");

    while (1)
    {
        printf("########################################################################################");
        printf("\n\n\nEntrez une commande client; exemple :\n\n"
               "AJOUT id_client id_compte MDP montant\n"
               "RETRAIT id_client id_compte MDP montant\n"
               "SOLDE id_client id_compte MDP\n"
               "OPERATIONS id_client id_compte MDP\n");

        printf("\nRequete: ");
        fgets(commande, BUFFER_SIZE, stdin);
        commande[strcspn(commande, "\n")] = 0;

        if (strcmp(commande, "exit") == 0)
        {
            printf("Déconnexion...\n");
            break;
        }

        sendto(sock, commande, strlen(commande), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received > 0)
        {
            printf("Réponse du serveur : %s\n", buffer);
        }
        else
        {
            printf("Erreur lors de la réception de la réponse.\n");
        }
    }

    close(sock);
    return 0;
}
