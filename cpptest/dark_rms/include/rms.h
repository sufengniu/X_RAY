#ifndef AVG_H_
#define AVG_H_

#include <math.h>
#include "sys_config.h"

class rms: public
{
public:
	
	int tid, pid;
	rms();
	int t_main(void *arg);

private:


};

/*
void *rms(void *arg);
int init(int tid);
int rms_op(int tid);	// rms operation
int image_syn(int tid);
*/
#endif /* avg.h */

