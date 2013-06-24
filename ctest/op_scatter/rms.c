#include "rms.h"

/*
 *
 *
 *
 *
 */

void rms_init()
{
	/* allocate mem space */
	rms_mem_alloc();
}


void *image_rms()
{
	int i, k;

	rms_init();

	for (i = (CPU_THRS_NUM-numthrds); i < CPU_THRS_NUM; i++) {
		sarg[i].tid = i;
		sarg[i].pid = pid;

		thread_status = thr_pool_queue(pool, rms, (void *)(sarg));
		if (thread_status == -1) {
			printf("Error occure in process %d: adding rms thread failed!\n", pid);
			exit(0);
		}
	}
	
	
	rms_clean();
	return (void *);
}

int rms_mem_alloc(int pid)
{
	int i;

	/* timer malloc */
	start = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	stop = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	accum = (double *)malloc(numthrds * sizeof(double));

	/* user memory allocate here */
	//_________________________________________________________________

	dk0 = (uint16 **)malloc(totalthrds * sizeof(void *));
	for(i = 0; i < totalthrds; i++){
		if((*(dk0+i) = (uint16 *)malloc(image_info->buffer_length * image_info->buffer_width * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for dk0!\n");
			exit(0);
		}
	}
	avg_buff = (int16 **)malloc(totalthrds * sizeof(void *));
	for(i = 0; i < totalthrds; i++){
		if((*(avg_buff+i) = (int16 *)malloc(image_info->buffer_length * image_info->buffer_width * sizeof(int16))) == NULL){
			printf("Could not allocate enough memory for avg_buff!\n");
			exit(0);
		}
	}
	rms_buff = (uint16 **)malloc(totalthrds * sizeof(void *));
	for(i = 0; i < totalthrds; i++){
		if((*(rms_buff+i) = (uint16 *)malloc(image_info->buffer_length * image_info->buffer_width * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for rms_buff!\n");
			exit(0);
		}
	}
	if((output_image_avg = (uint16 *)_TIFFmalloc(image_info->image_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for dark average output image!\n");
		exit(0);
	}
	if((output_image_std = (uint16 *)_TIFFmalloc(image_info->image_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for standard derivation output image!\n");
		exit(0);
	}

//_________________________________________________________________

	return 0;
}

int rms_clean()
{
	free(start);
	free(stop);
	free(accum);

	/* user memory free here */
	for (i = 0; i < totalthrds * numprocs; i++){
		free(dk0[i]);
	}
	free(dk0);

	for (i = 0; i < totalthrds * numprocs; i++){
		free(rms_buff[i]);
	}
	free(rms_buff);

	for (i = 0; i < totalthrds * numprocs; i++){
		free(avg_buff[i]);
	}
	free(avg_buff);

	tif_release(output_image_avg);
	tif_release(output_image_std);
}

void *rms(void *sarg)
{
	int i, j, k;	//i: column, j: row, k: page_num
	int tid, pid;
	long int image_index;
	struct slave_arg *p = (struct slave_arg *)sarg;

	tid = p->tid;
	MPI_Common_rank(MPI_COMM_WORLD, &pid);

	printf("process %d thread %d: actived\n", pid, tid);

	/* user function defined here */	
//__________________________________________________________________

	for (k = 0; k < image_info->page_num; k++) {

		MPI_Barrier(MPI_COMM_WORLD);

		if (k = 0){
			/* caculate in recursive method 3 */
			for (i = 0; i<image_info->buffer_length; i++){
				for (j = 0; j<image_info->buffer_width; j++){
					image_index = j + i * image_info->buffer_width + tid * image_info->buffer_size;
					*(*(dk0 + tid) + j + i * image_info->buffer_width) = input_image[image_index];
				}
			}
		}
		else {
			for (i = 0; i<image_info->buffer_length; i++){
				for (j = 0; j<image_info->buffer_width; j++){
					image_index = j + i * image_info->buffer_width + tid * image_info->buffer_size + k * image_info->image_size;

					/* caculate in recursive method 3 */
					diff_sum = (image[image_index] - *(*(dk0 + tid) + j + i * image_info->buffer_width));

					/* rms */
					*(*(avg_buff + tid) + j + i * image_info->buffer_width) += diff_sum;
					*(*(rms_buff + tid) + j + i * image_info->buffer_width) += pow(diff_sum, 2);

				}
			}
		}
	}
}

//__________________________________________________________________
}

int op_syn(int tid){
	int i, j;
	long int image_index = 0;
	int avg_diff_sq;
	int avg_diff;
	int rms_sq;
	int temp_value;

	for (i = 0; i < image_info->buffer_length; i++){
		for (j = 0; j < image_info->buffer_width; j++){
			image_index = j + i * image_info->buffer_width + tid * buffer_size;

			/* caculate in recursive method 3 */
			// DKstd = sqrt(A2-(A1^2/numDark)/(numDark-1))
			avg_diff_sq = pow(avg_buff[tid][j+i*image_info->buffer_width], 2);    // overflow may happen
			temp_value = avg_diff_sq / image_info->page_num;
			rms_sq = (rms_buff[tid][j+i*image_info->buffer_width] - temp_value) / (image_info->page_num - 1);
			*(*(rms_buff + tid) + j + i*image_info->buffer_width) = sqrt(rms_sq);

			output_image_std[image_index] = *(*(rms_buff + tid) + j + i*image_info->buffer_width);

			// DKavg = A1/numDark + dk0
			avg_diff = *(*(avg_buff + tid) + j + i*image_info->buffer_width) / image_info->page_num;
			*(*(avg_buff + tid) + j + i*image_info->buffer_width) = avg_diff + *(*(dk0 + tid) + j + i*image_info->buffer_width);
			output_image_avg[image_index] = *(*(avg_buff + tid) + j + i*image_info->buffer_width);
		}
	}
	return 0;
}
