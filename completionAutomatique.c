#include "completionAutomatique.h"
#include "commande_app.h"
#define MAX_READED 1024

char *nomsCommandes[];

/* retourne 1 tableau de strings (des commandes,des alias ou des fichiers et des dossiers) 
ayant pour plus long préfixe commun prefixe */
char **completionDuMotCourant(const char *prefixe,short debut,short fin){
	return rl_completion_matches(prefixe,genererLeMotCorrespondant);
}

/* retourne si elle existe la commande qui a le plus long préfixe commun avec prefixe
et sinon l'alias correspondant */ 
char *genererLeMotCorrespondant(const char *prefixe,int e){
	static int i,longueurPrefixe;
	if(!e){
		i=0;
		longueurPrefixe=strlen(prefixe);
	}
    char *nom=malloc(50);
	if(nom==NULL){
		perror("Problème de complétion\n");
		return NULL;
	}
    while(i<NB_COMMANDES){
		strcpy(nom,nomsCommandes[i++]);
        if (!strncmp(nom,prefixe,longueurPrefixe))
			return nom;
    }
	FILE *f =fopen(".alias.txt", "r");
	if(f==NULL)
		return NULL;
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f)!=NULL){
		strcpy(nom,strtok(buf,"="));
		if(!strncmp(nom,prefixe,longueurPrefixe)){
			fclose(f);
			return nom;
		}
	}
	free(nom);
	fclose(f);
    return NULL;
}
