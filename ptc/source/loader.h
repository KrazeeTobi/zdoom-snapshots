////////////////////////
// image loader class //
////////////////////////

#ifndef __PTC_LOADER_H
#define __PTC_LOADER_H

#include "format.h"







class ImageLoader
{
    public:

        // virtual destructor
        virtual ~ImageLoader() {};

        // interface
        virtual int info(int &width,int &height,FORMAT &format,int &palette)  = 0;
        virtual int load(void *image,int orientation,int pitch,void *palette) = 0; 
        virtual int save(int width,int height,int advance,
                         FORMAT const &src,FORMAT const &dest,
                         void *image,void *palette,char *options=NULL)        = 0;
};








#endif
