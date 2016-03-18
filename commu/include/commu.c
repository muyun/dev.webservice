/*
 * @author raymond
 *
 * Data: 03/16/2015
 *
 */

#include "commu.h"

/* a client establishes a connection with a server hostname;
which wraps the socket and connect functions.

  return the socket descriptor to the client
*/
int open_clientfd(char* hostname, int port) {
	
	int clientfd;                   // the socket descriptor to the client
	if ((clientfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
		return -1; //error
	}

	// fill in the server's IP address and port
	struct hostent *hp;             // structures return by network data
	struct sockaddr_in serveraddr;  //  Internet-style socket address structure

	if ((hp = gethostbyname(hostname)) == NULL) {
		return -2;
	}
	bzero((char *)&serveraddr, sizeof(serveraddr));
	//memset((char *) &serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_port = htons(port);
	bcopy((char *)hp->h_addr_list[0],
	(char *)&serveraddr.sin_addr.s_addr, hp->h_length);

	//memcpy((void *)&serveraddr.sin_addr, hp->h_addrtype, hp->h_length);

	//a client establishes a Internet connection with a server at socket address serveraddr
	if ((connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr))) < 0) {
		return -1;
	}

	return clientfd;
}

/*
  open and return a listening socket that is ready to receive
  connection requests on the well-known port
*/
int open_listenfd(int port) {
	
	//create a socket descriptor
	int listenfd;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket\n");
		return -1;
	}

	/*use the setsockopt function to configure the server so that it can be terminated and
     restarted immediately
	   
	 This  allow immediate reuse of the port
	*/
	int optval = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0) {
		return -1;
	}

	/* Listenfd will be an endpoint for all requests to port
	on any IP address for this host */
	struct sockaddr_in serveraddr;  /* address of this service */
	//bzero((char *) &serveraddr, sizeof(serveraddr));
	memset((char *) &serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //accept requests to any of the IP addresses for this host
	serveraddr.sin_port = htons((unsigned short)port);

	// the bind tells the kernel to associate the server's socket address with the socket descriptor listenfd
	if (bind(listenfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0) {
		printf("bind failed \n");
		return -1;
	}

	//make listenfd from an active socket to a listening socket ready to accept connection request
	if (listen(listenfd, QLEN) < 0) {
		printf("listen failed \n");
		return -1;
	}

	return listenfd;
}

// write n bytes (unbuffered) robustly
// some code references from the example
int rio_writen(int fd, void* usrbuf, size_t n) {
	size_t nleft = n;
	size_t nwritten;
	char* bufp = usrbuf;

	while (nleft > 0) {
		// write nleft bytes from bufp buffer to the file fd
		if ((nwritten = write(fd, bufp, nleft)) <= 0) { 
			if (errno == EINTR) { //Interrupted by sig handler return
				nwritten = 0;
			} else {
				return -1;  // an error
			}
		}

		nleft = nleft - nwritten;
		bufp = bufp + nwritten;
	}

	return n;
}

// Associate a descriptor with a read buffer  and reset the buffer
void rio_readinitb(rio_t *rp, int fd) {
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

/* a wrapper for the unix read()  - TO-CHECK
 borrows the below from the example 
 */
static int rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
			sizeof(rp->rio_buf));
		if (rp->rio_cnt < 0) {
			if (errno != EINTR) /* Interrupted by sig handler return */
				return -1;
		}
		else if (rp->rio_cnt == 0)  /* EOF */
			return 0;
		else
			rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
	}

	/* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
	cnt = n;
	if (rp->rio_cnt < n)
		cnt = rp->rio_cnt;
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;

	return cnt;
}

/* robustly read a text line (with an internal read buffer);
which automatically making a read call to refill the buffer
 */
int rio_readlineb(rio_t *rp, void* usrbuf, size_t maxlen) {

	int n, rc;
	char c, *bufp = usrbuf;

	for (n = 1; n < maxlen; n++) {
		if ((rc = rio_read(rp, &c ,1)) == 1) {
			*bufp++ = c;
			if (c == '\n') {
				n++;
				break;
			}
		} else if ( rc == 0) {
			if (n == 1) {
				return 0;  /* EOF, no data read */
			} else {
				break;     /* EOF, some data was read*/
			}
		} else {

			return -1;     /* Error */
		}
	}

	*bufp = 0;

	return n - 1;
}

// read up to n bytes
int rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
}
