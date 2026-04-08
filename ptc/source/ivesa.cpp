////////////////////
// VESA interface //
////////////////////
#include "ivesa.h"
#include "surface.h"

#ifdef __VESA__







IVESA::IVESA(WINDOW window)
{
    // advoid warnings
    if (window);

    // defaults
    Layout=0;
    NeedRestore=0;
    Window=NULL;
    PrimarySurface=NULL;
    Status=1;

    // vesa data defaults
    LFB=NULL;
    memset(&Info,0,sizeof(Info));
    memset(&ModeInfo,0,sizeof(ModeInfo));
    memset(ModeList,0,sizeof(ModeList));
    ModeList[0]=-1;
    ModeInfoList=NULL;
    DisplayStartX=0;
    DisplayStartY=0;
    DACWidth=6;

    // initialize VBE info
    if (!InitInfo()) Status=0;

    // native matrox support
    #ifdef __MATROX__
    NativeMatrox=0;
    Matrox=new MATROX;
    if (!Matrox->ok()) Matrox=NULL;
    //else printf("found matrox!\n");
    #endif
}


IVESA::~IVESA()
{
    // close matrox
    #ifdef __MATROX__
    delete Matrox;
    #endif

    // return to textmode if needed
    if (NeedRestore)
    {
        // this should be fixed to do a correct restore!
        WaitForRetrace();
        Palette black;
        SetPalette(black);
        #ifdef __DOS32__
        DPMI::REGS regs;
        memset(&regs,0,sizeof(regs));
        regs.eax=3;
        dpmi.int86(0x10,&regs,&regs);
        #endif
    }

    // close primary surface
    ClosePrimary();

    // close video memory
    CloseVideoMemory();

    // free lfb
    FreeLFB();

    // free modeinfo list
    delete[] ModeInfoList;
}







Interface::INFO IVESA::GetInfo()
{
    // return info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}


int IVESA::GetModeList(List<MODE> &modelist)
{
    // clear modelist
    modelist.free();

    // add modes from VBE ModeList
    MODEINFO modeinfo;
    for (short *p=ModeList; (*p)!=-1; p++)
    {
        if (GetModeInfo(*p,&modeinfo))
        {
            // add modes from modeinfo
            AddModes(modelist,modeinfo);
        }
    }

#ifdef __MATROX__

    // check
    if (Matrox)
    {
        // search for low resolution modes (320x200|240)
        int found=0;
        List<MODE>::Iterator iterator=modelist.first();
        MODE *current=iterator.current();
        while (current)
        {
            // check resolution
            if (current->x==320 && (current->y==200 || current->y==240)) found=1;
        
            // next mode
            current=iterator.next();
        }

        // if no low resolution modes are found: add native matrox modes
        if (!found)
        {
            //printf("adding native matrox modes!\n");

            // search for zoomable modes
            iterator=modelist.first();
            current=iterator.current();
            while (current)
            {
                // check if zoomable...
                if (current->x==640 && (current->y==400 || current->y==480))
                {
                    // add zoomed mode
                    MODE *mode=new MODE;
                    *mode=*current;
                    mode->x>>=1;
                    mode->y>>=1;
                    modelist.add(mode);
                }

                // next mode
                current=iterator.next();
            }
        }
    }

#endif
    
    // success
    return 1;
}







int IVESA::SetMode(MODE const &mode)
{
    // check mode interface name
    char name[8];
    GetName(name);
    if (stricmp(mode.i,name)!=0) return 0;

    // interface name ok - set mode
    return SetMode(mode.x,mode.y,mode.format,mode.output,mode.frequency,mode.layout);
}


int IVESA::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    // fail on invalid output parameter
    if (output!=FULLSCREEN && output!=DEFAULT) return 0;

    // fail on invalid frequency parameter
    if (frequency!=DEFAULT && frequency!=UNKNOWN) return 0;

    // DIRECT -> VIRTUAL32 (temporary!)
    if (id==DIRECT) id=VIRTUAL32;

    // INDEXED -> INDEX8
    if (id==INDEXED) id=INDEX8;

    // TODO: handle default (x,y)

    if (id==VIRTUAL32)
    {
        // VIRTUAL 32bit mode set
        if (SetMode(x,y,FORMAT(ARGB8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGBA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(BGRA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGB888),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR888),layout))   return 1;
        if (SetMode(x,y,FORMAT(RGB565),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR565),layout))   return 1;
        if (SetMode(x,y,FORMAT(ARGB1555),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR1555),layout)) return 1;
    }
    else if (id==VIRTUAL16)
    {
        // VIRTUAL 16bit mode set
        if (SetMode(x,y,FORMAT(RGB565),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR565),layout))   return 1;
        if (SetMode(x,y,FORMAT(ARGB1555),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR1555),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGB888),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR888),layout))   return 1;
        if (SetMode(x,y,FORMAT(ARGB8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGBA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(BGRA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR8888),layout)) return 1;
    }
    else if (id==VIRTUAL8)
    {
        // VIRTUAL 8bit mode set
        if (SetMode(x,y,FORMAT(INDEX8),layout))   return 1;
        if (SetMode(x,y,FORMAT(ARGB8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGBA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(BGRA8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR8888),layout)) return 1;
        if (SetMode(x,y,FORMAT(RGB888),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR888),layout))   return 1;
        if (SetMode(x,y,FORMAT(RGB565),layout))   return 1;
        if (SetMode(x,y,FORMAT(BGR565),layout))   return 1;
        if (SetMode(x,y,FORMAT(ARGB1555),layout)) return 1;
        if (SetMode(x,y,FORMAT(ABGR1555),layout)) return 1;
    }
    else
    {
        // attempt modeset from format id
        if (SetMode(x,y,FORMAT(id),layout)) return 1;

        // try modeset from bits per pixel
        if (SetMode(x,y,id,layout)) return 1;
    }

    // no suitable mode found
    return 0;
}


int IVESA::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // fail on invalid output parameter
    if (output!=FULLSCREEN && output!=DEFAULT) return 0;

    // fail on invalid frequency parameter
    if (frequency!=DEFAULT && frequency!=UNKNOWN) return 0;

    // set mode
    return SetMode(x,y,format,layout);
}


MODE IVESA::GetMode()
{
    // get mode info
    MODE mode;
    GetName(mode.i);
    mode.x=ModeInfo.XResolution;
    mode.y=ModeInfo.YResolution;
    mode.format=Format;
    mode.output=FULLSCREEN;
    mode.frequency=DEFAULT;
    mode.layout=Layout;
    return mode;
}







int IVESA::SetPalette(Palette &palette)
{
    // indexed formats only
    if (Format.type!=INDEXED) return 0;

    // get palette data (read only)
    uint *data=(uint*)palette.ReadOnly();
    if (!data) return 0;

    // get dpmi dos memory buffer
    #ifdef __DOS32__
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,1024)) return 0;
    #endif

    // check DAC width
    if (DACWidth!=8)
    {
        // copy data -> local
        uint local[256];
        memcpy(local,data,1024);

        // setup shift magnitude and mask
        int adjust=8-DACWidth;
        int mask=(1<<adjust)-1;
        mask+=mask<<8;
        mask+=mask<<16;

        // shift palette data down to DAC size
        uint *p=local;
        uint *pmax=p+256;
        while (p<pmax)
        {
            *p|=mask;
            *p>>=adjust;
            p++;
        }

        // copy to dosmem
        #ifdef __DOS32__
        far_memcpy(dosmem_selector,0,local,1024);
        #endif
    }
    else
    {
        // copy direct to dosmem (no shift required)
        #ifdef __DOS32__
        far_memcpy(dosmem_selector,0,data,1024);
        #endif
    }

    // call VESA bios to set palette
    #ifdef __DOS32__
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x4F09;
    regs.ebx=0;
    regs.ecx=256;
    regs.edx=0;
    regs.edi=0;
    regs.es=dosmem_segment;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0;
    #endif

    // update primary surface palette?
    return 1;
}


int IVESA::GetPalette(Palette &palette)
{
    // index formats only
    if (Format.type!=INDEXED) return 0;

#ifdef __DOS32__

    // get dpmi dos memory buffer
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,1024)) return 0;

    // call VESA bios to get palette
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x4F09;
    regs.ebx=1;
    regs.ecx=256;
    regs.edx=0;
    regs.edi=0;
    regs.es=dosmem_segment;
    dpmi.int86(0x10,&regs,&regs);

    // check vesa error value?

    // copy from dosmem to locked palette data
    uint *data=(uint*)palette.Lock();
    if (!data) return 0;
    far_memcpy(data,dosmem_selector,0,1024);

#else

    // lock palette
    uint *data=(uint*)palette.Lock();
    if (!data) return 0;

    // emulate vesa write to palette
    memset(data,random(256),1024);

#endif

    // check size adjust
    if (DACWidth!=8)
    {
        // setup shift magnitude and mask
        uint adjust=8-DACWidth;
        uint mask=(1<<DACWidth)-1;
        mask+=mask<<8;
        mask+=mask<<16;

        // shift palette back up from DAC size
        uint *p=data;
        uint *pmax=p+256;
        while (p<pmax)
        {
            *p&=mask;
            *p<<=adjust;
            p++;
        }
    }

    // unlock palette data
    palette.Unlock();
    return 1;
}







int IVESA::WaitForRetrace()
{
    // wait for vertical retrace (dummy display start set)
    SetDisplayStart(DisplayStartX,DisplayStartY,1);
    return 1;
}







int IVESA::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IVESA::GetPrimary()
{
    return PrimarySurface;
}


int IVESA::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IVESA::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int IVESA::GetTotalVideoMemory()
{
    return 0;
}


int IVESA::GetFreeVideoMemory()
{
    return 0;
}


int IVESA::CompactVideoMemory()
{
    return 0;
}







int IVESA::getch()
{
    return ::getch();
}


int IVESA::kbhit()
{
    return ::kbhit();
}







void IVESA::GetName(char name[]) const
{
    strcpy(name,"VESA");
}


int IVESA::GetXResolution() const
{
    return ModeInfo.XResolution;
}
        

int IVESA::GetYResolution() const
{
    return ModeInfo.YResolution;
}


int IVESA::GetBitsPerPixel() const
{
    return Format.bits;
}


int IVESA::GetBytesPerPixel() const
{
    return Format.bytes;
}
        

int IVESA::GetOutput() const
{
    return FULLSCREEN;
}
        

int IVESA::GetFrequency() const
{
    return UNKNOWN;
}


int IVESA::GetLayout() const
{
    return Layout;
}
        

FORMAT IVESA::GetFormat() const
{
    return Format;
}
        

WINDOW IVESA::GetWindow() const
{
    return NULL;
}







int IVESA::ok() const
{
    return Status;
}







Interface::SURFACE* IVESA::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // create native surface
    SURFACE *surface=new SURFACE(*this,width,height,format,type,orientation,advance,layout);
    if (surface && surface->ok()) return surface;
    else delete surface;

    // fallback to software
    return ISoftware::RequestSurface(width,height,format,type,orientation,advance,layout);
}







IVESA::SURFACE::SURFACE(IVESA &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
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
    
    // get pitch of vesa display (6/9/1998 gaffer)
    int pitch = i.ModeInfo.BytesPerScanLine;

    // calculate vesa advance
    int vesa_advance = pitch - width * format.bytes;

    // setup new advance
    int new_advance=advance;
    if (new_advance==DEFAULT) new_advance = vesa_advance;
    else
    {
        // check that requested advance matches vesa advance
        if (new_advance!=vesa_advance) return;
    }

    // calculate size of surface memory
    uint size = width*height*format.bytes + height*new_advance;
    if (!size) return;

    // allocate video memory block
    Block=new MemoryBlock(i.VideoMemory,size);
    if (!Block || !Block->ok())
    {
        delete Block;
        Block=NULL;
        return;
    }

    // clear video memory

    // setup data
    type=VIDEO;
    orientation=new_orientation;
    advance=new_advance;
    layout=LINEAR;

    // setup lock offset
    if (orientation==TOPDOWN) LockOffset=0;
    else LockOffset=(width*format.bytes+advance)*(height-1);
}


IVESA::SURFACE::~SURFACE()
{
    delete Block;
}


void* IVESA::SURFACE::Lock(int wait)
{
    // lock near base
    LockNearBase();

    // advoid warnings
    if (wait);

    // lock + offset (orientation)
    return ((char*)Block->lock()) + LockOffset;
}


void IVESA::SURFACE::Unlock()
{
    // unlock
    Block->unlock();

    // unlock near base
    UnlockNearBase();
}


int IVESA::SURFACE::LockCount()
{
    // surface lock count
    return Block->count();
}


int IVESA::SURFACE::Lockable()
{
    // lockable
    return 1;
}


int IVESA::SURFACE::Restore()
{
    // no restore
    return 0;
}


int IVESA::SURFACE::NativeType()
{
    // native type
    return NATIVE_UNAVAILABLE;
}


void* IVESA::SURFACE::GetNative()
{
    // return native
    return NULL;
}


int IVESA::SURFACE::ok()
{
    // status
    if (!Block) return 0;
    else return Block->ok();
}







int IVESA::SetMode(int mode)
{
    // fail on invalid mode
    if (!mode) return 0;

    // determine layout (banked|linear)
    int layout=BANKED;
    if (mode&0x4000) layout=LINEAR; 

    // no banked modes allowed yet
    if (layout==BANKED) return 0;

#ifdef __DOS32__

    // set vesa mode
    DPMI::REGS regs;                
    memset(&regs,0,sizeof(regs));
    regs.eax=0x00004F02;
    regs.ebx=mode;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0;

#endif

#ifdef __MATROX__

    // handle native matrox modes

#endif

    // need restore
    NeedRestore=1;

    // update layout
    Layout=layout;

    // update ModeInfo
    GetModeInfo(mode,&ModeInfo);

    // update display start (x,y)
    GetDisplayStart(DisplayStartX,DisplayStartY);

    // setup format structure
    Format=GetFormat(ModeInfo);

    // attempt to enlarge dac size (8bit only)
    if (Format.id==INDEX8)
    {
        //Palette old;
        //GetPalette(old);
        SetDACWidth(8);
        DACWidth=GetDACWidth();
        if (!DACWidth) DACWidth=6;     // on failure, fallback to 6 bits DAC width
        //SetPalette(old);
    }

    // setup LFB if linear
    if (Layout==LINEAR)
    {
        // initialize LFB
        if (!InitLFB()) return 0;
    }

    // initialize video memory
    if (!InitVideoMemory()) return 0;

    // initialize primary surface
    if (!InitPrimary()) return 0;
    
    // success
    return 1;
}


int IVESA::SetMode(int x,int y,int bits,int layout)
{
    // standard modeset
    if (layout==DEFAULT) layout=LINEAR;     // fix?
    int mode=GetModeNumber(x,y,bits,layout);
    return SetMode(mode);
}


int IVESA::SetMode(int x,int y,FORMAT const &format,int layout)
{
    // GREY8
    if (format.id==GREY8) return SetGREY8(x,y,layout);

    // RGB332
    if (format.id==RGB332) return SetRGB332(x,y,layout);

    // standard modeset
    if (layout==DEFAULT) layout=LINEAR;
    int mode=GetModeNumber(x,y,format,layout);
    return SetMode(mode);
}


int IVESA::SetGREY8(int x,int y,int layout)
{
    // create greyscale palette
    Palette palette;
    uint *data=(uint*)palette.Lock();
    for (int i=0; i<256; i++) data[i]=RGB32((uchar)i,(uchar)i,(uchar)i);
    palette.Unlock();
    
    // set INDEX8 format mode
    if (!SetMode(x,y,FORMAT(INDEX8),layout)) return 0;

    // set greyscale palette
    if (!SetPalette(palette)) return 0;

    // fudge format
    Format.init(GREY8);

    // re-initialize video memory
    if (!InitVideoMemory()) return 0;

    // re-initialize primary    
    if (!InitPrimary()) return 0;
    return 1;
}


int IVESA::SetRGB332(int x,int y,int layout)
{
    // setup RGB332 palette
    Palette palette;
    uint *data=(uint*)palette.Lock();
    for (int i=0; i<256; i++)
    {
        float r = (float)((i&0xE0)>>5) * (float)255.0 / (float)7.0;
        float g = (float)((i&0x1C)>>2) * (float)255.0 / (float)7.0;
        float b = (float)(i&0x03) * (float)255.0 / (float)3.0;
        data[i]=RGB32((uchar)r,(uchar)g,(uchar)b);
    }
    palette.Unlock();

    // set INDEX8 format mode
    if (!SetMode(x,y,FORMAT(INDEX8),layout)) return 0;

    // set our RGB332 palette
    if (!SetPalette(palette)) return 0;

    // fudge format
    Format.init(RGB332);

    // re-initialize video memory
    if (!InitVideoMemory()) return 0;

    // re-initialize primary    
    if (!InitPrimary()) return 0;
    return 1;
}







int IVESA::GetModeNumber(int x,int y,int bits,int layout)
{
    // #1 - try default formats for bits per pixel
    int mode=0;
    switch (bits)
    {
        case 32: mode=GetModeNumber(x,y,FORMAT(ARGB8888),layout); break;
        case 24: mode=GetModeNumber(x,y,FORMAT(RGB888),layout);   break;
        case 16: mode=GetModeNumber(x,y,FORMAT(RGB565),layout);   break;
        case 15: mode=GetModeNumber(x,y,FORMAT(ARGB1555),layout); break;
        case 8:  mode=GetModeNumber(x,y,FORMAT(INDEX8),layout);   break;
    }
    if (mode) return mode;

    // #2 - search through mode list for mode matching (x,y,bits)
    MODEINFO modeinfo;
    for (short *p=ModeList; (*p)!=-1; p++)
    {
        if (GetModeInfo(*p,&modeinfo))
        {
            // match bits per pixel -- should also match other criteria? DIRECT/INDEXED etc?
            if (modeinfo.XResolution==x && modeinfo.YResolution==y && modeinfo.BitsPerPixel==bits)
            {
                if (layout==LINEAR && (modeinfo.ModeAttributes & ModeLinear)!=0) return (*p | 0x4000);
                else if (layout==BANKED && (modeinfo.ModeAttributes & ModeNonBanked)==0) return *p;
                else if (layout==DEFAULT) return *p;
            }
        }
    }

    // nothing
    return 0;
}


int IVESA::GetModeNumber(int x,int y,FORMAT const &format,int layout)
{
    // fail on bad format
    if (!format.ok()) return 0;

    // search through mode list for mode number matching (x,y,format)
    MODEINFO modeinfo;
    for (short *p=ModeList; (*p)!=-1; p++)
    {
        if (GetModeInfo(*p,&modeinfo))
        {
            if (format.type==DIRECT && format.model==RGBA)
            {
                // TODO: check for properly handle different color models in FORMAT

                // direct color - check for component matches (TODO: check modeinfo more thouroughly)
                if ((int)modeinfo.XResolution==x && 
                    (int)modeinfo.YResolution==y &&
                    (int)modeinfo.RedFieldPosition==(int)format.c1.position &&
                    (int)modeinfo.RedMaskSize==(int)format.c1.size &&
                    (int)modeinfo.GreenFieldPosition==(int)format.c2.position &&
                    (int)modeinfo.GreenMaskSize==(int)format.c2.size &&
                    (int)modeinfo.BlueFieldPosition==(int)format.c3.position &&
                    (int)modeinfo.BlueMaskSize==(int)format.c3.size &&
                    (int)modeinfo.AlphaFieldPosition==(int)format.c4.position &&
                    (int)modeinfo.AlphaMaskSize==(int)format.c4.size)
                {
                    if (layout==LINEAR && (modeinfo.ModeAttributes & ModeLinear)!=0) return (*p | 0x4000);
                    else if (layout==BANKED && (modeinfo.ModeAttributes & ModeNonBanked)==0) return *p;   
                    else if (layout==DEFAULT)
                    {
                        //cout << "returning direct mode # (neither banked nor linear check...)\n";
                        return *p;
                    }
                }
            }
            else if (format.type==INDEXED)
            {   
                // indexed color - just match bits per pixel (TODO: check modeinfo more thouroughly)
                if (modeinfo.XResolution==x && modeinfo.YResolution==y &&
                    modeinfo.BitsPerPixel==format.bits)
                {
                    if (layout==LINEAR && (modeinfo.ModeAttributes & ModeLinear)!=0) return (*p | 0x4000);
                    else if (layout==BANKED && (modeinfo.ModeAttributes & ModeNonBanked)==0) return *p;
                    //else return *p;
                }
            }
        }
    }

    // nothing
    return 0;
}







MODE IVESA::GetMode(MODEINFO const &modeinfo)
{
    // initialize MODE structure from MODEINFO struct
    MODE mode;
    memset(&mode,0,sizeof(mode));
    GetName(mode.i);
    mode.x=modeinfo.XResolution;
    mode.y=modeinfo.YResolution;
    mode.format=GetFormat(modeinfo);
    mode.output=FULLSCREEN;
    mode.frequency=UNKNOWN;
    if ((modeinfo.ModeAttributes & ModeLinear)!=0) mode.layout=LINEAR;
    else if ((modeinfo.ModeAttributes & ModeNonBanked)==0) mode.layout=BANKED;
    else mode.layout=0;
    return mode;
}


FORMAT IVESA::GetFormat(MODEINFO const &modeinfo)
{
    // setup FORMAT struct from MODEINFO struct
    FORMAT format;
    if (modeinfo.BitsPerPixel==8)
    {
        // indexed color (TODO: make more robust!)
        format.init(INDEX8);
    }
    else
    {
        // direct color (TODO: make more robust!)
        format.init(modeinfo.RedFieldPosition,modeinfo.RedMaskSize,
                    modeinfo.GreenFieldPosition,modeinfo.GreenMaskSize,
                    modeinfo.BlueFieldPosition,modeinfo.BlueMaskSize,
                    modeinfo.AlphaFieldPosition,modeinfo.AlphaMaskSize);
    }
    return format;
}


int IVESA::GetModeInfo(int mode,MODEINFO *modeinfo)
{
    // mask out mode number options (lfb & clear memory bits)
    mode=mode & 0xFFFF0FFF;

#ifdef __DOS32__

    // error val
    int result=1;

    // get dpmi dos memory buffer
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,1024)) return 0;

    // clear realmode buffer
    far_memset(dosmem_selector,0,0,1024);

    // get VBE info
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x00004F01;
    regs.ecx=mode;
    regs.es=dosmem_segment;
    regs.edi=0;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) result=0;

    // copy from DOS memory -> InfoBuffer
    far_memcpy(modeinfo,dosmem_selector,0,256);

    // check if mode is available
    if ((modeinfo->ModeAttributes & ModeAvailable)==0) result=0;
    return result;

#else

    // emulate vesa bios modelist
    if (!ModeInfoList) return 0;

    // locate mode number
    short *p=ModeList;
    int index=0;
    while (p[index]!=mode && p[index]!=-1) index++;
    if (p[index]==-1) return 0;

    // assign modeinfo
    *modeinfo=ModeInfoList[index];
    return 1;

#endif
}


int IVESA::GetModeInfo(int x,int y,int bits,MODEINFO *modeinfo)
{
    int mode=GetModeNumber(x,y,bits,DEFAULT);
    if (!mode) return 0;
    return GetModeInfo(mode,modeinfo);
}


int IVESA::GetModeInfo(int x,int y,FORMAT const &format,MODEINFO *modeinfo)
{
    int mode=GetModeNumber(x,y,format,DEFAULT);
    if (!mode) return 0;
    return GetModeInfo(mode,modeinfo);
}







int IVESA::SetWindowPosition(int position)
{
#ifdef __DOS32__

    // set window position (banked modes)
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F05;
    regs.ebx=0;
    regs.edx=position;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0; 
    return 1;

#else

    // emulation
    return 1;

#endif
}


int IVESA::SetDisplayStart(int x,int y,int vrt)
{
#ifdef __DOS32__

    // set display start (x,y)
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F07;
    if (vrt) regs.ebx=0x80;
    else regs.ebx=0;
    regs.ecx=x & 0xFFFF;
    regs.edx=y & 0xFFFF;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0; 
    else return 1;

#else

    // emulation
    return 1;

#endif
}


int IVESA::GetDisplayStart(int &x,int &y)
{
#ifdef __DOS32__

    // get display start (x,y)
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F07;
    regs.ebx=1;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0;
    x=regs.ecx & 0xFFFF;
    y=regs.edx & 0xFFFF;
    return 1;

#else

    // emulation
    return 1;

#endif
}


int IVESA::SetDACWidth(int width)
{
#ifdef __DOS32__

    // set palette DAC width
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F08;
    regs.ebx=width<<8;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0;
    if ((regs.ebx>>8)!=(uint)width) return 0;
    return 1;

#else

    // emulation 
    switch (random(2))
    {
        case 0: DACWidth=6; break;
        case 1: DACWidth=8; break;
    }
    return 1;

#endif
}                


int IVESA::GetDACWidth()
{
#ifdef __DOS32__

    // set palette DAC width
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F08;
    regs.ebx=1;
    dpmi.int86(0x10,&regs,&regs);
    if (Error(regs.eax)) return 0;
    return (regs.ebx>>8);

#else

    // emulation 
    switch (random(2))
    {
        case 0:  return 6;
        default: return 8;
    }
    
#endif
}


int IVESA::InitInfo()
{
#ifdef __DOS32__

    // get dpmi dos memory buffer
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,1024)) return 0;

    // clear dosmem buffer
    far_memset(dosmem_selector,0,0,1024);

    // set VBE2 signature (gaffer 6/9/98)
    far_memcpy(dosmem_selector,0,"VBE2",4);

    // Get VBE info
    DPMI::REGS regs;
    memset(&regs,0,sizeof regs);
    regs.eax=0x00004F00;
    regs.es=dosmem_segment;
    regs.edi=0;
    dpmi.int86(0x10,&regs,&regs);

    // check if vesa error
    if (Error(regs.eax)) return 0;

    // copy from DOS memory -> InfoBuffer
    far_memcpy((void*)&Info,dosmem_selector,0,512);

    // check for 'VESA' signature
    if (strncmp(&Info.VESASignature[0],"VESA",4)!=0) return 0;

    // copy VESA modelist to local memory
    LockNearBase();
    if (Info.VideoModePtr)
    {
        short *p1=(short*)MapRealPointer(Info.VideoModePtr);
        short *p2=(short*)ModeList;
        p1=(short*)AdjustNearPointer(p1);
        if (!p1)
        {
            // failure
            ModeList[0]=-1;
            UnlockNearBase();
            return 0;
        }
        while(*p1!=-1 && *p1!=0) *p2++=*p1++;          // note: added check for NULL modelist terminate because of ET6000 bug
        *p2=-1;
    }
    else ModeList[0]=-1;
    UnlockNearBase();

    // save bios settings
    //Save("diamond.vbe"); 

    // done
    return 1;

#else

    // load bios settings from file
    return Load("d:/progra~1/projects/ptc/060/src/stress/diamond.vbe");

#endif
}


int IVESA::Error(int eax)
{
#ifdef __DOS32__

    // check for vesa error indicator 
    int result=(eax & 0x0000FFFF);
    if (result!=0x0000004F) return 1;
    else return 0;

#else

    // no error
    return 0;

#endif
}







int IVESA::InitLFB()
{
#ifdef __DOS32__

    // free old mapping
    FreeLFB();

    // initalize LFB physical mapping
    LFB=dpmi.MapPhysicalAddress((uint)ModeInfo.PhysBasePtr,4096*1024-1);
    if (!LFB) return 0;
    else return 1;

#else

    // free
    FreeLFB();

    // allocate emulated lfb
    uint size=Info.TotalMemory<<16;
    if (!size) return 0;
    LFB=malloc(size);
    return 1;

#endif
}


void IVESA::FreeLFB()
{
#ifdef __DOS32__

    // free LFB physical mapping
    if (LFB) dpmi.FreePhysicalMapping((uint)LFB);
    LFB=NULL;

#else

    // free emul LFB
    free(LFB);

#endif
}







int IVESA::InitVideoMemory()
{
    // close old manager
    CloseVideoMemory();

    // fail on invalid LFB
    if (!LFB) return 0;

#ifdef __DOS32__

    // get lfb adjusted for conventional base
    void *lfb=AdjustNearPointer(LFB);
    if (!lfb) return 0;

#else
    
    // point to emulated LFB
    void *lfb=LFB;

#endif

    // get video memory size
    uint size=Info.TotalMemory<<16;

    // initialize video memory manager
    VideoMemory.Init(lfb,size);
    if (!VideoMemory.ok()) return 0;
    
    // success
    return 1;
}


void IVESA::CloseVideoMemory()
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







int IVESA::InitPrimary()
{
    // close old primary
    ClosePrimary();

    // initialize new video memory surface
    PrimarySurface=new Surface(this,ModeInfo.XResolution,ModeInfo.YResolution,Format,VIDEO,DEFAULT,DEFAULT,Layout);
    if (!PrimarySurface) return 0;
    if (!PrimarySurface->ok())
    {
        delete PrimarySurface;
        PrimarySurface=NULL;
        return 0;
    }

    // set video surface to primary
    if (!SetPrimary(*PrimarySurface))
    {
        // failure
        //delete PrimarySurface;
        //return 0;
    }

    // success
    return 1;
}


void IVESA::ClosePrimary()
{
    // free primary surface
    delete PrimarySurface;
    PrimarySurface=NULL;
}







int IVESA::AddModes(List<MODE> &modelist,MODEINFO const &modeinfo)
{
    // get mode from modeinfo
    MODE mode=GetMode(modeinfo);
    
    // add linear mode to list
    if (mode.layout==LINEAR) AddMode(modelist,mode);

    // if linear add banked mode also (if exists)
    /*
    if (mode.layout==LINEAR && (modeinfo.ModeAttributes & ModeNonBanked)==0)
    {
        mode.layout=BANKED;
        AddMode(modelist,mode);
    }
    */
    return 1;
}


void IVESA::AddMode(List<MODE> &modelist,MODE const &mode)
{
    // check mode info
    if (!mode.format.ok()) return;

    // alloc mode pointer
    MODE *pointer=new MODE;
    if (!pointer) return;

    // initialize
    *pointer=mode;

    // if INDEX8, add GREYSCALE and RGB332
    if (mode.format.id==INDEX8)
    {
        // add greyscale
        MODE temp=mode;
        temp.format.init(GREY8);
        AddMode(modelist,temp);
    
        // add rgb332
        temp.format.init(RGB332);
        AddMode(modelist,temp);
    }

    // add to list
    if (!modelist.add(pointer)) delete pointer;
}







int IVESA::Save(char filename[])
{
    // open file
    File file(filename);
    if (!file.ok()) return 0;

    // write vesa bios information
    if (!file.write(&Info,sizeof(Info))) return 0;

    // count number of modes in list
    int count=0;
    short *p=ModeList;
    while (*p!=-1)
    {
        count++;
        p++;
    }

    // write number of modes
    if (!file.write(&count,sizeof(count))) return 0;

    // write mode numbers
    if (!file.write(ModeList,sizeof(short)*count)) return 0;

    // write each vbe mode information entry
    MODEINFO modeinfo;
    for (int i=0; i<count; i++)
    {
        // get mode information for mode #i
        if (!GetModeInfo(ModeList[i],&modeinfo)) return 0;
        
        // write mode information data
        if (!file.write(&modeinfo,sizeof(modeinfo))) return 0;
    }

    // success
    return 1;
}


int IVESA::Load(char filename[])
{
    // open file
    File file(filename);
    if (!file.ok()) return 0;

    // read vesa bios information
    if (!file.read(&Info,sizeof(Info))) return 0;

    // read number of modes
    int count;
    if (!file.read(&count,sizeof(count))) return 0;

    // read modelist numbers
    if (!file.read(ModeList,count*sizeof(short))) return 0;
    ModeList[count]=-1;

    // allocate mode information list
    if (ModeInfoList) delete ModeInfoList;
    ModeInfoList=new MODEINFO[count];

    // read mode information list
    if (!file.read(ModeInfoList,sizeof(MODEINFO)*count)) return 0;

    // success
    return 1;
}








#endif
