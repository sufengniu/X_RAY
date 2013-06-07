#include "../include/platform_init.h"

void setup(){
	char mode, source;
	char config_sel;

	printf("=============================================\n");
	printf("---------------------------------------------\n");
	printf("------------ fccd X-ray camera  ------------\n");
	printf("---------------------------------------------\n");
	printf("-- Group: APS X-ray division sector 8\n");
	printf("-- Application: X-ray data pre-processing\n");
	printf("-- Description: \n");
	printf("=============================================\n\n");
	printf("Type ctr+c to quit\n");
	
	printf("Please select source: 1. tif file; 2. udp socket sim; 3. 10G UDP\n");
	printf("-- Usage: type 1, 2 or 3 to select\n");
	printf("-- tif file: image loadded from local hard disk\n");
	printf("-- udp socket: simulate the udp socket send and receive");
	printf("-- 10G UDP: image loadded from camera in real time\n");		
	
	scanf("%d", &source);
	if (source == 1){
		printf("tif source is selected\n");
	}
	else if (source == 2){
		printf("udp socket source is selected\n");
	}
	else if (source == 3){
		printf("10G udp source is selected\n");
	}
	else {
		printf("Error: source selection is incorrect!\n");
		exit(0);
	}
	
	printf("Process and threads setup:\n");
	printf("-- Default configuration:\n");
	printf("-- thread number: %d\n", DEFAULT_THREADS_NUM);
	printf("-- process number: %d\n", DEFAULT_PROCESS_NUM);
	printf("Using default configuration? y or n");
	
	scanf("%c", &config_sel);
	if ( config_sel == 'y' || config_sel == 'Y'){
		
	}
	else if( config_sel == 'n' || config_sel == 'N'){
		
		
	}
	else {
		printf("configuration selection is illeagle!\n");
		exit(0);
	}
	
	printf("Please select mode: 1.dark mode; 2.data mode\n");
	printf("-- Usage: type 1 and 2 to select\n");
	printf("-- Dark mode: dark image average and root mean square operation\n");
	printf("-- Data mode: background noise subtraction and compression\n");	
	
	scanf("%d", &mode);
	if (mode == 1){
		printf("Dark mode is selected\n");
	}
	else if (mode == 2) {
		printf("Data mode is selected\n");
	}
	else {
		printf("Error: mode selection is incorrect!\n");
		exit(0);
	}
}

void hw_info(){


	printf("-- hardware information: \n");
	printf("-- \tcore number: %d\n", NUM_CORE);
	printf("-- \tblades number: %d\n", NUM_BLADES);
	printf("-- software definition: \n");
	printf("-- \tthreads number: %d\n", NUM_THREADS);
	printf("-- \tprocess number: %d\n", NUM_PROCESS);
	
}

void dk_mem_alloc(){
	int i;	
	
	/* setup to measure threads time */
	start = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	stop = (struct timespec *)malloc(NUM_THREADS * sizeof(struct timespec));
	accum = (double *)malloc(NUM_THREADS * sizeof(double));
	
	starg = (struct slave_thread_arg *)malloc(NUM_PROCESS_THREADS * sizeof(struct slave_thread_arg));
	
	/* malloc buffer for multiple sub threads */
	dk0 = (uint16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(dk0+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for dk0!\n");
			exit(0);
		}
	}
	avg_buffer = (int16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(avg_buffer+i) = (int16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(int16))) == NULL){
			printf("Could not allocate enough memory for avg_buffer!\n");
			exit(0);
		}
	}
	rms_buffer = (uint16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(rms_buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for rms_buffer!\n");
			exit(0);
		}
	}

	/* mask to remove bad strips */
	mask_buffer = (uint16 **)malloc(NUM_PROCESS_THREADS * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(mask_buffer+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for rms_buffer!\n");
			exit(0);
		}
	}
	
	if((input_image = (uint16 *)_TIFFmalloc(pages * page_size * sizeof(uint16))) == NULL){
		fprintf(stderr, "Could not allocate enough memory for the uncompressed image!\n");
		exit(42);
	}

	if((output_image_avg = (uint16 *)_TIFFmalloc(page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for dark average output image!\n");
		exit(0);
	}
	if((output_image_std = (uint16 *)_TIFFmalloc(page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for standard derivation output image!\n");
		exit(0);
	}
}

void dt_mem_alloc(){
	int i;
	x_low_bound = (int *)malloc(BAD_STRIP * sizeof(int));
	y_low_bound = (int *)malloc(BAD_STRIP * sizeof(int));
	x_high_bound = (int *)malloc(BAD_STRIP * sizeof(int));
	y_high_bound = (int *)malloc(BAD_STRIP * sizeof(int));

	if((data_image = (uint16 *)_TIFFmalloc(pages * page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for data image!\n");
		exit(0);
	}

	if((res_image = (uint16 *)_TIFFmalloc(pages * page_size * sizeof(uint16))) == NULL){
		printf("Could not allocate enough memory for res image!\n");
		exit(0);
	}
	
/*		
	data_buffer = (uint16 **)malloc(pages * sizeof(void *));
	for(i = 0; i<NUM_PROCESS_THREADS; i++){
		if((*(dk0+i) = (uint16 *)malloc(buffer_length * buffer_width * NUM_PROCESS_THREADS * sizeof(uint16))) == NULL){
			printf("Could not allocate enough memory for dk0!\n");
			exit(0);
		}
	}
*/	
	
}

void dk_mem_free(){
	int i;

	/* Clean up and exit */
	printf("-- free memory space and clean up \n");
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
	tif_release(output_image_std);
	
}

void dt_mem_free(){
	int i;

	free(data_image);
	free(res_image);
	/*
	for (i = 0; i < NUM_PROCESS_THREADS; i++){
		free(data_buffer[i]);
	}
	free(data_buffer);	
	*/
	
}
