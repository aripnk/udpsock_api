/*Arif Nur Khoirudin
  hello@arifnurkhoirudin.com*/

#include "udpsock.h"

int
udpsock_init(udpsock_t* udpsock, char *host, char *port, int flags, int bcast)
{
	if (udpsock == NULL) return -1;
	memset(udpsock, 0, sizeof(udpsock_t));

	struct addrinfo aInfo, *rstInfo, *rp;
	int sock, on = 1;
	struct sockaddr sockAddr;
	socklen_t sockAddrLen;

	memset(&aInfo, 0, sizeof(struct addrinfo));
	aInfo.ai_family 		= AF_INET;
	aInfo.ai_socktype 	= SOCK_DGRAM;
	aInfo.ai_flags 			= flags;
	aInfo.ai_protocol 	= 0;
	aInfo.ai_canonname	= NULL;
	aInfo.ai_addr 			= NULL;
	aInfo.ai_next 			= NULL;

	lastError = UDPSOCK_ERR_GETADDRINFO; // if fail, error in getaddrinfo.
	if (getaddrinfo(host, port, &aInfo, &rstInfo) != 0) return FALSE;

  lastError = UDPSOCK_ERR_CREATESOCKET; //if fail
	for (rp = rstInfo; rp != NULL; rp = rp->ai_next){
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (flags == AI_PASSIVE){
			fcntl(sock, F_SETFL, O_NONBLOCK);
			lastError = UDPSOCK_ERR_SOCKBIND; //if fail
			if (bind(sock, rp->ai_addr, rp->ai_addrlen) != 0){
				close(sock);
				continue;
			}
			// ai_flags is AI_PASSIVE, and bind successfull...
			break;
		}
		memcpy(&sockAddr, rp->ai_addr, sizeof(sockAddr));
		sockAddrLen 	= rp->ai_addrlen;
		break;
	}

	udpsock->sock = sock;
	udpsock->sockAddr = sockAddr;
	udpsock->sockAddrLen = sockAddrLen;

	freeaddrinfo(rstInfo);
	if (sock <= -1) return -1;
	return 0;
}

int
udpsock_free(udpsock_t* udpsock)
{
	lastError = UDPSOCK_ERR_CLOSE;
	if (udpsock == NULL) return 0;
	if (udpsock->sock != -1) close(udpsock->sock);
	memset(udpsock, 0, sizeof(udpsock_t));
	return 0;
}

int
udpsock_fetch(udpsock_t* udpsock, void *packet, int psize)
{
	int recvLen, flags;
	struct sockaddr_storage srcAddr;
	socklen_t sockAddrLen = sizeof(srcAddr);

	lastError = UDPSOCK_ERR_RECVFROM; //if fail
	recvLen = recvfrom(udpsock->sock, packet, psize, MSG_DONTWAIT, (struct sockaddr *) &srcAddr, &sockAddrLen);
	if (recvLen == -1) return -1;
	if (recvLen != psize) return -1;

	//sementara
	udpsock->srcAddr = srcAddr;
	udpsock->sockAddrLen = sockAddrLen;

	if (recvLen > 0){
		if (udpsock->srcAddress != NULL) memcpy(udpsock->srcAddress, (void *) &srcAddr, sizeof(struct sockaddr));
		return 0;
	}
	return -1;
}

int
udpsock_send(udpsock_t* udpsock, void *packet, int psize)
{
	lastError = UDPSOCK_ERR_SEND;
	return (sendto(udpsock->sock, packet, psize, 0, &udpsock->sockAddr, udpsock->sockAddrLen) == psize);
}

int
udpsock_getfd(udpsock_t* udpsock)
{
	return udpsock->sock;
}
