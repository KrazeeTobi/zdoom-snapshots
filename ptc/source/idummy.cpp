////////////////////////
// Dummy interface //
////////////////////////
#include "idummy.h"
#include "surface.h"









IDummy::IDummy(WINDOW window)
{
    // advoid warnings
    if (window);
}


IDummy::~IDummy()
{
    // advoid warnings
    if (random(25));
}







Interface::INFO IDummy::GetInfo()
{
    // null info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}


int IDummy::GetModeList(List<MODE> &modelist)
{
    // return empty list
    modelist.free();
    return 0;
}







int IDummy::SetMode(MODE const &mode)
{
    // advoid warnings
    if (&mode);
    return 0;
}


int IDummy::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    // advoid warnings
    if (x || y || id || output || frequency || layout);
    return 0;
}


int IDummy::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // advoid warnings
    if (x || y || format.ok() || output || frequency || layout);
    return 0;
}


MODE IDummy::GetMode()
{
    // null mode
    MODE mode;
    memset(&mode,0,sizeof(mode));
    return mode;
}







int IDummy::SetPalette(Palette &palette)
{
    // advoid warnings
    if (palette.ok());
    return 0;
}


int IDummy::GetPalette(Palette &palette)
{
    // advoid warnings
    if (palette.ok());
    return 0;
}







int IDummy::WaitForRetrace()
{
    return 0;
}







int IDummy::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IDummy::GetPrimary()
{
    return NULL;
}


int IDummy::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IDummy::GetOrigin(int &x,int &y)
{
    // zero origin
    x=0; y=0;
    return 0;
}







int IDummy::GetTotalVideoMemory()
{
    return 0;
}


int IDummy::GetFreeVideoMemory()
{
    return 0;
}


int IDummy::CompactVideoMemory()
{
    return 0;
}







int IDummy::getch()
{
    return ::getch();
}


int IDummy::kbhit()
{
    return ::kbhit();
}







int IDummy::SetTitle(char title[])
{
    // advoid warnings
    if (title[0]);
    return 0;
}


int IDummy::GetTitle(char title[])
{
    // advoid warnings
    if (title[0]);
    return 0;
}







int IDummy::NativeType()
{                         
    return NATIVE_UNAVAILABLE;
}


void* IDummy::GetNative()
{
    return NULL;
}







void IDummy::GetName(char name[]) const
{
    // null name
    name[0]=0;
}


int IDummy::GetXResolution() const
{
    return 0;
}
        

int IDummy::GetYResolution() const
{
    return 0;
}


int IDummy::GetBitsPerPixel() const
{
    return 0;
}


int IDummy::GetBytesPerPixel() const
{
    return 0;
}
        

int IDummy::GetOutput() const
{
    return 0;
}
        

int IDummy::GetFrequency() const
{
    return UNKNOWN;
}


int IDummy::GetLayout() const
{
    return 0;
}
        

FORMAT IDummy::GetFormat() const
{
    FORMAT format;
    return format;
}
        

WINDOW IDummy::GetWindow() const
{
    return NULL;
}







int IDummy::ok() const
{
    return 0;
}







Interface::SURFACE* IDummy::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // advoid warnings
    if (width || height || format.ok() || type || orientation || advance || layout);

    // allocate surface
    SURFACE *surface=new SURFACE;
    return surface;
}







int IDummy::Attach(Surface *surface)
{
    // advoid warnings
    if (surface);
    return 1;
}


int IDummy::Detach(Surface *surface)
{
    // advoid warnings
    if (surface);
    return 1;
}






void* IDummy::SURFACE::Lock(int wait)
{
    return NULL;
}


void IDummy::SURFACE::Unlock()
{
}


int IDummy::SURFACE::LockCount()
{
    return 0;
}


int IDummy::SURFACE::Lockable()
{
    return 0;
}


int IDummy::SURFACE::Restore()
{
    return 0;
}


int IDummy::SURFACE::NativeType()
{
    return NATIVE_UNAVAILABLE;
}


void* IDummy::SURFACE::GetNative()
{
    return NULL;
}


int IDummy::SURFACE::ok()
{
    return 0;
}


int IDummy::SURFACE::Clear(Surface &surface,COLOR const &color)
{
    return 0;
}


int IDummy::SURFACE::Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color)
{
    return 0;
}


int IDummy::SURFACE::BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return 0;
}


int IDummy::SURFACE::BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    return 0;
}


int IDummy::SURFACE::StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return 0;
}


int IDummy::SURFACE::StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    return 0;
}
