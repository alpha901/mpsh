#include "variables.h"

/* Alloue 1 nouvelle variable et renvoie 1 pointeur vers cette variable en cas de succès
et NULL en cas d'échec */
list_variables allouer_variable(char *nom_variable, char *valeur_variable){
  list_variables v = malloc(sizeof(variables));
  if(v == NULL){
    return NULL;
  }
  v->nom_variable = nom_variable;
  v->valeur_variable = valeur_variable;
  v->next = NULL;
  return v;
}
/* Ajoute 1 nouvelle variable à la liste chaînée l et la met à jour si elle existe déjà.
Retourne 1 en cas de succès et 0 en cas d'échec */
list_variables add_variable(list_variables l, char *nom_variable, char *valeur_variable){
	list_variables nouvelleListe,variableExistante;
	if( (variableExistante=find_variable(l,nom_variable))!=NULL ){
		//la variable existe,on la met à jour
		variableExistante->valeur_variable=valeur_variable;
		return l;
	}
	else{
		nouvelleListe=allouer_variable(nom_variable, valeur_variable);
		if(nouvelleListe==NULL){
			printf("Echec de création de la variable\n");
			return l;
		}
		nouvelleListe->next = l;
		return nouvelleListe;
	}
}
/* Retourne 1 pointeur vers la variable nom_variable si elle est dans la liste chaînée l et
NULL sinon */ 
list_variables find_variable(list_variables l, char *nom_variable){
  list_variables p = l;
  while(p != NULL){
    if (strcmp(nom_variable, p->nom_variable) == 0){
      return p;
    }
    p = p->next;
  }
  return NULL;
}

