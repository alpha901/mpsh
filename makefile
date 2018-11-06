CC=gcc

CFLAGS= -Wall -g

LDLIBS= -lreadline

$(ALL): mpsh

mpsh: mpsh.c

cleanall:
	rm -f *~ $(ALL)


