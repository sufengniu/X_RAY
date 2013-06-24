/**********************************************************************************
*
*This class sets up a pool of threads. the threads get spawned and just wait for something to do.
* You pass a t_executable, or subclass thereof, and tell the treads to get to work.
* you call synchronize to wait for the threasd to finish.
***********************************************************************************/
#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "pthreadWin32.h"

#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "t_thread.h"

#ifndef _T_THREADPOOL_H
#define _T_THREADPOOL_H


class t_threadpool
{
public:

	//list of t_threads
	t_thread *threadlist[64];
	int num_threads;
        
	// call to create N threads for the pool.
	void fillPool(int N);

	//grab a thread that is not busy
	t_thread* getFreeThread(void);

	//start the job for the said t_executable
	void startJob(t_executable *executable_x);
            
	// put thread back into free list.
	void recycle(void);
    
	// wait for threads to finish up.
	void synchronize(void);
        
};

#endif
