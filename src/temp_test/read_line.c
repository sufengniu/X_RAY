#include <tiffio.h>

void main()
{
    TIFF* tif = TIFFOpen("myfile.tif", "r");
    if (tif) {
        uint32 imagelength;
        tdata_t buf;
        uint32 row;
        
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
        buf = _TIFFmalloc(TIFFScanlineSize(tif));
        if (config == PLANARCONFIG_CONTIG) {
            for (row = 0; row < imagelength; row++)
                tiffreadscanline(tif, buf, row);
        } else if (config == planarconfig_separate) {
            uint16 s, nsamples;
            
            tiffgetfield(tif, tifftag_samplesperpixel, &nsamples);
            for (s = 0; s < nsamples; s++)
                for (row = 0; row < imagelength; row++)
                    tiffreadscanline(tif, buf, row, s);
        }
        _tifffree(buf);
        tiffclose(tif);
    }
}
