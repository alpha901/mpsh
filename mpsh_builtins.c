/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/

#include "commande_app.h"

//On traite la commande echo
void traitementCommandeECHO(char **elementsDeLaCommande, int nbMots){
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
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots) {
	if ((nbMots==1)||((nbMots==2)&&(elementsDeLaCommande[1][0]=='~')))
		chdir(getenv("HOME"));
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


void traitementCommandeAlias(char **elementsDeLaCommande, int nbMots){
	if(nbMots == 1)//dans la commande on a que le mot "alias" => on affiche la liste des alias
		print_all_alias();
	else{
		for (int i = 1; i < nbMots; ++i)
		{
			char *nom_alias = strtok(elementsDeLaCommande[i], "=");
			char *valeur_alias = strtok(NULL, "=");
			if(valeur_alias == NULL){
				valeur_alias=find_alias(nom_alias);
				if(valeur_alias!=NULL)
					fprintf(stdout, "%s=%s",nom_alias, valeur_alias );
			}
			else{
				if(strtok(NULL, "=") != NULL)
					perror("-mpsh : alias : Erreur\n");
				else
					update_alias(nom_alias, valeur_alias);
			}
		}	
	}
}

void traitementCommandeUnalias(char **elementsDeLaCommande, int nbMots){
	if(nbMots == 1)//dans la commande on a que le mot "alias" => on affiche la liste des alias
		perror("-mpsh : unalias\n");
	else{
		for (int i = 1; i < nbMots; ++i)
		{
			delete_alias(elementsDeLaCommande[i]);
		}	
	}
}

void traitementCommandeExport(char **elementsDeLaCommande, int nbMots){
	if(nbMots == 1)//dans la commande on a que le mot "alias" => on affiche la liste des alias
		print_all_env();
	else{
		for (int i = 1; i < nbMots; ++i)
		{
			list_variables p = find_variable(listeDesVariables, elementsDeLaCommande[i]);
			if(p!=NULL){
				update_env(p->nom_variable, p->valeur_variable);
			}
		}	
	}
}

void traitementCommandeHistory(char **elementsDeLaCommande, int nbMots){
	if (nbMots == 1)
		list_history();
	else if(nbMots == 2){
		double n = atoi(elementsDeLaCommande[1]);
		if(n < 0)
			update_max_nombre_de_commandes((int)fabs(n));
		else{
			char *commande = find_history((int)n);
			if(commande == NULL)
				perror("-mpsh : history : numero non valide\n");
			else{
				char *q=strtok(commande, "\n");
				executer_commande(q);
			}
		}
	}
	else
		perror("-mpsh : history : Erreur\n");
}
void traitementCommandeType(char **elementsDeLaCommande, int nbMots){
	char *nomsCommandes[]={"cd","alias","cat","echo","exit","history","ls","mkdir","pwd",
		"type","unalias","umask","export"};//mauvais
	for(int i = 1 ; i<nbMots; i++){
		char *commande = find_alias(elementsDeLaCommande[i]);
		if (commande != NULL)		
			fprintf(stdout, "%s est un alias vers : %s", elementsDeLaCommande[i], commande);
		else{
			int trouve=0;
			for(int j=0; j<NB_COMMANDES; j++){
				if(strcmp(elementsDeLaCommande[i], nomsCommandes[j]) == 0 ){
					fprintf(stdout, "%s est une primitive de mpsh\n", elementsDeLaCommande[i]);
					trouve=1;
					break;
				}
			}
			if(!trouve)
				fprintf(stdout, "-mpsh : type : %s : non trouve\n", elementsDeLaCommande[i]);
		}
	}
}