/*------------------------------------------------------
* FILE: mthreads_dark_avg.c
* DESCRIPTION: 
*  
* 
* AUTHOR: Sufeng Niu 
* LAST REVISED: 
*------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <tiffio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "sys_config.h"
#include "tif.h"	// tif loading thread
#include "avg.h"	// dark average operation
//#include "thr_pool.h"

/* add customized thread header file here */

int main(int argc, char *argv[])
{

/*
* initiliazation, threads number definition, parameters definition
*/
	int rc;
	void *status;
	int i;	
	
	NUM_THREADS = DEFAULT_THREADS_NUM;	// defined in sys_config.h	
	NUM_PROCESS = NUM_BLADES;
	buffer_counter = 0;

	if (argc == 3){
		NUM_THREADS = atoi(argv[1]);
	} else{
		printf("Usage: exe n tif\n");
		printf("exe: executable file\n");
		printf("n: threads number\n");
		printf("tif: tif image file\n");
		exit(1);
	}
	
	if ((NUM_THREADS < 1)||(NUM_THREADS > MAX_THREAD)){
		printf("Error: the number of thread should between 1 and %d.\n", MAX_THREAD);
		exit(1);
	}
	
	printf("---------------------------------------------\n");
	printf("---- X-ray camera dark average operation ----\n");
	printf("---------------------------------------------\n");
	printf("-- hardware information: \n");
	printf("-- \tcore number: %d\n", NUM_CORE);
	printf("-- \tblades number: %d\n", NUM_BLADES);
	printf("-- software definition: \n");
	printf("-- \tthreads number: %d\n", NUM_THREADS);
	printf("-- \tprocess number: %d\n", NUM_PROCESS);
	
	NUM_PROCESS_THREADS = NUM_THREADS - 2;
	
	/* pthread initial */
	pthread_t *threads;
	pthread_attr_t attr;
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	struct slave_thread_arg *starg; // slave thread arguments	
	
	starg = (slave_thread_arg *)malloc((NUM_PROCESS_THREADS) * sizeof(slave_thread_arg));	// 1 tif thread and 1 synthesis thread	

/*
* the main thread will launch NUM_THREADS-2 subthreads
* tif/udp socket can be implemented in main thread
* one thread responds synthesis subthreads
* 
*/
	/*------------------ tif loading ---------------------*/
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	
	tif_load(argv+2);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
        printf("thread %d: done in %lf second\n", 0, accum);
	
	/*------------------ processing threads ---------------------*/	
	/* malloc buffer for multiple sub threads */	
	buffer = (uint16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		*(buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * sizeof(uint16));
	}
	buffer_size = buffer_length * buffer_width;
	printf("master thread: buffer size is %d\n", buffer_size);
	printf("master thread: buffer length is %d, buffer width is %d\n", buffer_length, buffer_width);
		
	/* assign thread number from 1 to NUM_PROCESS_THREADS */
	printf("master thread: create %d processing threads\n", NUM_PROCESS_THREADS);
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i+1].tid = i+1;
		starg[i+1].pid = 0;
		
		rc = pthread_create(&threads[i+1], &attr, avg, (void *)(starg+i));
		if (rc){
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(1);
		}
	}

	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		pthread_join(threads[i], &status);
	}

/*	
	printf("create synthesis thread\n");
	rc = pthread_create(&threads[NUM_THREADS-1], &attr, syn, (void *)(syntarg));
	if (rc){
		printf("Error: return code from pthread_create() is %d\n", rc);
		exit(1);
	}
*/

	/* wait for all threads complete */
	

	/* Clean up and exit */
	printf("-- free memory space and clean up \n");
	pthread_attr_destroy(&attr);
	free(starg);

	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		free(buffer[i]);
	}
	free(buffer);

	tif_release(input_image);

	pthread_exit(NULL);		
	return 0;
}
