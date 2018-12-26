/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/
#include "mpsh_builtins.h"
#include "variables.h"
list_variables listeDesVariables;

//On traite la commande echo
void traitementCommandeECHO(char **elementsDeLaCommande, int nbMots, short*traitementReussi){
	list_variables l;
	for(int i=1;i<nbMots;i++){
		if(elementsDeLaCommande[i][0]=='$' && elementsDeLaCommande[i][1]!='\0'){
			//c'est 1 variable
			sprintf(elementsDeLaCommande[i],"%s",elementsDeLaCommande[i]+1);
			l=find_variable(listeDesVariables,elementsDeLaCommande[i]);
			if(l!=NULL) //si la variable existe, on affiche sa valeur
				printf("%s ",l->valeur_variable);
		}
		else	printf("%s ",elementsDeLaCommande[i]);
	}
	printf("\n");
}

//On traite la commande cd
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots, short*traitementReussi) {
	if ((nbMots==1)||((nbMots==2)&&(elementsDeLaCommande[1][0]=='~'))){
		if(chdir(getenv("HOME"))==-1){
			*traitementReussi = 0;
			perror("-bash: cd");
		}
	}
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
			if (chdir(elementsDeLaCommande[1])==-1) {
				*traitementReussi = 0;
				perror("-mpsh: cd");
			}
		}
	}
}

//On traite la commande exit
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots, short*traitementReussi) {
	if (nbMots==1) exit(0);
	else if(nbMots>2) perror("-mpsh: exit");
	else exit(atoi(elementsDeLaCommande[1]));
}
