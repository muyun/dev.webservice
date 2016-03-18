//#pragma once
/*
 *  The client
 *
 *  @author raymond
 *    
 *  Date: 03/16/2016
 *
 *  TODO: test
 */
#include "../include/commu.h"

#define SERVER "localhost"
#define PORT 4897
//#define MAXLINE 4096  /* Max text line length */

int main(int argc, char **argv) {
	/*
	if (argc != 3) {
		fpprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	*/
	//char* host = argv[1];
	//int port = atoi(argv[2]);
	char host[] = SERVER;
	int port = PORT;

#ifdef _WIN32
	int err;
	WSADATA wsa;
	//extern int write();

	err = WSAStartup(MAKEWORD(2, 2), &wsa);  // winsock 2.2
	net_assert(err, "sample client: WSAStartup");
#endif

	// the client establishes a Internet connection with a server
	int clientfd = open_clientfd(host, port);

	// write the socket
	/*
	char buffer[BUFFERLEN];
	if ((write(clientfd, buffer, strlen(buffer)) < 0)) {
		fpprintf(stderr, "writing to socket\n");
		exit(0);
	}*/
	rio_t rio;
	rio_readinitb(&rio, clientfd);  //associate the clientfd with the read buffer rio

	//TEST: read from the stdin
	char buf[MAXLINE];
	while (fgets(buf, MAXLINE, stdin) != NULL) { 
        printf("write the bytes to the server\n");
		// write strlen(buf) bytes from buf to the clientfd
		rio_writen(clientfd, buf, strlen(buf));

		// The next is for the test
		// read a text line (from the internal buffer to user buf)
		rio_readlineb(&rio, buf, MAXLINE);

		fputs(buf, stdout); /* print the buffer to the screen*/
	}

	//close
	close(clientfd);

#ifdef _WIN32
	WSACleanup();
#endif

	exit(0);
	//return 0;

}
