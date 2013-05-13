
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
	int i, j, k;	// i: column, j: row, k: buffer number

	printf("thread %d: dark average operation begin ... \n", tid);

	/* threads wake up when thread_sel_cv signal sent */
	for (k = 0; k<NUM_PROCESS_THREADS * pages; k++){

		page_num = k/NUM_PROCESS_THREADS;

		for (i = 0; i<buffer_length; i++){
			for (j = 0; j<buffer_width; j++){
				image_index = j + i * buffer_width + (thread_sel-1) * buffer_size + page_num * page_size;
                                /* caculate in recursive way */
				*(*(buffer + thread_sel - 1) + j + i * buffer_width) += input_image[image_index];
				*(*(buffer + thread_sel - 1) + j + i * buffer_width) /= page_num+1;

			}
		}			
	}
	printf("thread %d: average computation done\n", tid);

	return 0;
}
