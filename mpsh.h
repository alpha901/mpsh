#ifndef MPSH_H
#define MPSH_H

/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH**************/

short commandeCorrecte(char *commande,char **motsDeLaCommande,short *indiceRedirection,short *affectation,short *contientConnecteur,short *contientPipe);
short estUneAffectationDeVariable(char *mot,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
void traitementCommande(char **motsDeLaCommande,int nbMots,short *traitementReussi);
void traitementCommandeAvecRedirection(char **motsDeLaCommande,int nbMots,int indiceRedirection,short *traitementReussi);
void traitementCommandeAvecConnecteur(char **motsDeLaCommande,int nbMots,short *traitementReussi);
void traitementCommandeAvecPipe(char **motsDeLaCommande,int nbMots, short*traitementReussi);
void traitementAffectation(char *nomVariable,char *valeur);
#endif
