
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

#ifndef _IMGCOMP_EXE_H
#define _IMGCOMP_EXE_H

/**
 * for cummulative summing images. maine will do ONE cumm. sum. Have input data, ,
 *  and stored sum data. 
 */
class imgcomp_exe: public t_executable
{
public:

	// thread id and process id of this job... 
	int tid, pid;
  
	imgcomp_exe();
   
	void maine(void);
    
	void setParams(
	int threshold, 
	int numpix, 
	int offs,
	unsigned short* indata,
	unsigned short* darkdata,
	unsigned short* outdatas);
	
private:
 
	// outdata
	unsigned short *imgouts;
  
	unsigned short *imgin;

	unsigned short *imgdark;
   
	// number of pixels to compute
	int npixels;
   
	int thr;
	unsigned long address;
   
	unsigned long mem_offset;
   
 
    
};
#endif
