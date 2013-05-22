            

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

#ifndef _IMGAVE_EXE_H
#define _IMGAVE_EXE_H

/**
 * for cummulative summing images. maine will do ONE cumm. sum. Have input data, ,
 *  and stored sum data. 
 
 
 */
class imgave_exe: public t_executable
{
public:

   //id of this job... 
    int id;
  
   
   
    
    imgave_exe();
    
    
    
    void maine(void);
    
    void setParams(
    	int naverages, 
	int numpix, 
	unsigned short* indata,
	double* outdata,
	double* outdata2,
	unsigned short* outdatas);
	
    
 private:
   
    //summed data
    double *imgsum;
    //outdata
    unsigned short *imgouts;
    //summed data
    double *imgsumsq;
    //
    unsigned short *imgin;
    //number of imgs to averate
    unsigned short numave;
   
   // number of pixels to compute
   int npixels;
   
   
   int avecnt;
   double avecoef;
   
 
    
};
#endif
