#ifndef VARIABLES_ENV_H
#define  VARIABLES_ENV_H

void print_all_env();
void print_one_env(const char *variable_env_name);
char *find_env(const char *variable_env_name);
int delete_env(const char *variable_env_name);
int update_env(const char *variable_env_name, const char *value);

#endif
