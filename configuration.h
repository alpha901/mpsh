#ifndef CONFIGURATION_H
#define CONFIGURATION_H
/**************FICHIER PERMETTANT DE CREER ET DE LIRE LE FICHIER DE CONFIGURATION .mpshrc***************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> 
#define NB_CARACTERES_PAR_LIGNE 1024

typedef struct associationCommande{
	char *commande;
	char *extension;
	struct associationCommande* associationSuivante;
}associationCommande;
typedef associationCommande* listeAssociations;
extern listeAssociations listeDesAssociations;

short lectureDuFichierDeConfiguration();
short creationDuFichierDeConfiguration();
short fichierDeConfigurationExiste();
listeAssociations allouerAssociation(char *nomCommande, char *nomExtension);
listeAssociations ajouterAssociation(listeAssociations l, char *nomCommande, char *nomExtension);
listeAssociations trouverAssociation(listeAssociations l, char *nomCommande);
#endif
