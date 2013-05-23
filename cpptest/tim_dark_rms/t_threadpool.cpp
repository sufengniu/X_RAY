
#include "t_threadpool.h"
    
    void t_threadpool::fillPool(int N)
    {
    	int s;
	
		//make a dummy thread just to init the global static mutexes
		t_thread dummythread;

        num_threads=N;
	//threadlist= (t_thread*)malloc(sizeof(*t_thread)*N);
	
        for (int k=0;k<num_threads;k++)
	{
	    s=t_thread::getSignalCount();
	    
            threadlist[k] = new t_thread();
	    //wait until thread is created and starts..
	    //t_thread::waitBusySignal();
	    //the above wont work because the signal may happen before the wait.
	    // then we wait forever.
	    //instead we poll to make sure signal count is increased. 
	    //pooling is OK because we are just creating the pool.
	    while(t_thread::getSignalCount()==s)
	    	usleep(10);
         }
	    
    }

    t_thread* t_threadpool::getFreeThread(void)
    {
        for (int k=0;k<num_threads;k++)
	{
            if (!threadlist[k]->getIsBusy())
                return(threadlist[k]);
	}
        return(0);
    }





    void t_threadpool::startJob(t_executable *executable_x)
    {
        t_thread *t=getFreeThread();
	if (t!=0)
        {
	  //printf("Got free thread %d\n",t->threadid);
	  t->setExecutable(executable_x);
          //t->is_busy=true;
          //t->is_need_recycle=true;
        
	// store signal couint, incase we miss a signal. this fixes race condition.
	  int s = t_thread::getSignalCount();
	  pthread_mutex_lock (&(t->mylock));
	  pthread_cond_signal(&(t->mycond));
	  pthread_mutex_unlock (&(t->mylock));
	  // wait until thread actually starts...
	  //by passing last signal count, we assure that if we miss the signal then
	  // we assume the thread is started.
	  t_thread::waitBusySignal(s);
	  
	}
	else
		printf("All threads busy\n");
     }       
 
            //deprecated
    void t_threadpool::recycle(void)
    {
        for (int k=0;k<num_threads;k++)
	{
            if (threadlist[k]->is_need_recycle==1)
	    {
	      pthread_mutex_lock (&(threadlist[k]->mylock));
              threadlist[k]->is_need_recycle=false;
               printf( "Recycled a thread\n");
	       }
	}  	
    }
    
    
    void t_threadpool::synchronize(void)
    {
    
    
    	t_thread::waitNoneBusy();
	
	
        //wait for all threads to be done
      /*  bool not_done = true;
        while(not_done)
	{
            not_done =false;
           
            for (int k=0;k<num_threads;k++)
	    {
                if (threadlist[k]->is_busy)
                    not_done = true;
	    }    
	}
        recycle();
       //printf ("all threads done\n");*/
       
     /*  usleep(10);
       
        for (int k=0;k<num_threads;k++)
	{
           //attampt to get lock on all threads.
	   // when this for is done we have al locks meaning all threads are done.
	      
	       if (threadlist[k]->is_busy)
	       		pthread_mutex_lock (&(threadlist[k]->mylock));
			
              threadlist[k]->is_need_recycle=false;
               printf( "Recycled a thread %d\n", threadlist[k]->threadid);
	       
	} 
	printf("All threads done and recycled\n"); 	 */
     
    }
 
