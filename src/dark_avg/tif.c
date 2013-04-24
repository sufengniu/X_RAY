#include "tif.h"

int tif_load(int argc, char **argv)
{
	int r, c;	// height index, width index
	uint16 s;
	int dircount = 0;
	
	struct tiff_info *info;	
	info = (struct tiff_info *)malloc(sizeof(struct tiff_info));
	
	uint32 data_size;
	unsigned long image_offset;
	uint16 *scanline;	
	TIFF *tif;
	
	tif = TIFFOpen(argv[1], "r");
	if(tif == NULL){
		fprintf(stderr, "ERROR: Could not open input image!\n");
		exit(1);
	}
	printf("opening tiff file...\n");

	do {
		dircount++;
	} while (TIFFReadDirectory(tif));
	printf("%d images in %s\n", dircount, argv[1]);
	
	// input image paramters
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &info->length);
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &info->width);
	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &info->bps);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &info->spp);
	TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &info->photo_metric);
	TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &info->config);
		
	printf("length = %d, width = %d\n", info->length, info->width);
	printf("bit per sample = %d\n", info->bps);
	printf("sample per pixel = %d\n", info->spp);
	printf("photo metirc = %d\n", info->photo_metric);
	printf("planar config = %d\n", info->config);

	info->line_size = TIFFScanlineSize(tif);
	info->image_size = info->line_size * info->length;
	data_size = info->image_size * dircount;	

	if(info->spp != 1){
		info->spp = 1;
		printf("Warnning:sample per pixel value automatically set to 1!\n");
	}
	
	if((input_image = (uint16 *)_TIFFmalloc(data_size)) == NULL){
		fprintf(stderr, "Could not allocate enough memory for the uncompressed image!\n");
		exit(42);
	}

	if((scanline = (uint16 *)_TIFFmalloc(info->line_size)) == NULL){
		fprintf(stderr, "Could not allocate enough memory for the scan buffer!\n");
		exit(42);
	}
	
	image_offset = 0;
	printf("the line size is %d\n", info->line_size);
	printf("the image size is %d\n", info->image_size);
	printf("image offset is %ld\n", image_offset);
	printf("the total sequence image size is %d\n", data_size);
	printf("loading tif files ... \n");

	TIFFSetDirectory(tif, 0);

	do {
		if (info->config == PLANARCONFIG_CONTIG){
			for(r = 0; r < info->length; r++){
				TIFFReadScanline(tif, scanline, r, s);
			
				for(c = 0; c < info->width; c++)
				{		
					input_image[image_offset + info->width * r + c] = *(scanline + c);
				}
			}
		} else if (info->config == PLANARCONFIG_SEPARATE){
			for(s = 0; s < info->spp; s++){
				for(r = 0; r < info->length; r++){
					TIFFReadScanline(tif, scanline, r, s);
					for(c = 0; c < info->width; c++)
	                        	{
        	                        	input_image[image_offset + info->width * r + c] = *(scanline + c);
                	        	}
              			}
			}
		}
		image_offset += info->image_size/2;
	} while (TIFFReadDirectory(tif));
	printf("reading done.\n");

	_TIFFfree(scanline);
	
	TIFFClose(tif);	
	
	return 0;
}

void tif_release(uint16 *image)
{
	_TIFFfree(image);
}

