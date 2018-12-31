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
int print_one_env(const char *variable_env_name){
	FILE *f = fopen(".variables_env.txt", "r");
	if(f == NULL){
		printf("mpsh : alias : Eurreur\n");
		return 0;
	}
	int b=0;
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f) != NULL){
		char *q = strtok(buf, "=");
		if(q==NULL){
			fclose(f);
			return 0;
		}
		if(strcmp(q, variable_env_name) == 0){
			fclose(f);
			b=1;
			fprintf(stdout, "%s=%s\n",q, buf);
		}
	}
	if(!b){
		fprintf(stderr, "la variable \"%s\" n'est pas definie.\n", variable_env_name);
	}
	fclose(f);
	return 1;
}

/*
*Verifie si une variable existe
*retourne  sa valeur si oui NULL si non
*/
char *find_env(const char *variable_env_name){
	FILE *f = fopen(".variables_env.txt", "r");
	if(f == NULL){
		return NULL;
	}
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f) != NULL){
		int n = strlen(buf);
		char *s = malloc(sizeof(char)* (n+1));
		memmove(s, buf, n);
		char *q = strtok(buf, "=");
		if(q==NULL){
			fclose(f);
			return NULL;
		}
		if(strcmp(q, variable_env_name) == 0){
			fclose(f);
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