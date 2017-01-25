all	: listen_libevent udp_sender

listen_libevent	:
	clang listen_libevent.c ../udpsock/udpsock.c -o listen_libevent -levent

udp_sender	:
	clang udp_sender.c ../udpsock/udpsock.c -o udp_sender

clean	:
	rm -rf listen_libevent
	rm -rf udp_sender
