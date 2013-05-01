
/* average thread */
#include "avg.h"

/*
*
*
*/

void *avg(void *arg)
{
	slave_thread_arg *p = (slave_thread_arg *)arg;
	int i, j, k;	// i: column, j: row, k: image number
	
	for (){
		pthread_cond_wait(&thread_sel_cv, NULL);
	
		if (thread_sel == p->tid){
			printf("----------------------------------------------------------\n");	
			printf("Starting processing thread, thread %d, process %d\n", p->tid, p->pid);
			printf("----------------------------------------------------------\n");
		
			for (i = 0; i<buffer_length; i++){
				for (j = 0; j<buffer_width; j++){
					*(*(buffer + thread_sel)+j) = input_image[j + i * buffer_width + thread_sel * ];
					
				}
			}
		}
	}
	
}

