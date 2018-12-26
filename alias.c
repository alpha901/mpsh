#include "alias.h"

#define MAX_READED 1024

/*
*Affiche toute les alias
*/
int print_all_alias(){
	FILE *f = fopen(".alias.txt", "a+");
	if(f == NULL){
		printf("-mpsh : alias : Eurreur\n");
		return 0;
	}
	char buf[MAX_READED];
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
		fprintf(stdout, "%s", buf);
	}
	fclose(f);
	return 1;
}

//affiche l'alias dont le nom est passe en parametre
char *find_alias(const char *alias_name){
	FILE *f = fopen(".alias.txt", "r");
	if(f == NULL){
		printf("mpsh : alias : Eurreur\n");
		return NULL;
	}
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f) != NULL){
		char *q = strtok(buf, "=");
		if(q==NULL){
			fclose(f);
			return 0;
		}
		if(strcmp(q, alias_name) == 0){
			fclose(f);
			return strtok(NULL,"=");
		}
	}
	fclose(f);
	return NULL;
}

/*
*supprime un alias
*exemple unalias
*/
int delete_alias(const char *alias_name){
	FILE *f = fopen(".alias.txt", "a+");
	FILE *alias_aux = fopen(".alias_aux", "w");
	if(f == NULL || alias_aux == NULL)
		return 0;
	char buf[MAX_READED];
	int b=0;
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
		char *q = strtok(buf, "=");
		if(q==NULL)
			return 0;
		if(strcmp(q, alias_name) != 0)
			fprintf(alias_aux, "%s=%s\n", q,buf);
		else
			b=1;
	}
	if(!b)
		fprintf(stderr, "-mpsh: unalias : \"%s\" : non trouve.\n", alias_name);
	fclose(f);
	fclose(alias_aux);
	remove(".alias.txt");
	rename(".alias_aux", ".alias.txt");
	return 1;
}
/*
*modofie ou cree un alias
*exemple alias alias_name="val alias"
*/
int update_alias(const char *alias_name, const char *value){
	FILE *f = fopen(".alias.txt", "a+");
	FILE *alias_aux = fopen(".alias_aux", "w");
	if(f == NULL || alias_aux == NULL){
		return 0;
	}
	char buf[MAX_READED];
	char *new_alias = malloc(sizeof(char)*(strlen(alias_name)+strlen(value) + 1));
	sprintf(new_alias, "%s=%s\n", alias_name, value);
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
		char *q = strtok(buf, "=");
		if(q==NULL)
			return 0;
		if(strcmp(q, alias_name) != 0){
			fprintf(alias_aux, "%s=%s\n", q,buf);
		}
	}
	fprintf(alias_aux, "%s=%s\n", alias_name,value);
	fclose(f);
	fclose(alias_aux);
	remove(".alias.txt");
	rename(".alias_aux", ".alias.txt");
	return 1;
}

