#include "../include/udp_server.h"

#include "../include/sys_config.h"

int udp_setup()
{
	struct sockaddr_in *server_addr, *client_addr;
	int sid;	// socket id
	if ((socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		printf("Error: socket initial failed!\n");
		exit(0);
	}	
	
	server_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	client_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(PORT);	// host to network, short
	server_addr->sin_addr.s_addr = inet_addr(ipaddr);

	if (bind(socketid, (struct sockaddr *)serveraddr, sizeof(struct sockaddr))==-1){
		printf("error: bind failed\n");
		exit(0);
	}
	
	socklen_t size=sizeof(*client_addr);
	
	
	return 0;
}
