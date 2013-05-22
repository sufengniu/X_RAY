#include "imgave_exe.h"
#include <math.h>
    
    imgave_exe::imgave_exe()
    {
    	
	id = 0;
	
	
	
	
    }
    
void imgave_exe::maine(void)
{
       int k;
       double pixin,pixin2;
       
       for (k=0;k<npixels;k++)
       {
       pixin = (double)imgin[k];
         imgsum[k]=imgsum[k] + avecoef*pixin;
       
         imgsumsq[k]=imgsumsq[k] + avecoef*pixin*pixin;
	 imgouts[k]=(unsigned short)floor(imgsum[k]);
       
       }
}
	
	
	

void imgave_exe::setParams(
    int naverages, 
    int numpix, 
    unsigned short* indata,
    double* outdata,
    double* outdata2,
    unsigned short* outdatas)

{


    //summed data
    imgsum=outdata;
    //summed data
    imgsumsq=outdata2;
    //
    imgin=indata;
    
    
    imgouts=outdatas;
    
    //number of imgs to averate
    numave=naverages;
   
   // number of pixels to compute
   npixels=numpix;
   
   avecnt=0;
   avecoef=1.0/(double)numave;


}


