
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpuinfo
{
	float freq;
	int core_num;
	int processor;
}  cpuinfo;

int main(int argc, char *argv[])
{
	struct cpuinfo info = {0, 0.0, 0};
	char *partten;

	FILE *cmdline = fopen("/proc/cpuinfo", "rb");
	char *arg = 0;
	size_t size = 0;
	ssize_t read;
	
	while(read = getdelim(&arg, &size, 0, cmdline) != -1){
		/*if(strcmp(arg, "processor") == 0){
                        printf("find processor\n");
                        
              	}*/

		partten = strstr(arg, "core");
		
		puts(arg);
	}
	free(arg);
	fclose(cmdline);
	return 0;
	
}

/*
#include <stdio.h>
#include <libcpuid.h>

int main(void)
{
    if (!cpuid_present()) {
        printf("Sorry, your CPU doesn't support CPUID!\n");
        return -1;
    }

    struct cpu_raw_data_t raw; 
    struct cpu_id_t data;     

    if (cpuid_get_raw_data(&raw) < 0) { 
        printf("Sorry, cannot get the CPUID raw data.\n");
        printf("Error: %s\n", cpuid_error());
        return -2;
    }

    if (cpu_identify(&raw, &data) < 0) {    
        printf("Sorrry, CPU identification failed.\n");
        printf("Error: %s\n", cpuid_error());
        return -3;
    }

    printf("Processor has %d physical cores\n", data.num_cores);
    return 0;
}
*/
