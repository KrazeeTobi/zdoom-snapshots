///////////////////////////////
// 256 color palette example //
///////////////////////////////
#include "ptc.h"





int main(int argc,char *argv[])
{
    // initialize ptc from command line
    PTC ptc(argc,argv);
    if (!ptc.ok())
    {
        // default to virtual 8bit
        ptc.Init(320,200,VIRTUAL8);
        if (!ptc.ok())
        {
            // failure
            ptc.Error("could not initialize ptc");
            return 1;
        }
    }
    
    // get display resolution
    int xres=ptc.GetXResolution();
    int yres=ptc.GetYResolution();

    // create fullscreen 256 color surface
    Surface surface(ptc,xres,yres,INDEX8);
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not create surface");
        return 1;
    }

    // create palette
    Palette palette;
    
    // lock palette
    uint *data=(uint*)palette.Lock();
    if (!data)
    {
        // failure
        ptc.Error("could not lock palette");
        return 1;
    }

    // setup palette gradient
    for (int i=0; i<256; i++) data[i]=RGB32(i/2,i/4,i);
    
    // unlock palette
    palette.Unlock();

    // set surface palette
    surface.SetPalette(palette);

    // set display palette if required
    FORMAT format=ptc.GetFormat();
    if (format.type==INDEXED && format.model!=GREYSCALE) ptc.SetPalette(palette);

    // lock the surface
    uchar *buffer=(uchar*)surface.Lock();
    if (!buffer) 
    {
        // failure
        ptc.Error("could not lock surface");
        return 1;
    }

    // draw a color gradient across the surface
    int pitch=surface.GetPitch();
    for (int y=0; y<yres; y++)
    {
        for (int x=0; x<xres; x++)
        {
            *(buffer+y*pitch+x)=(uchar)x;
        }
    }       
    
    // unlock surface
    surface.Unlock();

    // update to display
    surface.Update();
    ptc.getch();
    return 0;
}
