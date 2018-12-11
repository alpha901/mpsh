CC=gcc

CFLAGS= -Wall -g -std=c11

LDLIBS= -lreadline

$(ALL): mpsh

mpsh: mpsh.o mpsh_builtins.o variables.o variables_env.o redirections.o 

mpsh.o: mpsh.c mpsh.h mpsh_builtins.h redirections.h variables.h

mpsh_builtins.o: mpsh_builtins.c mpsh_builtins.h variables.h variables_env.h

redirections.o : redirections.c redirections.h

variables.o : variables.c variables.h

variables_env.o : variables_env.c variables_env.h

cleanall:
	rm -f *~ $(ALL)
	rm -f *#* $(ALL)
	rm -f *.o $(ALL)
	
