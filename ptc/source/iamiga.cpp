/////////////////////
// Amiga interface //
/////////////////////
#include "iamiga.h"
#include "surface.h"

#ifdef __AMIGA__







struct RTGMasterBase *RTGMasterBase;

IAmiga::IAmiga(WINDOW window)
{
    // advoid warnings
    if (window);

    // defaults
    XResolution=0;
    YResolution=0;
    Layout=0;
    PrimarySurface=NULL;
    Status=1;
    LFB=NULL;
    LFBSize=0;
    RTGMasterBase = NULL;
    ScreenReq = NULL;
    RtgScreen = NULL;
}


IAmiga::~IAmiga()
{
    // KLUDGE close down system here

    if (RTGMasterBase)
    {
      if (RtgScreen)
      {
        UnlockRtgScreen(RtgScreen);
        CloseRtgScreen(RtgScreen);
      }

      if (ScreenReq)
      {
        FreeRtgScreenModeReq(ScreenReq);
      }

      CloseLibrary((struct Library *) RTGMasterBase);
    }

    // close primary surface
    ClosePrimary();

    // close video memory
    CloseVideoMemory();
}







Interface::INFO IAmiga::GetInfo()
{
    // return info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}


int IAmiga::GetModeList(List<MODE> &modelist)
{
    // clear list
    modelist.free();

    // KLUDGE add video modes to list

    return 1;
}





    

int IAmiga::SetMode(MODE const &mode)
{
    // check mode interface name
    char name[8];
    GetName(name);
    if (stricmp(mode.i,name)!=0) return 0;

    // interface name ok - set mode
    return SetMode(mode.x,mode.y,mode.format,mode.output,mode.frequency,mode.layout);
}


int IAmiga::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    return SetLinearMode(x,y,id);
}


int IAmiga::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    return SetLinearMode(x,y,format.id);
}


MODE IAmiga::GetMode()
{
    // get mode info
    MODE mode;
    memset(&mode,0,sizeof(mode));
    GetName(mode.i);
    mode.x=XResolution;
    mode.y=YResolution;
    mode.format=Format;
    mode.output=FULLSCREEN;
    mode.frequency=UNKNOWN;
    mode.layout=Layout;
    return mode;
}







int IAmiga::SetPalette(Palette &palette)
{
    UBYTE Colour;
    ULONG Colours[5], RGB;
    UWORD PaletteEntry;

    // only set palette when indexed format
    if (Format.type!=INDEXED) return 0;

    // access palette data
    uint *data_argb=(uint*)palette.ReadOnly();
    if (!data_argb) return 0;

    for (PaletteEntry = 0; PaletteEntry < 256; ++PaletteEntry)
    {
      RGB = *data_argb++;
      Colours[0] = (0x00010000 | PaletteEntry);
      Colour = ((RGB >> 16) & 0xff);
      Colours[1] = ((Colour << 24) | (Colour << 16) | (Colour << 8));
      Colour = ((RGB >> 8) & 0xff);
      Colours[2] = ((Colour << 24) | (Colour << 16) | (Colour << 8));
      Colour = (RGB & 0xff);
      Colours[3] = ((Colour << 24) | (Colour << 16) | (Colour << 8));
      Colours[4] = 0;
      LoadRGBRtg(RtgScreen, Colours);
    }

    return 1;
}


int IAmiga::GetPalette(Palette &palette)
{
    // only set palette when indexed format
    if (Format.type!=INDEXED) return 0;

    // access palette palette data
    uint *data_argb=(uint*)palette.Lock();
    if (!data_argb) return 0;

    // KLUDGE - read palette here

    // unlock
    palette.Unlock();
    return 1;
}







int IAmiga::WaitForRetrace()
{
    // wait for vertical retrace

    // KLUDGE - wait for retrace

    return 1;
}







int IAmiga::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IAmiga::GetPrimary()
{
    return PrimarySurface;
}


int IAmiga::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IAmiga::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int IAmiga::GetTotalVideoMemory()
{
    return 0;
}


int IAmiga::GetFreeVideoMemory()
{
    return 0;
}


int IAmiga::CompactVideoMemory()
{
    return 0;
}







int IAmiga::getch()
{
    return ::getch();
}


int IAmiga::kbhit()
{
    return ::kbhit();
}







void IAmiga::GetName(char name[]) const
{
    strcpy(name,"Amiga");
}


int IAmiga::GetXResolution() const
{
    return XResolution;
}
        

int IAmiga::GetYResolution() const
{
    return YResolution;
}


int IAmiga::GetBitsPerPixel() const
{
    return Format.bits;
}


int IAmiga::GetBytesPerPixel() const
{
    return Format.bytes;
}
        

int IAmiga::GetOutput() const
{
    return FULLSCREEN;
}
        

int IAmiga::GetFrequency() const
{
    return UNKNOWN;
}


int IAmiga::GetLayout() const
{
    return Layout;
}
        

FORMAT IAmiga::GetFormat() const
{
    return Format;
}
        

WINDOW IAmiga::GetWindow() const
{
    return NULL;
}







int IAmiga::ok() const
{
    return Status;
}







Interface::SURFACE* IAmiga::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // create native surface
    SURFACE *surface=new SURFACE(*this,width,height,format,type,orientation,advance,layout);
    if (surface && surface->ok()) return surface;
    else delete surface;

    // fallback to software
    return ISoftware::RequestSurface(width,height,format,type,orientation,advance,layout);
}







IAmiga::SURFACE::SURFACE(IAmiga &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // defaults
    Block=NULL;

    // check parameters
    if ((width<=0 || height<=0) || !format.ok() || (type!=VIDEO && type!=OFFSCREEN) ||
        (orientation!=TOPDOWN && orientation!=BOTTOMUP && orientation!=DEFAULT) || 
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=DEFAULT)) return;

    // setup new orientation
    int new_orientation=orientation;
    if (new_orientation==DEFAULT) new_orientation=TOPDOWN;
    
    // setup new advance
    int new_advance=advance;
    if (new_advance==DEFAULT) new_advance=0;

    // setup new layout
    int new_layout=layout;

    // default layout is LINEAR
    if (new_layout!=DEFAULT && new_layout!=LINEAR) return;
    else new_layout=LINEAR;

    // linear only
    if (new_layout!=LINEAR) return;

    // calculate size of surface memory
    uint size = width*height*format.bytes + height*new_advance;
    if (!size) return;

    // allocate video memory block
    Block=new MemoryBlock(i.VideoMemory,size);

    // check block
    if (!Block || !Block->ok())
    {
        delete Block;
        Block=NULL;
        return;
    }

    // KLUDGE - clear video memory of surface ?

    // setup data
    type=VIDEO;
    orientation=new_orientation;
    advance=new_advance;
    layout=new_layout;
    
    // setup lock offset
    if (orientation==TOPDOWN) LockOffset=0;
    else LockOffset=(width*format.bytes+advance)*(height-1);
}


IAmiga::SURFACE::~SURFACE()
{
    delete Block;
}


void* IAmiga::SURFACE::Lock(int wait)
{
    // advoid warnings
    if (wait);
    
    // lock + offset (orientation)
    return ((char*)Block->lock()) + LockOffset;
}


void IAmiga::SURFACE::Unlock()
{
    // unlock
    Block->unlock();
}


int IAmiga::SURFACE::LockCount()
{
    // surface lock count
    return Block->count();
}


int IAmiga::SURFACE::Lockable()
{
    // lockable
    return 1;
}


int IAmiga::SURFACE::Restore()
{
    // no restore
    return 0;
}


int IAmiga::SURFACE::NativeType()
{
    // native type
    return NATIVE_UNAVAILABLE;
}


void* IAmiga::SURFACE::GetNative()
{
    // return native
    return NULL;
}


int IAmiga::SURFACE::ok()
{
    // status
    if (!Block) return 0;
    else return Block->ok();
}







int IAmiga::SetLinearMode(int x,int y,int id)
{
    int RetVal = 0;

// KLUDGE - set display mode (see "globals.h" for a list of pixel format id's...)

// KLUDGE you must setup "LFB" to point to video memory, setup "LFBSize" to the size of the vram
// setup all the data like "Format", "XResolution", "YResolution", "Layout" - once this is done
// call "InitVideoMemory (fail if returns 0), then "InitPrimary" (again fail on zero return).
// then you are DONE... and the display should fucking work like a charm >B)

    struct TagItem ScreenMode[] =
    {
      smr_ChunkySupport, (1 << 9), // KLUDGE
      smr_Buffers, 1, // KLUDGE
      smr_TitleText, (ULONG) "Welcome to Amiga PTC", // KLUDGE - Shouldn't be hard coded!
      smr_MinWidth, 320,
      smr_MinHeight, 200,
      smr_MaxWidth, 2000,
      smr_MaxHeight, 2000,
      TAG_DONE
    };

    struct TagItem Screen[] =
    {
      rtg_Buffers, 1,
      TAG_DONE
    };

    struct TagItem ScreenInfo[] =
    {
      grd_Width, 0,
      grd_Height, 0,
      TAG_DONE
    };

    if ((RTGMasterBase = (struct RTGMasterBase *) OpenLibrary((UBYTE *) "rtgmaster.library", (ULONG) 0)) != NULL)
    {
      if ((ScreenReq = RtgScreenModeReq(ScreenMode)) != NULL)
      {
        if ((RtgScreen = OpenRtgScreen(ScreenReq, Screen)) != NULL)
        {
          GetRtgScreenData(RtgScreen, ScreenInfo);
          XResolution = ScreenInfo[0].ti_Data;
          YResolution = ScreenInfo[1].ti_Data;
          Layout = LINEAR;
          Format = INDEX8;
          LFB = LockRtgScreen(RtgScreen);
          LFBSize = (XResolution * YResolution);
          //pubHidden1 = pubCurrent = (UBYTE *) GetBufAdr(RtgScreen, 1); //KLUDGE!  Check return!
          //pubHidden2 = (UBYTE *) GetBufAdr(RtgScreen, 2); //KLUDGE!  Check return!

          if (InitVideoMemory())
          {
            if (InitPrimary())
            {
              RetVal = 1;
            }
          }
        }
      }
    }

    return RetVal;
}







int IAmiga::InitVideoMemory()
{
    // check lfb
    if (!LFB) return 0;

    // close old manager
    CloseVideoMemory();

    // initialize video memory manager
    VideoMemory.Init(LFB,LFBSize);
    if (!VideoMemory.ok()) return 0;
    else return 1;
}


void IAmiga::ClearVideoMemory()
{
    // KLUDGE - clear all video memory
}


void IAmiga::CloseVideoMemory()
{
    // free all video surfaces
    List<Surface>::Iterator iterator=SurfaceList.first();
    Surface *current=iterator.current();
    while (current)
    {
        if (current->Type==VIDEO) current->Free();
        current=iterator.next();
    }

    // free all video memory
    VideoMemory.Close();
}







int IAmiga::InitPrimary()
{
    // close old primary
    ClosePrimary();

    // initialize new video memory surface
    PrimarySurface=new Surface(this,XResolution,YResolution,Format,VIDEO,DEFAULT,DEFAULT,Layout);
    if (!PrimarySurface) return 0;
    if (!PrimarySurface->ok())
    {
        // failure
        ClosePrimary();
        return 0;
    }

    // set surface to primary
    if (!SetPrimary(*PrimarySurface))
    {
        // failure
        //delete PrimarySurface;
        //return 0;
    }

    // success
    return 1;
}


void IAmiga::ClosePrimary()
{
    // free primary surface
    delete PrimarySurface;
    PrimarySurface=NULL;
}







#endif
