#ifndef AVG_H_
#define AVG_H_

#include <math.h>
#include "sys_config.h"

class rms: public t_executable
{
public:

	int tid, pid;
	
	rms();
	void maine(void);

private:
	
	
};

void rms(void *arg);
int init(int tid);
int rms_op(int tid);	// rms operation
int image_syn(int tid);

#endif /* avg.h */

