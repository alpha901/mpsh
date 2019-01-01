#include "configuration.h"
#include "variables.h"

list_variables listeDesVariables;
//lit le fichier .mpshrc pour mettre à jour la variable INVITE et les fichiers cibles des complétions automatiques
short lectureDuFichierDeConfiguration(){
	if(creationDuFichierDeConfiguration()){
		int fd=open(".mpshrc",O_RDONLY,0644),n;
		if(fd==-1){
			perror("Le fichier de configuration n'a pas pu être lu\n");
			return 0;
		}
		else{
			char buffer[NB_CARACTERES_PAR_LIGNE];
			char *mot,*commande,*extension,*valeurInvite;
			while((n=read(fd,buffer,NB_CARACTERES_PAR_LIGNE))){
				mot=strtok(buffer," ");
				if(!strcmp(mot,"complete")){
		 				commande=strtok(NULL," ");
					if(commande==NULL){
						printf("Fichier de configuration incorrect\n");
						return 0;
					}
					extension=strtok(NULL," ");
					if(extension==NULL){
						printf("Fichier de configuration incorrect\n");
						return 0;
					}
					ajouterAssociation(listeDesAssociations,commande,extension);
				}
				else if(!strcmp(mot,"INVITE")){
					valeurInvite=strtok(mot,"=");
					valeurInvite=strtok(NULL,"=");
					if(valeurInvite==NULL){
						printf("Fichier de configuration incorrect\n");
						return 0;
					}
					listeDesVariables=add_variable(listeDesVariables,"INVITE",valeurInvite);
				}
			}//while
			return 1;
		}//else
	}
	return 0;
}
//créé le fichier .mpshrc s'il n'existe pas, retourne 1 s'il existe déjà ou qu'il a bien été crée et 0 sinon
short creationDuFichierDeConfiguration(){
	if(fichierDeConfigurationExiste())
		return 1;
	else{
		int fd=open(".mpshrc",O_WRONLY,0644);
		if(fd==-1){
			perror("Le fichier de configuration n'a pas pu être créé\n");
			return 0;
		}
		char ligneInvite[]="red";
		if(write(fd,ligneInvite,NB_CARACTERES_PAR_LIGNE)!=strlen(ligneInvite))
			return 0;
		return 1;
	}
}
//retourne 1 si le fichier .mpshrc existe dans le dossier courant et 0 sinon
short fichierDeConfigurationExiste(){
	struct stat *x=malloc(sizeof(struct stat));
	int i=stat(".mpshrc",x);
	if(i==-1){
		perror("le fichier n'existe pas");
		return 0;
	}
	else	return ((x->st_mode) & S_IFMT)==S_IFREG;
}

listeAssociations allouerAssociation(char *nomCommande, char *nomExtension){
  listeAssociations l=malloc(sizeof(associationCommande));
  if(l==NULL)
    return NULL;
  l->commande=nomCommande;
  l->extension=nomExtension;
  l->associationSuivante=NULL;
  return l;
}
/* Ajoute 1 nouvelle association d'extension à 1 commande à la liste chaînée l et la met à jour si elle existe déjà */
listeAssociations ajouterAssociation(listeAssociations l, char *nomCommande, char *nomExtension){
	listeAssociations nouvelleListe,associationExistante;
	if( (associationExistante=trouverAssociation(l,nomCommande))!=NULL){
		//l'association existe,on la met à jour
		associationExistante->extension=nomExtension;
		return l;
	}
	else{
		nouvelleListe=allouerAssociation(nomCommande,nomExtension);
		if(nouvelleListe==NULL){
			perror("Problème dans la lecture du fichier de configuration\n");
			return l;
		}
		nouvelleListe->associationSuivante=l;
		return nouvelleListe;
	}
}
/* Retourne 1 pointeur vers la variable nom_variable si elle est dans la liste chaînée l et
NULL sinon */ 
listeAssociations trouverAssociation(listeAssociations l, char *nomCommande){
  while(l!=NULL){
    if (!strcmp(nomCommande,l->commande))
      return l;
    l=l->associationSuivante;
  }
  return NULL;
}

