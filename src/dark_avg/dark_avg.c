#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "tif.h"
#include "avg.h"

#define BILLION 1000000000L;

int main(int argc, char *argv[])
{
	// clock_t start, end;
        struct timespec start, stop;
        double accum;
	
	/* start MPI */
	MPI_Init(&argc, &argv);
	
	/* find out number of process */
	int num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	/* find out each process ID */
	int procs_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_rank);
	
	MPI_Status status;		
	
	if (procs_rank == 0){
                printf("process %d start loading data ...\n", procs_rank);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
		/* loading function */
		tif_load(argc, argv);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
		accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
        	printf("done in %lf second\n", accum);
			
		tif_release(input_image);	
	}	
	
	
	
	MPI_Finalize();

	return 0;
}
