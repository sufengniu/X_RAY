#include <stdio.h>
#include <stdlib.h>

#include "tinytiff.h"

int main(int argc, char *argv[])
{
 	class tinytiff t;

	unsigned short imagedata[1000000];
	
	// imagedata = (unsigned short *)malloc(t.spec);
	
	t.tifRd("AVG_Dark_FCCD_40V_MoK_OSSub_50ms_att0_01_00001-00768.tif", imagedata);
	
	printf("%d %d \n", t.specs.width, t.specs.length);	
	
	t.tifWr("output.tif", imagedata, t.specs.width, t.specs.length);
	
	return 0;
}
