#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "variables_env.h"
#include "variables_affectation.h"

list_variables liste_of_variables = NULL;


//Foncyion incomplette
void echo(char *variable_name){
	list_variables l = find_affectation(liste_of_variables, variable_name);
	if(l != NULL){
		write(STDOUT_FILENO, l->valeur_variable, strlen(l->valeur_variable));
	}
	else{
		//a suivre...
	}
}
