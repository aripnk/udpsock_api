#include "commons.h"

// list of error
#define UDPSOCK_ERR_GETADDRINFO 	1
#define UDPSOCK_ERR_CREATESOCKET	2
#define UDPSOCK_ERR_SOCKBIND		3
#define UDPSOCK_ERR_RECVFROM	 	4
#define UDPSOCK_ERR_GETNAMEINFO 	5
#define UDPSOCK_ERR_RECVSIZE		6
#define UDPSOCK_ERR_READ		11
#define UDPSOCK_ERR_WRITE	 	12
#define UDPSOCK_ERR_SEND	 	13
#define UDPSOCK_ERR_CLOSE		14

#define TRUE 0
#define FALSE 1

struct udpsock_t{
	int sock;
	struct event* ev;
	struct sockaddr_storage srcAddr;
	struct sockaddr sockAddr;
	struct sockaddr *srcAddress;
	socklen_t sockAddrLen;
};
typedef struct udpsock_t udpsock_t;

int lastError;

int
udpsock_init(udpsock_t* udpsock, char *host, char *port, int flags, int bcast);

int
udpsock_free(udpsock_t* udpsock);

int
udpsock_fetch(udpsock_t* udpsock, void *packet, int psize);

int
udpsock_send(udpsock_t* udpsock, void *packet, int psize);

int
udpsock_getfd(udpsock_t* udpsock);

struct event*
udpsock_getev(udpsock_t* udpsock);
