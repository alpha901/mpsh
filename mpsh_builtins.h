#ifndef MPSH_BUILTINS_H
#define MPSH_BUILTINS_H


/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/

void traitementCommandeAlias(char **elementsDeLaCommande, int nbMots,short *traitementReussi);
void traitementCommandeUnalias(char **elementsDeLaCommande, int nbMots,short *traitementReussi);
void traitementCommandeExport(char **elementsDeLaCommande, int nbMots,short *traitementReussi);
void traitementCommandeHistory(char **elementsDeLaCommande, int nbMots,short *traitementReussi);
void traitementCommandeType(char **elementsDeLaCommande, int nbMots,short *traitementReussi);
void traitementCommandeECHO(char ** elementsDeLaCommande, int nbMots, short *traitementReussi);
void traitementCommandeUMASK(char ** elementsDeLaCommande, int nbMots, short *traitementReussi);
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots, short *traitementReussi);
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots, short *traitementReussi);
#endif
