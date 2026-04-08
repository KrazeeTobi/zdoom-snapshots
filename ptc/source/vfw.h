///////////////////////////////
// video for windows wrapper //
///////////////////////////////

#ifndef __PTC_VFW_H
#define __PTC_VFW_H

#include "misc.h"
#include "rect.h"








class VFW
{
    public:
    
        // drawdib flags
        enum FLAGS
        {
            UPDATE        = 0x0002,         // redraw the last DIB
            SAME_HDC      = 0x0004,         // HDC same as last call (all setup)
            SAME_DRAW     = 0x0008,         // draw params are the same
            DONTDRAW      = 0x0010,         // dont draw frame, just decompress
            ANIMATE       = 0x0020,         // allow palette animation
            BUFFER        = 0x0040,         // always buffer image
            JUSTDRAWIT    = 0x0080,         // just draw it with GDI
            FULLSCREEN    = 0x0100,         // use DisplayDib
            BACKGROUNDPAL = 0x0200,	        // realize palette in background
            NOTKEYFRAME   = 0x0400,         // this is a partial frame update
            HURRYUP       = 0x0800,         // hurry up please!
            HALFTONE      = 0x1000,         // always halftone
            PREROLL       = DONTDRAW,
            SAME_DIB      = SAME_DRAW,
            SAME_SIZE     = SAME_DRAW
        };

        // setup
        inline VFW();
        inline ~VFW();

        // interface
        inline int DrawDib(HDC dc,RECTANGLE const &src,RECTANGLE const &dest,BITMAPINFOHEADER *info,void *bits,int flags=0); 

        // status
        inline int ok();

    private:
                                  
        // function typedefs
        #ifndef __CYGNUS__
        typedef HANDLE (WINAPI *DRAWDIBOPEN_FUNCTION)  (void);
        typedef BOOL   (WINAPI *DRAWDIBDRAW_FUNCTION)  (HANDLE,HDC,int,int,int,int,LPBITMAPINFOHEADER,LPVOID,int,int,int,int,UINT); 
        typedef BOOL   (WINAPI *DRAWDIBCLOSE_FUNCTION) (HANDLE);
        #else
        typedef HANDLE WINAPI (*DRAWDIBOPEN_FUNCTION)  (void);
        typedef BOOL   WINAPI (*DRAWDIBDRAW_FUNCTION)  (HANDLE,HDC,int,int,int,int,LPBITMAPINFOHEADER,LPVOID,int,int,int,int,UINT); 
        typedef BOOL   WINAPI (*DRAWDIBCLOSE_FUNCTION) (HANDLE);
        #endif

        // function pointers
        #ifndef __CYGNUS__
        DRAWDIBOPEN_FUNCTION  DrawDibOpenFunction;
        DRAWDIBDRAW_FUNCTION  DrawDibDrawFunction;
        DRAWDIBCLOSE_FUNCTION DrawDibCloseFunction;
        #else
        HANDLE WINAPI (*DrawDibOpenFunction)(void);
        BOOL   WINAPI (*DrawDibDrawFunction)(HANDLE,HDC,int,int,int,int,LPBITMAPINFOHEADER,LPVOID,int,int,int,int,UINT); 
        BOOL   WINAPI (*DrawDibCloseFunction)(HANDLE);
        #endif

        // vfw32.dll instance
        HMODULE LibraryInstance;

        // drawdib handle
        HANDLE DrawDibHandle;

        // status
        int Status;
};








inline VFW::VFW()
{
    // defaults
    DrawDibHandle=NULL;
    DrawDibOpenFunction=NULL;
    DrawDibDrawFunction=NULL;
    DrawDibCloseFunction=NULL;

    // load vfw32.dll
    LibraryInstance=(HMODULE)LoadLibrary("msvfw32.dll");
    if (!LibraryInstance) Status=0;
    else
    {
        // get function addresses
        DrawDibOpenFunction  = (DRAWDIBOPEN_FUNCTION)  GetProcAddress(LibraryInstance,"DrawDibOpen");
        DrawDibDrawFunction  = (DRAWDIBDRAW_FUNCTION)  GetProcAddress(LibraryInstance,"DrawDibDraw");
        DrawDibCloseFunction = (DRAWDIBCLOSE_FUNCTION) GetProcAddress(LibraryInstance,"DrawDibClose");

        // check
        if (DrawDibOpenFunction)
        {
            // get drawdib handle
            DrawDibHandle=(*DrawDibOpenFunction)();

            // check handle
            if (DrawDibHandle) Status=1;
            else Status=0;
        }
        else Status=0;
    }
}


inline VFW::~VFW()
{
    // close drawdib handle
    if (DrawDibHandle && DrawDibCloseFunction) (*DrawDibCloseFunction)(DrawDibHandle);
        
    // free library instance    
    if (LibraryInstance) FreeLibrary(LibraryInstance);
}






inline int VFW::DrawDib(HDC dc,RECTANGLE const &src,RECTANGLE const &dest,BITMAPINFOHEADER *info,void *bits,int flags)
{
    // drawdib blitting
    if (DrawDibDrawFunction)
    {
        return (*DrawDibDrawFunction)(DrawDibHandle,dc,
                                      dest.x1,dest.y1,dest.width(),dest.height(),
                                      info,bits,
                                      src.x1,src.y1,src.width(),src.height(),
                                      (UINT)flags);
    }
    else return 0;
}






inline int VFW::ok()
{    
    // vfw status
    return Status;
}








#endif
