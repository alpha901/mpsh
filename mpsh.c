/*****************FICHIER GERANT LES COMMANDES SAISIES SUR LE PROMPT DE MPSH (contient le main) **************/
#include "commande_app.h"

//traitement d'une commande contenant un connecteur
void traitementCommandeAvecConnecteur(char **motsDeLaCommande,int nbMots, short*traitementReussi){
		int i = 0, et = 0, ou = 0;
		// on cherche la position de 1 er connecteur
		for (i = 0; i < nbMots; i++) {
			if (strcmp(motsDeLaCommande[i],"||")==0){
				ou = i; break;
			}
			else if (strcmp(motsDeLaCommande[i],"&&")==0) {
				et = i; break;
			}
		}
		// on vérifier s'il reste que 1 commande, on la traite directement
		if ((i==1)&&(et==0)&&(ou==0)){
			traitementCommande(motsDeLaCommande,nbMots,traitementReussi);
			return;
		}
		// on sépare la commande en deux parties et les stocke
		char **elementsGauche=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
		char **elementsDroite=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
		if ((elementsGauche==NULL)||(elementsDroite==NULL)){
			perror("Problème d'allocation mémoire\n");
			return;
		}
		for (int j = 0; j < i; j++)
			elementsGauche[j]=motsDeLaCommande[j];
		for (int z = 0, k = i+1; k < nbMots; z++,k++)
			elementsDroite[z]=motsDeLaCommande[k];
		// on traite la partie gauche avec son connecteur
		if (ou > 0) {
			traitementCommande(elementsGauche,ou,traitementReussi);
			free(elementsGauche);
			// si "ou" et la commande gauche ne fonctionne pas, on contenue à traiter la partie droite
			if (!(*traitementReussi)) {
				*traitementReussi = 1;
				traitementCommandeAvecConnecteur(elementsDroite,nbMots-i-1,traitementReussi);
				free(elementsDroite);
			}else
				free(elementsDroite);
		}else if (et > 0) {
			traitementCommande(elementsGauche,et,traitementReussi);
			// si "et" et la commande gauche fonctionne, on contenue à traiter la partie droite
			if (*traitementReussi) {
				*traitementReussi = 1;
				traitementCommandeAvecConnecteur(elementsDroite,nbMots-i-1,traitementReussi);
				free(elementsDroite);
			}else
				free(elementsDroite);
		}
}

//traitement d'une commande contenant un Pipe |
void traitementCommandeAvecPipe(char **motsDeLaCommande,int nbMots, short*traitementReussi){
	int i;
	for (i = 0; i < nbMots; i++) {
		if (strcmp(motsDeLaCommande[i],"|")==0)
			break;
	}
	char **elementsGauche=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
	char **elementsDroite=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
	if ((elementsGauche==NULL)||(elementsDroite==NULL)){
		perror("Problème d'allocation mémoire\n");
		return;
	}
	for (int j = 0; j < i; j++)
		elementsGauche[j]=motsDeLaCommande[j];
	for (int z = 0, k = i+1; k < nbMots; z++,k++)
		elementsDroite[z]=motsDeLaCommande[k];
	int fd[2];
	if(pipe(fd)>=0){
			if(fork()==0){
					dup2(fd[1],1);
					close(fd[0]);
					close(fd[1]);
					traitementCommande(elementsGauche,i,traitementReussi);
					free(elementsGauche);
			}
			else{
					dup2(fd[0],0);
					close(fd[1]);
					close(fd[0]);
					traitementCommande(elementsDroite,nbMots-i-1,traitementReussi);
					free(elementsDroite);
			}
	}
	else
	  perror("Erreur");
}

//liste chaînée stockant les variables
list_variables listeDesVariables;
int main(void){
	clear_terminal();
	while(1){
		char *commande=readline("mon prompt>>");
	return 0;
}
