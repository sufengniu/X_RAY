#include "count_exe.h"

     volatile int count_exe::cnt=0;
       
     
     pthread_mutex_t count_exe::cntlock;
     volatile bool count_exe::is_lockinit=false;
    
    
    count_exe::count_exe()
    {
    	waittime = 25;
	id = 0;
	
	if (!is_lockinit)
	{
		pthread_mutex_init(&cntlock, NULL);
		is_lockinit=true;
	}
	
    }
    
    void count_exe::maine(void)
    {
        printf("running a counter job %d\n",id);
        usleep(waittime);
	pthread_mutex_lock(&cntlock);
	cnt++;
	pthread_mutex_unlock(&cntlock);
        printf("done with counter %d\n",id);
	}
  
