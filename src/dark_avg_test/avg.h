#ifndef AVG_H_
#define AVG_H_

#include <math.h>
#include "sys_config.h"

int image_index;	// input_image index	

void *avg(void *arg);
int init(int tid);
int avg_op(int tid);	// average operation

#endif /* avg.h */

