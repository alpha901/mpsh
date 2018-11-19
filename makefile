CC=gcc

CFLAGS= -Wall -g -std=c11

LDLIBS= -lreadline

$(ALL): mpsh

mpsh: mpsh.c

cleanall:
	rm -f *~ $(ALL)
	rm -f *#* $(ALL)


