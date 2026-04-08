//////////////////////////
// prometheus truecolor //
//////////////////////////

#ifndef __PTC_H
#define __PTC_H

#include "lang.h"
#include "misc.h"
#include "config.h"
#include "globals.h"
#include "classes.h"
#include "iface.h"
#include "idummy.h"







class PTC
{
    public:
        
        // setup
        PTC();
        ~PTC();

        // user constructors
        PTC(MODE const &mode);
        PTC(int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(WINDOW const &window);
        PTC(WINDOW const &window,MODE const &mode);
        PTC(WINDOW const &window,int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(WINDOW const &window,int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        
        // interface name string constructors
        PTC(char name[]);
        PTC(char name[],int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(char name[],int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(char name[],WINDOW const &window);
        PTC(char name[],WINDOW const &window,int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(char name[],WINDOW const &window,int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);

        // command line constructors
        PTC(int argc,char *argv[],int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        PTC(int x,int y,int argc,char *argv[],int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);

        // user initialize
        int Init(MODE const &mode);
        int Init(int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(WINDOW const &window);
        int Init(WINDOW const &window,MODE const &mode);
        int Init(WINDOW const &window,int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(WINDOW const &window,int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);

        // interface name string constructors
        int Init(char name[]);
        int Init(char name[],int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(char name[],int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(char name[],WINDOW const &window);
        int Init(char name[],WINDOW const &window,int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(char name[],WINDOW const &window,int x,int y,FORMAT format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);

        // command line initialize
        int Init(int argc,char *argv[],int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        int Init(int x,int y,int argc,char *argv[],int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);

        // error
        void Error(char error[]);

        // close
        void Close();

        // mode list information
        int GetModeList(List<MODE> &modelist);

        // interface information
        Interface::INFO GetInfo();

        // mode set
        inline int SetMode(MODE const &mode);
        inline int SetMode(int x,int y,int id=VIRTUAL32,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        inline int SetMode(int x,int y,FORMAT const &format,int output=DEFAULT,int frequency=DEFAULT,int layout=DEFAULT);
        inline MODE GetMode();

        // palette control
        inline int SetPalette(Palette &palette);
        inline int GetPalette(Palette &palette);
        
        // hardware functions
        inline void WaitForRetrace();

        // primary surface operations
        inline int SetPrimary(Surface &surface);
        inline Surface* GetPrimary();
        inline int SetOrigin(int x,int y);
        inline int GetOrigin(int &x,int &y);

        // console routines
        inline int getch();
        inline int kbhit();

        // window routines
        inline int SetTitle(char title[]);
        inline int GetTitle(char title[]);

        // data access
        inline void GetName(char name[]) const;
        inline uint GetXResolution() const;
        inline uint GetYResolution() const;
        inline uint GetBitsPerPixel() const;
        inline uint GetBytesPerPixel() const;
        inline int GetOutput() const;
        inline int GetFrequency() const;
        inline int GetLayout() const;
        inline WINDOW GetWindow() const;
        inline FORMAT GetFormat() const;

        // interface access
        inline Interface* GetInterface() const; 

        // object status
        inline int ok() const;

    protected:

        // interface access
        Interface* CreateInterface(int index,WINDOW const &window);

        // setup data defaults
        void Defaults();

        // local interface
        Interface *LocalInterface;

        // dummy interface
        static IDummy DummyInterface;
};







// interdependant classes
#include "surface.h"







// inline wrapper functions
inline Interface::INFO PTC::GetInfo()
{
    return LocalInterface->GetInfo();
}


inline int PTC::SetMode(MODE const &mode)
{
    return LocalInterface->SetMode(mode);
}


inline int PTC::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    return LocalInterface->SetMode(x,y,id,output,frequency,layout);
}


inline int PTC::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    return LocalInterface->SetMode(x,y,format,output,frequency,layout);
}


inline MODE PTC::GetMode()
{
    return LocalInterface->GetMode();
}


inline int PTC::SetPalette(Palette &palette)
{
    return LocalInterface->SetPalette(palette);
}
        

inline int PTC::GetPalette(Palette &palette)
{
    return LocalInterface->GetPalette(palette);
}
        

inline void PTC::WaitForRetrace()
{
    LocalInterface->WaitForRetrace();
}

        
inline int PTC::SetPrimary(Surface &surface)
{
    return LocalInterface->SetPrimary(surface);
}


inline Surface* PTC::GetPrimary()
{
    return LocalInterface->GetPrimary();
}


inline int PTC::SetOrigin(int x,int y)
{
    return LocalInterface->SetOrigin(x,y);
}


inline int PTC::GetOrigin(int &x,int &y)
{
    return LocalInterface->GetOrigin(x,y);
}


inline int PTC::getch()
{
    return LocalInterface->getch();
}


inline int PTC::kbhit()
{
    return LocalInterface->kbhit();
}


inline int PTC::SetTitle(char title[])
{
    return LocalInterface->SetTitle(title);
}


inline int PTC::GetTitle(char title[])
{
    return LocalInterface->GetTitle(title);
}


inline void PTC::GetName(char name[]) const
{
    LocalInterface->GetName(name);
}
        

inline uint PTC::GetXResolution() const
{
    return LocalInterface->GetXResolution();
}
        

inline uint PTC::GetYResolution() const
{
    return LocalInterface->GetYResolution();
}


inline uint PTC::GetBitsPerPixel() const
{
    return LocalInterface->GetBitsPerPixel();
}
        

inline uint PTC::GetBytesPerPixel() const
{
    return LocalInterface->GetBytesPerPixel();
}


inline int PTC::GetOutput() const
{
    return LocalInterface->GetOutput();
}


inline int PTC::GetFrequency() const
{
    return LocalInterface->GetFrequency();
}


inline int PTC::GetLayout() const
{
    return LocalInterface->GetLayout();
}


inline FORMAT PTC::GetFormat() const
{
    return LocalInterface->GetFormat();
}


inline WINDOW PTC::GetWindow() const
{
    return LocalInterface->GetWindow();
}







// get interface object
inline Interface* PTC::GetInterface() const
{                                     
    if (LocalInterface!=(Interface*)&DummyInterface) return LocalInterface;
    else return NULL;
}







// get object status
inline int PTC::ok() const
{
    // return interface
    if (LocalInterface==(Interface*)&DummyInterface) return 0;
    else return LocalInterface->ok();
}











#endif
