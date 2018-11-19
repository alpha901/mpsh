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
*fonction appelee quand une fonction ou un appel system renvoi une erreur
*/
static void sortie_error(){
	fprintf(stderr, "%s in file %s in line %d\n", strerror( errno ), __FILE__, __LINE__);
	exit(EXIT_FAILURE);
}

/*
*Affiche toutes les variables d'environnement
*/
void print_all_env(){
	int fd = open("variables_env.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd <  0){
		sortie_error();
	}
	char read_line[MAX_READED];
	char read_char;
	int w, i=0;
	while(read(fd, &read_char, sizeof(char)) > 0){
		read_line[i++] = read_char;
		if(read_char == '\n'){//find de la ligne -> une nouvelle variable a afficher
			w = write(STDOUT_FILENO, read_line, i);
			if(w < 0){
				sortie_error();
			}
			i = 0;//debut d'une nouvelle ligne	
		}
	}
	close(fd);
}

/*
*Affiche la variable d'environnement correspondant a "variable_env_name"
*exemple echo $PATH
*/
void print_one_env(const char *variable_env_name){
	int fd = open("variables_env.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd <  0){
		sortie_error();
	}
	char read_line[MAX_READED];
	char read_char;
	char *first_word; 
	size_t variable_lenght = strlen(variable_env_name);
	first_word = malloc(variable_lenght * sizeof(char));
	if(first_word == NULL){
		sortie_error();
	}
	int w, i=0, bool=0;
	//lecture du fichier caractere par caractere
	while(read(fd, &read_char, sizeof(char)) > 0){
		read_line[i++] = read_char;
		if(read_char == '\n'){//fin de la ligne
			memmove(first_word, read_line, variable_lenght);//copie dans first_word variable_lenght caracteres depuis read_line a partir du debut
			if(strcmp(variable_env_name, first_word) == 0){
				bool = 1;
				w = write(STDOUT_FILENO, read_line, i);
				if(w < 0){
					close(fd);
					free(first_word);
					sortie_error();
				}
				break;
			}
			i = 0;//debut d'une nouvelle ligne	
		}
	}//while
	free(first_word);
	if(!bool){
		fprintf(stderr, "la variable \"%s\" n'est pas definie.\n", variable_env_name);
	}
	close(fd);
}

/*
*Verifie si une variable existe 
*retourne 1 si oui 0 sinon
*/
char is_exist_env(const char *variable_env_name){
	int fd = open("variables_env.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd <  0){
		sortie_error();
	}
	char read_line[MAX_READED];
	char read_char;
	int i=0;
	size_t variable_lenght = strlen(variable_env_name);
	char *first_word = malloc(variable_lenght * sizeof(char));
	if(first_word == NULL){
		sortie_error();
	}
	//lecture du fichier caractere par caractere
	while(read(fd, &read_char, sizeof(char)) > 0){
		read_line[i++] = read_char;
		if(read_char == '\n'){//fin de la ligne
			memmove(first_word, read_line, variable_lenght);
			if(strcmp(variable_env_name, first_word) == 0){
				free(first_word);
				return 1;
			}
			i = 0;//debut d'une nouvelle ligne	
		}
	}//while
	free(first_word);`
	close(fd);
	return 0;
}
/*
*supprime une variable d'environnemetn
*exemple unset
*/
void delete_env(const char *variable_env_name){
	int fd = open("variables_env.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	int fd_aux = open("file_aux", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd < 0 || fd_aux < 0){
		sortie_error();
	}
	char read_line[MAX_READED];
	char read_char;
	int i=0;
	size_t variable_lenght = strlen(variable_env_name);
	char *first_word = malloc(variable_lenght * sizeof(char));
	if(first_word == NULL){
		sortie_error();
	}
	//lecture du fichier caractere par caractere
	while(read(fd, &read_char, sizeof(char)) > 0){
		read_line[i++] = read_char;
		if(read_char == '\n'){//fin de la ligne	
			memmove(first_word, read_line, variable_lenght);
			if(strcmp(variable_env_name, first_word) != 0){
				if(write(fd_aux, read_line, i) != i){
					free(first_word);
					close(fd);
					close(fd_aux);
					sortie_error();
				}
			}
			i = 0;//debut d'une nouvelle ligne	
		}
	}//while
	free(first_word);
	close(fd);
	close(fd_aux);
	remove("variables_env.txt");
	rename("file_aux", "variables_env.txt");
}
/*
*modofie ou cree une variable d'environnement
*exemple export $variable (creation)
*exemple $CHEMIN=:/bin/src/tout ce que je veux (modification)
*/
void update_env(const char *variable_env_name, const char *value){
	char *new_variable = malloc(strlen(variable_env_name) + strlen(value) + 2);
	int fd = open("variables_env.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	int fd_aux = open("file_aux", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd < 0 || fd_aux < 0 || new_variable == NULL){
		sortie_error();
	}
	strcat(strcat(strcat(strcat(new_variable, variable_env_name), "="), value),"\n");
	char read_line[MAX_READED];
	char read_char;
	int i=0, bool=0;
	size_t variable_lenght = strlen(variable_env_name);
	char *first_word = malloc(variable_lenght * sizeof(char));
	if(first_word == NULL){
		sortie_error();
	}
	//lecture du fichier caractere par caractere
	while(read(fd, &read_char, sizeof(char)) > 0){
		read_line[i++] = read_char;
		if(read_char == '\n'){//fin de la ligne	
			memmove(first_word, read_line, variable_lenght);
			if(strcmp(variable_env_name, first_word) != 0){//on recopie la variable correspondante
				if(write(fd_aux, read_line, i) != i){
					free(first_word);
					close(fd);
					close(fd_aux);
					sortie_error();
				}
			}else{//on remplace l'ancienne variable par la nouvelle variable
			bool =1;
			if(write(fd_aux, new_variable, strlen(new_variable)) != strlen(new_variable)){
				free(first_word);
				close(fd);
				close(fd_aux);
				sortie_error();
			}
		}
			i = 0;//debut d'une nouvelle ligne	
		}
	}//while
	if(!bool){//on cree une nouvelle ligne pour la nouvelle variable
		if(write(fd_aux, new_variable, strlen(new_variable)) != strlen(new_variable)){
			free(first_word);
			close(fd);
			close(fd_aux);
			sortie_error();
		}
	}
	free(first_word);
	close(fd);
	close(fd_aux);
	remove("variables_env.txt");
	rename("file_aux", "variables_env.txt");	
}

/*********************************************************
*********************TEST*********************************
**********************************************************/
int main(void){
	printf("affiche toutes les variables\n");
	print_all_env();
	printf("affiche une seule variable\n");
	print_one_env("path");
	print_one_env("ENV");
	delete_env("pa");
	print_all_env();
	update_env("alpha","je teste ma fonction");
	update_env("variable non existant","un deuxieme test");
	return 0;
}	
