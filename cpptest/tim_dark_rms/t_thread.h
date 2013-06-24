/******************************************************************************
*This class represents objects corresponding to a thread on the threadpool.
*it has a pthread, mutex, cond. variable, and pointer to a t_execuatable, that it executes
* 
******************************************************************************/



#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "pthreadWin32.h"

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "t_executable.h"

#ifndef _T_THREAD_H
#define _T_THREAD_H

class t_thread
{
	public:

    //ref to t_executable
    t_executable *myexe;
    
    volatile bool run;
    
    //each thread has an id, or number that is unique.
    int threadid;
    //id counter- so when we make a new thread, this is the number it gets
    static int nextid;
    
    //the pthread associalte with this thread obhect
    pthread_t pt;
    
    volatile bool is_need_recycle;
  
    // for app thread signaling the worker thread
    pthread_mutex_t mylock;
    pthread_cond_t mycond;
    
    
    
    void setExecutable(t_executable *e);
    
	
	
    void maine(void *arg1);
   
            
	// put thread into the freelist.	    
    void recycle(void);
    //returns true of thread is busy and occupied with doing something
    bool getIsBusy(void);
    
    //constructor
    t_thread();
    
    //statuc function that points to the function that runs on thread
   static void* stat_runfunction(void* arg1);
   
   //counts signals to keep track of if we receved a signal or not.
    static int getSignalCount(void);
   
   //waits until busycount is 0 , or all threads free
    static void waitNoneBusy(void);
 // waits until dec or inc busy count is called. usefil for waiting for thread to start
    static void waitBusySignal(int last_signal_cnt);

	
	//counts how many times this thread has run maine
	int thread_runcounter;
 private:
   volatile bool is_busy;
   
   
   
   //counts how many threads are busy- mutexed by busylock
   static volatile int busy_count;
   //counts number of times we inc or dec the busy counter, helps to avoid
   // losing signals, when we signal before other thread calls wait.
   //mutexed by busylock
   static volatile int signal_count;
   
     void incBusySemaphore(void);
     void decBusySemaphore(void);
  
   
    //semaphore for counting how many busy threads
    // for worker thread signalling app thread
    static pthread_mutex_t busylock;
    static pthread_cond_t busycond;
    static bool is_semephore_init;
    
};

#endif
