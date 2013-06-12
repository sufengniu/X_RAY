#include "../include/udp_client.h"

#include "../include/tif.h"

void sim_mem_alloc();

void sim_mem_free();

int main(int argc, char *argv[])
{
	/* loading tif into memory */
	tif_info(argv+1);
	sim_mem_alloc();
	tif_load(sim_image);	

	/* setup udp socket */
	struct sockaddr_in *server_addr;	
	int sid;	// socket ID

	if (((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1))
	{
		printf("Error: socket initial failed!\n");
		exit(0);
	}
	server_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));	
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(PORT);      // host to network, short

	inet_pton(AF_INET, ipaddr, );	
	socklen_t size=sizeof(server_addr);
	
	/* send the image */
	char hit[4];
	printf("----------------------------------------\n");
	printf("Ready to send, hit return for simulation\n");
	printf("----------------------------------------\n");
	fgets(hit, 4, stdin);

	printf("");
	
	// send image parameters
	sendto(sid, &);
	
	/* cleaning */	
	close(sid);

	sim_mem_free();
	return 0;
}

void sim_mem_alloc();
{
	if((sim_image = (uint16 *)_TIFFmalloc(pages * page_size * sizeof(uint16))) == NULL){
		printf("Error: could allocate enough memory for sim_image");
		exit(0);
	}

}


void sim_mem_free()
{
	free(sim_image);
	

}
