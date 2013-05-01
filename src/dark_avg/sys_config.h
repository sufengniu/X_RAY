#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tiffio.h>

#define BILLION 1000000000L;

#define MAX_THREAD              8       // maxium threads number, defined by the hardware
#define BIT_PER_SAMPLE          16      // defined by camera property
#define DEFAULT_THREADS_NUM     8       // default threads number

/* image tag, tif.c need to be modefied */
#define LENGTH                  2000
#define WIDTH                   1152

int NUM_THREADS, NUM_PROCESS_THREADS;

uint16 *input_image;
uint16 **buffer;
int buffer_width, buffer_length;
int buffer_offset;
int pages;

int thread_sel;		// switch to threads
pthread_cond_t thread_sel_cv;

/* clock_t start, end */
struct timespec start, stop;
double accum;

/* arg for slave threads */
typedef struct slave_thread_arg
{
        int tid;	// thread id
        int pid;	// process id
	
} slave_thread_arg;

/* arg for tif thread */
typedef struct tif_thread_arg
{
        int tid;
        int pid;
        char **argv;

} tif_thread_arg;

#endif
