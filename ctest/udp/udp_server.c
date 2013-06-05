
// hearder order is important!
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>	// prerequisite typedef
#include <errno.h>      // error number
#include <sys/socket.h>	// struct sockaddr; system prototypes and constants
#include <netdb.h>    // network info lookup prototypes and structures
#include <netinet/in.h>	// struct sockaddr_in; byte ordering macros
#include <arpa/inet.h>  // utility function prototypes

#define PORT	9930
#define BUFLEN 512
#define NPACK 10
#define ipaddr 127.0.0.1

int main()
{
	struct sockaddr_in, serveraddr;

	int socketid, i;
	if ((socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		printf("Error: socket failed!\n");
		exit(0);
	}
	char recv_buf[BUFLEN];
	
	const char *msg = {"hello world!"};
	int len = strlen(msg);
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT); 	// host to network, short
	serveraddr.sin_addr.s_addr = inet_addr(ipaddr);

	if (bind(socketid, (struct sockaddr *)&serveraddr, sizeof(serveraddr))==-1){
		printf("error: bind failed\n");
		exit(0);
	}

	
	
	close(socketid);
	
	return 0;
	
}

