/////////////////////
// Dummy interface //
/////////////////////

#ifndef __PTC_IDUMMY_H
#define __PTC_IDUMMY_H

#include "iface.h"







class IDummy : public Interface
{
    // friend classes
    friend class Surface;

    public:

        // setup
        IDummy(WINDOW window=NULL);
        virtual ~IDummy();

        // interface information
        virtual Interface::INFO GetInfo();
        virtual int GetModeList(List<MODE> &modelist);

        // display mode routines
        virtual int SetMode(MODE const &info);
        virtual int SetMode(int x,int y,int id,int output,int frequency,int layout);
        virtual int SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout);
        virtual MODE GetMode();

        // palette routines
        virtual int SetPalette(Palette &palette);
        virtual int GetPalette(Palette &palette);
        
        // hardware functions
        virtual int WaitForRetrace();
        
        // primary surface operations
        virtual int SetPrimary(Surface &surface);
        virtual Surface* GetPrimary();
        virtual int SetOrigin(int x,int y);
        virtual int GetOrigin(int &x,int &y);

        // video memory management
        virtual int GetTotalVideoMemory();
        virtual int GetFreeVideoMemory();
        virtual int CompactVideoMemory();

        // console routines
        virtual int getch();
        virtual int kbhit();

        // window routines
        virtual int SetTitle(char title[]);
        virtual int GetTitle(char title[]);

        // native access
        virtual int NativeType();
        virtual void* GetNative();

        // data access
        virtual void GetName(char name[]) const;
        virtual int GetXResolution() const;
        virtual int GetYResolution() const;
        virtual int GetBitsPerPixel() const;
        virtual int GetBytesPerPixel() const;
        virtual int GetOutput() const;
        virtual int GetFrequency() const;
        virtual int GetLayout() const;
        virtual FORMAT GetFormat() const;
        virtual WINDOW GetWindow() const;

        // object state 
        virtual int ok() const;       

    protected:

        // internal surface management
        virtual Interface::SURFACE* RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);

        // surface attach and detach
        virtual int Attach(Surface *surface);
        virtual int Detach(Surface *surface);

        // internal surface
        class SURFACE : public Interface::SURFACE
        {
            public:
            
                // surface memory
                virtual void* Lock(int wait);
                virtual void Unlock();
                virtual int LockCount();
                virtual int Lockable();
                virtual int Restore();

                // surface clear
                virtual int Clear(Surface &surface,COLOR const &color);
                virtual int Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color);

                // surface bitblt
                virtual int BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra);
                virtual int BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra);

                // surface stretchblt
                virtual int StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra);
                virtual int StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra);
                
                // native access
                virtual int NativeType();
                virtual void* GetNative();

                // status
                virtual int ok();
        };

};









#endif
