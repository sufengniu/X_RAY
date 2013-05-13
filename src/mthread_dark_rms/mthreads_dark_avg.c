/*------------------------------------------------------
* FILE: mthreads_dark_avg.c
* DESCRIPTION: 
*  
* 
* AUTHOR: Sufeng Niu 
* LAST REVISED: 
*------------------------------------------------------*/
#include "sys_config.h"
#include "tif.h"	// tif loading thread
#include "avg.h"	// dark average operation
//#include "thr_pool.h"

/* add customized thread header file here */

const char *output_filename[] = {"dark_avg.tif", "dark_rms.tif"};

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
	
	NUM_PROCESS_THREADS = NUM_THREADS - 1;
	
	/* pthread initial */
	pthread_t *threads;
	pthread_attr_t attr;
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	struct slave_thread_arg *starg; // slave thread arguments	
	
	starg = (struct slave_thread_arg *)malloc(NUM_PROCESS_THREADS * sizeof(struct slave_thread_arg));	// 1 tif thread and 1 synthesis thread	
	
	/* setup to measure threads time */
	start = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	stop = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	accum = (double *)malloc(NUM_THREADS * sizeof(double));
/*
* the main thread will launch NUM_THREADS-2 subthreads
* tif/udp socket can be implemented in main thread
* one thread responds synthesis subthreads
* 
*/
	/*------------------ tif loading ---------------------*/
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start);
	
	tif_load(argv+2);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop);

	accum[0] = (stop->tv_sec - start->tv_sec)+(double)(stop->tv_nsec-start->tv_nsec)/(double)BILLION;
        printf("thread %d: done in %lf second\n", 0, accum[0]);
	
	/*------------------ processing threads ---------------------*/	
	/* malloc buffer for multiple sub threads */	
	avg_buffer = (uint16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		*(avg_buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16));
	}
	rms_buffer = (uint16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		*(rms_buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16));
	}
	buffer_size = buffer_length * buffer_width;
	printf("thread 0: buffer size is %d\n", buffer_size);
	printf("thread 0: buffer length is %d, buffer width is %d\n", buffer_length, buffer_width);

	/* assign thread number from 1 to NUM_PROCESS_THREADS */
	printf("thread 0: create %d processing threads\n", NUM_PROCESS_THREADS);
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i].tid = i+1;
		starg[i].pid = 0;
		
		rc = pthread_create(&threads[i+1], &attr, avg, (void *)(starg+i));
		if (rc){
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(1);
		}
	}
	
	/* wait for all threads complete */
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		pthread_join(threads[i+1], &status);
	}
	
	/*------------------ tif -----------------------*/
	printf("thread 0: synthesis multiple chunks \n");
	printf("thread 0: writing processed image to dark_avg.tif file \n");
	tif_syn();

	/* Clean up and exit */
	printf("-- free memory space and clean up \n");
	pthread_attr_destroy(&attr);
	free(threads);
	free(starg);
	free(start);
	free(stop);
	free(accum);
	
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(rms_buffer[i]);
	}
	free(rms_buffer);
		
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(avg_buffer[i]);
	}
	free(avg_buffer);

	tif_release(input_image);
	tif_release(output_image);

	pthread_exit(NULL);		
	return 0;
}
