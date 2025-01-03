#ifndef COMPTE_H
#define COMPTE_H

#define MAX_OPERATIONS 10

typedef struct
{
    int id_client;
    int id_compte;
    char password[20];
    float solde;
    char operations[MAX_OPERATIONS][100];  // Historique des 10 dernières opérations
    int operations_count;                  // Compteur d'opérations
} Compte;



// Déclaration des fonctions
Compte* trouver_compte(int id_client, int id_compte, const char *password);
void ajouter_operation(Compte *compte, const char *operation);

#endif // COMPTE_H
