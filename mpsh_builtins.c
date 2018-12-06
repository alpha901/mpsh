#include "variables_env.h"
#include "variables_affectation.h"
#include "mpsh_builtins.h"

list_variables liste_of_variables = NULL;


//Fonction incomplette
void echo(char *variable_name){
	list_variables l = find_affectation(liste_of_variables, variable_name);
	if(l != NULL){
		write(STDOUT_FILENO, l->valeur_variable, strlen(l->valeur_variable));
	}
	else{
		//a suivre...
	}
}

//On traite la commande cd
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots) {
	if ((nbMots==1)||((nbMots==2)&&(elementsDeLaCommande[1][0]=='~'))){
		chdir(getenv("HOME"));
	}
	else if (nbMots>2) printf("Trop d'arguments\n");
	else {
		if (elementsDeLaCommande[1][0]=='.') {
			// on va dans le dossier parent
			char *newDir = malloc(60*sizeof(char*));
			int j = 0;
			for (int i = 0; i < strlen(elementsDeLaCommande[1])-1; i++) {
				if (elementsDeLaCommande[1][i]=='.') {
					if (elementsDeLaCommande[1][i+1]=='.') {
						char buf[255];;
						getcwd(buf, sizeof(buf));
						int nbDirs=0;//nombre de dossiers du chemin courant
						char *dir=strtok(buf,"/");
						while (dir!=NULL) {
							nbDirs++;
							dir=strtok(NULL,"/");
						}
						char buf2[255];
						getcwd(buf2, sizeof(buf2));
						char * dir2 = strtok(buf2,"/");
						char * newPath = malloc(255*sizeof(char));
						for (int i = 0; i < nbDirs-1; i++) {
							strcat(newPath,"/");
							strcat(newPath,dir2);
							dir2=strtok(NULL,"/");
						}
						chdir(newPath);
						free(newPath);
					}
				}else if(elementsDeLaCommande[1][i]=='/'){
					if (elementsDeLaCommande[1][i+1]!='.'){
						newDir[j]=elementsDeLaCommande[1][i];
						j++;
					}
				}else{
					newDir[j]=elementsDeLaCommande[1][i];
					j++;
				}
			}
			chdir(strtok(newDir,"/"));
			free(newDir);
		}
		else{
			// on va dans 1 sous-dossier
			chdir(elementsDeLaCommande[1]);
		}
	}
}

//On traite la commande exit
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots) {
	if (nbMots==1) exit(0);
	else if(nbMots>2) printf("Trop d'arguments\n" );
	else exit(atoi(elementsDeLaCommande[1]));
}
