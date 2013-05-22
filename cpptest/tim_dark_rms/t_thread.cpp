#include "t_thread.h"



    pthread_mutex_t t_thread::busylock;
     pthread_cond_t t_thread::busycond;
      volatile int t_thread::busy_count=0;
      
     volatile int t_thread::signal_count=0;

void* t_thread::stat_runfunction(void* arg1)
{
	t_thread *thrd;
	thrd=(t_thread*)arg1;
    thrd->maine(0);
	return(0);
}

int t_thread::nextid=0;
    
    void t_thread::setExecutable(t_executable *e)
     {
        myexe=e;
    	}
	
	
    void t_thread::maine(void *arg1)
    {
        //printf("Running t_thread.maine\n");
        while (run)
	{
            //pool will create thread and acq the lock.
            //printf("t_thread.maine() waiting for lock\n");
            pthread_mutex_lock (&mylock);
	    is_busy=false;
	    decBusySemaphore();
	    //wait on cv, mylock unlocked during wait
	    pthread_cond_wait(&mycond, &mylock);
	    //afer signal we are here, mylock is locked by worker thread. (maine)
	    //
            //printf("got lock\n");
            is_busy=true;
	    incBusySemaphore();
	    //release lock
	    pthread_mutex_unlock (&mylock);
	    //now we can safely check is_busy if need be
            
	    thread_runcounter++;
	    //when lockl rel;eased, then we go!
            myexe->maine();
            
	    //deprecared
            is_need_recycle=true;
	   
            //recycle();
	    
	    //signal threadpool that we are done...
	    
          }
	} 
        
	
    bool t_thread::getIsBusy(void)
    {
    	bool is_b;
    	
    	pthread_mutex_lock (&mylock);
	is_b=is_busy;
	pthread_mutex_unlock (&mylock);
	return(is_b);
    }
	
	//deprecated	    
    void t_thread::recycle(void)
     {
        //here we wait for pool to recucle the thread, and grab its lock.
        while (is_need_recycle)
           usleep(1000);
        }
        
 
     bool t_thread::is_semephore_init=false;
    
     int t_thread::getSignalCount(void)
     {
     int s;
     
     	pthread_mutex_lock (&busylock);
	s=signal_count;
	pthread_mutex_unlock (&busylock);
	return(s);
     
     }
     void t_thread::incBusySemaphore(void)
     {
     	pthread_mutex_lock (&busylock);
	busy_count+=1;
	//printf("incbusy %d\n",busy_count);
	//we inc this meaning that we sent a singnal, for keeping track in case noone is waiting
	signal_count++;
	pthread_cond_signal(&busycond);
     	pthread_mutex_unlock (&busylock);
	
     }
     void t_thread::decBusySemaphore(void)
     {
         pthread_mutex_lock (&busylock);
	 if (busy_count>0)
		busy_count=busy_count-1;
		
		//by counting, we can keep track of signals sent if noone is waiting.
	signal_count++;
	//printf("decbusy %d\n",busy_count);
	pthread_cond_signal(&busycond);
     	pthread_mutex_unlock (&busylock);
     }
    
    //static function called by app thread.
    void t_thread::waitNoneBusy(void)
    {
    	pthread_mutex_lock (&busylock);
	while (busy_count>0)
	{	
		//printf("waitbusy %d\n",busy_count);

		pthread_cond_wait(&busycond, &busylock);
	}
	
       pthread_mutex_unlock (&busylock);
    }
 
    
    //static function called by app thread.
    void t_thread::waitBusySignal(int last_signal_cnt)
    {
    	pthread_mutex_lock (&busylock);
	//if signal_count>0 then some sent signal before we called wait.
	//in that case we just dec the counter, like an event queue.
	//prevents wait for evers.
	
	if (signal_count==last_signal_cnt)
		pthread_cond_wait(&busycond, &busylock);
	
	
	
	//pthread_cond_wait(&busycond, &busylock);
       pthread_mutex_unlock (&busylock);
    }
 


    t_thread::t_thread()
    {
        
		thread_runcounter=0;
	if (!is_semephore_init)
	{
	is_semephore_init=true;
	//make a lock and cond
	pthread_mutex_init(&busylock, NULL);
	pthread_cond_init (&busycond, NULL);
        busy_count=0;
	
	}
	
        //threadpool is calliing this thread.
        run=true;
        myexe=  new t_executable();
	
	//make a lock and cond
	pthread_mutex_init(&mylock, NULL);
	pthread_cond_init (&mycond, NULL);
        
	
	
	
	threadid = nextid;
	nextid+=1;
	
        long rc = pthread_create(&pt,NULL,t_thread::stat_runfunction, (void*)this);
		
       } 

