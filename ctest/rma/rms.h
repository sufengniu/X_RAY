#ifndef IMAGE_OP_H_
#define IMAGE_OP_H_

#include "sys_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "thr_pool.h"
#include "mpi.h"

/* clock_t start, end */
struct timespec *start, *stop;
double *accum;

uint16 **dk0;			// first dark image
uint16 **avg_buff;		// thread level average operation buffer
uint16 **rms_buff;		// thread level root mean square buffer
uint16 *output_image_avg, *output_image_std;	// synthesised buffer

void *image_rms(int argc, char *argv[], uint16 *image);

int rms_mem_alloc();
int rms_clean();
void *rms(void *sarg);
int op_syn();

#endif /* image_op.h */
