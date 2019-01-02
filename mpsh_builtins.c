	/*******CONTIENT LES COMMANDES INTERNES DE MPSH (ECHO, CD ET EXIT) ******/

	#include "commande_app.h"

	//On traite la commande echo
	void traitementCommandeECHO(char **elementsDeLaCommande, int nbMots, short *traitementReussi){
		list_variables l;
		char *s=malloc(100);
		if(s==NULL){
			perror("-mpsh : echo : Erreur\n");
			*traitementReussi=1;
		}
		else{
			for(int i=1;i<nbMots;i++){
				if(elementsDeLaCommande[i][0]=='$' && elementsDeLaCommande[i][1]!='\0'){
					//c'est 1 variable
					l=find_variable(listeDesVariables,elementsDeLaCommande[i]+1);
					//si la variable existe, on affiche sa valeur
					if(l!=NULL){
						if(!strcmp(l->valeur_variable,""))	printf("0 ");
						else	printf("%s ",l->valeur_variable);
					}	
					//sinon on regarde si c'est 1 variable d'environnement
					else if((s=find_env(elementsDeLaCommande[i]+1))!=NULL)
							printf("%s ",strtok(&s[strlen(elementsDeLaCommande[i])],"\n"));
				}
				else	printf("%s ",elementsDeLaCommande[i]);
			}
			free(s);
			printf("\n");
			*traitementReussi=0;
		}
	}

	//On traite la commande cd
	void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots, short *traitementReussi) {
		*traitementReussi=0;
		if ((nbMots==1)||((nbMots==2)&&(elementsDeLaCommande[1][0]=='~'))){
			if(chdir(getenv("HOME"))==-1){
				*traitementReussi=1;
				perror("-mpsh: cd\n");
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
					*traitementReussi=1;
					perror("-mpsh: cd\n");
				}
			}
		}
		update_env("CHEMIN","/usr/local/sbin:/usr/local/bin:/usr/sbin:"
	"/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
	}

	//On traite la commande exit
	void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots, short*traitementReussi) {
		*traitementReussi=0;
		if (nbMots==1) exit(0);
		else if(nbMots>2){
			perror("-mpsh: exit\n");
			*traitementReussi=1;
		}
		else exit(atoi(elementsDeLaCommande[1]));
	}

	//On traite la commande umask
	void traitementCommandeUMASK(char ** elementsDeLaCommande, int nbMots, short*traitementReussi) {
		*traitementReussi=0;
		mode_t mask = umask(0);
		if (nbMots==1) {
			int tmp = (int)mask;
			int count = 0;
			while (tmp!=0) {
				tmp/=10;
				count++;
			}
			umask(mask);
			switch (count) {
				case 1 : printf("000%o\n", mask);break;
				case 2 : printf("00%o\n", mask);break;
				case 3 : printf("0%o\n", mask);break;
				case 4 : printf("%o\n", mask);break;
				default : perror("Error");*traitementReussi=1;
			}
		}else if(nbMots==2){
			if ((atoi(elementsDeLaCommande[1])<778)&&(atoi(elementsDeLaCommande[1])>=0)) {
				mask = strtol(elementsDeLaCommande[1], NULL, 8);
				umask(mask);
			}
		}
	}

		void traitementCommandeAlias(char **elementsDeLaCommande, int nbMots,short *traitementReussi){
			*traitementReussi=0;
			if(nbMots == 1)//dans la commande on a que le mot "alias" => on affiche la liste des alias
				print_all_alias();
			else{
			    int commande_lenght = 0;
			    for (int i = 1; i < nbMots; ++i)//calculer la longueur de la commande
			        commande_lenght += strlen(elementsDeLaCommande[i]);
			    char *s = malloc(sizeof(char)*(commande_lenght+nbMots-1));
			    if (s==NULL){    
			        perror("-mpsh : alias : Erreur\n");
							return;
					}
			    int n=0;
			    for (int i = 1; i < nbMots; ++i){//copier les element du tableau elementsDeLaCommande dans s
			        int t = strlen(elementsDeLaCommande[i]);
			        sprintf(s+n, "%s ", elementsDeLaCommande[i]);
			        n +=t+1;    
			    }
			    char *nom_alias, *valeur_alias;
			    char *pos = strchr(elementsDeLaCommande[1], '"');
			    if(pos != NULL){
			        nom_alias = strtok(s, "=");
			        valeur_alias = strtok(NULL, "\"");
			        update_alias(nom_alias, valeur_alias);
			    }
			    else{
			        for(int i = 1; i < nbMots; ++i){
			          nom_alias = strtok(elementsDeLaCommande[i], "=");
			          valeur_alias = strtok(NULL, "=");
			          if(valeur_alias == NULL){
			              valeur_alias=find_alias(nom_alias);
			              if(valeur_alias!=NULL)
			                  fprintf(stdout, "%s=%s",nom_alias, valeur_alias );
			              else{
							fprintf(stdout,"-mpsh : alias : %s : non trouve\n", nom_alias); 
											*traitementReussi=1;
										}       
			        }
			        else{
			            if(strtok(NULL, "=") != NULL){
			                perror("-mpsh : alias : Erreur\n");
												*traitementReussi=1;
										}
			              else   update_alias(nom_alias, valeur_alias);
			          }
			        }//for
				}//else
			}//else
		}

	void traitementCommandeUnalias(char **elementsDeLaCommande, int nbMots,short *traitementReussi){
		if(nbMots == 1){//aucun nom d'alias n'est donne pour etre supprime
			perror("-mpsh : unalias : utilisation : unalias nom [nom ...]\n");
			*traitementReussi=1;
		}
		else{
			for (int i = 1; i < nbMots; ++i)//supprime tous les alias dont le nom est donne dans la commande
				delete_alias(elementsDeLaCommande[i]);
			*traitementReussi=0;
		}
	}

	void traitementCommandeExport(char **elementsDeLaCommande, int nbMots,short *traitementReussi){
		if(nbMots == 1)//on affiche toutes les variable d'environnement
			print_all_env();
		else{
			for (int i = 1; i < nbMots; ++i)// ajouter les variables dont le nom est donne dans la commande
			{
				list_variables p = find_variable(listeDesVariables, elementsDeLaCommande[i]);
				if(p!=NULL){
					update_env(p->nom_variable, p->valeur_variable);
				}
			}
		}
		*traitementReussi=0;
	}

	void traitementCommandeHistory(char **elementsDeLaCommande, int nbMots,short *traitementReussi){
		*traitementReussi=0;
		if (nbMots == 1)//history sans parametre => afficher tout l'historique
			list_history();
		else if(nbMots == 2){
			double n=atoi(elementsDeLaCommande[1]);
			if(n < 0)//modifier le nombre de commande maximal autorise
				update_max_nombre_de_commandes((int)fabs(n));
			else{//executer la commande ayant le numero n dans l'historique
				char *commande = find_history((int)n);
				if(commande == NULL)
					perror("-mpsh : history : numero non valide\n");
				else{
					char *q=strtok(commande, "\n");
					executer_commande(q);
				}
			}
		}
		else{
			perror("-mpsh : history : Erreur\n");
			*traitementReussi=1;
		}
	}
	//char *nomsCommandes[];
	void traitementCommandeType(char **elementsDeLaCommande, int nbMots,short *traitementReussi){
		char *nomsCommandes[]={"cd","alias","cat","echo","exit","history","ls","mkdir","pwd",
		"type","unalias","umask","export"};
		*traitementReussi=0;
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
				if(!trouve){
					fprintf(stdout, "-mpsh : type : %s : non trouve\n", elementsDeLaCommande[i]);
					*traitementReussi=1;
				}
			}
		}
		
	}
