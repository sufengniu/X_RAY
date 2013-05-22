#include "imgcomp_exe.h"
#include <math.h>
    
    imgcomp_exe::imgcomp_exe()
    {
    	
	id = 0;
	
	
	
	
    }
    
void imgcomp_exe::maine(void)
{
       int k;
      unsigned short st_val;
       unsigned long int *adrx;
       
       address=0;
       
       for (k=0;k<npixels;k++)
       {
      	 int t=thr + imgdark[k];
	 
	 if (imgin[k]>t)
	 	st_val = imgin[k];
	else
		st_val=0;
		
	
	adrx = (unsigned long*)&imgouts[address];
	*adrx=k+mem_offset;
	address+=2;
	imgouts[address]=st_val;
	
       
       }
}
	
	
	

	
void imgcomp_exe::setParams(
    int threshold, 
	int numpix,
	int offs, 
	unsigned short* indata,
	unsigned short* darkdata,
	unsigned short* outdatas)

{


    //
    imgin=indata;
    
    imgdark=darkdata;
    imgouts=outdatas;
 
   // number of pixels to compute
   npixels=numpix;
  thr= threshold;
  address=0;
  mem_offset=offs;


}


