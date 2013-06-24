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

void *image_rms(int argc, char *argv[], uint16 *image);

int rms_mem_alloc();
int rms_clean();
void *rms(void *sarg);
int op_syn();

#endif /* image_op.h */
