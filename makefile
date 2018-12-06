CC=gcc

CFLAGS= -Wall -g -std=c11

LDLIBS= -lreadline

$(ALL): mpsh

mpsh: mpsh.c builtins.o;

mpsh.o: mpsh.c mpsh_builtins.h

builtins.o: mpsh_builtins.c mpsh_builtins.h

cleanall:
	rm -f *~ $(ALL)
	rm -f *#* $(ALL)
