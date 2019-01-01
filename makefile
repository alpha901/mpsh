CC=gcc

CFLAGS= -Wall -g 

LDLIBS= -lreadline

$(ALL): mpsh

mpsh: mpsh.o variables.o variables_env.o commande_app.o mpsh_builtins.o redirections.o alias.o history.o completionAutomatique.o configuration.o

mpsh.o: mpsh.c commande_app.h

alias.o : alias.c alias.h

variables.o : variables.c variables.h

redirections.o : redirections.c redirections.h

history.o : history.c history.h variables_env.h

variables_env.o : variables_env.c variables_env.h

commande_app.o : commande_app.c commande_app.h

mpsh_builtins.o: mpsh_builtins.c commande_app.h

completionAutomatique.o : completionAutomatique.c completionAutomatique.h

configuration.o : configuration.c configuration.h 

cleanall:
	rm -f *~ $(ALL)
	rm -f *#* $(ALL)
	rm -f *.o $(ALL)
	
