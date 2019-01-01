/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH (contient le main) **************/
#include "commande_app.h"
#include "completionAutomatique.h"
#include "configuration.h"

/* listes chaînée stockant les variables et les extensions associées aux commandes pour les complétions
automatiques du fichier .mpshrc */
list_variables listeDesVariables;
listeAssociations listeDesAssociations;
int main(void){
	CLEAR_TERMINAL;
	update_env("CHEMIN","/usr/local/sbin:/usr/local/bin:/usr/sbin:"
	"/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
	listeDesVariables=add_variable(NULL,"?","0");
	lectureDuFichierDeConfiguration();
	rl_attempted_completion_function=completionDuMotCourant;
	while(1){		
		char *commande=readline("mon prompt>>");
		executer_commande(commande);
	}
	return 0;
}
