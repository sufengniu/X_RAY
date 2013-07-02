#include "udp_server.h"

#include "tif.h"

void udp(MPI_Win& win)
{
	int i, j;
	int num_packets;
	int packet_size = PACKET_SIZE;
	int buff_len;	// receive buffer length
	int rank;

	image_info = (struct image_info_type *)malloc(sizeof(struct image_info_type));	
	
	/* setup udp socket */
	printf("~: UDP socket setup...\n");
	struct sockaddr_in *client_addr;
	int sid;
	if((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("~: Error: socket init failed!\n");
		exit(0);
	}
	client_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	
	client_addr->sin_family = AF_INET;
	client_addr->sin_port = htons(PORT);
	client_addr->sin_addr.s_addr = inet_addr(ipaddr);

	if(bind(sid, (struct sockaddr *)client_addr, sizeof(struct sockaddr)) == -1) {
		printf("~: Error: bind failed\n");
		exit(0);
	}
	socklen_t size = sizeof(struct sockaddr_in);
	
	/* hand shaking */	
	struct id_type sendid, recvid;
	sendid.bid = 1;	
	recvfrom(sid, &recvid, sizeof(struct id_type), MSG_WAITALL, (struct sockaddr *)client_addr, &size);
	printf("~: Receive hand shaking signal from Blade %d\n", recvid.bid);
	
	sendto(sid, &sendid, sizeof(struct id_type), 0, (struct sockaddr *)client_addr, size);

	//_________________________________________________________

	recvfrom(sid, image_info, sizeof(struct image_info_type), MSG_WAITALL, (struct sockaddr *)client_addr, &size);
	
	num_packets = ceil((float)image_info->image_size/packet_size);
	image_info->buffer_length = ceil((float)image_info->length/compprocs);
	image_info->buffer_width = image_info->width;
	image_info->buffer_size = image_info->buffer_length * image_info->buffer_width;
	printf("~: image info:\n");
	printf("~: image length: %d, image width: %d\n", image_info->length, image_info->width);
	printf("~: computation process number: %d", compprocs);
	printf("~: image threads buffer width: %d, buffer length: %d", image_info->buffer_width, image_info->buffer_length);
	printf("~: image page number: %d\n", image_info->page_num);
	printf("~: packet number per frame: %d\n", num_packets);
	
	mem_alloc();

	/* remote memory access */
	MPI_Win_create(image_buff, 2*image_info->image_size*sizeof(uint16), sizeof(uint16), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	
	image_ptr = image_buff;
	int ping_pong = 1;		
	
	// barrier here to ensure initial done for other process
	MPI_Barrier(MPI_COMM_WORLD);
	
	for (i = 0; i < image_info->page_num; i++)
	{	
		ping_pong != ping_pong; 
		image_ptr = image_buff + ping_pong*image_info->image_size;

		for (j = 0; j < num_packets; j++)
		{

			recvfrom(sid, image_ptr, packet_size, 0, (struct sockaddr *)client_addr, &size);
			image_ptr += packet_size;
		}

		printf("~: Receiving %dth image frame\n", i);
		
		/* MPI sync */
		MPI_Barrier(MPI_COMM_WORLD);
		
		
	}

	//___________________________________________________________

/*	
	while()
	{
		ping_pong != ping_pong;
		image_ptr = image_buff + ping_pong*image_info->image_size;

		for (j = 0; j < num_packets; j++)
		{

			recvfrom(sid, image_ptr, packet_size, 0, (struct sockaddr *)client_addr, &size);       
			image_ptr += packet_size;
		}

		printf("~ Receiving %dth image frame\n", i);
		image_info->page_num++;
		MPI_Barrier(MPI_COMM_WORLD);
	}
*/
	
	//___________________________________________________________
	
	printf("~: Receiving done\n");
	close(sid);

	printf("~: -- finished --\n");
	
	MPI_Win_free(win);
	free(client_addr);
	mem_free();

	return 0;
}

void mem_alloc()
{
	if((image_buff = (uint16 *)malloc(2 * image_info->image_size * sizeof(uint16))) == NULL)
	{
		printf("~: Error: could not allocate enought memory for image_buff!\n");
		exit(0);
	}

}

void mem_free()
{
	free(image_info);
	free(image_buff);
}
