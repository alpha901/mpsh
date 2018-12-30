#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int print_all_alias();
char *find_alias(const char *alias_name);
int delete_alias(const char *alias_name);
int update_alias(const char *alias_name, const char *value);

#endif
