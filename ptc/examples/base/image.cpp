///////////////////////////
// image loading example //
///////////////////////////
#include "ptc.h"





int main(int argc,char *argv[])
{
    // initialize from command line (ie. "image RGB565")
    PTC ptc(320,200,argc,argv);
    if (!ptc.ok())
    {
        // fallback to virtual 32bit
        if (!ptc.Init(320,200))
        {
            // failure
            ptc.Error("could not initialize ptc");
            return 1;
        }
    }

    // load image
    Surface surface(ptc,"image.tga");
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not load image");
        return 1;
    }

    // convert image
    if (!surface.Convert(ARGB8888))
    {
        // failure
        ptc.Error("could not convert image");
        return 1;
    }

    // display image
    surface.Update();
    ptc.getch();
    return 0;
}
