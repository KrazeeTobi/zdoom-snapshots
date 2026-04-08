///////////////////
// GDI interface //
///////////////////
#include "igdi.h"

#ifdef __GDI__







// static interface lookup list
List<IGDI::LOOKUP> IGDI::InterfaceLookupList;







IGDI::IGDI(WINDOW window)
     :IWin32(window)
{
    // clear status
    Status=1;

    // defaults
    Output=0;
    Frequency=UNKNOWN;
    PrimaryFlag=0;
    Primary=NULL;
    Secondary=NULL;
    LogicalPalette=NULL;
    PaletteHandle=NULL;

    // get display format
    Format=GetDisplayFormat();

    // initialize window
    if (ManagedWindow())
    {
        // register window classes
        RegisterWindowClasses();

        // add windowed mode entry
        MODE windowed=GetDisplayMode();
        windowed.output=WINDOWED;
        AddMode(windowed);

        /*
        // add fullscreen display modes
        int i=0;
        DEVMODE devmode;
        memset(&devmode,0,sizeof(devmode));
        devmode.dmSize=sizeof(devmode);
        while (EnumDisplaySettings(NULL,i,&devmode))
        {
            // setup "MODE" struct
            MODE mode;
            memset(&mode,0,sizeof(mode));
            GetName(mode.i);
            mode.x=devmode.dmPelsWidth;
            mode.y=devmode.dmPelsHeight;
            mode.layout=LINEAR;
            mode.output=FULLSCREEN;
            if ((devmode.dmFields & DM_DISPLAYFREQUENCY) && devmode.dmDisplayFrequency>1) mode.frequency=devmode.dmDisplayFrequency;
            else mode.frequency=UNKNOWN;
            switch (devmode.dmBitsPerPel)
            {
                case 8:  mode.format.init(INDEX8);   break;
                case 16: mode.format.init(RGB565);   break;
                case 24: mode.format.init(RGB888);   break;
                case 32: mode.format.init(ARGB8888); break;
            } 

            // only add mode if it is the same format as display
            if (mode.format==Format)
            {
                // add mode
                AddMode(mode);

                // if INDEX8 add GREY8 and RGB332 also...
                if (mode.format.id==INDEX8)
                {
                    mode.format=RGB332;
                    AddMode(mode);
                    mode.format=GREY8;
                    AddMode(mode);
                }
            }

            // next
            i++;
        }
        */

        // get original device mode
        OriginalDeviceMode=GetDisplayDeviceMode();

        // clear fullscreen device mode
        memset(&FullscreenDeviceMode,0,sizeof(FullscreenDeviceMode));
    }
    else
    {
        // intialize primary
        if (!InitPrimary()) Status=0;
        
        // initialize secondary
        if (!InitSecondary()) Status=0;
    }

    // initialize video for windows
    #ifdef __VFW__
    VideoForWindows=new VFW;
    #endif
}


IGDI::~IGDI()
{
    // cleanup user window
    if (!ManagedWindow())
    {
        // close surfaces
        CloseSecondary();
        ClosePrimary();
    }

    // restore original display mode
    RestoreDisplayMode();

    // close display
    CloseDisplay();

    // close palette
    ClosePalette();

    // close vfw
    #ifdef __VFW__
    delete VideoForWindows;
    #endif
}







Interface::INFO IGDI::GetInfo()
{
    // return info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}







int IGDI::SetMode(MODE const &mode)
{
    // check mode interface name
    char name[8];
    GetName(name);
    if (stricmp(mode.i,name)!=0) return 0;

    // interface name ok - set mode
    return SetMode(mode.x,mode.y,mode.format,mode.output,mode.frequency,mode.layout);
}


int IGDI::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
#ifdef __VFW__
    // initialize virtual display mode (vfw)
    if (id==VIRTUAL32 || id==VIRTUAL16 || id==VIRTUAL8 || id==DIRECT || id==INDEXED) return InitDisplay(x,y,output,frequency,layout);
#else
    // initialize virtual display mode (gdi only)
    if (((id==VIRTUAL32 || id==VIRTUAL16 || id==DIRECT || id==INDEXED) && Format.type==DIRECT) || (id==VIRTUAL8 || id==INDEXED)) return InitDisplay(x,y,output,frequency,layout);
#endif
    
    // set by exact format
    if (SetMode(x,y,FORMAT(id),output,frequency,layout)) return 1;

    // set by bits per pixel
    if (Format.bits==id) return InitDisplay(x,y,output,frequency,layout);
    else return 0;
}


int IGDI::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // initialize display (only same format as output?)
    if (format.ok() && format==Format) return InitDisplay(x,y,output,frequency,layout);
    else return 0;
}


MODE IGDI::GetMode()
{
    // get mode info
    MODE mode;
    GetName(mode.i);
    mode.x=XResolution;
    mode.y=YResolution;
    mode.format=Format;
    mode.output=Output;
    mode.frequency=Frequency;
    mode.layout=LINEAR;
    return mode;
}







int IGDI::SetPalette(Palette &palette)
{
    // enter critical section
    EnterPaletteCriticalSection();

    // check format
    int result=0; 
    if (Format.type==INDEXED)
    {
        // set palette               
        InternalPalette=palette;

        // initialize palette
        result=InitPalette();
    }
    
    // leave critical section
    LeavePaletteCriticalSection();
    return result;
}


int IGDI::GetPalette(Palette &palette)
{
    // enter critical section
    EnterPaletteCriticalSection();

    // check format
    int result=0; 
    if (Format.type==INDEXED)
    {
        // get palette
        palette=InternalPalette;
        result=1;
    }

    // leave critical section
    LeavePaletteCriticalSection();
    return result;
}







int IGDI::WaitForRetrace()
{
    return 0;
}







int IGDI::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IGDI::GetPrimary()
{
    // get primary
    return Primary;
}


int IGDI::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IGDI::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int IGDI::GetTotalVideoMemory()
{
    return 0;
}


int IGDI::GetFreeVideoMemory()
{
    return 0;
}


int IGDI::CompactVideoMemory()
{
    return 0;
}







void IGDI::GetName(char name[]) const
{
    strcpy(name,"GDI");
}


int IGDI::GetBitsPerPixel() const
{
    return Format.bits;
}


int IGDI::GetBytesPerPixel() const
{
    return Format.bytes;
}
        

int IGDI::GetOutput() const
{
    return Output;
}
        

int IGDI::GetFrequency() const
{
    return Frequency;
}


int IGDI::GetLayout() const
{
    return LINEAR;
}
        

FORMAT IGDI::GetFormat() const
{
    return Format;
}







int IGDI::ok() const
{
    // check status
    if (!IWin32::ok()) return 0;
    else return Status;
}







Interface::SURFACE* IGDI::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // create surface type
    SURFACE *surface=NULL;
    if (type==VIDEO && PrimaryFlag) surface=(SURFACE*)new PRIMARY(*this,width,height,format,type,orientation,advance,layout);
    else surface=new SURFACE(width,height,format,type,orientation,advance,layout);

    // check surface
    if (!surface || !surface->ok()) delete surface;
    else return surface;
    
    // fallback to software surface
    return ISoftware::RequestSurface(width,height,format,type,orientation,advance,layout);
}







IGDI::SURFACE::SURFACE(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // defaults
    Buffer=NULL;
    Bitmap=NULL;
    BitmapInfo=NULL;
    Count=0;

    // check parameters
    if ((width<=0 || height<=0) || !format.ok() || (type!=SYSTEM && type!=OFFSCREEN && type!=DEFAULT) ||
        (orientation!=TOPDOWN && orientation!=BOTTOMUP && orientation!=DEFAULT) || 
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=DEFAULT)) return;

    // setup new orientation
    int new_orientation=orientation;
    if (new_orientation==DEFAULT) new_orientation=BOTTOMUP;

    // setup new advance
    int new_advance=(width*format.bytes % sizeof(LONG));
    if (new_advance) new_advance=sizeof(LONG)-new_advance;
    if (new_advance!=advance && advance!=DEFAULT) return;

    // handle color model
    if (format.type==DIRECT && format.model!=RGBA) return;

    // allocate bitmap info header
    if (format.type==DIRECT && format.bits>=16)
    {
        // allocate bitmap info header
        int needs_bitfields=0;
        if (format.id==ARGB1555 || format.id==RGB888 || format.id==ARGB8888)
        {
            // default formats for 16, 24 and 32 bits per pixel
            BitmapInfo=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
        }
        else if (format.bits==16 || format.bits==32)
        {
            // BI_BITFIELDS
            needs_bitfields=1;
            BitmapInfo=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+3*sizeof(uint));
        }
        else return;
        
        // check bitmap header
        if (!BitmapInfo) return;

        // setup bitmap info header
        BitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        BitmapInfo->bmiHeader.biWidth=width;
        BitmapInfo->bmiHeader.biPlanes=1;
        BitmapInfo->bmiHeader.biBitCount=(WORD)format.bits;
        BitmapInfo->bmiHeader.biSizeImage=0;
        BitmapInfo->bmiHeader.biXPelsPerMeter=0;
        BitmapInfo->bmiHeader.biYPelsPerMeter=0;
        BitmapInfo->bmiHeader.biClrUsed=0;
        BitmapInfo->bmiHeader.biClrImportant=0;
        
        // handle orientation
        if (new_orientation==TOPDOWN) BitmapInfo->bmiHeader.biHeight=-height;
        else if (new_orientation==BOTTOMUP) BitmapInfo->bmiHeader.biHeight=height;
        else 
        {
            // failure
            free(BitmapInfo);
            BitmapInfo=NULL;
            return;
        }

        // handle bitfields
        if (needs_bitfields)
        {
            // set bitfields flag
            BitmapInfo->bmiHeader.biCompression=BI_BITFIELDS;

            // setup color component masks
            uint *mask=(uint*)(BitmapInfo->bmiColors);
            mask[0]=format.c1.mask;
            mask[1]=format.c2.mask;
            mask[2]=format.c3.mask;
        }
        else
        {
            // no bitfields required
            BitmapInfo->bmiHeader.biCompression=0;
        }
    }
    else if (format.type==INDEXED && format.bits==8)
    {
        // setup bitmap info header
        BitmapInfo = (BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
        BitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        BitmapInfo->bmiHeader.biWidth=width;
        BitmapInfo->bmiHeader.biPlanes=1;
        BitmapInfo->bmiHeader.biBitCount=(WORD)format.bits;
        BitmapInfo->bmiHeader.biCompression=BI_RGB;
        BitmapInfo->bmiHeader.biSizeImage=0;
        BitmapInfo->bmiHeader.biXPelsPerMeter=0;
        BitmapInfo->bmiHeader.biYPelsPerMeter=0;
        BitmapInfo->bmiHeader.biClrUsed=0;
        BitmapInfo->bmiHeader.biClrImportant=0;

        // handle orientation
        if (new_orientation==TOPDOWN) BitmapInfo->bmiHeader.biHeight=-height;
        else if (new_orientation==BOTTOMUP) BitmapInfo->bmiHeader.biHeight=height;
        else 
        {
            // failure
            free(BitmapInfo);
            BitmapInfo=NULL;
            return;
        }

        // clear palette
        memset(BitmapInfo->bmiColors,0,256*3);
    }
    else return;
            
    // create bitmap handle
    HDC dc=GetWindowDC(NULL);
    Bitmap=CreateDIBSection(dc,BitmapInfo,DIB_RGB_COLORS,(LPVOID*)&Buffer,NULL,0);
    ReleaseDC(NULL,dc);

    // check bitmap
    if (!Bitmap)
    {
        // failure
        free(BitmapInfo);
        BitmapInfo=NULL;
        return;
    }

    // success: setup data
    type=SYSTEM;
    orientation=new_orientation;
    advance=new_advance;
    layout=LINEAR;

    // setup lock offset
    if (orientation==TOPDOWN) LockOffset=0;
    else LockOffset=(width*format.bytes+advance)*(height-1);

    // zero lock count
    Count=0;
}


IGDI::SURFACE::~SURFACE()
{
    // free surface
    if (Bitmap)
    {
        // free bitmap
        DeleteObject(Bitmap);
        free(BitmapInfo);
    }
    else
    {
        // free memory block
        free(Buffer);
    }
}


void* IGDI::SURFACE::Lock(int wait)
{
    // advoid warnings
    if (wait);

    // wait for gdi
    GdiFlush();

    // lock
    Count++;
    return ((char*)Buffer)+LockOffset;
}


void IGDI::SURFACE::Unlock()
{
    // decrement lock count
    if (Count) Count--;
}


int IGDI::SURFACE::LockCount()
{
    // return surface lock count
    return Count;
}


int IGDI::SURFACE::Lockable()
{
    // lockable
    return 1;
}


int IGDI::SURFACE::Restore()
{
    // no restore
    return 0;
}


int IGDI::SURFACE::NativeType()
{
    // return native type
    return NATIVE_WIN32_HBITMAP;
}


void* IGDI::SURFACE::GetNative()
{
    // return native surface
    return &Bitmap;
}


int IGDI::SURFACE::ok()
{
    // surface status
    if (Buffer && Bitmap && BitmapInfo) return 1;
    else return 0;
}







IGDI::PRIMARY::PRIMARY(IGDI &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // defaults
    LocalInterface=NULL;

    // check parameters
    if ((width<=0 || height<=0) || !format.ok() || (type!=VIDEO && type!=OFFSCREEN && type!=DEFAULT) ||
        (orientation!=TOPDOWN && orientation!=BOTTOMUP && orientation!=DEFAULT) || 
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=DEFAULT)) return;

    // setup type
    type=VIDEO;

    // setup orientation
    if (orientation==DEFAULT) orientation=BOTTOMUP;

    // setup advance
    if (advance==DEFAULT) advance=0;

    // setup layout
    if (layout==DEFAULT) layout=LINEAR;

    // setup local interface
    LocalInterface=&i;
}


IGDI::PRIMARY::~PRIMARY()
{
    // cleanup
}


void* IGDI::PRIMARY::Lock(int wait)
{
    // advoid warnings
    if (wait);

    // no locking
    return NULL;
}


void IGDI::PRIMARY::Unlock()
{
}


int IGDI::PRIMARY::LockCount()
{
    // return lock count
    return 0;
}


int IGDI::PRIMARY::Lockable()
{
    // not lockable
    return 0;
}


int IGDI::PRIMARY::Restore()
{
    return 0;
}


int IGDI::PRIMARY::Clear(Surface &surface,COLOR const &color)
{
    return 0;
}


int IGDI::PRIMARY::Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color)
{
    return 0;
}


int IGDI::PRIMARY::BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return BitBlt(src,src.GetDimensions(),dest,dest.GetDimensions(),effects,extra);
}


int IGDI::PRIMARY::BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    // check for HBITMAP source
    if (src.NativeType()!=NATIVE_WIN32_HBITMAP)
    {
        // bitblt to secondary
        src.BitBlt(*(LocalInterface->Secondary),src_rect,dest_rect,effects);

        // update secondary to display
        return LocalInterface->UpdateDisplay(src,src_rect,dest_rect);
    }
    else
    {
        // update src to display
        return LocalInterface->UpdateDisplay(src,src_rect,dest_rect);
    }
}


int IGDI::PRIMARY::StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return 0;
}


int IGDI::PRIMARY::StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    return 0;
}

                
int IGDI::PRIMARY::NativeType()
{
    // no native access
    return NATIVE_UNAVAILABLE;      // todo: return HDC?
}


void* IGDI::PRIMARY::GetNative()
{
    // no native access
    return NULL;
}


int IGDI::PRIMARY::ok()
{
    // status
    return 1;
}







int IGDI::InitDisplay(int x,int y,int output,int frequency,int layout)
{
    // fail if not managed
    if (!ManagedWindow()) return 0;

    // fullscreen output is disabled!
    if (output==FULLSCREEN) return 0;

    // fail on bad output parameter
    if (output!=DEFAULT && output!=FULLSCREEN && output!=WINDOWED) return 0;

    // default output is windowed
    if (output==DEFAULT) output=WINDOWED;

    // check frequency for windowed output
    if (output==WINDOWED)
    {
        // reject non-matching frequency
        if (frequency!=UNKNOWN && frequency!=DEFAULT && frequency!=Frequency) return 0;
    }

    // default frequency is UNKNOWN
    if (frequency==DEFAULT) frequency=UNKNOWN;

    // check layout
    if (layout!=DEFAULT && layout!=LINEAR) return 0;

    // TODO: check modelist for fullscreen mode mode

    // initialize window
    if (!CreateDisplayWindow(x,y,output))
    {
        // failure
        CloseDisplayWindow();
        return 0;
    }

    // todo: set display mode for fullscreen

    // show display window
    if (!ShowDisplayWindow(x,y,output))
    {
        // failure
        CloseDisplayWindow();
        return 0;
    }

    // setup data
    XResolution=x;
    YResolution=y;
    Output=output;
    Frequency=frequency;

    // initialize primary and secondary
    if (!InitPrimary() || !InitSecondary())
    {
        // failure
        CloseDisplayWindow();
        return 0;
    }

    // success
    return 1;
}


void IGDI::CloseDisplay()
{
    // close display window
    CloseDisplayWindow();

    // close secondary
    CloseSecondary();

    // close primary
    ClosePrimary();
}







int IGDI::CreateDisplayWindow(int width,int height,int output)
{
    // check managed window
    HWND window=GetManagedWindow();

    // check for existing window (windowed output)
    if (window && output==WINDOWED)
    {
        // resize existing window
        if (ResizeDisplayWindow(width,height,output)) return 1;
    }
    else if (window && output==FULLSCREEN)
    {
        // do something
    }

    // close old
    CloseDisplayWindow();

    // compensate for window title and borders
    AdjustWindowSize(width,height,output);

    // specific initialization
    if (output==WINDOWED)
    {
        // create windowed output window
        if (!CreateManagedWindow("PTC_GDI_WINDOWED",WS_OVERLAPPEDWINDOW,SW_NORMAL,CW_USEDEFAULT,CW_USEDEFAULT,width,height)) return 0;
    }
    else if (output==FULLSCREEN)
    {
        // create fullscreen output window
        if (!CreateManagedWindow("PTC_GDI_FULLSCREEN",WS_POPUPWINDOW,SW_HIDE,0,0,width,height)) return 0;
    }
    else return 0;

    // success
    return 1;
}


int IGDI::ShowDisplayWindow(int width,int height,int output)
{
    // get the managed window
    HWND window=GetManagedWindow();
    if (!window) return 0;

    // show window
    if (output==FULLSCREEN)
    {
        // show the window
        ShowWindow(window,SW_SHOWMAXIMIZED);
    }

    // register window for interface lookup
    if (!RegisterWindow(window))
    {                       
        // failure
        CloseDisplayWindow();
        return 0;
    }
    
    // success
    return 1;
}


int IGDI::ResizeDisplayWindow(int width,int height,int output)
{
    // adjust window size
    AdjustWindowSize(width,height,output);

    // get managed window
    HWND window=GetManagedWindow();
    if (!window) return 0;

    // check output type
    if (output==WINDOWED)
    {
        // check if resize is possible without going off screen
        RECT rect;
        GetWindowRect(window,&rect);
        MODE mode=GetDisplayMode();
        if (rect.left+width>mode.x || rect.top+height>mode.y) return 0;
    }

    // resize managed window
    return ResizeManagedWindow(width,height);
}


void IGDI::CloseDisplayWindow()
{   
    // check if managed
    if (!ManagedWindow()) return;

    // nice cleanup for palette based display
    if (PaletteHandle)
    {
        // clear recent
        ClearRecent();

        // clear window with black
        PostMessage(GetManagedWindow(),WM_PAINT,0,0);
    }

    // close managed window
    CloseManagedWindow();
}







int IGDI::InitPrimary()           
{
    // enter critical section
    EnterWindowCriticalSection();
    
    // close old primary
    ClosePrimary();

    // result
    int result=1;

    // set primary flag
    PrimaryFlag=1;

    // initialize new video memory surface (primary)
    Primary=new Surface(this,XResolution,YResolution,Format,VIDEO);
    if (!Primary || !Primary->ok()) 
    {
        // failure
        ClosePrimary();
        result=0;
    }

    // clear primary flag
    PrimaryFlag=0;

    // leave critical section
    LeaveWindowCriticalSection();
    
    // finished
    return result;
}


void IGDI::ClosePrimary()
{
    // enter critical section
    EnterWindowCriticalSection();
    
    // close primary
    delete Primary;
    Primary=NULL;

    // leave critical section
    LeaveWindowCriticalSection();
}







int IGDI::InitSecondary()           
{
    // enter critical section
    EnterWindowCriticalSection();
    
    // close old
    CloseSecondary();

    // result
    int result=1;

    // create HBITMAP based surface
    Secondary=new Surface(this,XResolution,YResolution,Format);
    if (!Secondary || !Secondary->ok() || Secondary->NativeType()!=NATIVE_WIN32_HBITMAP)
    {
        // failure
        CloseSecondary();
        result=0;
    }
    
    // leave critical section
    LeaveWindowCriticalSection();
    
    // finished
    return result;
}


void IGDI::CloseSecondary()
{
    // enter critical section
    EnterWindowCriticalSection();

    // close secondary
    delete Secondary;
    Secondary=NULL;

    // leave critical section
    LeaveWindowCriticalSection();
}







int IGDI::InitPalette()
{
    // enter critical section
    EnterPaletteCriticalSection();
    
    // close old
    ClosePalette();

    // setup logical palette
    int i=0;
    int result=0;
    uchar *data=(uchar*)InternalPalette.Lock();
    if (!data) goto DONE;
    LogicalPalette=(LOGPALETTE*)malloc(sizeof(LOGPALETTE)+255*sizeof(PALETTEENTRY));
    LogicalPalette->palVersion=0x0300;
    LogicalPalette->palNumEntries=256;
    for (i=0; i<256; i++)
    {
        LogicalPalette->palPalEntry[i].peRed   = data[i*4+2];
        LogicalPalette->palPalEntry[i].peGreen = data[i*4+1];
        LogicalPalette->palPalEntry[i].peBlue  = data[i*4+0];
        LogicalPalette->palPalEntry[i].peFlags = 0;
    }
    InternalPalette.Unlock();
                    
    // create palette handle
    PaletteHandle=CreatePalette(LogicalPalette);
    if (PaletteHandle) result=1;

    // send message to update palette
    PostMessage(GetManagedWindow(),WM_PALETTECHANGED,0,0);

DONE:

    // leave critical section
    LeavePaletteCriticalSection();
    return result;
}

                
void IGDI::ClosePalette()
{
    // enter critical section
    EnterPaletteCriticalSection();

    // free palettes
    free(LogicalPalette);
    if (PaletteHandle) DeleteObject(PaletteHandle);
    LogicalPalette=NULL;
    PaletteHandle=NULL;

    // leave critical section
    LeavePaletteCriticalSection();
}







int IGDI::RegisterWindowClasses()
{
    // register window class (windowed output)
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WndProcWindowed;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;       
    wc.hInstance     = NULL;
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL),"IDI_PTC_ICON");
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "PTC_GDI_WINDOWED";
    int result_windowed=RegisterClass(&wc);

    // register window class (fullscreen output)
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WndProcFullscreen;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL),"IDI_PTC_ICON");
    wc.hCursor       = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "PTC_GDI_FULLSCREEN";
    int result_fullscreen=RegisterClass(&wc);

    // check results
    if (!result_windowed || !result_fullscreen) return 0;
    else return 1;
}


void IGDI::AdjustWindowSize(int &width,int &height,int output)
{
    if (output==WINDOWED)
    {
        // get system metrics
        int frame_x=GetSystemMetrics(SM_CXFRAME);
        int frame_y=GetSystemMetrics(SM_CYFRAME);
        int title_y=GetSystemMetrics(SM_CYCAPTION);

        // adjust size
        width+=frame_x*2;
        height+=frame_y*2+title_y;
    }
}


LRESULT CALLBACK IGDI::WndProcCommon(IGDI *i,HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // message handler
    if (i)
    {
        switch (message) 
        { 
            case WM_QUERYNEWPALETTE:
            {
                // check
                if (i)
                {
                    // enter palette critical section
                    i->EnterPaletteCriticalSection();
                
                    // check palette
                    if (i->PaletteHandle)
                    {
                        // realize palette
                        HDC dc=GetWindowDC(hWnd);
                        HPALETTE OldPalette=SelectPalette(dc,i->PaletteHandle,FALSE);
                        if (RealizePalette(dc)) SendMessage(hWnd,WM_PAINT,0,0);
                        SelectPalette(dc,OldPalette,TRUE);
                        ReleaseDC(hWnd,dc);
                    }

                    // leave palette critical section
                    i->LeavePaletteCriticalSection();
                }
                break;
            }

            case WM_PALETTECHANGED:
            {
                // check
                if (i)
                {
                    // enter palette critical section
                    i->EnterPaletteCriticalSection();

                    // check palette
                    if (i->PaletteHandle && (HWND)wParam!=hWnd)
                    {
                        // palette changed
                        HDC dc=GetWindowDC(hWnd);
                        HPALETTE OldPalette=SelectPalette(dc,i->PaletteHandle,TRUE);
                        if (RealizePalette(dc)) SendMessage(hWnd,WM_PAINT,0,0);
                        SelectPalette(dc,OldPalette,TRUE);
                        ReleaseDC(hWnd,dc);
                    }

                    // leave palette critical section
                    i->LeavePaletteCriticalSection();
                }
                break;
            }

            default:
            {
                // unhandled messages
                return IWin32::WndProc(i,hWnd,message,wParam,lParam);
            }
        }
    }
    else
    {
        // unhandled messages
        return IWin32::WndProc(i,hWnd,message,wParam,lParam);
    }

    // done
    return 0;
}


LRESULT CALLBACK IGDI::WndProcWindowed(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // lookup interface
    IGDI *i=LookupInterface(hWnd);
        
    // enter window critical section
    if (i) i->EnterWindowCriticalSection();

    // locals
    int result=0;

    // common functionality
    switch (message)
    {
        case WM_CREATE:

            // force window focus!

        default:
            
            // common functionality
            result=WndProcCommon(i,hWnd,message,wParam,lParam);
    }
    
    // leave window critical section                                       
    if (i) i->LeaveWindowCriticalSection();
    return result;
}


LRESULT CALLBACK IGDI::WndProcFullscreen(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // lookup interface
    IGDI *i=LookupInterface(hWnd);
        
    // enter window critical section
    if (i) i->EnterWindowCriticalSection();

    // locals
    int result=0;

    /*
    // message handler
    switch (message) 
    { 
        case WM_CREATE:
    
            // hide cursor
            SetCursor(NULL);
            break;

        case WM_ACTIVATE:

            // handle focus switching
            if (wParam==WA_ACTIVE && i)
            {
                // set window as topmost
                SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); 

                // switch to fullscreen display mode
                i->SetDisplayMode(i->FullscreenDeviceMode);

                // hide cursor
                SetCursor(NULL);

                // show display window
                ShowWindow(hWnd,SW_MAXIMIZE); 
            }
            else if (wParam==WA_INACTIVE && i)
            {
                // switch to original display mode
                i->SetDisplayMode(i->OriginalDeviceMode);
                    
                // set window as bottom
                SetWindowPos(hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); 
                
                // minimize display window
                ShowWindow(hWnd,SW_MINIMIZE); 
            }

        default:

            // common functionality
            result=WndProcCommon(i,hWnd,message,wParam,lParam);
    }
    */

    // leave window critical section                                       
    if (i) i->LeaveWindowCriticalSection();
    return result;
}







int IGDI::SetDisplayMode(int x,int y,int frequency)
{
    // check that mode change is needed
    DEVMODE devmode=GetDisplayDeviceMode();
    if (devmode.dmPelsWidth==(uint)x && devmode.dmPelsHeight==(uint)y && devmode.dmDisplayFrequency==(uint)frequency) return 1;

    // setup devmode
    memset(&devmode,0,sizeof(devmode));
    devmode.dmSize=sizeof(devmode);
    devmode.dmPelsWidth=x;
    devmode.dmPelsHeight=y;
    if (frequency==DEFAULT || frequency==UNKNOWN)
    {
        // default frequency
        devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    }
    else
    {
        // specific frequency
        devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
        devmode.dmDisplayFrequency=frequency;
    }

    // set display mode
    return SetDisplayMode(devmode);
}


int IGDI::SetDisplayMode(DEVMODE devmode)
{
    // disabled
    if (devmode.dmSize) return 1;
    else return  0;

    // change display settings
    /*
    if (ChangeDisplaySettings(&devmode,0)==DISP_CHANGE_SUCCESSFUL) 
    {
        // success
        return 1;
    }
    else return 0;
    */
}


void IGDI::RestoreDisplayMode()
{
    // restore original display mode    
    SetDisplayMode(OriginalDeviceMode); 
}


MODE IGDI::GetDisplayMode()
{
    // get display device mode
    DEVMODE devmode=GetDisplayDeviceMode();

    // get display mode
    MODE mode;
    memset(&mode,0,sizeof(mode));
    GetName(mode.i);
    mode.x=devmode.dmPelsWidth;
    mode.y=devmode.dmPelsHeight;
    mode.format=GetDisplayFormat();
    mode.layout=LINEAR;
    mode.output=FULLSCREEN;
    mode.frequency=devmode.dmDisplayFrequency;
    return mode;
}


DEVMODE IGDI::GetDisplayDeviceMode()
{
    // get device mode
    DEVMODE devmode;
    memset(&devmode,0,sizeof(devmode));
    devmode.dmSize=sizeof(devmode);
    devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
    devmode.dmPelsWidth=GetSystemMetrics(SM_CXSCREEN);
    devmode.dmPelsHeight=GetSystemMetrics(SM_CYSCREEN);
    devmode.dmDisplayFrequency=GetDisplayFrequency();
    return devmode;
}


int IGDI::GetDisplayFrequency()
{
    // get display frequency
    HDC dc=GetDC(NULL);
    int frequency=GetDeviceCaps(dc,VREFRESH);
    if (frequency==0 || frequency==1) frequency=UNKNOWN;
    ReleaseDC(NULL,dc);
    return frequency;
}

FORMAT IGDI::GetDisplayFormat()
{
    // get display format
    HDC dc=GetDC(NULL);
    FORMAT format=GetDeviceFormat(dc);
    ReleaseDC(NULL,dc);
    return format;
}


FORMAT IGDI::GetDeviceFormat(HDC dc)
{
    // get device format
    int bits=GetDeviceCaps(dc,BITSPIXEL);
    switch (bits)
    {
        case 32: return FORMAT(ARGB8888);
        case 24: return FORMAT(RGB888);
        case 16: return FORMAT(RGB565);
        case 8:  return FORMAT(INDEX8);
        default: return FORMAT();
    }
}







int IGDI::RegisterWindow(HWND window)
{
    // enter critical section
    EnterStaticCriticalSection();

    // register for interface lookup
    LOOKUP *lookup=new LOOKUP;
    lookup->i=this;
    lookup->window=window;
    int result=InterfaceLookupList.add(lookup);

    // leave critical section
    LeaveStaticCriticalSection();
    return result;
}


int IGDI::UnregisterWindow(HWND window)
{
    // enter critical section
    EnterStaticCriticalSection();

    // unregister window
    int result=0;
    List<LOOKUP>::Iterator iterator=InterfaceLookupList.first();
    LOOKUP *current=iterator.current();
    while (current)
    {
        // check for window match
        if (current->window==window)
        {
            iterator.free();
            result=1;
            goto DONE;
        }
        
        // next
        current=iterator.next();
    }

DONE:
    
    // leave critical section
    LeaveStaticCriticalSection();
    return result;
}


IGDI* IGDI::LookupInterface(HWND window)
{
    // enter critical section
    EnterStaticCriticalSection();

    // lookup interface via window handle
    IGDI *i=NULL;
    List<LOOKUP>::Iterator iterator=InterfaceLookupList.first();
    LOOKUP *current=iterator.current();
    while (current)
    {
        // check for window match
        if (current->window==window)
        {
            i=current->i;
            goto DONE;
        }
        
        // next
        current=iterator.next();
    }
    
DONE: 
    
    // leave critical section
    LeaveStaticCriticalSection();
    return i;
}







int IGDI::UpdateDisplay(Surface &src,RECTANGLE const &src_rect,RECTANGLE const &dest_rect)
{
    // enter critical section
    EnterUpdateCriticalSection();

    // locals
    int result=0;
    int recent=0;
    HWND window=GetManagedWindow();
    HDC window_dc=GetManagedWindowDC();

    // get window res
    RECT window_rect;
    GetClientRect(window,&window_rect);
    int xres=window_rect.right-window_rect.left;
    int yres=window_rect.bottom-window_rect.top;

    // calculate stretch ratios
    float x_ratio = (float)xres / (float)dest_rect.width();
    float y_ratio = (float)yres / (float)dest_rect.height();

    // setup stretched coords
    float stretched_x1 = (float)dest_rect.x1 * x_ratio;
    float stretched_y1 = (float)dest_rect.y1 * y_ratio;
    float stretched_x2 = (float)dest_rect.x2 * x_ratio;
    float stretched_y2 = (float)dest_rect.y2 * y_ratio;

    // setup stretched destination rectangle
    RECTANGLE stretched_rect( (int) ceil(stretched_x1),
                              (int) ceil(stretched_y1),
                              (int) ceil(stretched_x2),
                              (int) ceil(stretched_y2) );

    // clip stretched (hack?)
    if (stretched_rect.x2>xres) stretched_rect.x2=xres;
    if (stretched_rect.y2>yres) stretched_rect.y2=yres;

    // check if full window update (for recent)
    if (stretched_rect.x1==0 && stretched_rect.y1==0 && stretched_rect.x2==xres && stretched_rect.y2==yres) recent=1;

    // get internal surface
    SURFACE *surface=(SURFACE*)src.InternalSurface;

    // check native surface type
    if (src.NativeType()!=NATIVE_WIN32_HBITMAP) goto DONE;

    // perform bitblt
    if (surface->Bitmap)
    {
        if (src.Format.type==INDEXED)
        {
            if (Format.type==DIRECT)
            {
                // INDEXED -> DIRECT

                // locals
                void *data;

                // video for windows drawdib
                #ifdef __VFW__
                    
                // only bottomup dibs are supported
                if (src.Orientation==BOTTOMUP)
                {
                    // setup dib color table entries
                    data=src.LocalPalette->Lock();
                    if (!data) goto DONE;
                    memcpy(surface->BitmapInfo->bmiColors,data,256*4);
                    src.LocalPalette->Unlock();

                    // drawdib to screen
                    if (VideoForWindows) result=VideoForWindows->DrawDib(window_dc,src_rect,stretched_rect,&surface->BitmapInfo->bmiHeader,surface->Buffer);
                    if (result) goto DONE;
                }

                #endif

                // create bitmap dc
                HDC bitmap_dc=CreateCompatibleDC(window_dc);
                HBITMAP old_bitmap=(HBITMAP)SelectObject(bitmap_dc,surface->Bitmap);
                if (!old_bitmap) goto DONE;

                // set bitmap palette (yuck)
                data=src.LocalPalette->Lock();
                if (!data) goto DONE;
                SetDIBColorTable(bitmap_dc,0,256,(RGBQUAD*)data);
                src.LocalPalette->Unlock();

                // stretch blt to window
                result=::StretchBlt(window_dc,stretched_rect.x1,stretched_rect.y1,stretched_rect.width(),stretched_rect.height(),
                                    bitmap_dc,src_rect.x1,src_rect.y1,src_rect.width(),src_rect.height(),SRCCOPY);
                    
                // cleanup bitmap dc
                SelectObject(bitmap_dc,old_bitmap);
                DeleteDC(bitmap_dc);
                goto DONE;
            }
            else
            {                                          
                // INDEXED -> INDEXED
            
                // check palette
                if (!LogicalPalette || !PaletteHandle) goto DONE;

                // video for windows drawdib
                #ifdef __VFW__
                    
                // only bottomup dibs are supported
                if (src.Orientation==BOTTOMUP)
                {
                    // setup dib color table entries
                    void *data=src.LocalPalette->Lock();
                    if (!data) goto DONE;
                    memcpy(surface->BitmapInfo->bmiColors,data,256*4);
                    src.LocalPalette->Unlock();

                    // drawdib to screen
                    if (VideoForWindows) result=VideoForWindows->DrawDib(window_dc,src_rect,stretched_rect,&surface->BitmapInfo->bmiHeader,surface->Buffer);
                    if (result) goto DONE;
                }

                #endif
                
                // create bitmap dc
                HDC bitmap_dc=CreateCompatibleDC(window_dc);
                HBITMAP old_bitmap=(HBITMAP)SelectObject(bitmap_dc,surface->Bitmap);
                if (!old_bitmap) goto DONE;

                // set bitmap palette to display palette (ick)
                SetDIBColorTable(bitmap_dc,0,256,(RGBQUAD*)LogicalPalette->palPalEntry);
                  
                // select palette
                HPALETTE old_palette=SelectPalette(window_dc,PaletteHandle,FALSE);
                if (!old_palette) goto DONE;

                // stretch blt to window
                result=::StretchBlt(window_dc,stretched_rect.x1,stretched_rect.y1,stretched_rect.width(),stretched_rect.height(),
                                    bitmap_dc,src_rect.x1,src_rect.y1,src_rect.width(),src_rect.height(),SRCCOPY);
                   
                // restore palette
                SelectPalette(window_dc,old_palette,TRUE);

                // cleanup bitmap dc
                SelectObject(bitmap_dc,old_bitmap);
                DeleteDC(bitmap_dc);
                    
                // done
                goto DONE;
            }
        }
        else
        {
            // DIRECT -> X
        
            #ifdef __VFW__

            // video for windows drawdib
            if (VideoForWindows && src.Orientation==BOTTOMUP)
            {
                if (Format.type==DIRECT)
                {
                    // DIRECT -> DIRECT
                    result=VideoForWindows->DrawDib(window_dc,src_rect,stretched_rect,&surface->BitmapInfo->bmiHeader,surface->Buffer);
                }
                else if (Format.id==INDEX8)    //type==INDEXED)
                {
                    // DIRECT -> INDEXED (dithered)
                    result=VideoForWindows->DrawDib(window_dc,src_rect,stretched_rect,&surface->BitmapInfo->bmiHeader,surface->Buffer,VFW::HALFTONE);
                }

                // done                             
                if (result) goto DONE;
            }        

            #endif

            // create bitmap dc
            HDC bitmap_dc=CreateCompatibleDC(window_dc);
            HBITMAP old_bitmap=(HBITMAP)SelectObject(bitmap_dc,surface->Bitmap);
            if (!old_bitmap) goto DONE;

            // stretch blt to display
            result=::StretchBlt(window_dc,stretched_rect.x1,stretched_rect.y1,stretched_rect.width(),stretched_rect.height(),
                                bitmap_dc,src_rect.x1,src_rect.y1,src_rect.width(),src_rect.height(),SRCCOPY);
                    
            // cleanup bitmap dc
            SelectObject(bitmap_dc,old_bitmap);
            DeleteDC(bitmap_dc);
        }
    }

DONE:

    // update recent if specified
    if (result && recent) SetRecent(src,src_rect,dest_rect);

    // leave critical section
    LeaveUpdateCriticalSection();

    // sleep
    Sleep(0);

    // finished
    return result;
}








#endif
