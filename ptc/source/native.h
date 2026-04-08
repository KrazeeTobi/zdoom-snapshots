///////////////////////////////
// native access definitions //
///////////////////////////////

#ifndef __PTC_NATIVE_H
#define __PTC_NATIVE_H





// no native access
const NATIVE_UNAVAILABLE                   = 0;          // native access is not available

// native surface types                                                             
const NATIVE_WIN32_HBITMAP                 = 1;          // microsoft win32 bitmap handle
const NATIVE_WIN32_LPDIRECTDRAWSURFACE     = 2;          // microsoft win32 directdraw surface

// native interface types
const NATIVE_WIN32_HDC                     = 100;        // microsoft win32 device context
const NATIVE_WIN32_LPDIRECTDRAW            = 101;        // microsoft win32 directdraw object
const NATIVE_X11_Window                    = 102;        // X11 window structure
                                           





#endif
