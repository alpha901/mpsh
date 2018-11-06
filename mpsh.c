#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
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
				printf("%c ",caractereCourant);
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

int main() {
  while(1){
    char *commande=readline("mon prompt>>");
	printf("%s \n",commande);
    printf("%d \n",commandeCorrecte(commande));
    free(commande);
  }
  return 0;
}


