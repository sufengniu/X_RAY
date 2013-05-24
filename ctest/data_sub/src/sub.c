#include "../include/sub.h"

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
        sub_op(tid);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
        accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
        printf("thread %d: data subtraction done in %lf second\n", tid, accum[tid]);
	
        //image_syn(tid);
	
}

int sub_op(int tid)
{
	int i, j, k;	// i: column, j: row, k: page num
	long int image_index = 0;
	int lld;
	int res_pixel;
	printf("thread %d: data subtraction operation begin ... \n", tid);
	
	for (k = 0; k < pages; k++){

		for (i = 0; i<buffer_length; i++){
			for (j = 0; j<buffer_width; j++){
				image_index = j + i * buffer_width + tid * buffer_size + k * page_size;
				
				lld = alpha * rms_buffer[tid][j+i*buffer_width] + beta;
				res_pixel = data_image[image_index] - avg_buffer[tid][j+i*buffer_width];
			
				if (res_pixel >= lld){
					res_image[image_index] = res_pixel;
				}
				else {
					res_image[image_index] = 0;
				}
			}
		}
	}
	return 0;
}
