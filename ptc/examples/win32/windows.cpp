/////////////////////////////
// windows example program //
/////////////////////////////
#include "ptc.h"





int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{
    // initialize ptc
    PTC ptc(320,200);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("could not initialize");
        return 1;
    }

    // set window title
    ptc.SetTitle("windows example");

    // get display resolution
    int xres=ptc.GetXResolution();
    int yres=ptc.GetYResolution();

    // create fullscreen surface
    Surface surface(ptc,xres,yres,ARGB8888);
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
            uint *pixel=(uint*)(buffer+pitch*y+x*4);
            *pixel=RGB32(random(255),random(255),random(255));
        }

        // unlock surface
        surface.Unlock();

        // update to display
        surface.Update();
    }
    return 0;
}
