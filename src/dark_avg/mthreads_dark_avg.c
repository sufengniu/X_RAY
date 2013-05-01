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

/* add customized thread header file here */

int main(int argc, char *argv[])
{
	int rc;
	void *status;

	/* threads processing data begin adress and end address  */
	int i, begin, end;
		
	NUM_THREADS = DEFAULT_THREADS_NUM;	// defined in sys_config.h	
	NUM_PROCESS_THREADS = NUM_THREADS-2;

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
	
	pthread_t *threads;
	pthread_attr_t attr;
	
	/* for tif sub thread, comment it when call tif in main thread */
	struct tif_thread_arg *ttarg; // tif thread arguments	

	struct slave_thread_arg *starg; // slave thread arguments	
	
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	
	/* for tif sub thread, comment it when call tif in main thread */
	ttarg = (tif_thread_arg *)malloc(sizeof(tif_thread_arg));
	
	starg = (slave_thread_arg *)malloc((NUM_PROCESS_THREADS) * sizeof(slave_thread_arg));	// 1 tif thread and 1 synthesis thread
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

/*
* the main thread will launch NUM_THREADS-2 subthreads
* tif/udp socket can be implemented in main thread
* 
*
*/
	printf("create tif loading thread\n");
	/* tif loading thread, comment it when call tif in main thread */
	ttarg->tid = 0;
	ttarg->pid = 0;
	ttarg->argv = argv+2;
	
	/* create tif sub thread, comment it when call tif in main thread */
	rc = pthread_create(&threads[0], &attr, tif, (void *)ttarg);
	if (rc) {
		printf("Error: return code from pthread_create() is %d\n", rc);
		exit(1);
	}
	/* active it when call tif in main thread
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	tif_load(argv+2);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

        accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
        printf("done in %lf second\n", accum);
	*/
	
	/* malloc buffer for multiple sub threadsi */
	
	buffer = (uint16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		*(buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * sizeof(uint16));
	}
	printf("buffer length is %d, buffer width is %d\n", buffer_length, buffer_width);
	
	printf("create %d processing threads\n", NUM_PROCESS_THREADS);
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i+1].tid = i+1;
		starg[i+1].pid = 0;
		
		rc = pthread_create(&threads[i+1], &attr, avg, (void *)(starg+i));
		if (rc){
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(1);
		}
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
	for (i = 0; i<NUM_THREADS; i++){
		pthread_join(threads[i], &status);	
	}

	/* Clean up and exit */
	pthread_attr_destroy(&attr);

	tif_release(input_image);
	pthread_mutex_destroy(&sel_mutex);	

	pthread_exit(NULL);	
	return 0;
}
