
/* average thread */
#include "avg.h"

/*
*
*
*/

void *avg(void *arg)
{
	slave_thread_arg *p = (slave_thread_arg *)arg;

	printf("thread %d: actived\n", (p+1)->tid);	// add 1 is required, starg started from 1
	
	init((p+1)->tid);
	
	/* hand shaking to trigger tif thread */
	pthread_mutex_lock(&init_mutex);
	pthread_cond_signal(&thread_init_rdy);
	thread_status++;
	pthread_mutex_unlock(&init_mutex);
	
	avg_op((p+1)->tid);
	
	pthread_exit(NULL);
}

/* initialization to 0 for each pixel */
int init(int tid)
{
	int i, j, k;	// i: column, j: row, k: buffer number
	
	for (k = 0; k<NUM_PROCESS_THREADS; k++){
		for (i = 0; i<buffer_length; i++){
			for (j = 0; j<buffer_width; j++){
				*(*(buffer + k) + i * buffer_width + j) = 0;
			}
		}
	}
	printf("thread %d: buffer initial done.\n", tid);
	return 0;
}

/* average operation */
int avg_op(int tid)
{
	int i, j;	// i: column, j: row, k: buffer number

	printf("thread %d: dark average operation begin ... \n", tid);

	/* threads wake up when thread_sel_cv signal sent */
	while(buffer_counter < NUM_PROCESS_THREADS * pages){
	
		pthread_mutex_lock(&sel_mutex);
                pthread_cond_wait(&thread_sel_cv, &sel_mutex);

                // testing point
                //pthread_mutex_unlock(&sel_mutex);

		if(thread_sel == tid){
			printf("thread %d: triggered, buffer_counter is %d, thread_sel is %d\n", tid, buffer_counter, thread_sel);
			
			// testing point: measure computation time
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start[tid]);
			
			page_num = (buffer_counter+1)/NUM_PROCESS_THREADS;

			for (i = 0; i<buffer_length; i++){
				for (j = 0; j<buffer_width; j++){
					image_index = j + i * buffer_width + (thread_sel-1) * buffer_size + page_num * page_size;
                                        /* caculate in recursive way */
					*(*(buffer + thread_sel - 1) + j + i * buffer_width) += input_image[image_index];
					*(*(buffer + thread_sel - 1) + j + i * buffer_width) /= page_num+1;

				}
			}
			
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop[tid]);
			accum[tid] = (stop[tid].tv_sec - start[tid].tv_sec)+(double)(stop[tid].tv_nsec-start[tid].tv_nsec)/(double)BILLION;
			printf("thread %d done in %lf second, thread_sel is %d, buffer counter is %d\n", tid, accum[tid], thread_sel, buffer_counter);
		}
		pthread_mutex_unlock(&sel_mutex);
	}
	printf("thread %d: average computation done\n", tid);

	return 0;
}
