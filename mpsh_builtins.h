#ifndef MPSH_BUILTINS_H
#define MPSH_BUILTINS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
void echo(char *variable_name);
void traitementCommandeCD(char ** elementsDeLaCommande, int nbMots);
void traitementCommandeEXIT(char ** elementsDeLaCommande, int nbMots);
#endif
