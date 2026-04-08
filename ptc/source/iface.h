//////////////////////
// device interface //
//////////////////////

#ifndef __PTC_INTERFACE_H
#define __PTC_INTERFACE_H

#include "lang.h"
#include "misc.h"
#include "list.h"
#include "rect.h"
#include "color.h"
#include "config.h"
#include "format.h"
#include "native.h"
#include "raster.h"
#include "effects.h"
#include "globals.h"
#include "palette.h"
#include "classes.h"








// mode info (change to Interface::MODE ??)
struct MODE
{
    char i[8];                     // interface name
    int x;                         // x resolution
    int y;                         // y resolution
    FORMAT format;                 // pixel format
    int output;                    // output method
    int frequency;                 // video refresh frequency
    int layout;                    // memory layout
};





class Interface
{
    // friend classes
    friend class Surface;
    friend class IDummy;
    friend class ISoftware;
    friend class IVGA;
    friend class IVESA;
    friend class IGDI;
    friend class IDirectX;

    public:

        // interface info
        struct INFO
        {  
            // name
            char name[8];                 // interface name
    
            // surfaces
            int system     : 1;           // interface allows system surfaces
            int video      : 1;           // interface allows video surfaces
            int primary    : 1;           // interface allows primary surface access
    
            // layouts
            int linear     : 1;           // interface is capable of setting modes with linear layout
            int banked     : 1;           // interface is capable of setting modes with banked layout
            int planar     : 1;           // interface is capable of setting modes with planar layout
            int fakemode   : 1;           // interface is capable of setting modes with fakemode layout

            // output
            int fullscreen : 1;           // interface is capable of fullscreen output
            int windowed   : 1;           // interface is capable of windowed output

            // mode setting
            int frequency  : 1;           // interface is able to work with mode refresh frequency
    
            // acceleration
            int clear      : 1;           // interface is capable of hardware accelerated clears
            int bitblt     : 1;           // interface is capable of hardware accelerated bitblt
            int stretchblt : 1;           // interface is capable of hardware accelerated stretchblt
        };  

        //  virtual destructor
        virtual ~Interface() {};

        // interface information
        virtual INFO GetInfo()                        = 0;
        virtual int GetModeList(List<MODE> &modelist) = 0;

        // display mode routines
        virtual int SetMode(MODE const &info)                                                       = 0;
        virtual int SetMode(int x,int y,int id,int output,int frequency,int layout)                 = 0;
        virtual int SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)   = 0;
        virtual MODE GetMode()                                                                      = 0;

        // palette routines
        virtual int SetPalette(Palette &palette) = 0;
        virtual int GetPalette(Palette &palette) = 0;
        
        // hardware functions
        virtual int WaitForRetrace() = 0;
        
        // primary surface operations
        virtual int SetPrimary(Surface &surface) = 0;
        virtual Surface* GetPrimary()            = 0;
        virtual int SetOrigin(int x,int y)       = 0;
        virtual int GetOrigin(int &x,int &y)     = 0;

        // video memory management
        virtual int GetTotalVideoMemory() = 0;
        virtual int GetFreeVideoMemory()  = 0;
        virtual int CompactVideoMemory()  = 0;

        // console routines
        virtual int getch() = 0;
        virtual int kbhit() = 0;

        // window routines
        virtual int SetTitle(char title[]) = 0;
        virtual int GetTitle(char title[]) = 0;

        // native access
        virtual int NativeType()  = 0;
        virtual void* GetNative() = 0;

        // data access
        virtual void GetName(char name[]) const = 0;
        virtual int GetXResolution() const      = 0;
        virtual int GetYResolution() const      = 0;           // add "GetOrientation" ?
        virtual int GetBitsPerPixel() const     = 0;
        virtual int GetBytesPerPixel() const    = 0;
        virtual int GetOutput() const           = 0;
        virtual int GetFrequency() const        = 0;
        virtual int GetLayout() const           = 0;
        virtual FORMAT GetFormat() const        = 0;
        virtual WINDOW GetWindow() const       = 0;

        // object state 
        virtual int ok() const = 0;       

    public:

        // internal surface
        class SURFACE
        {
            public:
            
                // virtual destructor
                virtual ~SURFACE() {};

                // surface memory
                virtual void* Lock(int wait) = 0;
                virtual void Unlock()        = 0;
                virtual int LockCount()      = 0;
                virtual int Lockable()       = 0;
                virtual int Restore()        = 0;

                // surface clear
                virtual int Clear(Surface &surface,COLOR const &color)                       = 0;
                virtual int Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color) = 0;

                // surface bitblt
                virtual int BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra) = 0;
                virtual int BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra) = 0;

                // surface stretchblt
                virtual int StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra) = 0;
                virtual int StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra) = 0;
                
                // native access
                virtual int NativeType()  = 0;
                virtual void* GetNative() = 0;

                // status
                virtual int ok() = 0;
        };        

    protected:

        // internal surface management
        virtual SURFACE* RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout) = 0;

        // surface attach and detach
        virtual int Attach(Surface *surface) = 0;
        virtual int Detach(Surface *surface) = 0;
};











#endif
