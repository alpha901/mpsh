#ifndef MPSH_BUILTINS_H
#define MPSH_BUILTINS_H


/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/

void traitementCommandeECHO(char ** elementsDeLaCommande, int nbMots);
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots);
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots);
#endif
