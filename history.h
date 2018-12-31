#ifndef HISTORY_H
#define HISTORY_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define MAX_READED 1024
#define TAILLE 10

void init_var(char *name);
int length(int n);
int ajout_history(const char *commande);

int delete_first_n_lines(FILE *f, int n);
int list_history();
int numeroter_ligne(FILE *f, char *file);
char *find_history(int n);
void update_max_nombre_de_commandes(int n);

#endif
