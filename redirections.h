#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H
/*****************FICHIER GERANT LES REDIRECTIONS DE COMMANDES SUR MPSH**************/

/* les fonctions redirectionSortie,redirectionEntree et redirectionErreur appellent juste
 redirectionGenerale, elles retournent 1 si elles s'exécutent normalement et 0 en cas d'erreur */

/* redirection d'1 entree à 1 sortie dans le cas général, prenant 1 commande d'entrée et 1 sortie et
 ouvrant 1 descripteur de fichier sur sortie en mode modeOuverture */
int redirectionGenerale(char *sortie,char *modeOuverture,short erreur,int *fd);

//stocke dans le fichier sortie le contenu de la sortie standard de la commande 
int redirectionSortie(char *sortie,int *fd);

//l'entrée standard de la commande devient entree 
int redirectionEntree(char *entree,int *fd);

//stocke dans le fichier sortie le contenu de la sortie d'erreur de la commande 
int redirectionErreur(char *sortie,int *fd);
#endif
