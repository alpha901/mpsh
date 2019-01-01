#ifndef COMMANDE_APP_H
#define COMMANDE_APP_H

#include "redirections.h"
#include "mpsh_builtins.h"
#include "variables.h"
#include "history.h"
#include "variables_env.h"
#include "alias.h"
#include <math.h>
#include <ctype.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NB_COMMANDES 13
#define CARACTERES_AUTORISES 13
#define NB_MAX_MOTS_COMMANDE 5

#define CLEAR_TERMINAL printf("\033[H\033[J") //efface le contenu du terminal et positionne le curseur au point de coordonnees (0,0)


/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH**************/
void executer_commande(char *commande);
short commandeCorrecte(char *commande,char **motsDeLaCommande,short *indiceRedirection,short *affectation,short *contientConnecteur,short *contientPipe,short *alias);
short estUneAffectationDeVariable(char *mot,char **motsDeLaCommande,short *indiceRedirection,short *affectation);
void traitementCommande(char **motsDeLaCommande,int nbMots,short *traitementReussi,short alias);
void traitementCommandeAvecRedirection(char **motsDeLaCommande,int nbMots,int indiceRedirection,short *traitementReussi,short alias);
void traitementCommandeAvecConnecteur(char **motsDeLaCommande,int nbMots,short *traitementReussi,short alias);
void traitementCommandeAvecPipe(char **motsDeLaCommande,int nbMots,short *traitementReussi,short alias);
void traitementAffectation(char *nomVariable,char *valeur,short *traitementReussi);
void executer_alias(char *alias_name);
#endif
