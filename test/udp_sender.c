#include "../udpsock/udpsock.h"


char host[100];
int port;

struct payload{
	char magic[1024];
	int data;
	char string[20];
};
typedef struct payload payload;


int main (int argc, char **argv)
{
	if (argc < 2) { printf("parameter <host> <port>\n"); return 1;}
	strcpy(host, argv[1]);
	int port = atoi(argv[2]);
	char sport[10];
	sprintf(sport, "%i", port);

  char *resp = (char *)malloc(4096);
  udpsock_t udpsock;
  int ret = udpsock_init(&udpsock, host, sport, 0, 0);
  if (ret != 0){
  	printf("Init failed.\n");
  	return 0;
  }

	int len;
	// char * payload = (char *)"aaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	payload payld;

	int iter=0;
	printf("udptest succcess bind address %s:%i\n", host, port);
	while (1)
	{
		payld.data = iter;
		strcpy(payld.string, "helloooooo");
		udpsock_send(&udpsock, &payld, sizeof(payload));
		iter++;
		sleep(1);
    // printf("I: %i  \n", ++iter);
		// sleep(1);
	}
	return 0;
}

