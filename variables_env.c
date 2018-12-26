#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "variables_env.h"

#define MAX_READED 1024

/*
*Affiche toutes les variables d'environnement
*/
void print_all_env(){
	FILE *f = fopen(".variables_env.txt", "r");
	if(f == NULL)
		printf("Eurreur\n");
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f) != NULL){
		fprintf(stdout, "%s", buf);
	}
	fclose(f);
}

/*
*Affiche la variable d'environnement correspondant a "variable_env_name"
*exemple echo $PATH
*/
void print_one_env(const char *variable_env_name){
	FILE *f = fopen(".variables_env.txt", "r");
	if(f == NULL)
		printf("Eurreur\n");
	int b=0;
	char buf[MAX_READED];
	char tmp[strlen(variable_env_name)];
	while(fgets(buf, MAX_READED, f) != NULL){
		memmove(tmp, buf, strlen(variable_env_name));
		if(strcmp(tmp, variable_env_name) == 0){
			fprintf(stdout, "%s", buf);
			break;
		}
	}
	if(!b){
		fprintf(stderr, "la variable \"%s\" n'est pas definie.\n", variable_env_name);
	}
	fclose(f);
}

/*
*Verifie si une variable existe
*retourne 1 si oui 0 sinon
*/
char *find_env(const char *variable_env_name){
	FILE *f = fopen(".variables_env.txt", "r");
	if(f == NULL){
		return NULL;
	}
	char buf[MAX_READED];
	char tmp[strlen(variable_env_name)];
	while(fgets(buf, MAX_READED, f) != NULL){
		memmove(tmp, buf, strlen(variable_env_name));
		if(strcmp(tmp, variable_env_name) == 0){
			fclose(f);
			char *s = malloc(sizeof(char)* (strlen(buf) +1));
			memmove(s, buf, strlen(buf)+1);
			return s;
		}
	}

	fclose(f);
	return NULL;
}
/*
*supprime une variable d'environnemetn
*exemple unset
*/
int delete_env(const char *variable_env_name){
	FILE *f = fopen(".variables_env.txt", "a+");
	FILE *aux = fopen("aux", "w");
	if(f == NULL || aux == NULL)
		return 0;
	char buf[MAX_READED];
	char tmp[strlen(variable_env_name)];
	while(fgets(buf, MAX_READED, f) != NULL){
		memmove(tmp, buf, strlen(variable_env_name));
		if(strcmp(tmp, variable_env_name) != 0){
			fputs(buf,aux);
		}
	}
	fclose(f);
	fclose(aux);
	remove(".variables_env.txt");
	rename("aux", ".variables_env.txt");
	return 1;
}
/*
*modofie ou cree une variable d'environnement
*exemple export $variable (creation)
*exemple $CHEMIN=:/bin/src/tout ce que je veux (modification)
*/
int update_env(const char *variable_env_name, const char *value) {
	FILE *f = fopen(".variables_env.txt", "a+");
	FILE *aux = fopen("aux", "w");
	if(f == NULL || aux == NULL){
		return 0;
	}
	char buf[MAX_READED];
	char tmp[strlen(variable_env_name)];
	char *new_env = malloc(sizeof(char)*(strlen(variable_env_name)+strlen(value) + 1));
	sprintf(new_env, "%s=%s\n", variable_env_name, value);
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
		memmove(tmp, buf, strlen(variable_env_name));
		if(strcmp(tmp, variable_env_name) != 0){
			if(fputs(buf,aux) <0)
				return 0;
		}
	}
	if(fputs(new_env,aux) < 0)
		return 0;
	fclose(f);
	fclose(aux);
	remove(".variables_env.txt");
	rename("aux", ".variables_env.txt");
	return 1;
}