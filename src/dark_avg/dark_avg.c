#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <tiffio.h>
#include "tif.h"
#include "avg.h"

int main(int argc, char *argv[])
{
	
	uint16 *input_image;	
	
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
		/* loading function */
		tif_load(argc, argv, input_image);
		
		printf("process %d start loading data ...\n", procs_rank);
		_TIFFfree(input_image);
		//tif_release(input_image);	
	}
	
	
	MPI_Finalize();

	return 0;
}
