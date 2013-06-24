/*------------------------------------------------------
 * FILE: main.c
 * DESCRIPTION: 
 *  
 * 
 * AUTHOR: Sufeng Niu 
 * LAST REVISED: 
 *------------------------------------------------------*/
#include "../include/sys_config.h"
#include "../include/platform_init.h"
#include "../lib/thr_pool.h"
#include "../include/dark_mode.h"
#include "../include/data_mode.h"

/* add customized function thread header file here */
#include "../include/tif.h"	// tif loading thread, dark mode operation
#include "../include/rms.h"	// dark average operation, dark mode operation
#include "../include/sub.h"	// image subtraction, data mode operation

int main(int argc, char *argv[])
{

/*
 * initiliazation, parameters, configuration setup
 */

	char mode, source;
	char config_sel;

	printf("=============================================\n");
	printf("---------------------------------------------\n");
	printf("---- X-ray camera dark average operation ----\n");
	printf("---------------------------------------------\n");
	printf("-- Group: X-ray division sector 8\n");
	printf("-- Application: X-ray data pre-processing\n");
	printf("-- Description: \n");
	printf("=============================================\n\n");
	printf("Type Ctr+c to quit\n");	
	printf("Please select mode: 1.dark mode; 2.data mode\n");
	printf("-- Usage: type 1 and 2 to select\n");
	printf("-- Dark mode: dark image average and root mean square operation\n");
	printf("-- Data mode: noise subtraction and compression\n");	
	
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
	
	printf("Please select source: 1.tif file; 2.10G UDP\n");
	printf("-- Usage: type 1 and 2 to select\n");
	printf("-- tif file: image loadded from local hard disk\n");
	printf("-- 10G UDP: image loadded from camera in real time\n");		
	
	scanf("%d", &source);
	if (source == 1){
		printf("tif source is selected\n");
	}
	else if (source == 2) {
		printf("udp source is selected\n");
	}
	else {
		printf("Error: source selection is incorrect!\n");
		exit(0);
	}

	printf("Process and threads setup:\n");
	printf("-- Default configuration:\n");
	printf("-- \n");
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

	NUM_THREADS = DEFAULT_THREADS_NUM;	// defined in sys_config.h	
	NUM_PROCESS = NUM_BLADES;
	
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
	
	NUM_PROCESS_THREADS = NUM_THREADS;

	hw_info();	// defined in platform_init.c

	
		
	
/*
 * the main thread will launch NUM_THREADS-2 subthreads when using UDP
 * 1 thread for UDP read and 1 thread for UDP write, when reading based
 * on tif library, tif function will be implemented in main, since it has
 * no timing issues.
 */
	/*------------------ tif loading ---------------------*/
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_start);
	
	tif_load(argv+2);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tif_stop);

	tif_accum = (tif_stop.tv_sec-tif_start.tv_sec)+(double)(tif_stop.tv_nsec-tif_start.tv_nsec)/(double)BILLION;
        printf("master thread: done in %lf second\n", tif_accum);
	
	/*------------------ processing threads ---------------------*/	
	mem_alloc();
	buffer_size = buffer_length * buffer_width;
	printf("master thread: buffer size is %d\n", buffer_size);
	printf("master thread: buffer length is %d, buffer width is %d\n", buffer_length, buffer_width);

	/* assign thread number from 1 to NUM_PROCESS_THREADS */
	printf("master thread: create %d processing threads\n", NUM_PROCESS_THREADS);
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		starg[i].tid = i;
		starg[i].pid = 0;
		
		rc = pthread_create(&threads[i], &attr, rms, (void *)(starg+i));
		if (rc){
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(1);
		}
	}
	
	/* wait for all threads complete */
	for (i = 0; i<NUM_PROCESS_THREADS; i++){
		pthread_join(threads[i], &status);
	}
	
	/*------------------ tif -----------------------*/
	printf("master thread: synthesis multiple chunks \n");
	printf("master thread: writing processed image to %s file \n", output_filename[1]);
	tif_syn();

	mem_free();

	pthread_exit(NULL);		
	return 0;
}
