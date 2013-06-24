<<<<<<< HEAD
#include "rms.h"
=======

/* root mean square thread */
#include "../include/rms.h"
>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde

/*
 *
 *
<<<<<<< HEAD
 *
 *
 */

void *image_rms(int argc, char *argv[], uint16 *image)
{

//____________________________________________________

	int pid;
	pthread_attr_t attr;

//____________________________________________________

	/* MPI Initialization */
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &pid);

	/* thread pool initial */
	thr_pool_t *pool;
	pool = (thr_pool_t *)malloc(sizeof(thr_pool_t));

	/* pthread initial */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if ((pool = thr_pool_create(1, MAX_THREAD, 2, &attr)) == NULL) {
		printf("Error: create thread pool failed");
		exit(0);
	}

	/* allocate mem space*/
	if(rms_mem_alloc() == -1)
	{
		printf("Error: cannot malloc enough memory space!\n");
		exit(0);
	}
	
	for (i = 0; i < numthrds; i++) {
		
		thread_status = thr_pool_queue(pool, rms, (void *)(sarg));
		if (thread_status == -1) {
			printf("Error occure in process %d: adding a thread failed!\n", pid)
			exit(0);
		}
	}
	
	rms_clean();

	MPI_Finalize();

	return (void *);
}

int rms_mem_alloc()
{
	int i;
	
	/* timer malloc */
	start = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	stop = (struct timespec *)malloc(numthrds * sizeof(struct timespec));
	accum = (double *)malloc(numthrds * sizeof(double));

	/* user memory allocate here */
//_________________________________________________________________
	dk0 = (uint16 **)malloc(numthrds * numprocs * sizeof(void *));
	for(i = 0; i < numthrds * numprocs; i++){
		if((*(dk0+i) = (uint16 *)malloc(image_info->buffer_length * image_info->buffer_width * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for dk0!\n");
			exit(0);
		}
	}
	avg_buff = (int16 **)malloc(numthrds * numprocs * sizeof(void *));
	for(i = 0; i < numthrds * numprocs; i++){
		if((*(avg_buff+i) = (int16 *)malloc(image_info->buffer_length * image_info->buffer_width * sizeof(int16))) == NULL){
			printf("Could not allocate enough memory for avg_buff!\n");
			exit(0);
		}
	}
	rms_buff = (uint16 **)malloc(numthrds * numprocs * sizeof(void *));
	for(i = 0; i < numthrds * numprocs; i++){
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
	for (i = 0; i < numthrds * numprocs; i++){
		free(dk0[i]);
	}
	free(dk0);

	for (i = 0; i < numthrds * numprocs; i++){
		free(rms_buff[i]);
	}
	free(rms_buff);

	for (i = 0; i < numthrds * numprocs; i++){
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
	/* caculate in recursive method 3 */
	for (i = 0; i<buffer_length; i++){
		for (j = 0; j<buffer_width; j++){
			image_index = j + i * buffer_width + tid * buffer_size;
			*(*(dk0 + tid) + j + i * buffer_width) = input_image[image_index];
		}
	}	
	
	for (i = 0; i<buffer_length; i++){
		for (j = 0; j<buffer_width; j++){
			image_index = j + i * buffer_width + tid * buffer_size + k * page_size;

			/* caculate in recursive method 3 */
			diff_sum = (image[image_index] - *(*(dk0 + tid) + j + i * image_info->buffer_width));

			/* rms */
			*(*(avg_buff + tid) + j + i * image_info->buffer_width) += diff_sum;
			*(*(rms_buff + tid) + j + i * image_info->buffer_width) += pow(diff_sum, 2);

		}
	}

//__________________________________________________________________
}

int op_syn(int tid){
=======
 */

void *rms(void *arg)
{
	int tid;
	slave_thread_arg *p = (slave_thread_arg *)arg;
	
	tid = p->tid;
	printf("thread %d: actived\n", tid);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);	
	rms_init(tid);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
	accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
	printf("thread %d: initial done in %lf second\n", tid, accum[tid]);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, start + tid);

	rms_op(tid);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, stop + tid);
	accum[tid] = ((stop+tid)->tv_sec - (start+tid)->tv_sec)+(double)((stop+tid)->tv_nsec - (start+tid)->tv_nsec)/(double)BILLION;
	printf("thread %d: average computation done in %lf second\n", tid, accum[tid]);
	
	image_syn(tid);

	//pthread_exit(NULL);
}

/* initialization to 0 for each pixel */
int rms_init(int tid)
{
	int i, j;	// i: column, j: row
	
	for (i = 0; i < image_info->buffer_length; i++){
		for (j = 0; j < image_info->buffer_width; j++){
			*(*(avg_buffer + tid) + i * image_info->buffer_width + j) = 0;
			*(*(rms_buffer + tid) + i * image_info->buffer_width + j) = 0;
		}
	}
	printf("thread %d: buffer initial done.\n", tid);
	return 0;
}

/* rms operation */
int rms_op(int tid)
{
	int i, j, k;	// i: column, j: row, k: page num
	int diff_sum;
	long int image_index = 0;
	i = 0;
	j = 0;
	printf("thread %d: dark rms operation begin ... \n", tid);
	
	/* caculate in recursive method 3 */
	for (i = 0; i < image_info->buffer_length; i++){
		for (j = 0; j < image_info->buffer_width; j++){
			image_index = j + i * image_info->buffer_width + tid * buffer_size;
			*(*(dk0 + tid) + j + i * image_info->buffer_width) = input_image[image_index];
		}
	}
					
	for (i = 0; i < image_info->buffer_length; i++){
		for (j = 0; j < image_info->buffer_width; j++){
			image_index = j + i * image_info->buffer_width + tid * buffer_size + k * page_size;
			/* caculate in recursive method 3 */
			diff_sum = (input_image[image_index] - *(*(dk0 + tid) + j + i * image_info->buffer_width));
			/* rms */
			*(*(avg_buffer + tid) + j + i * image_info->buffer_width) += diff_sum;
			*(*(rms_buffer + tid) + j + i * image_info->buffer_width) += pow(diff_sum, 2);
				
		}
	}

	return 0;
}

int image_syn(int tid){
>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde
	int i, j;
	long int image_index = 0;
	int avg_diff_sq;
	int avg_diff;
	int rms_sq;
	int temp_value;

	for (i = 0; i < image_info->buffer_length; i++){
		for (j = 0; j < image_info->buffer_width; j++){
			image_index = j + i * image_info->buffer_width + tid * buffer_size;
<<<<<<< HEAD

			/* caculate in recursive method 3 */
			// DKstd = sqrt(A2-(A1^2/numDark)/(numDark-1))
			avg_diff_sq = pow(avg_buff[tid][j+i*image_info->buffer_width], 2);    // overflow may happen
			temp_value = avg_diff_sq / pages;
			rms_sq = (rms_buff[tid][j+i*image_info->buffer_width] - temp_value) / (pages - 1);
			*(*(rms_buff + tid) + j + i * image_info->buffer_width) = sqrt(rms_sq);

			output_image_std[image_index] = *(*(rms_buff + tid) + j + i * image_info->buffer_width);

			// DKavg = A1/numDark + dk0
			avg_diff = *(*(avg_buff + tid) + j + i * image_info->buffer_width) / pages;
			*(*(avg_buff + tid) + j + i * image_info->buffer_width) = avg_diff + *(*(dk0 + tid) + j + i * image_info->buffer_width);
			output_image_avg[image_index] = *(*(avg_buff + tid) + j + i * image_info->buffer_width);
=======
			
			/* caculate in recursive method 3 */
			// DKstd = sqrt(A2-(A1^2/numDark)/(numDark-1))
			avg_diff_sq = pow(avg_buffer[tid][j+i*image_info->buffer_width], 2);	// overflow may happen
			temp_value = avg_diff_sq / pages;
			rms_sq = (rms_buffer[tid][j+i*image_info->buffer_width] - temp_value) / (pages - 1);
			*(*(rms_buffer + tid) + j + i * image_info->buffer_width) = sqrt(rms_sq);
	
			output_image_std[image_index] = *(*(rms_buffer + tid) + j + i * image_info->buffer_width);

			// DKavg = A1/numDark + dk0
			avg_diff = *(*(avg_buffer + tid) + j + i * image_info->buffer_width) / pages;
			*(*(avg_buffer + tid) + j + i * image_info->buffer_width) = avg_diff + *(*(dk0 + tid) + j + i * image_info->buffer_width);
			output_image_avg[image_index] = *(*(avg_buffer + tid) + j + i * image_info->buffer_width);
>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde
		}
	}
	return 0;
}
<<<<<<< HEAD
=======



>>>>>>> f4329a25722409d8a6c855ce5763d888f5a7efde
