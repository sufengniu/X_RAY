#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <tiffio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define BILLION 1000000000L;

#define NUM_CORE		8
#define NUM_BLADES		1

#define MAX_THREAD              8       // maxium threads number, defined by the hardware
#define BIT_PER_SAMPLE          16      // defined by camera property
#define DEFAULT_THREADS_NUM     8       // default threads number

/* image tag, tif.c need to be modefied */
#define LENGTH                  2000
#define WIDTH                   1152

int NUM_THREADS, NUM_PROCESS_THREADS;	// threads number, number of processing threads
int NUM_PROCESS;			// process number

uint16 *input_image, *output_image;
uint16 **avg_buffer;
uint16 **rms_buffer;
int buffer_width, buffer_length;
int buffer_size;
int pages, page_size;

// const char *output_filename[] = {"dark_avg.tif", "dark_rms.tif"};

int thread_status;	// threads status for hand shaking

pthread_cond_t thread_sel_cv, image_tag_rdy, thread_init_rdy;
						// thread_sel_cv: active each thread after loading buffer
						// thread_init_rdy: processing threads initial done
						// image_tag_rdy: tag initialization flag
pthread_mutex_t sel_mutex, init_mutex;

/* clock_t start, end */
struct timespec *start, *stop;
double *accum;

/* arg for slave threads */
typedef struct slave_thread_arg
{
	int tid;	// thread id
	int pid;	// process id
} slave_thread_arg;

/* arg for tif thread */
typedef struct tif_thread_arg
{
	int tid;	// thread id
	int pid;	// process id
	char **argv;

} tif_thread_arg;

#endif
