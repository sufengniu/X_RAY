#include "sys_config.h"
#include "dark_mode.h"
#include "mpi.h"

/* add customized header here */
#include "rms.h"
#include "udp_server.h"
#include "tif.h"

/*
 * dkm has two level init: threads level init and process level init
 * process level init: MPI init
 * threads level init: pthread init, barrier init and threadpool init
 */
int dkm(int argc, char *argv[])
{
	int pid;
	int i;
	int totalthrds = 0;
	struct udp_arg_type *udp_arg;
	int provided, flag, claimed;
	int errs = 0;
		
	/* master node printf information */
	printf("terminal informatino marked as:\n");
	printf("pid 0: ~\tpid 1: !\n");
	printf("pid 2: @\tpid 3: #\n");
	printf("pid 4: $\tpid 5: %\n");
	printf("pid 6: ^\tpid 7: &\n");
	printf("pid 8: *\n");

	printf("---------------------------------------------\n");
        printf("---- X-ray camera dark average operation ----\n");
        printf("---------------------------------------------\n");
        printf("-- hardware information: \n");
        printf("-- cpu infomation:\n");

	/* -- MPI Initialization -- */
	MPI_Init_thread (&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

	// check whether threads level support or not
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

	compthrds = numthrds;
	// udp thread counted
	ttlcompthrds = totalthrds - 1;

	/* -- thread pool and pthread initial -- */ 
	thr_pool_t *pool;
	pool = (thr_pool_t *)malloc(sizeof(thr_pool_t));
	
	pthread_attr_t attr;
	pthread_barrierattr_t barr_attr
	pthread_attr_init(&attr);
	pthread_barrier_init(&barrier, &barr_attr, numthrds);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, CPU_THRS_NUM, 2, &attr)) == NULL) {
		printf("Error: create thread pool failed");
		exit(0);
	}	
		
	MPI_Barrier(MPI_COMM_WORLD);

	/* 
	 * create a thread for udp in master node (pid=0) 
	 * master node is iocfccd1. in hostfile iocfccd1 should be placed as the 1st
	 * check iocfccd1's rank. it should be pid = 0
	 */
	if (pid == 0) {

		/* udp threads */
		udp_arg = (struct udp_arg_type *)malloc(sizeof(struct udp_arg_type));
		udp_arg->numprocs = numprocs;	
		udp_arg->pid = pid;
		
		udp_recv();
		image_rms(pid, );	
	}
	else {
				
		image_rms(pid, );

	}

	thr_pool_wait(pool);
	
	/* threads level clean up */
	pthread_attr_destroy(&attr);
	thr_pool_destroy(pool);
	/* mpi clean up*/
	MPI_Finalize();

	return 0;	
}


/*
 * hardware information check
 * if GPU available, GPU check should be included
 * the hw check should include how many threads, stream processor, etc. 
 * the results use global variables, defined in sys_config.h file
 */
void hw_info()
{
	/* CPU processor checked here */
	system("lscpu");

	system("nproc > thrs_num");

	FILE *cpu_info;
	cpu_info = fopen("thrs_num", "r");
	fscanf(cpu_info, "%d", &CPU_THRS_NUM);
	printf("-- threads in each node: %d\n", CPU_THRS_NUM);

	fclose(cpu_info);

	/* GPU info checked here */


	/* other platform */


}

