/*------------------------------------------------------
* FILE: mthreads_dark_avg.c
* DESCRIPTION: 
*  
* 
* AUTHOR: Sufeng Niu 
* LAST REVISED: 
*------------------------------------------------------*/
#include "syds_config.h"
#include "tif.h"	// tif loading thread
#include "rms.h"	// dark average operation
#include "threadpool.h"
#include "platform_init.h"

/* add customized thread header file here */

int main(int argc, char *argv[])
{

/*
* initiliazation, threads number definition, parameters definition
*/
	int rc;
	void *status;
	int i;	
	struct threadpool *pool;

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
	
	NUM_PROCESS_THREADS = NUM_THREADS;

	hw_info();	// defined in platform_init.c

	/* pthread pool initial */
	if ((pool = threadpool_init(NUM_PROCESS_THREADS)) == NULL) {
			
	
		
	
/*
* the main thread will launch NUM_THREADS-2 subthreads
* tif/udp socket can be implemented in main thread
* one thread responds synthesis subthreads
*/
	/*------------------ tif loading ---------------------*/
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_start);
	
	tif_load(argv+2);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_stop);

	tif_accum = (tif_stop.tv_sec-tif_start.tv_sec)+(double)(tif_stop.tv_nsec-tif_start.tv_nsec)/(double)BILLION;
        printf("master thread: done in %lf second\n", tif_accum);
	
	/*------------------ processing threads ---------------------*/	
	mem_alloc();
	buffer_size = buffer_length * buffer_width;
	printf("master thread: buffer size is %d\n", buffer_size);
	printf("master thread: buffer length is %d, buffer width is %d\n", buffer_length, buffer_width);

	/* assign thread number from 1 to NUM_PROCESS_THREADS */
	printf("master thread: create %d processing threads\n", NUM_PROCESS_THREADS);
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i].tid = i;
		starg[i].pid = 0;
		
		rc = pthread_create(&threads[i], &attr, rms, (void *)(starg+i));
		if (rc){
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(1);
		}
	}
	
	/* wait for all threads complete */
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		pthread_join(threads[i], &status);
	}
	
	/*------------------ tif -----------------------*/
	printf("master thread: synthesis multiple chunks \n");
	printf("master thread: writing processed image to %s file \n", output_filename[1]);
	tif_syn();

	mem_free();

	pthread_exit(NULL);		
	return 0;
}
