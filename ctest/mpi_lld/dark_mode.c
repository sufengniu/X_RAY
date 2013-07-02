#include "sys_config.h"
#include "thr_pool.h"
#include "dark_mode.h"
#include "mpi.h"

/* add customized header here */
#include "rms.h"
#include "udp_server.h"
#include "tif.h"

int dkm()
{
	int rank;
	int i;
	int totalthrds = 0;
	struct udp_arg_type *udp_arg;

	printf("---------------------------------------------\n");
	printf("---- X-ray camera dark average operation ----\n");
	printf("---------------------------------------------\n");
	printf("-- hardware information: \n");
	printf("-- cpu infomation:\n");

	hw_info();

	/* MPI Initialization */
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	MPI_Status status;
	// public window for rma
	MPI_Win win;

	MPI_Allreduce(&numthrds, &ttlprocs, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	if (rank == 0){
		if (ttlprocs != numprocs)
		{
			printf("Warning: number of process is not the same as cpu core number\n");
			printf("this will affect computation performance\n");
		}
	}

	// check processor rank
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	printf("-- processor %s, rank %d out of %d processors\n", processor_name, rank, numprocs);
	
	// udp need 1 process
	compprocs = numprocs - 1;

	MPI_Barrier(MPI_COMM_WORLD);

	/* 
	 * master node is iocfccd1. in hostfile iocfccd1 should be placed as the 1st
	 * check iocfccd1's rank. it should be rank = 0
	 */
	if (rank == 0) {

		/*
		 * udp routine here
		 * the input image mem space is defined in sys_config.h
		 * input image stores in image_buff (global)
		 * image_buff mem allocated in udp 
		 */
		udp(win);	
		
	}
	else {
		
		/* computation routine here */
		image_rms(rank, win);

	}

	image_rms();	
	
	/* clean up */
	MPI_Finalize();

	return 0;	
}

void hw_info()
{
	/* CPU processor check here */
	system("lscpu");

	system("nproc > procs_num");

	FILE *cpu_info;
	cpu_info = fopen("procs_num", "r");
	fscanf(cpu_info, "%d", &numthrds);

	fclose(cpu_info);

}

