#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define NB_COMMANDES 13
#define CARACTERES_AUTORISES 13

char caracteresAutorises[]={'.','_','$','/','~','-','=','?',':','>','<','|','&'};
char *nomsCommandes[]={"cd","alias","cat","echo","exit","history","ls","mkdir","pwd","type","unalias","umask","export"};

//retourne 1 si la chaine commande est 1 commande valide pour mpsh, 0 sinon
short commandeCorrecte(char *commande){
	char *mot=strtok(commande," "); // on parcours tous les mots de la commande séparés par des espaces
	int premierMotEstUneCommande=0;
	char caractereCourant;
	while(mot!=NULL){
		if(!premierMotEstUneCommande){ //on traite d'abord le premier mot qui doit être une commande
			for(int j=0;j<NB_COMMANDES;j++){
				if(strcmp(nomsCommandes[j],mot)==0){
					premierMotEstUneCommande=1;
					break;
				}
			}
			if(!premierMotEstUneCommande) //commande incorrecte
				return 0;
		}
		//mots suivants
		else{
			int caractereValide=0,i=0;
			caractereCourant=mot[i];
			while(caractereCourant!='\0'){
				// printf("%c ",caractereCourant);
				if(!isalnum(caractereCourant)){
					for(int j=0;j<CARACTERES_AUTORISES;j++){
						if(caracteresAutorises[j]==caractereCourant){
							caractereValide=1;
							break;
						}
					}
					if(!caractereValide)
						return 0;
				}
				caractereCourant=mot[++i];
			}
		}//else
		mot=strtok(NULL," ");
	}//while
	return 1;
}

//On traite la commande correcte
void traitementCommande(char *commande){
	printf("test : le commande est %s\n", commande);
	char *cle;
	int argc = 1;
	cle =strtok(commande," "); // on parcours tous les mots de la commande séparés par des espaces
	int pos = -1;
	//pour localiser la commande en utilisant le tab nomsCommandes
	for(int j=0;j<NB_COMMANDES;j++){
		if(strcmp(nomsCommandes[j],cle)==0){
			pos = j;
			break;
		}
	}
	// pour compter qu'il y a combien d'argument
	while (cle != NULL)
	{
		cle = strtok(NULL, " ");
		argc++;
	}

	// stocker les arguments de commande dans un tab de chaine de caratère
	char * cmd[argc];
	cle =strtok(commande," ");
	cmd[0] = cle;
	printf("test : cmd[0]%s\n",cmd[0]);
	for (int i = 1; i < argc; i++) {
		cmd[i] = strtok(NULL, "\0");
		printf("test : cmd[%d]%s\n",i,cmd[i]);
	}

	// appliquer exec
	if (pos>0) {
		if (pos==6) {
			printf("test : [%s]\n",cle );
			execl("/bin/ls", "ls", cmd[1], (char *)0);
		}
	}

}

int main() {
  while(1){
    char *commande=readline("mon prompt>>");
		printf("%s\n",commande);

		if(!commandeCorrecte(commande)){
			// si la commande n'est pas correcte, on affiche error et refait la boucle.
			printf("Votre commande n'est pas correcte.\n");
		}else{
			// sinon la commande est correcte, on la traite.
			int pid = fork();//on crée des fils
			if (pid<0) {
				printf("Échec");
				exit(1);
			}else if (pid==0){
				traitementCommande(commande);
			}else{
				waitpid(pid,NULL,0);// le père attend le fils
			}
		}
    free(commande);
  }
  return 0;
}
