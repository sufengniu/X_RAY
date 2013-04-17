#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "tif_load.h"

int main(int argc, char *argv[])
{
	
	uint16 *input_image;	
	// loading function
	tif_load(&argc, &argv, input_image);
	
	/* start MPI */
	MPI_Init(&argc, &argv);
	
	/* find out number of process */
	int num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	/* find out each process ID */
	int procs_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_rank);
	
	MPI_Status status;		
	
	
	
	MPI_Finalize();
	tif_release(input_image);

	return 0;
}
