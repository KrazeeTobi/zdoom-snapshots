//////////////////////
// tga loader class //
//////////////////////
#include "tga.h"







TGALoader::TGALoader()
{
    // defaults
    Defaults();    
}


TGALoader::TGALoader(File &file)
{
    // defaults
    Defaults();

    // assign
    ImageFile=&file;

    // read tga header
    ImageFile->seek(0);
    ImageFile->read(Header,18);

//#endif

    // setup width
    Width=Header[13];
    Width<<=8;
    Width+=Header[12];

    // setup height
    Height=Header[15];
    Height<<=8;
    Height+=Header[14];

    // setup format
    switch (Header[16])
    {
        case 8:  Format.init(INDEX8);   break;        // todo: handle indexed formats > 8bits?
        case 16: Format.init(ARGB1555); break;
        case 24: Format.init(RGB888);   break;
        case 32: Format.init(ARGB8888); break;        // endian fix needed here?
    }

    // setup palette flag
    PaletteFlag=Header[1];
}


TGALoader::~TGALoader()
{
    // advoid warnings
    if (ImageFile);
}







int TGALoader::info(int &width,int &height,FORMAT &format,int &palette)
{
    // setup info
    width=Width;
    height=Height;
    format=Format;
    palette=PaletteFlag;
    return 1;
}


int TGALoader::load(void *image,int orientation,int pitch,void *palette)
{
    // fail on bad image file
    if (!ImageFile) return 0;

    // setup palette data
    unsigned palette_offset=Header[4];
    palette_offset<<=8;
    palette_offset+=Header[3];
    unsigned palette_length=Header[6];
    palette_length<<=8;
    palette_length+=Header[5];
    unsigned palette_bits=Header[7];

    // read palette
    if (palette && PaletteFlag && palette_length)
    {
        // reject anything > 256 palette entries
        if (palette_length>256) return 0;

        // setup temp palette buffer
        uchar *temp=new uchar[palette_length*palette_bits/8];
        if (!temp) return 0;

        // read into temp buffer
        ImageFile->seek(18+palette_offset,SEEK_SET);
        ImageFile->read(temp,palette_length*palette_bits/8);

        // initialize palette format
        FORMAT palette_format;
        switch (palette_bits)
        {
            case 16: palette_format.init(ARGB1555); break;
            case 24: palette_format.init(RGB888);   break;
            case 32: palette_format.init(ARGB8888); break;
        }

        // convert from temp buffer to real palette buffer
        RASTER raster;                                                                          
        RASTER::CONVERTER *converter=raster.RequestConverter(palette_format,ARGB8888);
        if (!converter || !converter->Convert(temp,palette,palette_length,NULL))
        {
            // failed convert
            delete converter;
            delete[] temp;
            return 0;
        }

        // cleanup
        delete converter;
        delete[] temp;
    }

    // read image data
    if (image)
    {
        if (Header[2]==1 || Header[2]==2) 
        {
            // TGA types 1 & 2
            unsigned image_offset=18+Header[0];
            if (PaletteFlag) image_offset=18+palette_offset+palette_length*palette_bits/8;
            ImageFile->seek(image_offset,SEEK_SET);
            
            // image orientation
            int tga_orientation=Header[17] & 0x10;
            if (tga_orientation==0) tga_orientation=BOTTOMUP;
            else if (tga_orientation==0x10) tga_orientation=TOPDOWN;
            else return 0;

            // read image data (todo: check all these!)
            if (tga_orientation==TOPDOWN && orientation==TOPDOWN)
            {
                // topdown -> topdown
                int line_size=Width*Format.bytes;
                for (int y=Height-1; y>=0; y--) if (ImageFile->read((char*)image+pitch*y,line_size)!=line_size) return 0;    // 1 or 0?
            }
            else if (tga_orientation==TOPDOWN && orientation==BOTTOMUP)
            {
                // topdown -> bottomup
                int line_size=Width*Format.bytes;
                for (int y=0; y<Height; y++) if (ImageFile->read((char*)image+pitch*y,line_size)!=line_size) return 0;
            }
            else if (tga_orientation==BOTTOMUP && orientation==BOTTOMUP)
            {
                // bottomup -> bottomup
                int line_size=Width*Format.bytes;
                for (int y=Height-1; y>=0; y--) if (ImageFile->read((char*)image+pitch*y,line_size)!=line_size) return 0;
            }   
            else if (tga_orientation==BOTTOMUP && orientation==TOPDOWN)
            {
                // bottomup -> topdown
                int line_size=Width*Format.bytes;
                for (int y=Height-1; y>=0; y--) if (ImageFile->read((char*)image+pitch*y,line_size)!=line_size) return 0;
            }   
            else return 0;
        }
        else
        {
            // unknown
            return 0;
        }
    }

    // success
    return 1;
}


int TGALoader::save(int width,int height,int pitch,FORMAT const &src,FORMAT const &dest,void *image,void *palette,char *options)
{
    // advoid warnings
    if (width || height || pitch || src.ok() || dest.ok() || image || palette || options);
    return 0;
}







void TGALoader::Defaults()
{
    // defaults
    Width=0;
    Height=0;
    PaletteFlag=0;
    ImageFile=NULL;
    memset(Header,0,18);
}
