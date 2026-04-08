///////////////////////
// DirectX interface //
///////////////////////

#ifndef __PTC_IDIRECTX_H
#define __PTC_IDIRECTX_H

#include "iwin32.h"

#ifdef __DIRECTX__
#include "ddraw.h"
#endif







#ifdef __DIRECTX__

class IDirectX : public IWin32
{
    public:

        // setup
        IDirectX(WINDOW window=NULL);
        virtual ~IDirectX();

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

        // native access
        virtual int NativeType();
        virtual void* GetNative();

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
            friend class IDirectX;

            public:
            
                // setup
                SURFACE(IDirectX &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
                virtual ~SURFACE();

                // surface memory
                virtual void* Lock(int wait);
                virtual void Unlock();
                virtual int LockCount();
                virtual int Lockable();
                virtual int Restore();

                /*
                // surface clear
                virtual int Clear(Surface &surface,COLOR const &color);
                virtual int Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color);

                // surface bitblt
                virtual int BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra);
                virtual int BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra);

                // surface stretchblt
                virtual int StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra);
                virtual int StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra);
                */

                // native access
                virtual int NativeType();
                virtual void* GetNative();

                // status
                virtual int ok();

            private:

                // data
                void *Buffer;                   // surface buffer
                LPDIRECTDRAWSURFACE lpDDS;      // directdraw surface
                LPDIRECTDRAWSURFACE2 lpDDS2;    // directdraw surface 2
                int Count;                      // surface lock count
                int Primary;                    // primary surface flag
                IDirectX *LocalInterface;       // local interface
        };

        // friend classes
        friend class SURFACE;

    private:

        // display management
        int InitDisplay(int x,int y,int id,int output,int frequency,int layout);
        int InitDisplay(int x,int y,FORMAT const &format,int output,int frequency,int layout);
        void CloseDisplay();

        // display mode management
        int EnterDisplayMode(int x,int y,int id,int frequency);
        int EnterDisplayMode(int x,int y,FORMAT const &format,int frequency);
        int SetDisplayMode(int x,int y,int bits,int frequency);
        int SetDisplayMode(int x,int y,FORMAT const &format,int frequency);
        int CheckDisplayMode(int x,int y,int id,int frequency);
        int CheckDisplayMode(int x,int y,FORMAT const &format,int frequency);
        void LeaveDisplayMode();
        
        // exclusive mode management
        int EnterExclusiveMode();
        void LeaveExclusiveMode();

        // directdraw management
        int InitDirectDraw();
        void CloseDirectDraw();

        // display window management
        int CreateDisplayWindow(int width,int height,int output);
        int ResizeDisplayWindow(int width,int height,int output);
        void CloseDisplayWindow();

        // primary management
        int InitPrimary();
        void ClosePrimary();

        // modelist routines
        int SetupModeList();
        static HRESULT CALLBACK EnumDisplayModesCallback(LPDDSURFACEDESC desc,IDirectX *owner);
        
        // translation routines
        int Translate(DDPIXELFORMAT const &ddpf,FORMAT &format);
        int Translate(FORMAT const &format,DDPIXELFORMAT &ddpf);

        // display internals
        int InitDisplayStart(int &x,int &y,int &output,int &frequency,int &layout);
        int InitDisplayFinish(int &x,int &y,FORMAT const &format,int &output,int &frequency,int &layout);
        FORMAT GetDisplayFormat();

        // window to interface lookup
        int RegisterWindow(HWND window);
        int UnregisterWindow(HWND window);
        static IDirectX* LookupInterface(HWND window);

        // internal window management
        int RegisterWindowClasses();
        void AdjustWindowSize(int &width,int &height,int output);
        static LRESULT CALLBACK WndProcCommon(IDirectX *i,HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
        static LRESULT CALLBACK WndProcWindowed(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
        static LRESULT CALLBACK WndProcFullscreen(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

        // debug routines
        void PrintGUID(GUID const &guid);

        // internals
        FORMAT Format;                      // pixel format of display
        int Output;                         // output mode
        int Frequency;                      // output frequency
        int PrimaryFlag;                    // flag indicating primary creation
        Surface *Primary;                   // primary surface
        Surface *PrimaryBackup;             // primary backup surface
        Palette PrimaryPalette;             // primary surface palette
        List<MODE> ModeList;                // directx modelist

        // directx data
        LPDIRECTDRAW lpDD;                  // IDirectDraw object
        LPDIRECTDRAW2 lpDD2;                // IDirectDraw2 object
        HMODULE DirectDrawInstance;         // "ddraw.dll" instance

        // static interface lookup list
        struct LOOKUP
        {
            HWND window;
            IDirectX *i;
        };
        static List<LOOKUP> InterfaceLookupList;

        // directdraw function types
        #ifndef __CYGNUS__
        typedef HRESULT (WINAPI *DIRECTDRAWCREATE_FUNCTION) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
        #else
        typedef HRESULT WINAPI (*DIRECTDRAWCREATE_FUNCTION) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
        #endif

        // window control data
        int ForceActiveWindow;

        // status
        int Status;
};

#else

class IDirectX : public IDummy
{
    public:

        // dummy object
        IDirectX(WINDOW window=NULL) { if (window); };
};

#endif








#endif
