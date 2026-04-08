////////////////////////
// native gdi example //
////////////////////////
#include "ptc.h"





int DrawNativeText(Surface &surface,char text[])
{
    // check native surface type
    if (surface.NativeType()!=NATIVE_WIN32_HBITMAP) return 0;

    // get pointer to native surface data
    void *native=surface.GetNative();
    if (!native) return 0;

    // cast native surface data to HBITMAP
    HBITMAP hBitmap=*((HBITMAP*)native);

    // create a device context for the HBITMAP
    HDC dc=CreateCompatibleDC(NULL);
    HBITMAP hOldBitmap=SelectObject(dc,hBitmap);

    // draw text
    RECT rect;
    rect.left=0;
    rect.right=320;
    rect.top=0;
    rect.bottom=200;
    DrawText(dc,text,-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // cleanup
    SelectObject(dc,hOldBitmap);
    DeleteDC(dc);
    return 1;
}




int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{
    // initialize ptc (GDI)
    PTC ptc("GDI",320,200);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("could not initialize");
        return 1;
    }

    // set window title
    ptc.SetTitle("native gdi example");

    // get display resolution
    int xres=ptc.GetXResolution();
    int yres=ptc.GetYResolution();

    // load image to surface
    Surface surface(ptc,"../base/image.tga");
    if (!surface.Convert(ARGB8888))
    {
        // failure
        ptc.Error("could not load image");
        return 1;
    }

    // draw text on surface
    if (!DrawNativeText(surface,"this text was drawn with native GDI"))
    {
        // failure
        ptc.Error("native access failed");
        return 1;
    }

    // update display
    surface.Update();
    ptc.getch();
    return 0;
}
