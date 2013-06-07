#include "../include/udp_client.h"



int main(int argc, char *argv[])
{
	struct sockaddr_in clienti_addr, server_addr;
	
	int sid;	// socket ID
	if (((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1))
	{
		printf("Error: socket initial failed!\n");
		exit(0);
	}

	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);      // host to network, short
	
	socklen_t size=sizeof(serveraddr);


}
