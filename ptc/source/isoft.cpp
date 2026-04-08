////////////////////////
// Software interface //
////////////////////////
#include "isoft.h"
#include "surface.h"
#include <stdlib.h>










ISoftware::ISoftware(WINDOW window)
{
    // advoid warnings
    if (window);
}


ISoftware::~ISoftware()
{
    // free bound surfaces
    FreeSurfaces();

    // invalidate bound surfaces
    InvalidateSurfaces();
}







Interface::INFO ISoftware::GetInfo()
{
    // null info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}


int ISoftware::GetModeList(List<MODE> &modelist)
{
    // return empty list
    modelist.free();
    return 0;
}







int ISoftware::SetMode(MODE const &mode)
{
    // advoid warnings
    if (&mode);
    return 0;
}


int ISoftware::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    // advoid warnings
    if (x || y || id || output || frequency || layout);
    return 0;
}


int ISoftware::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // advoid warnings
    if (x || y || format.ok() || output || frequency || layout);
    return 0;
}


MODE ISoftware::GetMode()
{
    // null mode
    MODE mode;
    memset(&mode,0,sizeof(mode));
    return mode;
}







int ISoftware::SetPalette(Palette &palette)
{
    // advoid warnings
    if (palette.ok());
    return 0;
}


int ISoftware::GetPalette(Palette &palette)
{
    // advoid warnings
    if (palette.ok());
    return 0;
}







int ISoftware::WaitForRetrace()
{
    return 0;
}







int ISoftware::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* ISoftware::GetPrimary()
{
    return NULL;
}


int ISoftware::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int ISoftware::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int ISoftware::GetTotalVideoMemory()
{
    return 0;
}


int ISoftware::GetFreeVideoMemory()
{
    return 0;
}


int ISoftware::CompactVideoMemory()
{
    return 0;
}







int ISoftware::getch()
{
    return ::getch();
}


int ISoftware::kbhit()
{
    return ::kbhit();
}







int ISoftware::SetTitle(char title[])
{
    // advoid warnings
    if (title[0]);
    return 0;
}


int ISoftware::GetTitle(char title[])
{
    // advoid warnings
    if (title[0]);
    return 0;
}







int ISoftware::NativeType()
{                         
    return NATIVE_UNAVAILABLE;
}


void* ISoftware::GetNative()
{
    return NULL;
}









void ISoftware::GetName(char name[]) const
{
    // null name
    name[0]=0;
}


int ISoftware::GetXResolution() const
{
    return 0;
}
        

int ISoftware::GetYResolution() const
{
    return 0;
}


int ISoftware::GetBitsPerPixel() const
{
    return 0;
}


int ISoftware::GetBytesPerPixel() const
{
    return 0;
}
        

int ISoftware::GetOutput() const
{
    return 0;
}
        

int ISoftware::GetFrequency() const
{
    return UNKNOWN;
}


int ISoftware::GetLayout() const
{
    return 0;
}
        

FORMAT ISoftware::GetFormat() const
{
    return FORMAT();
}
        

WINDOW ISoftware::GetWindow() const
{
    return NULL;
}







int ISoftware::ok() const
{
    return 1;
}

        





void ISoftware::FreeSurfaces()
{
    // free all bound surfaces
    List<Surface>::Iterator i=SurfaceList.first();
    Surface *current=i.current();
    while (current)
    {
        current->Free();
        current=i.next();
    }
}


void ISoftware::InvalidateSurfaces()
{
    // invalidate all bound surfaces
    List<Surface>::Iterator i=SurfaceList.first();
    Surface *current=i.current();
    while (current)
    {
        current->Invalidate();         // will detach from interface
        i=SurfaceList.first();
        current=i.current();
    }
}


Interface::SURFACE* ISoftware::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // allocate surface
    SURFACE *surface=new SURFACE(width,height,format,type,orientation,advance,layout);
    if (surface && surface->ok()) return surface;
    else 
    {
        // failure
        delete surface;
        return NULL;
    }
}







int ISoftware::Attach(Surface *surface)
{
    // add to surface list
    return SurfaceList.add(surface);
}


int ISoftware::Detach(Surface *surface)
{
    // remove from surface list
    return SurfaceList.remove(surface);
}







// static raster object
RASTER ISoftware::SURFACE::Raster;


ISoftware::SURFACE::SURFACE()
{
    // defaults
    Buffer=NULL;
    LockAddress=NULL;
    Count=0;
}


ISoftware::SURFACE::SURFACE(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // defaults
    Buffer=NULL;
    LockAddress=NULL;
    Count=0;

    // check parameters
    if ((width<=0 || height<=0) || !format.ok() || (type!=SYSTEM && type!=OFFSCREEN && type!=DEFAULT) ||
        (orientation!=TOPDOWN && orientation!=BOTTOMUP && orientation!=DEFAULT) || 
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=DEFAULT)) return;

    // setup new orientation
    int new_orientation=orientation;
    if (new_orientation==DEFAULT) new_orientation=TOPDOWN;

    // setup new advance
    int new_advance=advance;
    if (new_advance==DEFAULT) new_advance=0;

    // calculate size of surface buffer required
    uint size = width*height*format.bytes + height*new_advance;
    if (!size) return;

    // allocate surface buffer
    Buffer=(void*)malloc(size);
    if (!Buffer) return;

    // clear surface buffer
    memset(Buffer,0,size);

    // setup data
    type=SYSTEM;
    orientation=new_orientation;
    advance=new_advance;
    layout=LINEAR;

    // setup lock address
    if (orientation==TOPDOWN) LockAddress=Buffer;
    else LockAddress = ((char*)Buffer) + (width*format.bytes + advance) * (height-1);

    // zero lock count
    Count=0;
}


ISoftware::SURFACE::~SURFACE()
{
    // free buffer
    free(Buffer);
}


void* ISoftware::SURFACE::Lock(int wait)
{
    // advoid warnings
    if (wait);

    // increment lock count
    Count++;

    // return lock address
    return LockAddress;
}


void ISoftware::SURFACE::Unlock()
{
    // decrement lock count
    if (Count) Count--;
}


int ISoftware::SURFACE::LockCount()
{
    // return lock count
    return Count;
}


int ISoftware::SURFACE::Lockable()
{
    // surface is lockable
    return 1;
}


int ISoftware::SURFACE::Restore()
{
    return 0;
}


int ISoftware::SURFACE::Clear(Surface &surface,COLOR const &color)
{
    return 0;
}


int ISoftware::SURFACE::Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color)
{
    return 0;
}


int ISoftware::SURFACE::BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    // no effects yet
    if (effects) return 0;

    // request converter
    RASTER::CONVERTER *converter=Raster.RequestConverter(src.GetFormat(),dest.GetFormat());
    if (!converter) return 0;

    // lock surfaces
    void *src_buffer=src.Lock();
    void *dest_buffer=dest.Lock();
    if (src_buffer && dest_buffer && src_buffer!=dest_buffer) 
    {
        // adjust for fast BOTTOMUP->BOTTOMUP
        if (src.GetOrientation()==BOTTOMUP)
        {
            src_buffer = ((char*)src_buffer) + src.GetPitch()*(src.GetHeight()-1);
            dest_buffer = ((char*)dest_buffer) + dest.GetPitch()*(dest.GetHeight()-1);
        }
        
        // perform conversion bitblt
        int result=converter->Convert(src_buffer,dest_buffer,dest.GetArea(),extra);
    
        // unlock
        src.Unlock();
        dest.Unlock();

        // cleanup
        delete converter;
        return result;
    }
    else
    {
        // failure
        if (src_buffer) src.Unlock();
        if (dest_buffer) dest.Unlock();
        delete converter;
        return 0;
    }
}


int ISoftware::SURFACE::BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    // no effects yet
    if (effects) return 0;
    
    // get width and height
    int width=src_rect.width();
    int height=src_rect.height();

    // get surface data
    int src_pitch=src.GetPitch();
    int dest_pitch=dest.GetPitch();
    FORMAT src_format=src.GetFormat();
    FORMAT dest_format=dest.GetFormat();

    // request converter
    RASTER::CONVERTER *converter=Raster.RequestConverter(src_format,dest_format);
    if (!converter) return 0;

    // lock surfaces
    void *src_buffer=src.Lock();
    void *dest_buffer=dest.Lock();
    if (src_buffer && dest_buffer && src_buffer!=dest_buffer)
    {
        // initialize area data
        RASTER::CONVERTER::AREA data;
        data.src_x=(int)src_rect.x1;
        data.src_y=(int)src_rect.y1;
        data.src_base=src_buffer;
        data.src_start=((char*)src_buffer) + (int)src_rect.y1*src_pitch + (int)src_rect.x1*src_format.bytes;
        data.src_adjust=src_pitch - width*src_format.bytes;
        data.src_pitch=src_pitch;
        data.dest_x=(int)dest_rect.x1;
        data.dest_y=(int)dest_rect.y1;
        data.dest_base=dest_buffer;
        data.dest_start=((char*)dest_buffer) + (int)dest_rect.y1*dest_pitch + (int)dest_rect.x1*dest_format.bytes;
        data.dest_adjust=dest_pitch - width*dest_format.bytes;
        data.dest_pitch=dest_pitch;
        data.width=width;
        data.height=height;

        // perform conversion bitblt
        int result=converter->Convert(data,extra); 

        // unlock
        src.Unlock();
        dest.Unlock();

        // cleanup
        delete converter;
        return result;
    }        
    else
    {
        // failure
        if (src_buffer) src.Unlock();
        if (dest_buffer) dest.Unlock();
        delete converter;
        return 0;
    }
}


int ISoftware::SURFACE::StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return 0;
}


int ISoftware::SURFACE::StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    return 0;
}

                
int ISoftware::SURFACE::NativeType()
{
    // no native access
    return NATIVE_UNAVAILABLE;
}


void* ISoftware::SURFACE::GetNative()
{
    // no native access
    return NULL;
}


int ISoftware::SURFACE::ok()
{
    // status
    if (Buffer && LockAddress) return 1;
    else return 0;
}

