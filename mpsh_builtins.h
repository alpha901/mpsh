#ifndef MPSH_BUILTINS_H
#define MPSH_BUILTINS_H


/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/

<<<<<<< HEAD
void traitementCommandeAlias(char **elementsDeLaCommande, int nbMots);
void traitementCommandeUnalias(char **elementsDeLaCommande, int nbMots);
void traitementCommandeExport(char **elementsDeLaCommande, int nbMots);
void traitementCommandeHistory(char **elementsDeLaCommande, int nbMots);
void traitementCommandeType(char **elementsDeLaCommande, int nbMots);

=======
void traitementCommandeECHO(char ** elementsDeLaCommande, int nbMots, short*traitementReussi);
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots, short*traitementReussi);
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots, short*traitementReussi);
>>>>>>> baced3edcca9f6bc6432c1cd6d8cda4c91ceba4f
#endif
