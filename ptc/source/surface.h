///////////////////
// Surface class //
///////////////////

#ifndef __PTC_SURFACE_H
#define __PTC_SURFACE_H

#include "lang.h"
#include "misc.h"
#include "rect.h"
#include "color.h"
#include "clear.h"
#include "effects.h"
#include "palette.h"
#include "clipper.h"
#include "globals.h"
#include "classes.h"
#include "list.h"
#include "ptc.h"
#include "iface.h"
#include "idummy.h"
#include "image.h"

#if !defined(__GNU__) && !defined(__BORLANDC__)
#include "isoft.h"
#endif







class Surface
{
    // friend classes
    friend class ISoftware;
    friend class IVGA;
    friend class IVESA;
    friend class IGDI;
    friend class IDirectX;
    friend class ISVGALIB;

    // friend functions
    friend int Clip(Surface const &src,Surface const &dest,RECTANGLE &rect);
    friend int Clip(Surface const &src,RECTANGLE &src_rect,Surface const &dest,RECTANGLE &dest_rect);

    public:

        // setup
        Surface();
        ~Surface();
        
        // unbound constructors
        Surface(int x,int y,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        Surface(void *memory,int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        Surface(char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
 
        // bound constructors
        Surface(Interface *i,int type=SYSTEM);                          
        Surface(Interface *i,int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        Surface(Interface *i,char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        
        // file operations
        int Load(char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Load(Interface *i,char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Save(char filename,char *options=NULL);
        int Save(char filename,FORMAT const &format,char *options=NULL);       // add orientation?

        // interface binding
        int Bind(Interface *i);
        int Unbind();

        // surface memory
        void* Lock(int wait=1);
        void Unlock();
        int LockCount() const;
        int Lockable() const;
        int Restore();

        // surface clears
        int Clear(COLOR const &color);
        int Clear(RECTANGLE const &rect,COLOR const &color);

        // update to bound display
        int Update();
        int Update(RECTANGLE const &rect);
        int Update(RECTANGLE const &src,RECTANGLE const &dest);

        // update to a specific display
        int Update(Interface *i);
        int Update(Interface *i,RECTANGLE const &rect);
        int Update(Interface *i,RECTANGLE const &src,RECTANGLE const &dest);

        // bitblt to another surface
        int BitBlt(Surface &dest,EFFECTS const *effects=NULL);
        int BitBlt(Surface &dest,int x,int y,EFFECTS const *effects=NULL);
        int BitBlt(Surface &dest,RECTANGLE const &src_rect,RECTANGLE const &dest_rect,EFFECTS const *effects=NULL);

        // stretchblt to another surface
        int StretchBlt(Surface &dest,EFFECTS const *effects=NULL);
        int StretchBlt(Surface &dest,RECTANGLE const &src_rect,RECTANGLE const &dest_rect,EFFECTS const *effects=NULL);

        // general routines
        int Relocate(int type,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Orientate(int orientation=DEFAULT);
        int Convert(FORMAT const &format);
        int Resize(int width,int height);
        int Resample(int width,int height);
        int Crop(RECTANGLE const &rect);

        // surface palette
        int SetPalette(Palette const &palette);
        Palette* GetPalette();
        int AttachPalette(Palette *palette);
        void DetachPalette();
        void DefaultPalette();

        // surface clipper
        int SetClipper(Clipper const &clipper);
        Clipper* GetClipper();
        int AttachClipper(Clipper *clipper);
        void DetachClipper();
        void DefaultClipper();

        // primary surface operations
        int SetPrimary();
        int IsPrimary() const;
        int SetOrigin(int x,int y);
        int GetOrigin(int &x,int &y) const;

        // native access
        int NativeType();
        void* GetNative();

        // surface data access
        int GetWidth() const;
        int GetHeight() const;
        int GetType() const;
        int GetOrientation() const;
        int GetLayout() const;
        int GetAdvance() const;
        int GetPitch() const;
        int GetArea() const;
        int GetBitsPerPixel() const;
        int GetBytesPerPixel() const;
        FORMAT GetFormat() const;
        RECTANGLE GetDimensions() const;
        Interface* GetInterface() const;

        // surface status
        int ok() const;

        // operators
        void operator =(Surface const &other);

        // ptc -> interface helper functions
        Surface(PTC const &ptc,int type=SYSTEM);
        Surface(PTC const &ptc,int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        Surface(PTC const &ptc,char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Init(PTC const &ptc,int type=SYSTEM);
        int Init(PTC const &ptc,int height,int width,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Load(PTC const &ptc,char filename[],int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Bind(PTC const &ptc);
        int Update(PTC const &ptc);
        int Update(PTC const &ptc,RECTANGLE const &src);
        int Update(PTC const &ptc,RECTANGLE const &src,RECTANGLE const &dest);

    protected:

        // internal surface management
        int Init(int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Init(void *memory,int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        int Init(Interface *i,int type=SYSTEM);
        int Init(Interface *i,int width,int height,FORMAT const &format,int type=SYSTEM,int orientation=DEFAULT,int advance=DEFAULT,int layout=DEFAULT);
        void Free();
        void Invalidate();

        // data defaults
        void Defaults();

        // data
        int Width;                     // width of surface
        int Height;                    // height of surface
        int Type;                      // type of surface
        int Layout;                    // memory layout
        int Orientation;               // surface orientation
        int Advance;                   // advance bytes per line
        int Pitch;                     // total bytes per line
        int Primary;                   // primary flag
        FORMAT Format;                 // pixel format

        // local clipper object
        Clipper *LocalClipper;
        
        // local palette object
        Palette *LocalPalette;

        // local interface object
        Interface *LocalInterface;
       
        // internal surface object
        Interface::SURFACE *InternalSurface;

        // dummy objects
        static Palette DummyPalette;
        static IDummy DummyInterface;
        static IDummy::SURFACE DummySurface;

        // software interface
        #ifndef __DJGPP__
        static ISoftware SoftwareInterface;
        #endif
        void InitSoftware();
        void CloseSoftware();
};






// friend functions
int Clip(Surface const &src,Surface const &dest,RECTANGLE &rect);
int Clip(Surface const &src,RECTANGLE &src_rect,Surface const &dest,RECTANGLE &dest_rect);


// software interface
#if defined(__GNU__) || defined(__BORLANDC__)
#include "isoft.h"
#endif


// disgusting DJGPP work around
#ifdef __DJGPP__
extern ISoftware *DJGPP_SUCKS_SoftwareInterface;
extern int DJGPP_SUCKS_SoftwareInterfaceCount;
#endif







#endif
