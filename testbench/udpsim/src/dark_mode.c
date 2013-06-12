#include <mpi.h>
#include "../include/sys_config.h"
#include "../lib/thr_pool.h"
#include "../include/platform_init.h"
#include "../include/dark_mode.h"

#include "../include/tif.h"
#include "../include/rms.h"
#include "../include/udp_server.h"


/*------------------ dark mode operation threads ----------------*/
int dark_op(char *argv[]){
	
	
	

	return 0;
}

/*
 * tif.c API 
 * tif_infoi:   read the tif file to obtain image parameters. i.e: image size
 * tif_load:    load the tif file into the memory space
 * tif_syn:     tif file extracted out from memory,
 *              stored as dark_avg.tif, dark_rms.tif, data.tif
 * tif_release: free tif file memory
 */
int tif_io(char *argv[])
{

	/* pthread pool initial */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, MAX_THREAD, 2, &attr)) == NULL){
		printf("Error: create thread pool failed!\n");
		exit(1);
	}

	/* tif loading */
	tif_info(argv+2);	
	dk_mem_alloc();		// dark image mem allocation  
	
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_start);
	tif_load(input_image);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_stop);
	tif_accum = (tif_stop.tv_sec-tif_start.tv_sec)+(double)(tif_stop.tv_nsec-tif_start.tv_nsec)/(double)BILLION;
	printf("master thread: dark image loading done in %lf second\n", tif_accum);
	
	/* assign threads based on thread pool */
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i].tid = i;
		starg[i].pid = 0;

		thread_status = thr_pool_queue(pool, rms, (void *)(starg+i));
		if (thread_status == -1){
			printf("An error had occurred while adding a task!\n");
			exit(0);
		}
	}
	/* wait for all threads complete */
	thr_pool_wait(pool);	
	
	printf("master thread: dark mode is done!\n");	

	/* clean up */
	pthread_attr_destroy(&attr);
	thr_pool_destroy(pool);
	
	return 0;
}

int udp_io()
{
	/* pthread pool initial */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, MAX_THREAD, 2, &attr)) == NULL){
		printf("Error: create thread pool failed!\n");
		exit(1);
	}
	
	
	
	printf("master thread: dark mode is done!\n");
	
	/* clean up */
	pthread_attr_destroy(&attr);
	thr_pool_destroy(pool);
	
	return 0;
}
