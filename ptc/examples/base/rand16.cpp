//////////////////////////////////
// random 16bit RGB565 putpixel //
//////////////////////////////////
#include "ptc.h"





int main(int argc,char *argv[])
{
    // initialize ptc from command line (eg: "rand16 640 480 RGB565")
    PTC ptc(argc,argv);
    if (!ptc.ok())
    {
        // fallback to virtual 16bit
        if (!ptc.Init(320,200,VIRTUAL16))
        {
            // failure
            ptc.Error("could not initialize");
            return 1;
        }
    }

    // get display resolution
    int xres=ptc.GetXResolution();
    int yres=ptc.GetYResolution();

    // create fullscreen surface
    Surface surface(ptc,xres,yres,RGB565);
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not create surface");
        return 1;
    }

    // main loop
    while (!ptc.kbhit())
    {
        // lock surface
        char *buffer=(char*)surface.Lock();
        if (!buffer)
        {
            // failure
            ptc.Error("could not lock surface");
            return 1;
        }

        // plot 100 random pixels
        int pitch=surface.GetPitch();
        for (int i=0; i<100; i++)
        {
            int x=random(xres);
            int y=random(yres);
            ushort *pixel=(ushort*)(buffer+pitch*y+x*2);
            *pixel=RGB16(random(255),random(255),random(255));
        }

        // unlock surface
        surface.Unlock();

        // update to display
        surface.Update();
    }
    return 0;
}
