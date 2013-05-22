               
#ifndef WIN32
#include <unistd.h>
#else
#include "pthreadWin32.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef _T_EXECUTABLE_H
#define _T_EXECUTABLE_H


/*****************************************************************************
*
*	This class is the super class for all jobs sent to the threadpool. make
*	your own class that fills in maine(), inherting this t_executable.
*	You can add your own extra functions if needed to your OWN subclass.
*	
******************************************************************************/

class t_executable
{
	public:
  	int id;
	virtual void maine();
    	
	t_executable();
    	
};

#endif
