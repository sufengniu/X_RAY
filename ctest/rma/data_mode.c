#include "sys_config.h"
#include "thr_pool.h"
#include "data_mode.h"
#include "mpi.h"

/* add customized header here */
#include "sub.h"
#include "udp_server.h"
#include "tif.h"

int dtm(int argc, char *argv[])
{	
	int pid;
	int i;
	int totalthrds = 0;
	struct udp_arg_type *udp_arg;
	int provided, flag, claimed;
	int errs = 0;

	printf("---------------------------------------------\n");
	printf("---- data subtraction begins  ----\n");

	/* MPI Initialization */
	MPI_Init_thread (&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

	/* check whether threads level support or not */
	MPI_Is_thread_main( &flag );
	if (!flag) {
		errs++;
		printf( "This thread called init_thread but Is_thread_main gave false\n" );fflush(stdout);
	}
	MPI_Query_thread( &claimed );
	if (claimed != provided) {
		errs++;
		printf( "Query thread gave thread level %d but Init_thread gave %d\n", claimed, provided );fflush(stdout);
	}

	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &pid);
	MPI_Status status;

	hw_info();

	// check processor rank
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	printf("-- processor %s, rank %d out of %d processors\n", processor_name, pid, numprocs);

	numthrds = CPU_THRS_NUM;
	MPI_Allreduce(&numthrds, &totalthrds, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	// udp threads need 1 thread
	compthrds = totalthrds - 1;

	/* thread pool and pthread initial */
	thr_pool_t *pool;
	pool = (thr_pool_t *)malloc(sizeof(thr_pool_t));

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, CPU_THRS_NUM, 2, &attr)) == NULL) {
		printf("Error: create thread pool failed");
		exit(0);
	}

	switch(pid)
	{
		case 0:
			printf("~: thread pool initial done!\n");
		case 1:
			printf("!: thread pool initial done!\n");
		case 2:
			printf("@: thread pool initial done!\n");
		case 3:
			printf("#: thread pool initial done!\n");
		case 4:
			printf("$: thread pool initial done!\n");
		case 5:
			printf("%: thread pool initial done!\n");
		case 6:
			printf("^: thread pool initial done!\n");
		case 7:
			printf("&: thread pool initial done!\n");
		case 8:
			printf("*: thread pool initial done!\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);




	return ;
}


void hw_info()
{
	system("lscpu");

	system("nproc > thrs_num");

	FILE *cpu_info;
	cpu_info = fopen("thrs_num", "r");
	fscanf(cpu_info, "%d", &CPU_THRS_NUM);
	printf("-- threads in each node: %d\n", CPU_THRS_NUM);

	fclose(cpu_info);
}

