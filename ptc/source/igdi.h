///////////////////
// GDI interface //
///////////////////

#ifndef __PTC_IGDI_H
#define __PTC_IGDI_H

#include "iwin32.h"

#ifdef __VFW__
#include "vfw.h"
#endif







#ifdef __GDI__

class IGDI : public IWin32
{
    public:

        // setup
        IGDI(WINDOW window=NULL);
        virtual ~IGDI();

        // interface information
        virtual Interface::INFO GetInfo();

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

        // data access
        virtual void GetName(char name[]) const;
        virtual int GetBitsPerPixel() const;
        virtual int GetBytesPerPixel() const;
        virtual int GetOutput() const;
        virtual int GetFrequency() const;
        virtual int GetLayout() const;
        virtual FORMAT GetFormat() const;

        // object state
        virtual int ok() const;

    protected:
        
        // internal surface management
        virtual Interface::SURFACE* RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);

        // internal surface
        class SURFACE : public ISoftware::SURFACE
        {
            // friend classes
            friend class IGDI;

            public:
            
                // setup
                SURFACE(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
                virtual ~SURFACE();

                // surface memory
                virtual void* Lock(int wait);
                virtual void Unlock();
                virtual int LockCount();
                virtual int Lockable();
                virtual int Restore();

                // native access
                virtual int NativeType();
                virtual void* GetNative();

                // status
                virtual int ok();

            private:

                // data
                void *Buffer;                   // memory buffer
                HBITMAP Bitmap;                 // bitmap handle
                BITMAPINFO *BitmapInfo;         // bitmap information
                int LockOffset;                 // lock offset
                int Count;                      // lock count
        };

        // internal primary surface
        class PRIMARY : public Interface::SURFACE
        {
            public:

                // setup
                PRIMARY(IGDI &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
                virtual ~PRIMARY();

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

            private:

                // local interface
                IGDI *LocalInterface;
        };

        // friend classes
        friend class PRIMARY;

    private:

        // display management
        int InitDisplay(int x,int y,int output,int frequency,int layout);
        void CloseDisplay();

        // display window management
        int CreateDisplayWindow(int width,int height,int output);
        int ShowDisplayWindow(int width,int height,int output);
        int ResizeDisplayWindow(int width,int height,int output);
        void CloseDisplayWindow();

        // primary management
        int InitPrimary();
        void ClosePrimary();

        // secondary management
        int InitSecondary();
        void CloseSecondary();

        // palette management
        int InitPalette();
        void ClosePalette();

        // window internals
        int RegisterWindowClasses();
        void AdjustWindowSize(int &width,int &height,int output);
        static LRESULT CALLBACK WndProcCommon(IGDI *i,HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
        static LRESULT CALLBACK WndProcWindowed(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
        static LRESULT CALLBACK WndProcFullscreen(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

        // internal gdi routines
        int SetDisplayMode(int x,int y,int frequency);
        int SetDisplayMode(DEVMODE devmode);
        void RestoreDisplayMode();
        MODE GetDisplayMode();
        DEVMODE GetDisplayDeviceMode();
        int GetDisplayFrequency();
        FORMAT GetDisplayFormat();
        FORMAT GetDeviceFormat(HDC dc);

        // interface lookup
        int RegisterWindow(HWND window);
        int UnregisterWindow(HWND window);
        static IGDI* LookupInterface(HWND window);

        // update to display
        int UpdateDisplay(Surface &src,RECTANGLE const &src_rect,RECTANGLE const &dest_rect);

        // internals
        FORMAT Format;                      // pixel format of display
        int Output;                         // output mode
        int Frequency;                      // output frequency
        int PrimaryFlag;                    // flag indicating primary creation
        Surface *Primary;                   // primary surface
        Surface *Secondary;                 // secondary surface
        Palette InternalPalette;            // internal palette object
        LOGPALETTE *LogicalPalette;         // logical palette
        HPALETTE PaletteHandle;             // palette handle
        DEVMODE OriginalDeviceMode;         // original device mode
        DEVMODE FullscreenDeviceMode;       // fullscreen device mode

        // video for windows
        #ifdef __VFW__
        VFW *VideoForWindows;
        #endif

        // static interface lookup list
        struct LOOKUP
        {
            HWND window;
            IGDI *i;
        };
        static List<LOOKUP> InterfaceLookupList;

        // status
        int Status;
};

#else

class IGDI : public IDummy
{
    public:

        // dummy object
        IGDI(WINDOW window=NULL) { if (window); };
};

#endif








#endif
