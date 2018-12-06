#ifndef VARIABLES_AFFECTATION_H
#define VARIABLES_AFFECTATION_H

typedef struct variables variables;
typedef variables *list_variables;

struct variables{
  char *nom_variable;
  char *valeur_variable;
  list_variables next;
};

list_variables add_variable(list_variables l, char *nom_variable, char *valeur_variable);
short is_variable_exist(list_variables l, char *nom_variable);
list_variables find_variable(list_variables l, char *nom_variable);
short update_variable(list_variables l, char *nom_variable);

#endif
