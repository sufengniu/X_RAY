#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>  // prerequisite typedef
#include <errno.h>      // error number
#include <sys/socket.h> // struct sockaddr; system prototypes and constants
#include <netdb.h>    // network info lookup prototypes and structures
#include <netinet/in.h> // struct sockaddr_in; byte ordering macros
#include <arpa/inet.h>  // utility function prototypes
#include <unistd.h>

#define BUFLEN 512
#define PORT 9930


int main()
{
	struct sockaddr_in clientaddr, serveraddr;
	
	const char *msg = {"hello world!"};
	int len = strlen(msg);
	
	char recv_buf[BUFLEN];

	int socketid;
	if (((socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1))
	{
		printf("Error: socket failed!\n");
		exit(0);
	}
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);      // host to network, short
	
	socklen_t size=sizeof(serveraddr);
	
	char hit[4];
	printf("start sender, hit return to send\n");
	fgets(hit, 4, stdin);
		
	printf("client send %s\n", msg);
	sendto(socketid, msg, len, 0, (struct sockaddr *)&serveraddr, size);
	
	recvfrom(socketid, recv_buf, BUFLEN, 0, (struct sockaddr *)&serveraddr, &size);
	
	printf("client receive %s\n", recv_buf);
	
	close(socketid);
	return 0;
}
