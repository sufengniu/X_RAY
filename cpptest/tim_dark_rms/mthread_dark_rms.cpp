#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifndef WIN32
#include <sys/time.h>
#else
#include <windows.h>
#include <tchar.h>
#endif

#include "t_threadpool.h"
#include "count_exe.h"
#include "imgave_exe.h"
#include "imgcomp_exe.h"
#include "sys_config.h"
#include "tif.h"
#include "rms.h"

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main( int argc, const char* argv[] )
#endif
{
/*
 * initialization threads and parameters definition
 */	
	int i, j;	
	
	NUM_THREADS = DEFAULT_THREADS_NUM;	// defined in sys_config.h	
	NUM_PROCESS = NUM_BLADES;
	


	if (argc == 3){
		NUM_THREADS = atoi(argv[1]);
	} else{
		printf("Usage: exe n tif\n");
		printf("exe: executable file\n");
		printf("n: threads number\n");
		printf("tif: tif image file\n");
		exit(1);
	}
	
	if ((NUM_THREADS < 1)||(NUM_THREADS > MAX_THREAD)){
		printf("Error: the number of thread should between 1 and %d.\n", MAX_THREAD);
		exit(1);
	}
	
	printf("---------------------------------------------\n");
	printf("---- X-ray camera dark average operation ----\n");
	printf("---------------------------------------------\n");
	printf("-- hardware information: \n");
	printf("-- \tcore number: %d\n", NUM_CORE);
	printf("-- \tblades number: %d\n", NUM_BLADES);
	printf("-- software definition: \n");
	printf("-- \tthreads number: %d\n", NUM_THREADS);
	printf("-- \tprocess number: %d\n", NUM_PROCESS);
	
	NUM_PROCESS_THREADS = NUM_THREADS;

	//declare a threadpool
	t_threadpool tp;

	//declare 16 executables as pointers, so we can have up to 16 threads doing something
	t_executable *a[16];

	if (argc==1)
	printf("Running defaults\n");

	// you can tell how many threads to run from command line: t_test 6 for 6 threads. Defaults to 8 w/out args
	if (argc>1)
		NUM_THREADS=atoi(argv[1]);

	// on command line, give 2nd arg for which alg to run. def to averaging.
	int ALGORITHM=0;

	if (argc>2)
		if (strcmp("comp",argv[2])==0)
		{
			printf("Running compression\n");
			ALGORITHM=1;
		}
		else
		{
			printf("Running averaging\n");
			ALGORITHM=0;
		}
	
	int nt=0;

	//we run the whole test 100 times.
	int NTESTS=100;

	int NAVE=16;
	timeval tim;

	//image mem size
	int memsize=1024*1024;

	//we divide image up between threads, so we skip memory
	int memskip = memsize/NUM_THREADS;

	//num pixels  per thread.
	int NPIX=memskip;

	// declare memory for images.

	//add 1024 in case we have non power of 2 num threads, so we cant overrun mem and seg faul
	unsigned short *a_in= new unsigned short[memsize+1024];	
	unsigned short *a_incpy= new unsigned short[memsize+1024];	
	unsigned short *a_outcpy= new unsigned short[memsize+1024];	

	//declare memory for results
	double 	*a_sum= new  double[memsize+1024];	
	double 	*a_sumsq= new  double[memsize+1024];	

	// create objects for each job to run, t_executable objhects.
	// these objects will be passed to the threads. 
	for ( j=0;j<NUM_THREADS;j++)
	{

		if (ALGORITHM==0)
			a[j] = new imgave_exe();
  		else
			a[j] = new imgcomp_exe();
	}

/*
 * this is a hack. we put a few extra threads in the pool.  don't know why,but it works better.
 * cygwin sometimes runs out of threads for some reason...
 */
  tp.fillPool(NUM_THREADS+2);

	while(nt<NTESTS)
	{
		nt++;

	//get start tiem before taking NAVE frames
  
	//we are doing a timing to see how fast this runs... so we get start time of day.
		gettimeofday(&tim, NULL);
		double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

/*
 * set up all parameters of the calculation jobs- call setParams on the t_execuatables. Have to cast to subclass.
 * we tell calc obhects where the image memory is, and where to put results.
 * we tell calc objhects how large images are, and how many pixels to do, and were the image mem starts for that
 * partial image.
 */
		for ( j=0;j<NUM_THREADS;j++)
		{

			a[j]->id = j+1;
  
			if (ALGORITHM==0)
				((imgave_exe*)a[j])->setParams(
				NAVE,
				NPIX,
				&(a_in[memskip*j]),
				&(a_sum[memskip*j]),
				&(a_sumsq[memskip*j]),
				&(a_outcpy[memskip*j]));
			else
   				((imgcomp_exe*)a[j])->setParams(
				0,//threshold
				NPIX,
				memskip*j,//offset
				&(a_in[memskip*j]),
				&(a_incpy[memskip*j]),
				&(a_outcpy[memskip*j]));
		}
 
/*
 * We only have to fill the threadpool and set up the t_executable obhects ONCE.
 * When the detector takes data, all we have to do is get the image data from the camera or socket
 * and start the threads, and wait until done.
 * we run the test NAVE times, like we take NAVE images fromt eh camera in real time.
 */

		for (int nn=0;nn<NAVE;nn++)
		{

		//pass raw image, from say, the detector a_incpy, to the working memory of the thread pool, a_in.
		// this corresponds to reading the image from a file or socket.

			memcpy(a_incpy,a_in,memsize*sizeof(short));

		// We now start the jobs. 
			for ( j=0;j<NUM_THREADS;j++)
			{
				tp.startJob(a[j]);
			}

		//here we wait for the jobs to finish. synchronize() blocks until all threads are done.
			tp.synchronize(); 

		}//for( j=0;j<NUM_THREADS;j++)

		//we calc time it took for the threads to run, and take recip. to find the max possible frame rate.
		gettimeofday(&tim, NULL);
		double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
		printf("Frame rate %.6lf fps \n", (double)NAVE/(t2-t1));
	
	} // end while

}
