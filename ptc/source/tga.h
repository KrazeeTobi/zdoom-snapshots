//////////////////////
// tga loader class //
//////////////////////

#ifndef __PTC_TGA_LOADER_H
#define __PTC_TGA_LOADER_H

#include "file.h"
#include "loader.h"
#include "raster.h"







class TGALoader : public ImageLoader
{
    public:

        // setup
        TGALoader();
        TGALoader(File &file);
        virtual ~TGALoader();

        // interface
        virtual int info(int &width,int &height,FORMAT &format,int &palette);
        virtual int load(void *image,int orientation,int pitch,void *palette); 
        virtual int save(int width,int height,int pitch,
                         FORMAT const &src,FORMAT const &dest,
                         void *image,void *palette,char *options=NULL);
    private:

        // defaults
        void Defaults();

        // file
        File *ImageFile;

        // tga header
        uchar Header[18];

        // image data
        int Width;
        int Height;
        int Orienation;
        int PaletteFlag;
        FORMAT Format;
};








#endif

