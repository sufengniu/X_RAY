#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "tif_load.h"

int main(int argc, char *argv[])
{
	
	MPI_Init(&argc, &argv);
	
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Status status;

	if ()		
	
	MPI_Finalize();
	return 0;
}
