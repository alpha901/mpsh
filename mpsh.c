/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH (contient le main) **************/
#include "commande_app.h"


//liste chaînée stockant les variables
list_variables listeDesVariables;
int main(void){
	clear_terminal();
	while(1){
		char *commande=readline("mon prompt>>");
		executer_commande(commande);
	}
	return 0;
}
