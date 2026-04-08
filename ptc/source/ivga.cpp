///////////////////
// VGA interface //
///////////////////
#include "ivga.h"
#include "surface.h"

#ifdef __VGA__







IVGA::IVGA(WINDOW window)
{
    // advoid warnings
    if (window);

    // defaults
    XResolution=0;
    YResolution=0;
    Layout=0;
    NeedRestore=0;
    PrimarySurface=NULL;
    Status=1;

#ifdef __DOS32__

    // initialize vga lfb
    LFB=AdjustNearPointer((void*)0xA0000);

#else

    // emulated vga lfb
    LFB=malloc(65536);

#endif
}


IVGA::~IVGA()
{
    // return to textmode if needed
    if (NeedRestore)
    {
        WaitForRetrace();
        SetMode(3);
    }

    // close primary surface
    ClosePrimary();

    // close video memory
    CloseVideoMemory();

#ifndef __DOS32__

    // free emulated lfb
    free(LFB);

#endif
}







Interface::INFO IVGA::GetInfo()
{
    // return info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}


int IVGA::GetModeList(List<MODE> &modelist)
{
    // clear list
    modelist.free();

    // add video modes to list
    AddMode(modelist,320,200,INDEX8,FULLSCREEN,LINEAR);
    AddMode(modelist,320,200,GREY8,FULLSCREEN,LINEAR);
    AddMode(modelist,320,200,RGB332,FULLSCREEN,LINEAR);
    AddMode(modelist,320,200,FAKEMODE1A,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE1B,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE1C,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE2A,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE2B,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE2C,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE3A,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE3B,FULLSCREEN,FAKEMODE);
    AddMode(modelist,320,200,FAKEMODE3C,FULLSCREEN,FAKEMODE);
    return 1;
}





    

int IVGA::SetMode(MODE const &mode)
{
    // check mode interface name
    char name[8];
    GetName(name);
    if (stricmp(mode.i,name)!=0) return 0;

    // interface name ok - set mode
    return SetMode(mode.x,mode.y,mode.format,mode.output,mode.frequency,mode.layout);
}


int IVGA::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    // fail on invalid output parameter
    if (output!=FULLSCREEN && output!=DEFAULT) return 0;

    // fail on invalid frequency parameter
    if (frequency!=DEFAULT && frequency!=UNKNOWN) return 0;

    // 320x200xVIRTUAL8 -> 320x200xINDEX8
    if (x==320 && y==200 && id==VIRTUAL8) id=INDEX8;

    // VIRTUAL32/VIRTUAL16 -> fakemode
    if (id==VIRTUAL32 || id==VIRTUAL16 || id==VIRTUAL8) id=FAKEMODE;

    // DIRECT -> RGB332
    if (id==DIRECT) id=RGB332;

    // INDEXED -> INDEX8
    if (id==INDEXED) id=INDEX8;

    // FAKEMODE -> FAKEMODE2A
    if (id==FAKEMODE) id=FAKEMODE2A;

    // (x,y) DEFAULT -> (320,200)
    if (x==DEFAULT) x=320;
    if (y==DEFAULT) y=200;

    // set the mode
    if (layout==DEFAULT)
    {
        if (SetLinearMode(x,y,id)) return 1;
        if (SetPlanarMode(x,y,id)) return 1;
        if (SetFakeMode(x,y,id))   return 1;
    }
    else if (layout==LINEAR)   return SetLinearMode(x,y,id);
    else if (layout==PLANAR)   return SetPlanarMode(x,y,id);
    else if (layout==FAKEMODE) return SetFakeMode(x,y,id);
    return 0;
}


int IVGA::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // fail on invalid output parameter
    if (output!=FULLSCREEN && output!=DEFAULT) return 0;

    // fail on invalid frequency parameter
    if (frequency!=DEFAULT && frequency!=UNKNOWN) return 0;

    // set the mode
    if (layout==DEFAULT)
    {
        if (SetLinearMode(x,y,format.id)) return 1;
        if (SetPlanarMode(x,y,format.id)) return 1;
        if (SetFakeMode(x,y,format.id))   return 1;
    }
    else if (layout==LINEAR)   return SetLinearMode(x,y,format.id);
    else if (layout==PLANAR)   return SetPlanarMode(x,y,format.id);
    else if (layout==FAKEMODE) return SetFakeMode(x,y,format.id);
    return 0;
}


MODE IVGA::GetMode()
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







int IVGA::SetPalette(Palette &palette)
{
    // only set palette when indexed format
    if (Format.type!=INDEXED) return 0;

    // access palette data
    uint *data_argb=(uint*)palette.ReadOnly();
    if (!data_argb) return 0;

    // shift palette data down to 6bits per primary & pack into RGB
    uchar data_rgb[768];
    uchar *p=data_rgb;
    uint *q=data_argb;
    uint *qmax=q+256;
    while (q<qmax)
    {
        uchar r,g,b;
        UnpackRGB32(*q,r,g,b);
        *(p+0) = (uchar)(r>>2);
        *(p+1) = (uchar)(g>>2);
        *(p+2) = (uchar)(b>>2);
        p+=3;
        q++;
    }

#ifdef __DOS32__

    // get dpmi dos memory buffer
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,768)) return 0;

    // copy over to dosmem
    far_memcpy(dosmem_selector,0,data_rgb,768);

    // call VGA bios to set palette
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x1012;
    regs.ebx=0;
    regs.ecx=256;
    regs.edx=0;
    regs.es=dosmem_segment;
    dpmi.int86(0x10,&regs,&regs);

#endif

    // update primary surface palette?

    return 1;
}


int IVGA::GetPalette(Palette &palette)
{
    // only set palette when indexed format
    if (Format.type!=INDEXED) return 0;

    // access palette palette data
    uint *data_argb=(uint*)palette.Lock();
    if (!data_argb) return 0;

#ifdef __DOS32__

    // get dpmi dos memory buffer
    ushort dosmem_segment,dosmem_selector;
    if (!dpmi.GetDosMemory(&dosmem_segment,&dosmem_selector,768)) return 0;

    // get palette from VGA bios
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x1017;
    regs.ebx=0;
    regs.ecx=255;
    regs.edx=0;
    regs.es=dosmem_segment;
    dpmi.int86(0x10,&regs,&regs);

    // copy palette to temp data
    uchar data_rgb[768];
    far_memcpy(data_rgb,dosmem_selector,0,768);

#else

    // emulate palette
    uchar data_rgb[768];
    memset(data_rgb,random(256),768);

#endif

    // shift palette back up to 8 bits per component and pack into ARGB8888
    uchar *p=data_rgb;
    uint *q=data_argb;
    uint *qstop=q+256;
    while (q<qstop)
    {
        *q=RGB32((uchar)(*(p+1)<<2),(uchar)(*(p+1)<<2),(uchar)(*(p+2)<<2));
        p+=3;
        q++;
    }

    // unlock
    palette.Unlock();
    return 1;
}







int IVGA::WaitForRetrace()
{
    // wait for vertical retrace
    #ifdef __DOS32__
    while ( (inp(0x3da) & 8) ) {}
    while (!(inp(0x3da) & 8) ) {}
    #endif
    return 1;
}







int IVGA::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IVGA::GetPrimary()
{
    return PrimarySurface;
}


int IVGA::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IVGA::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int IVGA::GetTotalVideoMemory()
{
    return 0;
}


int IVGA::GetFreeVideoMemory()
{
    return 0;
}


int IVGA::CompactVideoMemory()
{
    return 0;
}







int IVGA::getch()
{
    return ::getch();
}


int IVGA::kbhit()
{
    return ::kbhit();
}







void IVGA::GetName(char name[]) const
{
    strcpy(name,"VGA");
}


int IVGA::GetXResolution() const
{
    return XResolution;
}
        

int IVGA::GetYResolution() const
{
    return YResolution;
}


int IVGA::GetBitsPerPixel() const
{
    return Format.bits;
}


int IVGA::GetBytesPerPixel() const
{
    return Format.bytes;
}
        

int IVGA::GetOutput() const
{
    return FULLSCREEN;
}
        

int IVGA::GetFrequency() const
{
    return UNKNOWN;
}


int IVGA::GetLayout() const
{
    return Layout;
}
        

FORMAT IVGA::GetFormat() const
{
    return Format;
}
        

WINDOW IVGA::GetWindow() const
{
    return NULL;
}







int IVGA::ok() const
{
    return Status;
}







Interface::SURFACE* IVGA::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // create native surface
    SURFACE *surface=new SURFACE(*this,width,height,format,type,orientation,advance,layout);
    if (surface && surface->ok()) return surface;
    else delete surface;

    // fallback to software
    return ISoftware::RequestSurface(width,height,format,type,orientation,advance,layout);
}







IVGA::SURFACE::SURFACE(IVGA &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // defaults
    Block=NULL;

    // check parameters
    if ((width<=0 || height<=0) || !format.ok() || (type!=VIDEO && type!=OFFSCREEN) ||
        (orientation!=TOPDOWN && orientation!=BOTTOMUP && orientation!=DEFAULT) || 
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=FAKEMODE && layout!=DEFAULT)) return;

    // setup new orientation
    int new_orientation=orientation;
    if (new_orientation==DEFAULT) new_orientation=TOPDOWN;
    
    // setup new advance
    int new_advance=advance;
    if (new_advance==DEFAULT) new_advance=0;

    // setup new layout
    int new_layout=layout;

    // format type
    if (format.type==FAKEMODE)
    {
        // TOPDOWN orientation only
        if (new_orientation!=TOPDOWN) return;

        // default layout is FAKEMODE
        if (new_layout!=DEFAULT && new_layout!=FAKEMODE) return;
        else new_layout=FAKEMODE;

        // fail on non-zero advance
        if (new_advance!=0) return;

        // allocate video memory block (take entire 64k)
        Block=new MemoryBlock(i.VideoMemory,0xFFFF);
    }                                           
    else
    {
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
    }

    // check block
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
    layout=new_layout;
    
    // setup lock offset
    if (orientation==TOPDOWN) LockOffset=0;
    else LockOffset=(width*format.bytes+advance)*(height-1);
}


IVGA::SURFACE::~SURFACE()
{
    delete Block;
}


void* IVGA::SURFACE::Lock(int wait)
{
    // lock near base
    LockNearBase();

    // advoid warnings
    if (wait);
    
    // lock + offset (orientation)
    return ((char*)Block->lock()) + LockOffset;
}


void IVGA::SURFACE::Unlock()
{
    // unlock
    Block->unlock();

    // unlock near base
    UnlockNearBase();
}


int IVGA::SURFACE::LockCount()
{
    // surface lock count
    return Block->count();
}


int IVGA::SURFACE::Lockable()
{
    // lockable
    return 1;
}


int IVGA::SURFACE::Restore()
{
    // no restore
    return 0;
}


int IVGA::SURFACE::NativeType()
{
    // native type
    return NATIVE_UNAVAILABLE;
}


void* IVGA::SURFACE::GetNative()
{
    // return native
    return NULL;
}


int IVGA::SURFACE::ok()
{
    // status
    if (!Block) return 0;
    else return Block->ok();
}







int IVGA::SetLinearMode(int x,int y,int id)
{
    if (id==8 || id==INDEX8)
    {
        // indexed 8bit color
        return SetINDEX8(x,y);
    }
    else if (id==RGB332)
    {
        // RGB332 
        return SetRGB332(x,y);
    }
    else if (id==GREY8)
    {
        // 8bit greyscale 
        return SetGREY8(x,y);
    }
    else return 0;
}


int IVGA::SetPlanarMode(int x,int y,int id)
{
    // advoid warnings
    if (x || y || id);
    return 0;
}


int IVGA::SetFakeMode(int x,int y,int id)
{
    // check resolution
    if (x!=320 || y!=200) return 0;

    // check fakemode
    if (id!=FAKEMODE1A && id!=FAKEMODE1B && id!=FAKEMODE1C &&
        id!=FAKEMODE2A && id!=FAKEMODE2B && id!=FAKEMODE2C &&
        id!=FAKEMODE3A && id!=FAKEMODE3B && id!=FAKEMODE3C) return 0;

    // set mode13h
    SetMode(0x13);

#ifdef __DOS32__

    // clear VGA palette
    int c;
    outp(0x3C8,0x00);
    for (c=0; c<256; c++)
    {
        outp(0x3C9,0);
        outp(0x3C9,0);
        outp(0x3c9,0);
    }

    // enter fakemode
    if (id==FAKEMODE1A || id==FAKEMODE1B || id==FAKEMODE1C)
    {
        // tweak VGA into 320x600 mode
        WaitForRetrace();
        outp(0x3D4,0x11);
        outp(0x3D5,inp(0x3D5) & 0x7F);
        outp(0x3C2,0xE7);
        outp(0x3D4,0x00); outp(0x3D5,0x5F);
        outp(0x3D4,0x01); outp(0x3D5,0x4F);
        outp(0x3D4,0x02); outp(0x3D5,0x50);
        outp(0x3D4,0x03); outp(0x3D5,0x82);
        outp(0x3D4,0x04); outp(0x3D5,0x54);
        outp(0x3D4,0x05); outp(0x3D5,0x80);
        outp(0x3D4,0x06); outp(0x3D5,0x70);
        outp(0x3D4,0x07); outp(0x3D5,0xF0);
        outp(0x3D4,0x08); outp(0x3D5,0x00);
        outp(0x3D4,0x09); outp(0x3D5,0x60);
        outp(0x3D4,0x10); outp(0x3D5,0x5B);
        outp(0x3D4,0x11); outp(0x3D5,0x8C);
        outp(0x3D4,0x12); outp(0x3D5,0x57);
        outp(0x3D4,0x13); outp(0x3D5,0x28);
        outp(0x3D4,0x14); outp(0x3D5,0x00);
        outp(0x3D4,0x15); outp(0x3D5,0x58);
        outp(0x3D4,0x16); outp(0x3D5,0x70);
        outp(0x3D4,0x17); outp(0x3D5,0xE3);
        outp(0x3C4,0x01); outp(0x3C5,0x01);
        outp(0x3C4,0x04); outp(0x3C5,0x06);
        outp(0x3CE,0x05); outp(0x3CF,0x40);
        outp(0x3CE,0x06); outp(0x3CF,0x05);
        outp(0x3CE,0x06); outp(0x3CF,0x05);
        ClearMemory();
    }
    else if (id==FAKEMODE2A || id==FAKEMODE2B || id==FAKEMODE2C ||
             id==FAKEMODE3A || id==FAKEMODE3B || id==FAKEMODE3C)
    {
        // tweak VGA into 320x400 mode
        WaitForRetrace();
        outp(0x3D4,0x11); outp(0x3D5,inp(0x3D5) & 0x7F);
        outp(0x3C2,0x63);
        outp(0x3D4,0x0);  outp(0x3D5,0x5F);
        outp(0x3D4,0x1);  outp(0x3D5,0x4F);
        outp(0x3D4,0x2);  outp(0x3D5,0x50);
        outp(0x3D4,0x3);  outp(0x3D5,0x82);
        outp(0x3D4,0x4);  outp(0x3D5,0x54);
        outp(0x3D4,0x5);  outp(0x3D5,0x80);
        outp(0x3D4,0x6);  outp(0x3D5,0xBF);
        outp(0x3D4,0x7);  outp(0x3D5,0x1F);
        outp(0x3D4,0x8);  outp(0x3D5,0x00);
        outp(0x3D4,0x9);  outp(0x3D5,0x40);
        outp(0x3D4,0x10); outp(0x3D5,0x9C);
        outp(0x3D4,0x11); outp(0x3D5,0x8E);
        outp(0x3D4,0x12); outp(0x3D5,0x8F);
        outp(0x3D4,0x13); outp(0x3D5,0x28);
        outp(0x3D4,0x14); outp(0x3D5,0x00);
        outp(0x3D4,0x15); outp(0x3D5,0x96);
        outp(0x3D4,0x16); outp(0x3D5,0xB9);
        outp(0x3D4,0x17); outp(0x3D5,0xE3);
        outp(0x3C4,0x01); outp(0x3C5,0x01);
        outp(0x3C4,0x04); outp(0x3C5,0x06);
        outp(0x3CE,0x05); outp(0x3CF,0x40);
        outp(0x3CE,0x06); outp(0x3CF,0x05);
        outp(0x3CE,0x06); outp(0x3CF,0x05);
        ClearMemory();
    }

    // wait
    delay(100);

    // set fakemode palette
    if (id==FAKEMODE1A || id==FAKEMODE1B || id==FAKEMODE1C ||
        id==FAKEMODE3A || id==FAKEMODE3B || id==FAKEMODE3C)
    {
        // FAKEMODE1x and FAKEMODE3x
        outp(0x3C8,0x00);
        for (c=0; c<64; c++)
        {
            outp(0x3C9,c);
            outp(0x3C9,0);
            outp(0x3c9,0);
        }
        for (c=0; c<64; c++)
        {
            outp(0x3C9,0);
            outp(0x3C9,c);
            outp(0x3C9,0);
        }
        for (c=0; c<64; c++)
        {
            outp(0x3C9,0);
            outp(0x3C9,0);
            outp(0x3C9,c);
        }
        for (c=0; c<64; c++)
        {
            outp(0x3C9,c);
            outp(0x3C9,c);
            outp(0x3C9,c);
        }
    }
    else if (id==FAKEMODE2A || id==FAKEMODE2B || id==FAKEMODE2C)
    {
        // setup palette for FAKEMODE2x
        Palette palette;
        uint *data=(uint*)palette.Lock();
        for (int i=0; i<256; i++)
        {
            if (!(i&0x80))
            {
                // bit 7 = 0 (top section)

                // red (4 bits)
                float r = (float)((i&0x78)>>3) * 255.0 / 15.0;

 
                // green
                float g = 0;

                // blue (3 bits)
                float b = (float)(i&0x07) * 255.0 / 7.0;

                // pack it
                data[i]=RGB32((uchar)r,(uchar)g,(uchar)b);
            }
            else
            {
                // bit 7 = 1 (bottom section)
    
                // red (2 bits)
                float r = 0;//(float)((i&0x60)>>5) * 255.0 / 3.0;

                // green
                float g = (float)(i&0x1F) * 255.0 / 31.0;

                // blue
                float b = 0;
                             
                // pack it
                data[i]=RGB32((uchar)r,(uchar)g,(uchar)b);
            }
        }
        palette.Unlock();

        // set it (kludge format)                                                                                                   
        Format.init(INDEX8);
        SetPalette(palette);
    }

#endif

    // setup format info
    Format.init(id);

    // setup data
    XResolution=320;
    YResolution=200;       
    Layout=FAKEMODE;

    // initialize video memory
    if (!InitVideoMemory()) return 0;

    // init primary surface
    if (!InitPrimary()) return 0;
    
    // success
    return 1;
}


int IVGA::SetINDEX8(int x,int y)
{
    // check resolution
    if (x!=320 || y!=200) return 0;

    // set mode13h (320x200x8)
    SetMode(0x13);
    Format.init(INDEX8);
    XResolution=320;
    YResolution=200;
    Layout=LINEAR;

    // initialize video memory
    if (!InitVideoMemory()) return 0;

    // init primary surface
    if (!InitPrimary()) return 0;
    
    // success
    return 1;
}


int IVGA::SetGREY8(int x,int y)
{
    // check resolution
    if (x!=320 && y!=200) return 0;

    // enter mode 13h
    SetMode(0x13);

#ifdef __DOS32__

    // set greyscale palette
    outp(0x3C8,0x00);
    for (int c=0; c<256; c++)
    {
        outp(0x3C9,c>>2);
        outp(0x3C9,c>>2);
        outp(0x3C9,c>>2);
    }

#endif

    // initialize
    Format.init(GREY8);
    XResolution=320;
    YResolution=200;
    Layout=LINEAR;

    // initialize video memory
    if (!InitVideoMemory()) return 0;

    // init primary surface
    if (!InitPrimary()) return 0;

    // success
    return 1;
}


int IVGA::SetRGB332(int x,int y)
{
    // check resolution
    if (x!=320 && y!=200) return 0;

    // setup RGB332 palette
    Palette palette;
    uint *data=(uint*)palette.Lock();
    for (int i=0; i<256; i++)
    {
        float r = (float)((i&0xE0)>>5) * (float)255.0 / (float)7.0;
        float g = (float)((i&0x1C)>>2) * (float)255.0 / (float)7.0;
        float b = (float) (i&0x03)     * (float)255.0 / (float)3.0;
        data[i]=RGB32((uchar)r,(uchar)g,(uchar)b);
    }
    palette.Unlock();

    // enter mode 13h
    SetMode(0x13);

    // set RGB332 palette (fudge format)
    Format.init(INDEX8);
    if (!SetPalette(palette)) return 0;

    // initialize data
    Format.init(RGB332);
    XResolution=320;
    YResolution=200;
    Layout=LINEAR;

    // initialize video memory
    if (!InitVideoMemory()) return 0;

    // init primary surface
    if (!InitPrimary()) return 0;

    // success
    return 1;
}


int IVGA::SetMode(int mode)
{
#ifdef __DOS32__

    // wait for VRT
    WaitForRetrace();

    // set the mode
    DPMI::REGS regs;
    memset((void*)&regs,0,sizeof(regs));
    regs.eax=mode;
    dpmi.int86(0x10,&regs,&regs);
    NeedRestore=1;
    
#endif
    
    return 1;
}







void IVGA::ClearMemory()
{
#ifdef __DOS32__

    // lock near base
    LockNearBase();

    // wait for retrace
    WaitForRetrace();

    // clear vga memory
    uchar *dest=(uchar*)LFB;
    for (int strip=0; strip<32; strip++)
    {
        outpw(0x3C4,0x102);
        memset(dest,0,2048);
        outpw(0x3C4,0x202);
        memset(dest,0,2048);
        outpw(0x3C4,0x402);
        memset(dest,0,2048);
        outpw(0x3C4,0x802);
        memset(dest,0,2048);
        dest+=2048;
    }

    // unlock near base
    UnlockNearBase();

#endif
}







int IVGA::InitVideoMemory()
{
    // close old manager
    CloseVideoMemory();

    // initialize video memory manager
    VideoMemory.Init(LFB,0xFFFF);
    if (!VideoMemory.ok()) return 0;
    else return 1;
}


void IVGA::CloseVideoMemory()
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







int IVGA::InitPrimary()
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


void IVGA::ClosePrimary()
{
    // free primary surface
    delete PrimarySurface;
    PrimarySurface=NULL;
}







int IVGA::AddMode(List<MODE> &modelist,int x,int y,FORMAT const &format,int output,int layout)
{
    // add mode to list
    MODE *mode=new MODE;
    memset(mode,0,sizeof(MODE));
    GetName(mode->i);
    mode->x=x;
    mode->y=y;
    mode->format=format;
    mode->output=output;
    mode->frequency=UNKNOWN;
    mode->layout=layout;
    return modelist.add(mode);
}







#endif
