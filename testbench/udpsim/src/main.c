/*------------------------------------------------------
 * FILE: mthreads_dark_avg.c
 * DESCRIPTION: 
 *  
 * 
 * AUTHOR: Sufeng Niu 
 * LAST REVISED: 
 *------------------------------------------------------*/
#include "../include/sys_config.h"
#include "../lib/thr_pool.h"
#include "../include/dark_mode.h"
#include "../include/data_mode.h"

/* add customized thread header file here */
#include "../include/sub.h"
#include "../include/tif.h"     // tif loading thread
#include "../include/rms.h"     // dark average operation
#include "../include/udp_server.h"	// udp socket 

int main(int argc, char *argv[])
{

/*
 * initiliazation, threads number definition, parameters definition, configuration setup
 */

	setup();
	
	NUM_THREADS = DEFAULT_THREADS_NUM;	// defined in sys_config.h	
	NUM_PROCESS = DEFAULT_PROCESS_NUM;	
	
	if (argc >= 1){
		NUM_THREADS = atoi(argv[1]);
	}
	
	if ((NUM_THREADS < 1)||(NUM_THREADS > MAX_THREAD)){
		printf("Error: the number of thread should between 1 and %d.\n", MAX_THREAD);
		exit(1);
	}
	
	NUM_PROCESS_THREADS = NUM_THREADS;

	hw_info();	// defined in platform_init.c


	printf("---------------------------------------------\n");
	printf("master thread: starting data mode...\n");

/*
 *
 *
 */
	
	/*------------------ data mode operation threads ---------------------*/
	/* loading data image */
	tif_info(argv+3);
	dt_mem_alloc();		// data image mem allocation

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_start);
	tif_load(data_image);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_stop);
	tif_accum = (tif_stop.tv_sec-tif_start.tv_sec)+(double)(tif_stop.tv_nsec-tif_start.tv_nsec)/(double)BILLION;
	printf("master thread: data image loading done in %lf second\n", tif_accum);
	/* assign thread number from 1 to NUM_PROCESS_THREADS */
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i].tid = i;
		starg[i].pid = 0;

		thread_status = thr_pool_queue(pool, sub, (void *)(starg+i));
		if (thread_status == -1){
			printf("An error had occurred while adding a task!\n");
			exit(0);
		}
	}
	thr_pool_wait(pool);

	/*------------------ tif write --------------------*/
	printf("master thread: synthesis multiple chunks \n");
	printf("master thread: writing average image to %s file \n", output_filename[0]);
	printf("master thread: writing rms image to %s file \n", output_filename[1]);

	tif_syn();
	
	/* clean up */
	pthread_attr_destroy(&attr);
	thr_pool_destroy(pool);
	dk_mem_free();          // free dark image mem
	dt_mem_free();
		
	return 0;
}
