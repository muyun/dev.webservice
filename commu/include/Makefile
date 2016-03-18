#
CC = gcc

CFLAGS ="-g" -DDEBUG -c
LIBS = -lncurses

#INCLUDE = -I/../include/


install: commu.o

commu.o:
	$(CC) $(CFLAGS) -o $@ commu.c

clean:
	-rm *.o
	-rm *~
