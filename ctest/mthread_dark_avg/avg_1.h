#ifndef AVG_H_
#define AVG_H_

#include <math.h>
#include "sys_config.h"

void *avg(void *arg);
int init(int tid);
int avg_op(int tid);	// average operation
int image_syn(int tid);

#endif /* avg.h */

