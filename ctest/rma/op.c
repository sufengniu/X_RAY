#include "op.h"

/*
 *
 *
 *
 *
 */

void *image_op(int argc, char *argv[], uint16 *image)
{

//____________________________________________________

	int numprocs;
	int numthrds;
	pthread_attr_t attr;

//____________________________________________________

	/* MPI Initialization */
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &(sarg->pid));

	/* thread pool initial */
	thr_pool_t *pool;
	pool = (thr_pool_t *)malloc(sizeof(thr_pool_t));

	/* pthread initial */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, MAX_THREAD, 2, &attr)) == NULL) {
		printf("Error: create thread pool failed");
		exit(0);
	}

	/* allocate mem space*/
	if(op_mem_alloc() == -1)
	{
		printf("Error: cannot malloc enough memory space!\n");
		exit(0);
	}
	
	for (i = 0; i < numthrds; i++) {
		
		thread_status = thr_pool_queue(pool, op, (void *)(sarg));
		if (thread_status == -1) {
			printf("Error occure in process %d: adding a thread failed!\n",sarg->pid)
			exit(0);
		}
	}
	
	op_clean();

	MPI_Finalize();

	return (void *);
}

int op_mem_alloc()
{
	/* timer malloc */
	start = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	stop = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	accum = (double *)malloc(numthrds * sizeof(double));

	/* user memory allocate here */

	return 0;
}

int op_clean()
{
	free(start);
	free(stop);
	free(accum);

	/* user memory free here */

}

void *op(void *sarg)
{
	int tid, pid;
	struct slave_arg *p = (struct slave_arg *)sarg;
	
	tid = p->tid;
	pid = p->pid;
	printf("process %d thread %d: actived\n", pid, tid);
	
	/* user function defined here */	
	
}
