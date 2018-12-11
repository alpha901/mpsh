#ifndef VARIABLES_H
#define VARIABLES_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
/*****************FICHIER GERANT LES VARIABLES DE MPSH**************/
typedef struct variables variables;
typedef variables *list_variables;

struct variables{
  char *nom_variable;
  char *valeur_variable;
  list_variables next;
};
/* variable globale stockant les variables et utilisée dans mpsh_builtins.c (fonction echo)
et mpsh.c */ 
extern list_variables listeDesVariables;

/* Alloue 1 nouvelle variable et renvoie 1 pointeur vers cette variable en cas de succès
et NULL en cas d'échec */
list_variables allouer_variable(char *nom_variable, char *valeur_variable);

/* Ajoute 1 nouvelle variable à la liste chaînée l et la met à jour si elle existe déjà.
Retourne 1 en cas de succès et 0 en cas d'échec */
list_variables add_variable(list_variables l,char *nom_variable, char *valeur_variable);

/* Retourne 1 pointeur vers la variable nom_variable si elle est dans la liste chaînée l et
NULL sinon */ 
list_variables find_variable(list_variables l, char *nom_variable);
#endif
