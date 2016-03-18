#
CC = GCC
CLT_DIR = client
SER_DIR = server
CFLAGS ="-G" -DDEBUG -c
LIBS = -lncurses

INCLUDE = -I/../include/

all: clean install

install: client

client: client.o commu.o
	$(CC) -o $@ $? $(LIBS)

client.o:
	$(CC) $(CFLAGS) -o $@ client.c
commu.o:
	$(CC) $(CFLAGS) -o $@ commu.c $(INCLUDE)

clean:
	-rm *.o
	-rm *~
