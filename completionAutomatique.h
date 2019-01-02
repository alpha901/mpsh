#ifndef COMPLETION_AUTOMATIQUE_H
#define COMPLETION_AUTOMATIQUE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

char **completionDuMotCourant(const char *text,short debut,short fin);
char *genererLeMotCorrespondant(const char *prefixe,int e);
#endif
