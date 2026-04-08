/////////////////////
// Win32 interface //
/////////////////////

#ifndef __PTC_IWIN32_H
#define __PTC_IWIN32_H

#include "idummy.h"
#include "isoft.h"
#include "surface.h"
#include "keyboard.h"

#ifdef __WIN32__
#include "thread.h"
#endif







#ifdef __WIN32__

class IWin32 : public ISoftware
{
    public:

        // setup
        IWin32(WINDOW window=NULL);
        virtual ~IWin32();

        // interface information
        virtual int GetModeList(List<MODE> &modelist);

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
        virtual int GetXResolution() const;
        virtual int GetYResolution() const;
        virtual WINDOW GetWindow() const;

        // object state
        virtual int ok() const;

    protected:

        // window management
        int CreateManagedWindow(char wndclass[],uint style,int show,int x,int y,int width,int height);
        int ResizeManagedWindow(int width,int height);
        void CloseManagedWindow();
        
        // managed window data access
        HWND GetManagedWindow();
        HDC GetManagedWindowDC();
        int ManagedWindow();

        // static window procedure
        static LRESULT CALLBACK WndProc(IWin32 *i,HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

        // recent surface management
        Surface* GetRecent();
        void SetRecent(Surface &surface,RECTANGLE const &src,RECTANGLE const &dest);
        void ClearRecent();

        // modelist management
        void AddMode(MODE const &mode);
        void ClearModeList();
        List<MODE>* GetModeList();

        // local critical section interface
        void EnterInputCriticalSection();
        void EnterUpdateCriticalSection();
        void EnterWindowCriticalSection();
        void EnterPaletteCriticalSection();
        void LeaveInputCriticalSection();
        void LeaveUpdateCriticalSection();
        void LeaveWindowCriticalSection();
        void LeavePaletteCriticalSection();

        // static critical section interface
        static void EnterStaticCriticalSection();
        static void LeaveStaticCriticalSection();

        // surface detach
        virtual int Detach(Surface *surface);

        // data
        int XResolution;
        int YResolution;

    private:

        // static criticial section manangement
        void InitStaticCriticalSection();
        void CloseStaticCriticalSection();

        // local critical section management
        void InitLocalCriticalSections();
        void CloseLocalCriticalSections();

        // window thread parm struct
        struct WINDOWTHREADINFO
        {
            IWin32 *i;          // interface
            char *wndclass;     // window class
            char *title;        // window title
            uint style;         // window style
            int show;           // window show command
            int x;              // window x
            int y;              // window y
            int width;          // window width
            int height;         // window height
            HANDLE event;       // window event
        };

        // static window thread function
        static void WindowThreadFunction(WINDOWTHREADINFO *info);

        // window internals
        HWND Window;                        // output window handle
        HDC WindowDC;                       // output window device context
        int Managed;                        // managed window flag
        Keyboard WindowKeyboard;            // window keyboard buffer
        HANDLE WindowKeyboardEvent;         // window signal event
        Thread *WindowThread;               // window thread
        char *WindowTitle;                  // window title
        int WindowShowCommand;              // window show command

        // static critical section data
        static CRITICAL_SECTION StaticCriticalSection;
        static int StaticCriticalSectionReferenceCount;

        // local critical sections
        CRITICAL_SECTION InputCriticalSection;
        CRITICAL_SECTION UpdateCriticalSection;
        CRITICAL_SECTION WindowCriticalSection;         
        CRITICAL_SECTION PaletteCriticalSection;         

        // recent surface
        Surface *Recent;                    // most recently updated surface
        RECTANGLE RecentSrcRect;            // recent update source rectangle
        RECTANGLE RecentDestRect;           // recent update destination rectangle

        // modelist data
        List<MODE> ModeList;

        // status
        int Status;
};

#endif








#endif
