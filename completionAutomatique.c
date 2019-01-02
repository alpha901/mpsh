#include "completionAutomatique.h"
#include "commande_app.h"
#define MAX_READED 1024
#define LONGUEUR_MAX_MOT 50
char *nomsCommandes[];
char **motsPossibles;//contient les commandes et les alias possibles

/* retourne 1 tableau de strings (des commandes,des alias ou des fichiers et des dossiers) 
ayant pour plus long préfixe commun prefixe */
char **completionDuMotCourant(const char *prefixe,short debut,short fin){
	char **resultat=rl_completion_matches(prefixe,genererLeMotCorrespondant);
	free(motsPossibles);
	return resultat;
}

/* retourne la commande ou l'alias qui a le plus long préfixe commun avec prefixe
et sinon NULL */ 
char *genererLeMotCorrespondant(const char *prefixe,int e){
	static int i,longueurPrefixe,nbAlias;
	if(!e){
			/* 1er appui sur la touche tab,on initialise l'indice des mots pouvant correspondre
			à 0 ainsi que la longueur du préfixe */
				i=0;
				longueurPrefixe=strlen(prefixe);
				nbAlias=nb_alias();	
				motsPossibles=(char **)malloc((NB_COMMANDES+nbAlias)*sizeof(char *));
			if(motsPossibles==NULL){
				perror("Problème de complétion\n");
				return NULL;
			}
			//on copie les commandes dans motsPossibles ...
			memcpy(motsPossibles,nomsCommandes,NB_COMMANDES*sizeof(char *));
			int j=NB_COMMANDES;
			FILE *f =fopen(".alias.txt", "r");
			if(f==NULL){
				perror("Problème de complétion\n");
				free(motsPossibles);
				return NULL;
			}
			char buf[MAX_READED];
			while(fgets(buf, MAX_READED, f)!=NULL){
				motsPossibles[j]=malloc(LONGUEUR_MAX_MOT);
				if(motsPossibles[j]==NULL){
					perror("Problème de complétion\n");
					free(motsPossibles);
					return NULL;
				}
				strcpy(motsPossibles[j++],strtok(buf,"="));//on récupère tous les alias
			}
			fclose(f);	
	}//if
  char *nom=malloc(LONGUEUR_MAX_MOT);
	if(nom==NULL){
		perror("Problème de complétion\n");
		free(motsPossibles);
		return NULL;
	}
  while(i<NB_COMMANDES+nbAlias){//on parcourt le tableau des commandes et des alias
		strcpy(nom,motsPossibles[i++]);
    if (!strncmp(nom,prefixe,longueurPrefixe))//si 1 mot correspond au préfixe,on le retourne
			return nom;
  }
	free(nom);
	return NULL;
}
