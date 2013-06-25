#include "udp_server.h"

#include "tif.h"

char *ipaddr = {"127.0.0.1"};

void *udp(void *udp_arg)
{
	int i, j;
	int num_packets;
	int packet_size = PACKET_SIZE;
	int buff_len;	// receive buffer length
	int pid;
	int msg_len;
	int resize;

	struct udp_arg_type *arg = (struct udp_arg_type *)udp_arg;
	pid = arg->pid;	
	
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
	image_info->buffer_length = ceil((float)image_info->length/arg->compthrds);
	image_info->buffer_width = image_info->width;
	image_info->buffer_size = image_info->buffer_length * image_info->buffer_width;
	printf("~: image info:\n");
	printf("~: image length: %d, image width: %d\n", image_info->length, image_info->width);
	printf("~: computation threads number: %d", arg->compthrds);
	printf("~: image threads buffer width: %d, buffer length: %d", image_info->buffer_width, image_info->buffer_length);
	printf("~: image page number: %d\n", image_info->page_num);
	printf("~: packet number per frame: %d\n", num_packets);

	msg_len = image_info->buffer_size * numthrds; 
	
	resize = mem_alloc();
	
	image_ptr = image_buff;
	int ping_pong = 1;		

	for (i = 0; i < image_info->page_num; i++)
	{	
		ping_pong != ping_pong;
		/* do not use image_info->image_size, the image size has been resized for multi-strip */ 
		image_ptr = image_buff + ping_pong*resize;

		for (j = 0; j < num_packets; j++)
		{

			recvfrom(sid, image_ptr, packet_size, 0, (struct sockaddr *)client_addr, &size);
			image_ptr += packet_size;
		}

		printf("~: Receiving %dth image frame\n", i);
	
		MPI_Scatter(
			(void *)(image_buff+ping_pong*image_info->image_size),
			image_info->buffer_size * arg->compthrds, 
			MPI_INT,
			strip_buff,	
			msg_len,
			MPI_INT,
			pid;
			MPI_COMM_WORLD);
		
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

	free(client_addr);
	mem_free();

	return 0;
}

int mem_alloc()
{
	/* modefy mem space for MPI_Scatter,
	 * MPI_Scatter must send messages in same size, if each process receive different message size
	 * resize the message size to keep them the same, assuming each blade is 4 core 8 threads computer
	 */
	int size;
	if (compthrds % numprocs == 0)
	{
		size = image_info->image_size;
	}
	else
	{	
		size = image_info->buffer_size * totalthrds;
	}
	
	if((image_buff = (uint16 *)malloc(2 * size * sizeof(uint16))) == NULL)
	{
		printf("~: Error: could not allocate enought memory for image_buff!\n");
		exit(0);
	}

	return size;
}

void mem_free()
{
	free(image_info);
	free(image_buff);
}

/* n: row, m: column */
int malloc2d(uint16 ***array, int n, int m) {

	/* allocate the n*m contiguous items */
	uint16 *p = (uint16 *)malloc(n*m*sizeof(uint16));
	if (!p) return -1;

	/* allocate the row pointers into the memory */
	(*array) = (uint16 **)malloc(n*sizeof(uint16*));
	if (!(*array)) {
		free(p);
		return -1;
	}

	/* set up the pointers into the contiguous memory */
	for (int i=0; i<n; i++) 
		(*array)[i] = &(p[i*m]);

	return 0;
}

int free2d(uint16 ***array) {
	/* free the memory - the first element of the array is at the start */
	free(&((*array)[0][0]));

	/* free the pointers into the memory */
	free(*array);

	return 0;
}
