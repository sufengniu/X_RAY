#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <tiffio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define BILLION 1000000000L;

/*
* hardware information
* NUM_CORE is threads number on a single blade
* if the thread number is greater than NUM_CORE, the main thread will lanuch MPI library
* by using multiple blades for scalable purpose, the system will run as mixed MPI and
* multi-thread mode
*/
#define NUM_CORE		8
#define NUM_BLADES		1

#define MAX_THREAD              100	// maxium threads number, defined by the hardware
#define DEFAULT_THREADS_NUM     8       // default threads number

#define BIT_PER_SAMPLE          16	// defined by camera property

/* image tag, tif.c need to be modefied */
#define LENGTH                  2000
#define WIDTH                   1152

int NUM_THREADS, NUM_PROCESS_THREADS;	// threads number, number of processing threads
int NUM_PROCESS;			// process number

/* image buffer adress */
uint16 *input_image, *output_image_avg, *output_image_std;
uint16 **dk0;		// first dark image for recursive computation
int16 **avg_buffer;	// set as int16 for avg method 3 computation
uint16 **rms_buffer;

/* image parameters */
int buffer_width, buffer_length;
int buffer_size;
int pages, page_size;

static const char *output_filename[] = {"dark_avg.tif", "dark_rms.tif"};

int thread_status;	// threads status for hand shaking

/* clock_t start, end */
struct timespec *start, *stop;
double *accum;
struct timespec tif_start, tif_stop;
double tif_accum;

/* arg for slave threads */
typedef struct slave_thread_arg
{
	int tid;	// thread id
	int pid;	// process id
} slave_thread_arg;

#endif
