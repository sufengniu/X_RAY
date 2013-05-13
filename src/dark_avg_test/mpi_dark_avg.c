#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

#include "sys_config.h"
#include "tif.h"
#include "avg.h"


int main(int argc, char *argv[])
{		
	/* start MPI */
	MPI_Init(&argc, &argv);
	
	/* find out number of process */
	int num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	/* find out each process ID */
	int procs_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_rank);
	
	MPI_Status status;		

	/*------master process------*/	
	if (procs_rank == 0){
                printf("process %d start loading data ...\n", procs_rank);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
		/* loading function */
		tif_load(argc, argv);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
		accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
        	printf("tif image loadingdone in %lf second\n", accum);
			
		tif_release(input_image);	
	}	
	
	/*------slave process------*/	
	if (procs_rank >0){
		
		
		
		
		
	}
	
	
	MPI_Finalize();

	return 0;
}
