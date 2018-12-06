#include "variables.h"

static list_variables make_variable(char *nom_variable, char *valeur_variable){
  list_variables v = malloc(sizeof(variables));
  if(v == NULL){
    return NULL;
  }
  v->nom_variable = nom_variable;
  v->valeur_variable = valeur_variable;
  v->next = NULL;
  return v;
}

list_variables add_variable(list_variables l, char *nom_variable, char *valeur_variable){
  list_variables v = make_variable(nom_variable, valeur_variable);
  if(v == NULL){
    return NULL;
  }
  if(l == NULL){
    return v;
  }
  v->next = l;
  return v;
}

short is_variable_exist(list_variables l, char *nom_variable){
  list_variables p = l;
  while(p!=NULL){
    if((strcmp(nom_variable, p->nom_variable)) == 0){
      return 1;
    }
    p = p->next;
  }
  return 0;
}

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

short update_variable(list_variables l, char *nom_variable){
  list_variables v = find_variable(l, nom_variable);
  if(v == NULL){
    return 0;
  }
  v->nom_variable = nom_variable;
  return 1;
}

void print_variables(list_variables l){
  list_variables p = l;
  while(p != NULL){
    printf("%s=%s\n", p->nom_variable, p->valeur_variable);
    p = p->next;
  }
}

int main(void){
  list_variables l = NULL;
  for(int i=0 ; i < 3; i++){
    char *nom="var", *val = "val";
    l = add_variable(l, nom, val);
  }
  print_variables(l);
  return 0;
}
