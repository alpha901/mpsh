
#include "history.h"
#include "variables_env.h"

#define MAX_CMD 100
int nb_cmd, max_cmd;
int taille=5;
//verifie si les la variable d'environnement "name" est defini si oui retourne sa valeur sinon l'initialise
// name prend la valeur "nb_cmd" et "max_cmd"
void init_var(char *name){
	char *line = find_env(name);
	if(line == NULL){//la variable n'existe pas
		if(strcmp(name, "nb_cmd") == 0){
			nb_cmd = 0; //initialise le nombre de commande a 0
		}
		else{
			max_cmd = MAX_CMD; //initialise le nombre de commande maximal a MAX_CMD
			char s_numero[length(max_cmd)];
			sprintf(s_numero, "%d", max_cmd);
			update_env("max_cmd", s_numero);
		}
	}
	else{
		int t_line = strlen(line);
		int t_name = strlen(name);
		char tmp[t_line- t_name -1];
		int k=0;
		for (int i = t_name+1; i < t_line; ++i)
			tmp[k++]=*(line+i);
		tmp[k]='\0';
		if(strcmp(name, "nb_cmd") == 0)
			nb_cmd = atoi(tmp);
		else
			max_cmd = atoi(tmp);
	}
}
//retourne le nombre de chiffres composant le nombre n
int length(int n){
	int l = 0, x = n;
	while(x != 0){
		l++;
		x /= 10;
	}
	return l;
}

//affiche les commandes dans history
int list_history(){
	FILE *f = fopen(".history.txt", "r");
	if (f==NULL)
		return 0;
	char buf[MAX_READED];
	while(fgets(buf, MAX_READED, f) != NULL){
		fputs(buf,stdout);
	}
	fclose(f);
	return 1;
}

//ajouter une commande dans history
int ajout_history(const char *commande){
	FILE *f = fopen(".history.txt", "a+");
	if (f==NULL)
		return 0;
	init_var("nb_cmd");//on initialise la variable nb_cmd
	nb_cmd++;
	fprintf(f, "%*d %s\n",taille, nb_cmd, commande);
	init_var("max_cmd");
	if(nb_cmd > max_cmd){//si le nombre de commande est supperieur au maximum on supprime les premieres lignes
		delete_first_n_lines(f, 1);
	}
	char s_numero[length(nb_cmd)];
	sprintf(s_numero, "%d", nb_cmd);
	update_env("nb_cmd", s_numero);//on met a jour le nombre de commande enregistre dans nb_cmd
	fclose(f);
	return 1;
}

//supprimer les n premieres lignes
int delete_first_n_lines(FILE *f, int n){
	FILE *aux_del = fopen(".aux_del", "w+");
	if(aux_del==NULL)
		return 0;
	char buf[MAX_READED];
	int i=0;
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
		i++;
		if(i>n){
			fputs(buf, aux_del);	
		}
	}
	fclose(aux_del);
	remove(".history.txt");
	rename(".aux_del", ".history.txt");
	return 1;
}

//remet le bon numero sur chaque ligne
int numeroter_ligne(FILE *f, char *file){
	FILE *aux_num = fopen(".aux_num", "w+");
	if (aux_num == NULL)
		return 0;
	int numero = 1;//premiere ligne lue
	char buf[MAX_READED];
	rewind(f);
	while(fgets(buf, MAX_READED, f) != NULL){
	    fprintf(aux_num, "%d%s", numero, &buf[length(numero)]);
	    numero++;
  	}
	fclose(aux_num);
	remove(file);
	rename(".aux_num", file);
	return 1;
}

//permet de retrouver une commande par son numero dans history
char *find_history(int n){
	FILE *f = fopen(".history.txt", "r");
	char buf[MAX_READED];
	int numero;
	while(fgets(buf, MAX_READED, f) != NULL){
		if(buf[strlen(buf) -1] == '\n'){
			sscanf(buf, "%d", &numero);
			if(n == numero)
				break;
		}
	}
	if(n == numero){
		//int l = length(numero);
		char *commande = malloc(sizeof(char)*(strlen(buf) - taille));
		memmove(commande, &buf[taille] , strlen(buf) - taille);
		return commande;
	}
	return NULL;
}
//modifie le nombre maximal de commande autorise dans l'historique
void update_max_nombre_de_commandes(int n){
	char s_numero[taille];
	sprintf(s_numero, "%d", n);
	init_var("max_cmd");
	update_env("max_cmd", s_numero);
	if(max_cmd > n){
		FILE *f = fopen(".history.txt", "r+");
		delete_first_n_lines(f, nb_cmd - n);
	}
}
