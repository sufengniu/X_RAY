#include "sys_config.h"
#include "dark_mode.h"
#include "data_mode.h"

int main(int argc, char *argv[])
{
	
	char mode;
	if (argc == 2){
		mode = atoi(argv[1]);	// 1: dark mode, 2: data mode
		numprocs = atoi(argv[2]);
	}
	else{
		printf("Usage: exe m\n");
		printf("exe: executable file\n");
		printf("m: mode");
		exit(1);
	}
	
	if (mode == 1){
		printf("dark mode is selected\n");
		// dkm();
		// dkm(image_info);
	}
	else if (mode == 2){
		printf("data mode is selected\n");
		// dtm();
		// dtm(image_info);
	}
	else{
		printf("Error: wrong mode selection, must be 1 (dark mode) or 2 (data mode)!\n");
		exit(1);
	}

	return 0;
}




















