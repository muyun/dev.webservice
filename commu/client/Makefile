#
CC = gcc

CFLAGS ="-g" -DDEBUG -c
LIBS = -lncurses

#the header files
INCLUDE = -I../include/

#all: clean install

install: client

client: client.o commu.o
	$(CC) -o $@ $? $(LIBS)

client.o:
	$(CC) $(CFLAGS) -o $@ client.c
commu.o:
	$(CC) $(CFLAGS) -o $@ commu.c $(INCLUDE)

clean:
	rm -rf *o
	rm -rf *~
	rm client
