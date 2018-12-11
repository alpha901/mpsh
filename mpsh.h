#ifndef MPSH_H
#define MPSH_H

/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH**************/

short commandeCorrecte(char *commande,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
short estUneAffectationDeVariable(char *mot,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
void traitementCommande(char **motsDeLaCommande,int nbMots);
void traitementCommandeAvecRedirection(char **motsDeLaCommande,int nbMots,int indiceRedirection);
void traitementAffectation(char *nomVariable,char *valeur);
#endif
