//#pragma once
/*
* @author raymond
*
* Data: 03/15/2015
*
* @Copyright © 2016 REALMAX co. All rights reserved.
*/

#ifndef __COMMU_H__
#define __COMMU_H__

//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#define close(sockdep) closesocket(sockdep)
#define perror(errmsg) { fprintf(stderr, "%s: %d\n", (errmsg), WSAGetLastError()); }
#endif

#define net_assert(err, errmsg) { if ((err)) { perror(errmsg); assert(!(err)); } }

/* TO-TEST
  strings we send to the server for the beta test 
*/

/* Generic socket address structure for bind(), connect(), and accept() */
typedef struct sockaddr SA;

#define MAXLINE 4096  /* Max text line length */
#define QLEN 1024     /* default connection requests in the listen() */

// client/server helper function
int open_clientfd(char* hostname, int portno);
int open_listenfd(int portno);

// robust I/O (Rio) package (references from the example code)
#define RIO_BUFSIZE 8192
typedef struct {
	int rio_fd;        // descriptor for the internal buffer
	int rio_cnt;       // unread bytes in internal buffer
	char* rio_bufptr;  //  next unread byte in internal buffer
	char rio_buf[RIO_BUFSIZE];  // Internal buffer
} rio_t;

int rio_writen(int fd, void* usrbuf, size_t n);

int rio_readlineb(rio_t *rp, void* usrbuf, size_t maxlen); /*read a text line*/
void rio_readinitb(rio_t *rp, int fd);

#endif 
/* $end */