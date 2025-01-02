#include "compte.h"
#include <string.h>

// Base de données simulée
static Compte comptes[] = {
    {1, 101, "azerty1", 500.0, {}, 0},
    {2, 102, "azerty2", 1000.0, {}, 0},
    {3, 103, "azerty3", 1500.0, {}, 0},
    {4, 104, "azerty4", 2000.0, {}, 0},
};
static int comptes_size = sizeof(comptes) / sizeof(comptes[0]);

Compte* trouver_compte(int id_client, int id_compte, const char *password) {
    for (int i = 0; i < comptes_size; i++) {
        if (comptes[i].id_client == id_client &&
            comptes[i].id_compte == id_compte &&
            strcmp(comptes[i].password, password) == 0) {
            return &comptes[i];
        }
    }
    return NULL;
}

void ajouter_operation(Compte *compte, const char *operation) {
    if (compte->operations_count < MAX_OPERATIONS) {
        strcpy(compte->operations[compte->operations_count], operation);
        compte->operations_count++;
    } else {
        // Décaler les anciennes opérations
        for (int i = 0; i < MAX_OPERATIONS - 1; i++) {
            strcpy(compte->operations[i], compte->operations[i + 1]);
        }
        strcpy(compte->operations[MAX_OPERATIONS - 1], operation);
    }
}
