/////////////////
// image class //
/////////////////

#ifndef __PTC_IMAGE_H
#define __PTC_IMAGE_H

#include "misc.h"
#include "file.h"
#include "format.h"
#include "globals.h"

#include "tga.h"
/*
#include "pcx.h"
#include "bmp.h"
#include "jpg.h"
#include "png.h"
*/







class Image
{
    public:

        // constants
        enum modes {READ=1,WRITE=2};

        // setup
        Image();
        Image(char filename[],int mode=READ);
        ~Image();

        // open and close
        int open(char filename[],int mode=READ);
        void close();

        // interface
        int info(int &width,int &height,FORMAT &format,int &palette);
        int load(void *image,int orientation,int pitch,void *palette);
        int save(int width,int height,int pitch,
                 FORMAT const &src,FORMAT const &dest,
                 void *image,void *palette,char *options=NULL);

        // status
        int ok();

    private:

        // file
        File file;
        
        // loader
        ImageLoader *loader;
};








#endif
