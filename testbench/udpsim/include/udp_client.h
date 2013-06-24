#include "sys_config.h"

<<<<<<< HEAD
#include <time.h>
=======
>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde
#include <string.h>
#include <unistd.h>
#include <sys/types.h>	// prerequisite typedef
#include <sys/socket.h> // struct sockaddr; system prototypes and constants
#include <netdb.h>    // network info lookup prototypes and structures
#include <netinet/in.h> // struct sockaddr_in; byte ordering macros
#include <arpa/inet.h>  // utility function prototypes

<<<<<<< HEAD
uint16 *sim_image;	// pointer for multiple images
uint16 *image_ptr;	// point for each frame

#define PACKET_SIZE	1024

typedef struct id_type {
	int bid;	// blade id
	// slave_thread_arg starg; // including pid and tid
} id_type;
=======
uint16 *sim_image;
>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde

