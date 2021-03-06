#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <tiffio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>

#define BILLION 1000000000L;

#define MAX_BLAEDS_NUM		2	// maxium number of blades
#define MAX_THREAD              100	// maxium threads number, defined by the hardware
#define BIT_PER_SAMPLE          16	// defined by camera property

#define alpha			3	// between [0, 4]
#define beta			15	// between [0, 20]

#define PORT			2345
static char *ipaddr = {"127.0.0.1"};

/* mask to remove bad strip, you can define more bad strip as XXX_N format */
int *x_low_bound, *x_high_bound;
int *y_low_bound, *y_high_bound;

#define BAD_STRIP               1
#define XLB_1                   109     // x low bound
#define XHB_1                   120     // x high bound
#define YLB_1                   0       // y low bound
#define YHB_1                   961     // y high bound

/* thread level global shared variables */
//___________________________________________________________

static int CPU_THRS_NUM;
int totalthrds;				// threads number in total
int ttlcompthrds;			// computation threads in total
int compthrds;				// computation threads in each thread
int numprocs;				// process number
int numthrds;				// threads number in each process

uint16 *strip_buff;     // threads image buffer

int thread_status;      // threads status for hand shaking

typedef struct image_info_type{
	int width;
	int length;
	int page_num;
	int buffer_width, buffer_length;
	int buffer_size;
	int image_size;

} image_info_type;
struct image_info_type *image_info;

// Threads level barrier for sync
pthread_barrier_t barrier;

//____________________________________________________________

static const char *output_filename[] = {"dark_avg.tif", "dark_rms.tif", "data.tif"};

struct timespec tif_start, tif_stop;
double tif_accum;

/* arg for slave threads */
typedef struct slave_arg
{
	int tid;	// thread id
	int pid;	// process id
	uint16 *image;	// image pointer
} slave_arg;
struct slave_arg *sarg; // slave arguments

#endif
