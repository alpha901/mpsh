#include "mpsh_builtins.h"

#define NB_COMMANDES 13
#define CARACTERES_AUTORISES 13
#define NB_MAX_MOTS_COMMANDE 5

char caracteresAutorises[]={'.','_','$','/','~','-','=','?',':','>','<','|','&'};
char *nomsCommandes[]={"cd","alias","cat","echo","exit","history","ls","mkdir","pwd",
"type","unalias","umask","export"};

//retourne le nombre de mots de commande si c'est 1 commande valide, 0 sinon (stocke les mots dans motsDeLaCommande)
short commandeCorrecte(char *commande,char **motsDeLaCommande){
	char tmp[strlen(commande)+1];
	strcpy(tmp,commande);
	char *mot=strtok(tmp," "); // on parcours tous les mots de la commande séparés par des espaces
	short premierMotEstUneCommande=0,nbMots=0;
	char caractereCourant;
	while(mot!=NULL){
		motsDeLaCommande[nbMots]=(char *)malloc(strlen(mot)+1);
		strcpy(motsDeLaCommande[nbMots++],mot);

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
	return nbMots;
}


//On traite la commande correcte
void traitementCommande(char **motsDeLaCommande,int nbMots){
	//préparation du tableau des mots de la commande pour le execv
	char **elementsDeLaCommande=(char **)malloc((nbMots+1)*sizeof(char *));
	if(elementsDeLaCommande==NULL)
		printf("Echec du traitement de la commande \n");
	else{
		memcpy(elementsDeLaCommande,motsDeLaCommande,nbMots*sizeof(char *));
		elementsDeLaCommande[nbMots]=NULL;
		struct stat *x=malloc(sizeof(struct stat));
		char *path1=malloc(10+strlen(motsDeLaCommande[0])),*path2=malloc(10+strlen(motsDeLaCommande[0]));
		strcpy(path1,"/bin/");
		strcpy(path2,"/usr/bin/");
		char *pathsCommandesExternes[]={path1,path2};
		//On traite des commandes internes;
		if (strcmp(elementsDeLaCommande[0],"cd")==0) {
			traitementCommandeCD(elementsDeLaCommande, nbMots);
		}
		else if (strcmp(elementsDeLaCommande[0],"exit")==0){
			traitementCommandeEXIT(elementsDeLaCommande, nbMots);
		}else{
			//On traite des commandes externes;
			for(int i=0;i<2;i++){ //on parcourt les répertoires contenant les exécutables des commandes externes
				strcat(pathsCommandesExternes[i],motsDeLaCommande[0]);
				int k=stat(pathsCommandesExternes[i],x);
				if(k==-1)
						perror(motsDeLaCommande[0]);
				else{
					if(S_ISREG(x->st_mode)){ // si l'exécutable existe bien, on l'exécute dans 1 nouveau processus
							int pid = fork();//on crée 1 processus fils qui exécute la commande
								if (pid<0){
									printf("Échec de la commande \n");
								}
								else if (pid==0){
										if (execv(pathsCommandesExternes[i],elementsDeLaCommande)<0){
												perror("Échec de exec");
												exit(1);
										}
								}
								else{
									waitpid(pid,NULL,0);// le père attend le fils
									for(int i=0;i<nbMots;i++)
									free(motsDeLaCommande[i]);
									free(elementsDeLaCommande);//on désalloue le tableau utilisé pour execv
									break;
								}
					}//if
				}//else
			}//for
		}
	}//else
}

int main() {
	while(1){
		char *commande=readline("mon prompt>>");
		char **motsCommande=(char **)malloc(NB_MAX_MOTS_COMMANDE*sizeof(char *));
		//si la commande n'est pas 1 chaîne vide,on la vérifie et on la traite si elle est correcte
		if(strcmp(commande,"")){
			if(motsCommande!=NULL){
				int nbMots;
				if(!(nbMots=commandeCorrecte(commande,motsCommande)))
					printf("La commande \"%s\" n'est pas correcte.\n",commande);
				else	traitementCommande(motsCommande,nbMots);
				free(motsCommande);
			}
		}
		free(commande);
	}
	return 0;
}
