//#pragma once
/*
 * @author raymond
 *
 * Date:  03/17/2016
 *
*/
#include "commu.h"

#define PORT 4897
#define BLEN 8192 

void echo_fn(int );

int main(int argc, char** argv) {

	/*
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <host> <port> \n", argv[0]);
		exit(0);
	}*/
#ifdef _WIN32
	int err;
	WSADATA wsa;

	err = WSAStartup(MAKEWORD(2, 2), &wsa);  // winsock 2.2
	net_assert(err, "sample server: WSAStartup");
#endif

	int port = PORT;

	// open a listening socket for the connection request
	int listenfd;
	listenfd = open_listenfd(port);

	// loop
	struct hostent *hp;
	struct sockaddr_in clientaddr;
	int clientlen;
	int connfd;  //connected descriptor is used to communicate with the client
	char* haddrp;
	while (1) {
		//server waits for a connection request from a client to arrive on the listening descriptor listenfd
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);

		// get the domain name and IP address of the client
		hp = gethostbyaddr((const char*) &clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET );
		haddrp = inet_ntoa(clientaddr.sin_addr);
		
		printf("Connected from %s (%s)\n", hp->h_name, haddrp);

	    // the byte transfer
		echo_fn(connfd);

		close(connfd);
    }

#ifdef _WIN32
	WSACleanup();
#endif

	exit(0);
}

// the echo function on the server
void echo_fn(int connfd) {
	
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	rio_readinitb(&rio, connfd);
	while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("server received %d bytes\n", (int)n);
		rio_writen(connfd, buf ,n);
        printf("server received: %s \n",buf);
        
	}
}
