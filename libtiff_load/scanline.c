#include <stdio.h>
#include <tiffio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_PER_SAMPLE		16	// defined by camera property
#define SAMPLE_PER_PIXEL	1	// sample per pixel default is 1

typedef struct tiff_info{
	unsigned short type;	/* little or big endian */
	int width;
	int length;
	int depth;
	unsigned short photo_metric;
	unsigned short spp;	// sample per pixel, 16 bits per pixel for ANL camera
	unsigned short bps;	// bit per sample, default is 1
	int line_size;
	int image_size;
} tiff_info;

int main(int argc, char **argv)
{
	int r, c;	// height index and width index
	uint16 s;

	struct tiff_info *info;	
	info = (struct tiff_info *)malloc(sizeof(struct tiff_info));
	
	unsigned char *input_image;
	unsigned char *output_image;
	unsigned char *scanline;	
	TIFF *input_file;
	int io_status;	// status of io operation
	
	/* according to tiffio library tsize_t is int32, tstrip_t is uint32 */		
	tsize_t strip_size, buffer_size;
	tstrip_t strip_max, strip_count;
	unsigned long image_offset, result;
	unsigned long count;
	unsigned char *input_buffer;
	FILE *output_file;	

	input_file = TIFFOpen(argv[1], "r");
	if(input_file == NULL){
		fprintf(stderr, "ERROR: Could not open input image!\n");
		exit(1);
	}
	printf("opening tiff file...\n");

	// input image paramters
	TIFFGetField(input_file, TIFFTAG_IMAGELENGTH, &info->length);
	TIFFGetField(input_file, TIFFTAG_IMAGEWIDTH, &info->width);
	TIFFGetField(input_file, TIFFTAG_BITSPERSAMPLE, &info->bps);
	TIFFGetField(input_file, TIFFTAG_SAMPLESPERPIXEL, &info->spp);
	TIFFGetField(input_file, TIFFTAG_PHOTOMETRIC, &info->photo_metric);
	
	printf("length = %d, width = %d\n", info->length, info->width);
	printf("bit per sample = %d\n", info->bps);
	printf("sample per pixel = %d\n", info->spp);
	printf("photo metirc is %d\n", info->photo_metric);
	
	info->line_size = TIFFScanlineSize(input_file);
	info->image_size = info->line_size * info->length;
	
	if((input_image = (unsigned char *)_TIFFmalloc(info->image_size)) == NULL){
		fprintf(stderr, "Could not allocate enough memory for the uncompressed image!\n");
		exit(42);
	}

	if((scanline = (unsigned char *)_TIFFmalloc(info->line_size)) == NULL){
		fprintf(stderr, "Could not allocate enough memory for the scan buffer!\n");
		exit(42);
	}

	printf("the line size is %d\n", info->line_size);
	printf("reading tif files ... \n");
	for(r = 0; r < info->length; r++){
		for (s = 0; s < info->spp; s++){
			TIFFReadScanline(input_file, scanline, r, s);
		}
		for(c = 0; c < 2*info->width; c++)
		{
			input_image[2*info->width * r + c] = *(scanline + c);
		}
		
	}
	
	if(info->photo_metric != PHOTOMETRIC_MINISWHITE){
                // Flip bits
                printf("Fixing the photometric interpretation\n");

                for(count = 0; count < info->image_size; count++)
                input_image[count] = ~input_image[count];
        }
	
	output_file = fopen("loaded_image_line.dat", "w");

        for(count = 0; count < info->image_size; count++){
                fprintf(output_file, "%02x", (unsigned char) input_image[count]);
                if((count + 1) % (info->width / 8) == 0) fprintf(output_file, "\n");
                else fprintf(output_file, " ");
        }

        fclose(output_file);
	
	_TIFFfree(input_image);
	_TIFFfree(scanline);

	TIFFClose(input_file);	
	
	return 0;
}
