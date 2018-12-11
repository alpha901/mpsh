/*****************FICHIER GERANT LES REDIRECTIONS DE COMMANDES SUR MPSH**************/
#include "redirections.h"
#include "mpsh.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

/* Redirection d'1 entree à 1 sortie dans le cas général, où on utilisera 1 descripteur de fichier
fd sur sortie en mode modeOuverture. Retourne 1 copie du descripteur de fichier (qui remplace stdin
si c'est 1 redirection d'entrée et stdout ou stderr si c'est 1 redirection de sortie en cas de
succès et 0 en cas d'échec. */
int redirectionGenerale(char *sortie,char *modeOuverture,short erreur,int *fd){
	int copieDescripteur;
	if(!strcmp(modeOuverture,"w")){
		/* redirection de sortie,on remplace la sortie standard par le descripteur fd
		ou la sortie d'erreur si erreur=1 */
		*fd=open(sortie,O_WRONLY | O_CREAT, 0644);
		if(*fd==-1){
			perror("fopen\n");
			return 0;
		}
		if(erreur){
			copieDescripteur=dup(2);
			dup2(*fd,2);
		}
		else{	
			copieDescripteur=dup(1);
			dup2(*fd,1);
		}
	}
	//redirection d'entrée,on remplace l'entrée standard par le descripteur fd
	else{
		*fd=open(sortie,O_RDONLY | O_CREAT, 0644);
		if(*fd==-1){
			perror("fopen\n");
			return 0;
		}
		copieDescripteur=dup(0);
		dup2(*fd,0);
	}
	return copieDescripteur;
}
//stocke dans le fichier sortie le contenu de la sortie standard de commande 
int redirectionSortie(char *sortie,int *fd){
	return redirectionGenerale(sortie,"w",0,fd);
}
//l'entrée standard de la prochaine commande devient entree 
int redirectionEntree(char *entree,int *fd){
	return redirectionGenerale(entree,"r",0,fd);
}
//stocke dans le fichier sortie le contenu de la sortie d'erreur de commande 
int redirectionErreur(char *sortie,int *fd){
	return redirectionGenerale(sortie,"w",1,fd);
}

