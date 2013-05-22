#include "init.h"


void hw_info(){

	printf("---------------------------------------------\n");
	printf("---- X-ray camera dark average operation ----\n");
	printf("---------------------------------------------\n");
	printf("-- hardware information: \n");
	printf("-- \tcore number: %d\n", NUM_CORE);
	printf("-- \tblades number: %d\n", NUM_BLADES);
	printf("-- software definition: \n");
	printf("-- \tthreads number: %d\n", NUM_THREADS);
	printf("-- \tprocess number: %d\n", NUM_PROCESS);
	
}



int mem_alloc(){
	
	/* setup to measure threads time */
	start = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	stop = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	accum = (double *)malloc(NUM_THREADS * sizeof(double));

	/* malloc buffer for multiple sub threads */
	dk0 = (uint16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(dk0+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for dk0!\n");
			exit(0);
		}
	}
	avg_buffer = (int16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(avg_buffer+i) = (int16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(int16))) == NULL){
			printf("Could not allocate enough memory for avg_buffer!\n");
			exit(0);
		}
	}
	rms_buffer = (uint16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(rms_buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for rms_buffer!\n");
			exit(0);
		}
	}

	if((output_image_avg = (uint16 *)_TIFFmalloc(page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for dark average output image!\n");
		exit(0);
	}
	if((output_image_std = (uint16 *)_TIFFmalloc(page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for standard derivation output image!\n");
		exit(0);
	}

	return 1;
}

void mem_free(){
	/* Clean up and exit */
	printf("-- free memory space and clean up \n");
	pthread_attr_destroy(&attr);
	free(threads);
	free(starg);
	free(start);
	free(stop);
	free(accum);
	
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(dk0[i]);
	}
	free(dk0);
	
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(rms_buffer[i]);
	}
	free(rms_buffer);
		
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(avg_buffer[i]);
	}
	free(avg_buffer);

	tif_release(input_image);
	tif_release(output_image_avg);
	tif_release(output_image_std)
	
}
