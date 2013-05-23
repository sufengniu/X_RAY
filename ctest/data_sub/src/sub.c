#include "sub.h"

/*
 *
 *
 */

void *sub(void *arg)
{
	int tid;
	slave_thread_arg *p = (slave_thread_arg *)arg;

	tid = p->tid;
	printf("thread %d: actived\n", tid);
	
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);
	init(tid);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
	accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
	printf("thread %d: initial done in %lf second\n", tid, accum[tid]);
	
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);
        sub_op(tid);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
        accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
        printf("thread %d: done in %lf second\n", tid, accum[tid]);
	
	printf("thread %d: writing to output_image\n", tid);
        image_syn(tid);
	
}

/* initialization to 0 for each pixel */
int init(int tid)
{
	int i, j;       // i: column, j: row

	for (i = 0; i<buffer_length; i++){
		for (j = 0; j<buffer_width; j++){
			*(*(data_buffer + tid) + i * buffer_width + j) = 0;
		}
	}
	printf("thread %d: buffer initial done.\n", tid);
	return 0;
}

int sub_op(int tid)
{
	int i, j, k;	// i: column, j: row, k: page num
	long int image_index = 0;
	printf("thread %d: data subtraction operation begin ... \n", tid);
	
	for (k = 1; k < pages; k++){

		for (i = 0; i<buffer_length; i++){
			for (j = 0; j<buffer_width; j++){
				image_index = j + i * buffer_width + tid * buffer_size + k * page_size;

				*(*(data_buffer + tid) + j + i * buffer_width) = ;

                        }
		}
	}
	return 0;
}
