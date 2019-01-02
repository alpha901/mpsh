#include "commande_app.h"
#define LONGUEUR_MAX_COMMANDE 100

char caracteresAutorises[]={'.','"','_','$','/','~','-','=','?',':','>','<','|','&'};
char *nomsCommandes[]={"cd","alias","cat","echo","exit","history","ls","mkdir","pwd",
"type","unalias","umask","export"};

/* retourne le nombre de mots de commande si c'est 1 commande valide,-1 si la commande est vide
et 0 sinon (stocke les mots dans motsDeLaCommande).Si la commande contient 1 redirection,
on affecte sa position dans la commande à indiceRedirection et si elle contient 1 affectation, on
met à jour affectation à 1 */
short commandeCorrecte(char *commande,char **motsDeLaCommande,short *indiceRedirection,short *affectation, short *contientConnecteur, short *contientPipe,short *alias){
	if(!strcmp(commande,""))
		return -1; // la commande est vide
	char tmp[strlen(commande)+1];
	strcpy(tmp,commande);
	char *mot=strtok(tmp," "); // on parcourt tous les mots de la commande séparés par des espaces
	short premierMotEstUneCommande=0,nbMots=0,positionCourant=0;
	char caractereCourant;
	//on vérifie s'il y'a des connecteurs logiques et pipe
	while(mot!=NULL){
		motsDeLaCommande[nbMots]=(char *)malloc(strlen(mot)+1);
		strcpy(motsDeLaCommande[nbMots++],mot);
		//on vérifie d'abord le premier mot est une commande
		if(!premierMotEstUneCommande){
			for(int j=0;j<NB_COMMANDES;j++){
				if(strcmp(nomsCommandes[j],mot)==0){
					premierMotEstUneCommande=1;
					break;
				}
			}
			if(!premierMotEstUneCommande){
				//commande incorrecte si ce n' est pas 1 affectation à 1 variable ou 1 alias
				if (estUneAffectationDeVariable(mot,motsDeLaCommande,indiceRedirection,affectation)){
					*affectation=1;
					return 1;
				}
				if(find_alias(mot)!=NULL){
					*alias=1;
					return 1;
				}
				return 0;
				//premierMotEstUneCommande=1;
			}
			positionCourant = positionCourant + (strlen(mot)+1); 
		}
		//mots suivants
		else{
			// on vérifie s'il y a des connecteurs logiques et pipe
			if((strcmp(mot,"||")==0)||(strcmp(mot,"&&")==0)||(strcmp(mot,"|")==0)){
				// s'il existe un connecteur ou pipe, on vérifie si la suite de commande est aussi correcte
				char tmp2[strlen(commande)+1];
				strcpy(tmp2,commande);
				if ((strcmp(mot,"||")==0)||(strcmp(mot,"&&")==0)){
					*contientConnecteur = 1;
				}
				else if (strcmp(mot,"|")==0)
					*contientPipe = 1;
				positionCourant = positionCourant + (strlen(mot)+1);
				short traitementDeLaSuiteDeCommande = commandeCorrecte(&tmp2[positionCourant],motsDeLaCommande+nbMots,indiceRedirection,affectation,contientConnecteur,contientPipe,alias);
				if(!traitementDeLaSuiteDeCommande)
						return 0;
				nbMots += traitementDeLaSuiteDeCommande;
			}
			else{
				int caractereValide=0,i=0;
				caractereCourant=mot[i];
				while(caractereCourant!='\0'){
					if(!isalnum(caractereCourant)){
						//on vérifie s'il y'a 1 redirection
						if(!strcmp(mot,"<") || !strcmp(mot,">") || !strcmp(mot,"2>"))
							*indiceRedirection=nbMots-1;
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
				}//while
				positionCourant = positionCourant + (strlen(mot)+1);
			}//else
		}//else mots suivants
		mot=strtok(NULL," ");
	}//while
	if(*indiceRedirection && *indiceRedirection==nbMots-1)
		return 0;
	return nbMots;
}

// si la commande est 1 affectation de valeur à 1 variable on retourne 1, sinon on retourne 0
short estUneAffectationDeVariable(char *mot,char **motsDeLaCommande,short *indiceRedirection,short *affectation){
	motsDeLaCommande[1]=(char *)malloc(strlen(mot)+1);
	for(int i=0;i<strlen(mot);i++){
		if(mot[i]=='='){//affectation de valeur à 1 variable
			//le 1er mot de la commande est la variable (à gauche du =),le 2eme la valeur (à droite)
			motsDeLaCommande[0][i]='\0';
			strncpy(motsDeLaCommande[1],mot+i+1,strlen(mot)-i);
			if(i==0 || atoi(motsDeLaCommande[0]))
				return 0;
			mot=strtok(NULL," ");//on vérifie ce qui suit la commande après l'affectation
			if(mot==NULL) //s'il n'y a rien la commande est correcte
				return 1;
			//si ce n'est pas 1 pipe ou 1 opérateur logique la commande est incorrecte
			else if(strcmp(mot,"&&") && strcmp(mot,"||") && strcmp(mot,"|"))
				return 0;
			//on traite la 1ere partie commande et on vérifie si la suite de la commande est correcte
			else{
				return 1;
			}
		}
	}
	return 0;
}
//On affecte valeur à la variable nomVariable
void traitementAffectation(char *nomVariable,char *valeur,short *traitementReussi){
	listeDesVariables=add_variable(listeDesVariables,nomVariable,valeur);
	*traitementReussi=0;
}

//On traite la commande correcte
void traitementCommande(char **motsDeLaCommande,int nbMots, short *traitementReussi,short alias){
	if(nbMots!=-1){  //si la commande est vide on ne la traite pas
		//préparation du tableau des mots de la commande pour le execv
		char **elementsDeLaCommande=(char **)malloc((nbMots+1)*sizeof(char *));
		if(elementsDeLaCommande==NULL){
			printf("Echec du traitement de la commande \n");
			*traitementReussi=0;
		}
		if(alias && find_alias(motsDeLaCommande[0])!=NULL){
			char *nouvelleCommande=(char *)malloc(LONGUEUR_MAX_COMMANDE);
			if(nouvelleCommande==NULL){
				perror("Echec de la commande\n");
				*traitementReussi=1;
				return;
			}
			strcpy(nouvelleCommande,find_alias(motsDeLaCommande[0]));
			strcat(nouvelleCommande," ");
			for(int i=1;i<nbMots;i++){
				strcat(nouvelleCommande,motsDeLaCommande[i]);
				strcat(nouvelleCommande," ");
			}
			executer_commande(nouvelleCommande);
			for(int i=0;i<nbMots;i++)
				free(motsDeLaCommande[i]);
			free(nouvelleCommande);
			return;
		}	
		else{
			memcpy(elementsDeLaCommande,motsDeLaCommande,nbMots*sizeof(char *));
			elementsDeLaCommande[nbMots]=NULL;	
			//On traite des commandes internes
			if (strcmp(elementsDeLaCommande[0],"cd")==0)
				traitementCommandeCD(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"exit")==0)
				traitementCommandeEXIT(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"echo")==0)
				traitementCommandeECHO(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"umask")==0)
				traitementCommandeUMASK(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"alias")==0)
				traitementCommandeAlias(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"unalias")==0)
				traitementCommandeUnalias(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"export")==0)
				traitementCommandeExport(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"history")==0)
				traitementCommandeHistory(elementsDeLaCommande, nbMots,traitementReussi);

			else if (strcmp(elementsDeLaCommande[0],"type")==0)
				traitementCommandeType(elementsDeLaCommande, nbMots,traitementReussi);

			else{ //On traite des commandes externes
				struct stat *x=malloc(sizeof(struct stat));
				if(x==NULL){
					perror("Échec de la commande \n");
					*traitementReussi=1;
					for(int i=0;i<nbMots;i++)
						free(elementsDeLaCommande[i]);
					free(elementsDeLaCommande);
					return;
				}
				char *pathDeChemin=strtok(&find_env("CHEMIN")[7],":");
				char pathCourant[50];
				*traitementReussi=0;
				while(pathDeChemin!=NULL){
					sprintf(pathCourant,"%s/%s",pathDeChemin,motsDeLaCommande[0]);
					//on parcourt les répertoires contenant les exécutables des commandes externes
					int k=stat(pathCourant,x);
					if(k==-1){
						pathDeChemin=strtok(NULL,":");
						continue;
					}
					else{
						if(S_ISREG(x->st_mode)){
							// si l'exécutable correspondant à la commande existe,on l'exécute dans le processus fils
							int pid = fork();
							if (pid<0)	{
								perror("Échec de la commande \n");
								*traitementReussi=1;
								break;
							}
							else if (pid==0){
									if (execv(pathCourant,elementsDeLaCommande)<0){
											perror("Échec de la commande\n");
											*traitementReussi=1;
											exit(1);
									}
							}
							else{
								waitpid(pid,NULL,0);// le père attend le fils
								break;
							}
						}//if
					}//else
					pathDeChemin=strtok(NULL,":");
				}//while	
				free(x);
			}//else commande externe
		//libération de la mémoire allouée
		for(int i=0;i<nbMots;i++)
				free(elementsDeLaCommande[i]);
		free(elementsDeLaCommande);
		}//else
		
	}//if
}

//traitement d'1 commande contenant 1 redirection
void traitementCommandeAvecRedirection(char **motsDeLaCommande,int nbMots,int indiceRedirection,short *traitementReussi,short alias){
	int fd=0, redirectionReussie=0;
	if(!strcmp(motsDeLaCommande[indiceRedirection],"<"))
		redirectionReussie=redirectionEntree(motsDeLaCommande[nbMots-1],&fd);
	else if(!strcmp(motsDeLaCommande[indiceRedirection],">"))
		redirectionReussie=redirectionSortie(motsDeLaCommande[nbMots-1],&fd);
	else
		redirectionReussie=redirectionErreur(motsDeLaCommande[nbMots-1],&fd);
	if(!redirectionReussie)
		perror("Échec de la commande \n");
	else{
		traitementCommande(motsDeLaCommande,nbMots-2,traitementReussi,alias);
		//on restaure stdin,stdout et stderr et on ferme les 2 descripteurs créés
		dup2(redirectionReussie,1);
		dup2(redirectionReussie,2);
		dup2(redirectionReussie,0);
		close(redirectionReussie);
		close(fd);
	}
}

void traitementCommandeAvecConnecteur(char **motsDeLaCommande,int nbMots, short*traitementReussi, short alias){
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
        if ((et==0)&&(ou==0)){
            traitementCommande(motsDeLaCommande,nbMots,traitementReussi, alias);
            return;
        }

        // on sépare la commande en deux parties et les stocke
        char **elementsGauche=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
        char **elementsDroite=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
        if ((elementsGauche==NULL)||(elementsDroite==NULL)){
            perror("Problème d'allocation mémoire\n");
            return;
        }
        for (int j = 0; j < i; j++){
            elementsGauche[j]=motsDeLaCommande[j];
        }
        for (int z = 0, k = i+1; k < nbMots; z++,k++){
            elementsDroite[z]=motsDeLaCommande[k];
        }
        // on traite la partie gauche avec son connecteur
        if (ou > 0) {
            traitementCommande(elementsGauche,ou,traitementReussi, alias);
            free(elementsGauche);
            // si "ou" et la commande gauche ne fonctionne pas, on contenue à traiter la partie droite
            if (*traitementReussi) {
                *traitementReussi = 1;
                traitementCommandeAvecConnecteur(elementsDroite,nbMots-i-1,traitementReussi, alias);
                free(elementsDroite);
            }else
                free(elementsDroite);
        }else if (et > 0) {
            traitementCommande(elementsGauche,et,traitementReussi, alias);
            // si "et" et la commande gauche fonctionne, on contenue à traiter la partie droite
            if (!(*traitementReussi)) {
                *traitementReussi = 1;
                traitementCommandeAvecConnecteur(elementsDroite,nbMots-i-1,traitementReussi, alias);
                free(elementsDroite);
            }else
                free(elementsDroite);
        }
}

//traitement d'une commande contenant un Pipe |
void traitementCommandeAvecPipe(char **motsDeLaCommande,int nbMots, short*traitementReussi,short alias){
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
					traitementCommande(elementsGauche,i,traitementReussi,alias);
					free(elementsGauche);
			}
			else{
					dup2(fd[0],0);
					close(fd[1]);
					close(fd[0]);
					traitementCommande(elementsDroite,nbMots-i-1,traitementReussi,alias);
					free(elementsDroite);
			}
	}
	else
	  perror("Erreur");
}
void executer_alias(char *alias_name){
    char *commande = find_alias(alias_name);
    if(commande != NULL){
        char *s= malloc(sizeof(char)*(strlen(commande)-1));
        int i;
        for (i = 0; i < strlen(commande)-1; ++i)
            s[i] = commande[i];    
        s[i] = '\0';
        executer_commande(s);
    }
}
//chaque commande saisie exécute cette fonction
void executer_commande(char *commande){
	ajout_history(commande);
	char **motsCommande=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
	if(motsCommande==NULL)
		perror("Problème d'allocation mémoire\n");
	else{
		int nbMots=0;
		short affectation=0,redirection=0,contientConnecteur=0,traitementReussi=1,contientPipe=0,alias=0;
		if(!(nbMots=commandeCorrecte(commande,motsCommande,&redirection,&affectation,
			&contientConnecteur,&contientPipe,&alias))){
			printf("La commande \"%s\" n'est pas correcte.\n",commande);
			listeDesVariables=add_variable(listeDesVariables,"?","1");
		}
		else{
			char valeurDeRetour[2];
			if(redirection)
				traitementCommandeAvecRedirection(motsCommande,nbMots,redirection,&traitementReussi,alias);
			else if(contientConnecteur)
				traitementCommandeAvecConnecteur(motsCommande,nbMots,&traitementReussi,alias);
			else if(contientPipe)
				traitementCommandeAvecPipe(motsCommande,nbMots,&traitementReussi,alias);
			else if(affectation)
				traitementAffectation(motsCommande[0],motsCommande[1],&traitementReussi);
			else	
				traitementCommande(motsCommande,nbMots,&traitementReussi,alias);
			sprintf(valeurDeRetour,"%d",traitementReussi);
			listeDesVariables=add_variable(listeDesVariables,"?",valeurDeRetour);
		}
		free(motsCommande);
	}
	free(commande); //libération de la chaîne contenant la commande lue
}
