/////////////////
// image class //
/////////////////
#include "image.h"







Image::Image()
{
    // defaults
    loader=NULL;
}


Image::Image(char filename[],int mode)
{
    // defaults
    loader=NULL;

    // open
    open(filename,mode);
}


Image::~Image()
{
    // close
    close();
}






int Image::open(char filename[],int mode)
{
    // close
    close();

    // handle file mode
    int file_mode=File::BINARY;
    if (mode&READ) file_mode|=File::READ;
    if (mode&WRITE) file_mode|=File::WRITE;

    // open file
    if (!file.open(filename,file_mode)) return 0;

    // attach a loader
    loader=new TGALoader(file);
    if (loader) return 1;
    else return 0;
}


void Image::close()
{
    // close loader
    delete loader;

    // close
    file.close();
}






int Image::info(int &width,int &height,FORMAT &format,int &palette)
{
    // return info
    if (loader) return loader->info(width,height,format,palette);
    else return 0;
}


int Image::load(void *image,int orientation,int pitch,void *palette)
{
    // load image
    if (loader) return loader->load(image,orientation,pitch,palette);
    else return 0;
}


int Image::save(int width,int height,int pitch,
                FORMAT const &src,FORMAT const &dest,
                void *image,void *palette,char *options)
{
    // save image
    if (loader) return loader->save(width,height,pitch,src,dest,image,palette,options);
    else return 0;
}






int Image::ok()
{
    // status
    if (!file.ok()) return 0;
    else if (!loader) return 0;
    else return 1;
}
