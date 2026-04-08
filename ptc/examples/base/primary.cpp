////////////////////////////////////////////////////////////////
// random pixels direct to the display (ARGB8888 modes only!) //
////////////////////////////////////////////////////////////////
#include "ptc.h"





int main()
{
    // display resolution
    int xres=320;
    int yres=200;

    // initialize ptc
    PTC ptc(xres,yres,ARGB8888);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("this example requires a 32bit ARGB8888 video mode");
        return 1;
    }

    // get primary surface
    Surface *primary=ptc.GetPrimary();
    if (!primary || !primary->ok())
    {
        // failure
        ptc.Error("could not access primary surface");
        return 0;
    }

    // main loop
    while (!ptc.kbhit())
    {
        // lock primary surface
        uchar* buffer=(uchar*)primary->Lock();
        if (!buffer)
        {
            // failure
            ptc.Error("could not lock primary surface!");
            return 1;
        }
        
        // plot 100 random pixels
        int pitch=primary->GetPitch();
        for (int i=0; i<100; i++)
        {
            int x=random(xres);
            int y=random(yres);
            uint *pixel=(uint*)(buffer+pitch*y+x*4);
            *pixel=RGB32(random(255),random(255),random(255));
        }

        // unlock primary
        primary->Unlock();
    }
    return 0;
}
