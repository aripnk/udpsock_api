#include "../udpsock/udpsock.h"


char host[100];
int port;

struct payload{
  char magic[1024];
	int data;
	char string[20];
};
typedef struct payload payload;

static void
fetch_data_cb(evutil_socket_t evfd, short evwhat, void * arg)
{
  udpsock_t* udpsock = (udpsock_t*)arg;
  payload payld;
  udpsock_fetch(udpsock, &payld, sizeof(payload));
  printf("data = %d string = %s size = %lu\n", payld.data, payld.string, sizeof(payld));

}

int
main(int argc, char *argv[])
{
  int ufd;
  fd_set fdset;
  struct event_base *base;
  struct event *tev, *uev;

  if (argc < 2) { printf("parameter <host> <port>\n"); return 1;}
  strcpy(host, argv[1]);
  int port = atoi(argv[2]);
  char sport[10];
  sprintf(sport, "%i", port);

  base = event_base_new();
  if (!base) return -1;

  udpsock_t udpsock;
  int ret = udpsock_init(&udpsock, host, sport, 1, 0);

  ufd = udpsock_getfd(&udpsock);

  if (ufd > -1) {
    uev = event_new(base, ufd, EV_READ|EV_PERSIST, fetch_data_cb, (void*)&udpsock);
    udpsock.ev = uev;
    event_add(uev, NULL);
    event_base_dispatch(base);
  }

  if (ufd != -1) udpsock_free(&udpsock);
  event_base_free(base);
  return 0;
}
