
/* average thread */
#include "avg.h"

/*
*
*
*/

void *avg(void *arg)
{
	int tid;
	slave_thread_arg *p = (slave_thread_arg *)arg;
	
	tid = p->tid;
	printf("thread %d: actived\n", tid);	// add 1 is required, starg started from 1

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);	
	init(tid);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
	accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
	printf("thread %d: initial done in %lf second\n", tid, accum[tid]);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);

	avg_op(tid);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
	accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
	printf("thread %d: computation done in %lf second\n", tid, accum[tid]);
	
	printf("thread %d: writing to output_image\n", tid);
	image_syn(tid);

	pthread_exit(NULL);
}

/* initialization to 0 for each pixel */
int init(int tid)
{
	int i, j;	// i: column, j: row
	
	for (i = 0; i<buffer_length; i++){
		for (j = 0; j<buffer_width; j++){
			*(*(avg_buffer + tid - 1) + i * buffer_width + j) = 0;
		}
	}
	printf("thread %d: buffer initial done.\n", tid);
	return 0;
}

/* average operation */
int avg_op(int tid)
{
	int i, j, k;	// i: column, j: row, k: page num
	int avg;
	long int image_index = 0;
	printf("thread %d: dark average operation begin ... \n", tid);
	
	for (k = 0; k < pages; k++){
					
		for (i = 0; i<buffer_length; i++){
			for (j = 0; j<buffer_width; j++){
				image_index = j + i * buffer_width + (tid-1) * buffer_size + k * page_size;
				
				/* caculate in recursive way */
				// *(*(avg_buffer + tid - 1) + j + i * buffer_width) += input_image[image_index];
				// *(*(avg_buffer + tid - 1) + j + i * buffer_width) /= (k+1);
				
				/* caculate in non-recursive way */
				*(*(avg_buffer + tid - 1) + j + i * buffer_width) += input_image[image_index] / pages;

			}
		}
	}

	printf("thread %d: average computation done\n", tid);

	return 0;
}

int image_syn(int tid){
	int i, j;
	long int image_index = 0;
		
	for (i = 0; i<buffer_length; i++){
		for (j = 0; j<buffer_width; j++){
			image_index = j + i * buffer_width + (tid-1) * buffer_size;

			output_image[image_index] = *(*(avg_buffer + tid - 1) + j + i * buffer_width);
		}
	}	
	return 0;
}
