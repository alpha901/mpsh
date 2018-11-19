#ifndef VARIABLES_ENV_H
#define  VARIABLES_ENV

void print_all_env();
void print_one_env(const char *variable_env_name);
char is_exist_env(const char *variable_env_name);
void delete_env(const char *variable_env_name);
void update_env(const char *variable_env_name, const char *value);

#endif
