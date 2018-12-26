#ifndef COMMANDE_APP_H
#define COMMANDE_APP

#include "redirections.h"
#include "mpsh_builtins.h"
#include "variables.h"
#include "history.h"
#include "variables_env.h"
#include "alias.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <math.h>


#define NB_COMMANDES 13
#define CARACTERES_AUTORISES 13
#define NB_MAX_MOTS_COMMANDE 5

#define clear_terminal() printf("\033[H\033[J") //efface le contenu du terminal et positionne le curseur au point de coordonnees (0,0) 


/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH**************/
void executer_commande(char *commande);
short commandeCorrecte(char *commande,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
short estUneAffectationDeVariable(char *mot,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
void traitementCommande(char **motsDeLaCommande,int nbMots);
void traitementCommandeAvecRedirection(char **motsDeLaCommande,int nbMots,int indiceRedirection);
void traitementAffectation(char *nomVariable,char *valeur);

#endif
