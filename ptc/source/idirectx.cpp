///////////////////////
// DirectX interface //
///////////////////////
#include "idirectx.h"

#ifdef __DIRECTX__







// static interface lookup list
List<IDirectX::LOOKUP> IDirectX::InterfaceLookupList;







IDirectX::IDirectX(WINDOW window)
         :IWin32(window)
{
    // debug
    //printf("IDirectX::IDirectX\n"); 

    // clear status
    Status=0;

    // defaults
    Output=0;
    Frequency=UNKNOWN;
    PrimaryFlag=0;
    Primary=NULL;
    PrimaryBackup=NULL;
    
    // directx data
    lpDD=NULL;
    lpDD2=NULL;
    DirectDrawInstance=NULL;

    // initialize directdraw
    if (!InitDirectDraw()) 
    {
        //printf("InitDirectDraw failed\n");
        return;
    }

    // get display format

    // initialize window
    if (ManagedWindow())
    {
        // register window classes
        RegisterWindowClasses();

        // setup mode list
        SetupModeList();

        // windowed display mode
    }

    // success
    Status=1;

    //printf("IDirectX::IDirectX ok\n");

    // set process priority to high
    SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
}


IDirectX::~IDirectX()
{
    // set process priority to normal
    SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);

    // close directdraw
    CloseDirectDraw();
}







Interface::INFO IDirectX::GetInfo()
{
    // return info
    INFO info;
    memset(&info,0,sizeof(info));
    return info;
}







int IDirectX::SetMode(MODE const &mode)
{
    // check mode interface name
    char name[8];
    GetName(name);
    if (stricmp(mode.i,name)!=0) return 0;

    // interface name ok - set mode
    return SetMode(mode.x,mode.y,mode.format,mode.output,mode.frequency,mode.layout);
}


int IDirectX::SetMode(int x,int y,int id,int output,int frequency,int layout)
{
    // debug
    //printf("IDirectX::SetMode\n");

    // DIRECT -> VIRTUAL32
    if (id==DIRECT) id=VIRTUAL32;

    // INDEXED -> VIRTUAL8
    if (id==INDEXED) id=VIRTUAL8;

    // try modeset from virtual mode / bits per pixel
    if (InitDisplay(x,y,id,layout,frequency,layout)) return 1;
    
    // attempt modeset from format id
    if (InitDisplay(x,y,FORMAT(id),layout,frequency,layout)) return 1;

    // failure
    return 0;
}


int IDirectX::SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    // debug
    //printf("IDirectX::SetMode\n");

    // initialize display
    if (InitDisplay(x,y,format,output,frequency,layout))
    {
        //printf("IDirectX::SetMode ok\n");
        return 1;
    }
    else return 0;
}


MODE IDirectX::GetMode()
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







int IDirectX::SetPalette(Palette &palette)
{
    // enter critical section
    EnterPaletteCriticalSection();

    // clear success flag
    int success=0; 
    
    // check format
    if (Format.type==INDEXED)
    {
        // get palette index table in ABGR8888 format
        FORMAT format(ABGR8888);
        uint *data=(uint*)palette.GetIndexTable(format,4);
        if (!data) goto DONE;

        // check primary surface
        if (!Primary) goto DONE;

        // get internal IDirectX::SURFACE from primary surface
        SURFACE *surface=(SURFACE*)Primary->InternalSurface;
        if (!surface) goto DONE;

        // grab primary surface palette object
        HRESULT result=~DD_OK;
        LPDIRECTDRAWPALETTE lpDDPalette=NULL;
        if (surface->lpDDS2) result=surface->lpDDS2->GetPalette(&lpDDPalette);
        else if (surface->lpDDS) result=surface->lpDDS->GetPalette(&lpDDPalette);
        else goto DONE;

        // check if palette is attached
        if (result==DDERR_NOPALETTEATTACHED)
        {
            // debug
            //printf("no palette attached: creating new lpDDPalette\n");

            // create palette object
            if (lpDD2)
            {
                // lpDD2 palette
                result=lpDD2->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256,(LPPALETTEENTRY)data,&lpDDPalette,NULL);
                if (result!=DD_OK)
                {
                    //printf("lpDD2->CreatePalette failed\n");
                    goto DONE;
                }
            }
            else if (lpDD)
            {
                // lpDD palette
                result=lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256,(LPPALETTEENTRY)data,&lpDDPalette,NULL);
                if (result!=DD_OK)
                {
                    //printf("lpDD->CreatePalette failed\n");
                    goto DONE;
                }
            }
            
            // attach palette object to primary surface
            result=~DD_OK;
            if (surface->lpDDS2) result=surface->lpDDS2->SetPalette(lpDDPalette);
            else if (surface->lpDDS) result=surface->lpDDS->SetPalette(lpDDPalette);
            
            // check result
            if (result!=DD_OK)
            {
                //printf("failed to attach palette to primary surface\n");
                goto DONE;
            }
        }
        else if (result!=DD_OK) goto DONE;

        // set palette entries
        result=lpDDPalette->SetEntries(0,0,256,(LPPALETTEENTRY)data);
        if (result!=DD_OK) goto DONE;

        // update primary palette
        PrimaryPalette=palette;

        // success
        success=1;
    }  
    
DONE:

    // leave critical section
    LeavePaletteCriticalSection();
    return success;
}


int IDirectX::GetPalette(Palette &palette)
{
    // clear success flag
    int success=0; 

    // check format
    if (Format.type==INDEXED)
    {
        // check primary surface
        if (!Primary)
        {
            //printf("bad primary\n");
            goto DONE;
        }

        // get internal IDirectX::SURFACE from primary surface
        SURFACE *surface=(SURFACE*)Primary->InternalSurface;
        if (!surface)
        {
            //printf("bad internal surface\n");
            goto DONE;
        }

        // grab primary surface palette object
        LPDIRECTDRAWPALETTE lpDDPalette;
        HRESULT result=~DD_OK;
        if (surface->lpDDS2) result=surface->lpDDS2->GetPalette(&lpDDPalette);
        else if (surface->lpDDS) result=surface->lpDDS->GetPalette(&lpDDPalette);
        if (result!=DD_OK) 
        {
            //printf("GetPalette failed\n");
            goto DONE;
        }

        // lock palette
        uint *data=(uint*)palette.Lock();
        if (!data)
        {
            //printf("bad palette lock\n");
            goto DONE;
        }

        // get palette entries
        result=lpDDPalette->GetEntries(0,0,256,(LPPALETTEENTRY)data);
        if (result!=DD_OK)
        {
            //printf("lpDDPalette->GetEntries failed\n");
            goto DONE;
        }

        // adjust palette data (switch red and blue bytes)
        uchar *p=(uchar*)data;
        uchar *pmax=p+1024;
        while (p<pmax)
        {
            char temp=*p;   // temp = r
            *p=*(p+2);      // r = b
            *(p+2)=temp;    // b = r
            p+=4;
        }
    
        // unlock palette
        palette.Unlock();

        // success
        success=1;
    }

DONE:

    // leave critical section
    LeavePaletteCriticalSection();
    return success;
}







int IDirectX::WaitForRetrace()
{
    // wait for vertical retrace
    if (lpDD2)
    {
        // lpDD2
        if (lpDD2->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,NULL)==DD_OK) return 1;
        else return 0;
    }
    else if (lpDD)
    {
        // lpDD
        if (lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,NULL)==DD_OK) return 1;
        else return 0;
    }
    else return 0;
}







int IDirectX::SetPrimary(Surface &surface)
{
    // advoid warnings
    if (surface.ok());
    return 0;
}


Surface* IDirectX::GetPrimary()
{
    // get primary
    return Primary;
}


int IDirectX::SetOrigin(int x,int y)
{
    // advoid warnings
    if (x || y);
    return 0;
}


int IDirectX::GetOrigin(int &x,int &y)
{
    x=0;
    y=0;
    return 0;
}







int IDirectX::GetTotalVideoMemory()
{
    return 0;
}


int IDirectX::GetFreeVideoMemory()
{
    return 0;
}


int IDirectX::CompactVideoMemory()
{
    return 0;
}







int IDirectX::NativeType()
{
    return NATIVE_WIN32_LPDIRECTDRAW;
}


void* IDirectX::GetNative()
{
    return (void*)&lpDD;
}







void IDirectX::GetName(char name[]) const
{
    strcpy(name,"DirectX");
}


int IDirectX::GetBitsPerPixel() const
{
    return Format.bits;
}


int IDirectX::GetBytesPerPixel() const
{
    return Format.bytes;
}
        

int IDirectX::GetOutput() const
{
    return Output;
}
        

int IDirectX::GetFrequency() const
{
    return Frequency;
}


int IDirectX::GetLayout() const
{
    return LINEAR;
}
        

FORMAT IDirectX::GetFormat() const
{
    return Format;
}







int IDirectX::ok() const
{
    // check status
    if (!IWin32::ok()) return 0;
    else return Status;
}





    

Interface::SURFACE* IDirectX::RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // create sureface type
    SURFACE *surface=new SURFACE(*this,width,height,format,type,orientation,advance,layout);

    // check surface
    if (!surface || !surface->ok()) delete surface;
    else return surface;
    
    // fallback to software surface
    return ISoftware::RequestSurface(width,height,format,type,orientation,advance,layout);
}







IDirectX::SURFACE::SURFACE(IDirectX &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout)
{
    // debug
    //printf("IDirectX::SURFACE::SURFACE\n");

    // defaults
    Buffer=NULL;
    lpDDS=NULL;
    lpDDS2=NULL;
    Count=0;
    Primary=0;

    // setup local interface
    LocalInterface=&i;

    // check parameters
    if ((width<=0 || height<=0) || (type!=SYSTEM && type!=VIDEO && type!=OFFSCREEN && type!=DEFAULT) ||
        (orientation!=TOPDOWN && orientation!=DEFAULT) ||
        (advance!=DEFAULT && advance<0) || (layout!=LINEAR && layout!=DEFAULT)) 
    {
        //printf("bad parameters\n");
        return;
    }

    // setup new type
    int new_type=type;
    if (new_type==DEFAULT || new_type==OFFSCREEN) new_type=SYSTEM;      // todo: proper OFFSCREEN->VIDEO/SYSTEM

    // setup new orientation
    int new_orientation=orientation;
    if (new_orientation==DEFAULT) new_orientation=TOPDOWN;

    // setup new advance
    int new_advance=advance;

    // create directdraw surface descriptor
    DDSURFACEDESC desc;
    memset(&desc,0,sizeof(desc));
    desc.dwSize=sizeof(desc);

    // setup surface descriptor
    if (new_type==SYSTEM)
    {
        // check pixel format
        if (!format.ok() || (format.type==DIRECT && format.model!=RGBA)) 
        {
            //printf("bad color model\n");
            return;
        }

        // setup surface format information
        i.Translate(format,desc.ddpfPixelFormat);

        // system surface
        desc.dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        desc.ddsCaps.dwCaps=DDSCAPS_SYSTEMMEMORY;
        desc.dwWidth=width;
        desc.dwHeight=height;
    }
    else if (new_type==VIDEO)
    {
        // check primary flag
        if (i.PrimaryFlag)
        {
            // primary surface
            desc.dwFlags=DDSD_CAPS;
            desc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;        // todo: handle primary in system memory on some cards?
            desc.dwWidth=width;
            desc.dwHeight=height;

            // set primary
            Primary=1;
        }
        else
        {
            // VIDEO surface
            //printf("VIDEO surfaces are not supported\n");
            return;
        }
    }
        
    // create directdraw surface
    HRESULT result=~DD_OK;
    if (i.lpDD2) result=i.lpDD2->CreateSurface(&desc,&lpDDS,NULL);
    else if (i.lpDD) result=i.lpDD->CreateSurface(&desc,&lpDDS,NULL);
 
    /*
    // print error infomation
    switch (result)
    {
        case DDERR_INCOMPATIBLEPRIMARY: printf("DDERR_INCOMPATIBLEPRIMARY\n"); break;
        case DDERR_INVALIDCAPS: printf("DDERR_INVALIDCAPS\n"); break;
        case DDERR_INVALIDOBJECT: printf("DDERR_INVALIDOBJECT\n"); break;
        case DDERR_INVALIDPARAMS: printf("DDERR_INVALIDPARAMS\n"); break;
        case DDERR_INVALIDPIXELFORMAT: printf("DDERR_INVALIDPIXELFORMAT\n"); break;
        case DDERR_NOALPHAHW: printf("DDERR_NOALPHAHW\n"); break;
        case DDERR_NOCOOPERATIVELEVELSET: printf("DDERR_NOCOOPERATIVELEVELSET\n"); break;
        case DDERR_NODIRECTDRAWHW: printf("DDERR_NODIRECTDRAWHW\n"); break;
        case DDERR_NOEMULATION: printf("DDERR_NOEMULATION\n"); break;
        case DDERR_NOEXCLUSIVEMODE: printf("DDERR_NOEXCLUSIVEMODE\n"); break; 
        case DDERR_NOFLIPHW: printf("DDERR_NOFLIPHW\n"); break; 
        case DDERR_NOMIPMAPHW: printf("DDERR_NOMIPMAPHW\n"); break; 
        case DDERR_NOZBUFFERHW: printf("DDERR_NOZBUFFERHW\n"); break; 
        case DDERR_OUTOFMEMORY: printf("DDERR_OUTOFMEMORY\n"); break; 
        case DDERR_OUTOFVIDEOMEMORY: printf("DDERR_OUTOFVIDEOMEMORY\n"); break; 
        case DDERR_PRIMARYSURFACEALREADYEXISTS: printf("DDERR_PRIMARYSURFACEALREADYEXISTS\n"); break; 
        case DDERR_UNSUPPORTEDMODE: printf("DDERR_UNSUPPORTEDMODE\n"); break;
    }
    */
    
    // check result
    if (result!=DD_OK)
    {
        // failure
        //printf("result!=DD_OK\n");
        lpDDS=NULL;
        return;
    }
    
    // get surface descriptor
    desc.dwSize=sizeof(DDSURFACEDESC);
    if (lpDDS->GetSurfaceDesc(&desc)!=DD_OK)
    {
        // failure
        //printf("couldnt get surface descriptor\n");
        if (lpDDS) lpDDS->Release();
        lpDDS=NULL;
        return;
    }

    // get pixel format
    DDPIXELFORMAT ddpf;
    memset(&ddpf,0,sizeof(ddpf));
    ddpf.dwSize=sizeof(ddpf);
    if (lpDDS->GetPixelFormat(&ddpf)!=DD_OK) 
    {
        // failure
        //printf("lpDDS->GetPixelFormat failed\n");
        lpDDS->Release();
        lpDDS=NULL;
        return;
    }

    // translate to FORMAT
    if (!i.Translate(ddpf,format)) 
    {
        // failure
        //printf("format translation failed\n");
        lpDDS->Release();
        lpDDS=NULL;
        return;
    }

    // calculate directx advance
    int directx_advance=desc.lPitch-width*format.bytes;
    //printf("directx_advance=%d\n",directx_advance);

    // check directx advance against requested advance
    if (directx_advance!=new_advance && new_advance==DEFAULT) new_advance=directx_advance;
    else
    {
        // advance doesnt match
        //printf("advance doesnt match\n");
        if (lpDDS) lpDDS->Release();
        lpDDS=NULL;
        return;
    }

    // setup IID_IDirectDrawSurface2
    GUID IID_IDirectDrawSurface2;
    IID_IDirectDrawSurface2.Data1=1468029061;
    IID_IDirectDrawSurface2.Data2=28396;
    IID_IDirectDrawSurface2.Data3=4559;
    IID_IDirectDrawSurface2.Data4[0]=148;
    IID_IDirectDrawSurface2.Data4[1]=65;
    IID_IDirectDrawSurface2.Data4[2]=168;
    IID_IDirectDrawSurface2.Data4[3]=35;
    IID_IDirectDrawSurface2.Data4[4]=3;
    IID_IDirectDrawSurface2.Data4[5]=193;
    IID_IDirectDrawSurface2.Data4[6]=14;
    IID_IDirectDrawSurface2.Data4[7]=39;

    // query DDS2 interface
    if (lpDDS->QueryInterface(IID_IDirectDrawSurface2,(LPVOID*)&lpDDS2)!=DD_OK)
    {
        // failure
        //printf("failed to query lpDDS2\n");
        lpDDS2=NULL;
    }

    // clear surface
    uchar *buffer=(uchar*)Lock(0);
    if (buffer)
    {
        // setup data
        int bytes=width*format.bytes;
        int pitch=bytes+new_advance;

        // line loop
        for (int y=0; y<height; y++)
        {
            // clear line
            memset(buffer,0,bytes);
            
            // next line
            buffer+=pitch;
        }

        // unlock
        Unlock();
    }

    // setup data
    type=new_type;
    orientation=new_orientation;
    advance=new_advance;
    layout=LINEAR;

    // debug
    //printf("IDirectX::SURFACE::SURFACE ok\n");
}


IDirectX::SURFACE::~SURFACE()
{
    // free directdraw2 surface
    if (lpDDS2) lpDDS2->Release();

    // free directdraw surface
    if (lpDDS) lpDDS->Release();
}


void* IDirectX::SURFACE::Lock(int wait)
{
    //printf("IDirectX::SURFACE::Lock\n");

    // todo: properly handle the "wait" parameter

    // lock lpDDS/lpDDS2 on the first lock, on successive locks return previously locked buffer
    if (Count==0)
    {
        // lock directdraw surface
        DDSURFACEDESC ddsd;
        ddsd.dwSize=sizeof ddsd;
        HRESULT result;
        if (lpDDS2) result=lpDDS2->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
        else if (lpDDS) result=lpDDS->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
        else 
        {
            //printf("no lpDDS2 or lpDDS found\n");
            return NULL;
        }

        // auto-restore if primary
        if (Primary && result==DDERR_SURFACELOST)
        {
            //printf("attempting to restore primary\n");

            // restore
            if (!Restore()) 
            {
                //printf("primary restore failed\n");
                Sleep(10);
                return NULL;
            }

            // check local interface
            if (LocalInterface)
            {
                // restore primary palette
                LocalInterface->SetPalette(LocalInterface->PrimaryPalette);
            }

            // try to lock again
            if (lpDDS2) result=lpDDS2->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
            else if (lpDDS) result=lpDDS->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
            else 
            {
                //printf("no lpDDS2 or lpDDS found (2)\n");
                return NULL;
            }
        }

        // check result
        if (result!=DD_OK) 
        {
            //printf("lock failed\n");
            return NULL;
        }

        // update Buffer
        Buffer=ddsd.lpSurface;
    
        // increment lock count
        Count++;

        //printf("IDirectX::SURFACE::Lock ok\n");

        // return locked buffer
        return ddsd.lpSurface;
    }
    else
    {
        //printf("previously locked buffer\n");

        // check previously locked buffer
        if (!Buffer) 
        {
            //printf("!Buffer\n");
            return NULL;
        }

        // check surface
        if (!ok())
        {
            //printf("surface !ok\n");

            // clear
            Count=0;
            Buffer=NULL;
            return NULL;
        }

        // increment lock count
        Count++;

        //printf("IDirectX::SURFACE::Lock ok\n");

        // return previously locked buffer
        return Buffer;
    }

    //printf("IDirectX::SURFACE::Lock failed\n");
    return NULL;
}   


void IDirectX::SURFACE::Unlock()
{
    // unlock surface
    if (Count==1)
    {
        // unlock lpDDS/lpDDS2
        if (lpDDS2) lpDDS2->Unlock(NULL);
        else if (lpDDS) lpDDS->Unlock(NULL);

        // clear buffer
        Buffer=NULL;
    }

    // decrement lock count
    if (Count>0) Count--;
}


int IDirectX::SURFACE::LockCount()
{
    // return surface lock count
    return Count;
}


int IDirectX::SURFACE::Lockable()
{
    // lockable
    return 1;
}


int IDirectX::SURFACE::Restore()
{
    //printf("IDirectX::SURFACE::Restore\n");

    // restore surface
    if (lpDDS2 && lpDDS2->Restore()!=DD_OK) 
    {
        // failure
        //printf("lpDDS2->Restore failed\n");
        return 0;
    }
    else if (lpDDS && lpDDS->Restore()!=DD_OK)
    {
        //printf("lpDDS->Restore failed\n");
        return 0;
    }
    else 
    {
        //printf("no valid lpDD2 or lpDD found\n");
        return 0;
    }

    //printf("IDirectX::SURFACE::Restore ok\n");

    // success
    return 1;
}

/*
int IDirectX::SURFACE::Clear(Surface &surface,COLOR const &color)
{
    return 0;
}


int IDirectX::SURFACE::Clear(Surface &surface,RECTANGLE const &rect,COLOR const &color)
{
    return 0;
}


int IDirectX::SURFACE::BitBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return BitBlt(src,src.GetDimensions(),dest,dest.GetDimensions(),effects,extra);
}


int IDirectX::SURFACE::BitBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    // ISoftware::SURFACE bitblt
    ISoftware::SURFACE *surface=(ISoftware::SURFACE*)this;
    int result=surface->BitBlt(src,src_rect,dest,dest_rect,effects,extra);

    // check for primary update
    if (result && &dest==LocalInterface->Primary)       // hack: todo fix to Surface::IsPrimary
    {
        // set recent
        LocalInterface->SetRecent(src,src_rect,dest_rect);
    }

    // success
    return result;
}


int IDirectX::SURFACE::StretchBlt(Surface &src,Surface &dest,EFFECTS const *effects,void *extra)
{
    return 0;
}


int IDirectX::SURFACE::StretchBlt(Surface &src,RECTANGLE const &src_rect,Surface &dest,RECTANGLE const &dest_rect,EFFECTS const *effects,void *extra)
{
    return 0;
}
*/
                
int IDirectX::SURFACE::NativeType()
{
    // return native type
    return NATIVE_WIN32_LPDIRECTDRAWSURFACE;
}


void* IDirectX::SURFACE::GetNative()
{
    // return native surface
    return &lpDDS;
}


int IDirectX::SURFACE::ok()
{
    // surface status
    if (lpDDS2)
    {
        // lpDDS2
        if (lpDDS2->IsLost()==DD_OK) return 1;
        else return 0;
    }
    else if (lpDDS)
    {
        // lpDDS
        if (lpDDS->IsLost()==DD_OK) return 1;
        else return 0;
    }       
    else return 0;
}







int IDirectX::InitDisplay(int x,int y,int id,int output,int frequency,int layout)
{
    //printf("IDirectX::InitDisplay\n");

    // fail on windowed output
    if (output==WINDOWED)
    {
        //printf("no WINDOWED output is allowed\n");
        return 0;
    }

    // check display mode
    if (!CheckDisplayMode(x,y,id,frequency)) 
    {
        //printf("check display mode failed\n");
        return 0;
    }
    
    // attempt direct mode set
    if (EnterDisplayMode(x,y,id,frequency)) 
    {
        //printf("direct mode set succeeded!\n");
        return 1;
    }
    
    // init display start setup
    if (!InitDisplayStart(x,y,output,frequency,layout)) 
    {
        // failure
        //printf("InitDisplayStart failed\n");
        CloseDisplay();
        return 0;
    }

    // enter display mode
    if (!EnterDisplayMode(x,y,id,frequency)) 
    {
        // failure
        //printf("EnterDisplayMode failed\n");
        CloseDisplay();
        return 0;
    }

    // get ddraw pixel format
    /*
    FORMAT format=GetDisplayFormat();
    if (!format.ok())
    {
        // failure
        printf("bad display format!\n");
        CloseDisplay();
        return 0;
    }
    */

    // hack
    FORMAT format=0;

    // init display finish setup
    if (!InitDisplayFinish(x,y,format,output,frequency,layout)) 
    {
        // failure
        //printf("InitDisplayFinish failed\n");
        CloseDisplay();
        return 0;
    }

    // success
    //printf("IDirectX::InitDisplay ok\n");
    return 1;
}


int IDirectX::InitDisplay(int x,int y,FORMAT const &format,int output,int frequency,int layout)
{
    //printf("IDirectX::InitDisplay(2)\n");

    // fail on windowed output
    if (output==WINDOWED) 
    {
        //printf("no WINDOWED output ya dorq!\n");
        return 0;
    }

    // check display mode
    if (!CheckDisplayMode(x,y,format,frequency)) 
    {
        //printf("check display mode failure\n");
        return 0;
    }
    
    // attempt direct mode set
    if (EnterDisplayMode(x,y,format,frequency))
    {
        //printf("direct mode set succeeded\n");
        return 1;
    }
    
    // init display start setup
    if (!InitDisplayStart(x,y,output,frequency,layout))
    {
        // failure
        //printf("init display start failed\n");
        CloseDisplay();
        return 0;
    }

    // enter display mode
    if (!EnterDisplayMode(x,y,format,frequency)) 
    {
        // failure
        //printf("enter display mode failed\n");
        CloseDisplay();
        return 0;
    }

    // init display finish setup
    if (!InitDisplayFinish(x,y,format,output,frequency,layout)) 
    {
        // failure
        //printf("init display finish failed\n");
        CloseDisplay();
        return 0;
    }

    // success
    //printf("IDirectX::InitDisplay(2) ok\n");
    return 1;
}


void IDirectX::CloseDisplay()
{   
    //printf("IDirectX::CloseDisplay\n");

    // leave display mode
    LeaveDisplayMode();

    // close primary
    ClosePrimary();

    // close display Window
    CloseDisplayWindow();

    //printf("IDirectX::CloseDisplay ok\n");
}







int IDirectX::EnterDisplayMode(int x,int y,int id,int frequency)
{ 
    //printf("IDirectX::EnterDisplayMode\n");

    // enter exclusive mode
    if (!EnterExclusiveMode())
    {
        //printf("EnterExclusiveMode failed\n");
        return 0;
    }

    // virtual modes
    if (id==VIRTUAL32)
    {
        // virtual 32bit
        if (SetDisplayMode(x,y,32,frequency)) return 1;
        if (SetDisplayMode(x,y,24,frequency)) return 1;
        if (SetDisplayMode(x,y,16,frequency)) return 1;
    }
    else if (id==VIRTUAL16)
    {
        // virtual 16bit
        if (SetDisplayMode(x,y,16,frequency)) return 1;
        if (SetDisplayMode(x,y,32,frequency)) return 1;
        if (SetDisplayMode(x,y,24,frequency)) return 1;
    }
    else if (id==VIRTUAL8)
    {
        // virtual 8bit
        if (SetDisplayMode(x,y,8,frequency))  return 1;
        if (SetDisplayMode(x,y,32,frequency)) return 1;
        if (SetDisplayMode(x,y,24,frequency)) return 1;
        if (SetDisplayMode(x,y,16,frequency)) return 1;
    }

    // set by bits per pixel
    if (SetDisplayMode(x,y,id,frequency)) return 1;
 
    // failure
    //printf("EnterDisplayMode fail!\n");
    return 0;
}


int IDirectX::EnterDisplayMode(int x,int y,FORMAT const &format,int frequency)
{   
    //printf("IDirectX::EnterDisplayMode\n");

    // check format
    if (!format.ok()) return 0;

    // enter exclusive mode
    if (!EnterExclusiveMode())
    {
        //printf("EnterExclusiveMode failed\n");
        return 0;
    }

    // set display mode
    if (!SetDisplayMode(x,y,format,frequency)) return 0;

    //printf("IDirectX::EnterDisplayMode ok\n");

    // success
    return 1;
}


int IDirectX::SetDisplayMode(int x,int y,int bits,int frequency)
{
    // set display mode
    if (lpDD2)
    {
        // lpDD2
        if (lpDD2->SetDisplayMode(x,y,bits,0,0)!=DD_OK) return 0;
    }
    else if (lpDD)
    {
        // lpDD
        if (lpDD->SetDisplayMode(x,y,bits)!=DD_OK) return 0;
    }
    else return 0;
    
    // success
    return 1;
}


int IDirectX::SetDisplayMode(int x,int y,FORMAT const &format,int frequency)
{
    // set display mode
    if (lpDD2)
    {
        // lpDD2
        if (lpDD2->SetDisplayMode(x,y,format.bits,0,0)!=DD_OK) return 0;
    }
    else if (lpDD)
    {
        // lpDD
        if (lpDD->SetDisplayMode(x,y,format.bits)!=DD_OK) return 0;
    }
    else return 0;

    // check display format matches requested format
    /*
    FORMAT display_format=GetDisplayFormat();
    if (!display_format.ok() || display_format!=format)
    {
        // failure
        printf("display format and requested format dont match!\n");
        return 0;
    }
    */

    // success
    return 1;
}


int IDirectX::CheckDisplayMode(int x,int y,int id,int frequency)
{
    // get modelist
    List<MODE> *list=GetModeList();
    if (!list) return 0;

    // check modelist size
    if (list->size()==0) return 1;

    // setup mode iterator
    List<MODE>::Iterator i=list->first();
    MODE *mode=i.current();

    // check id
    switch (id)
    {
        case VIRTUAL32:
        {
            // virtual 32bit mode
            while (mode)
            {
                // check if mode matches specs
                if (x==mode->x && y==mode->y && 
                    (mode->format.id==ARGB8888 ||
                     mode->format.id==ABGR8888 ||
                     mode->format.id==RGBA8888 ||
                     mode->format.id==BGRA8888 ||
                     mode->format.id==RGB888   ||
                     mode->format.id==BGR888   ||
                     mode->format.id==RGB565   ||
                     mode->format.id==BGR565   ||
                     mode->format.id==ARGB1555 ||
                     mode->format.id==ABGR1555)) return 1;

                // next mode
                mode=i.next();
            }
        }
        break;

        case VIRTUAL16:
        {
            // virtual 16bit mode
            while (mode)
            {
                // check if mode matches specs
                if (x==mode->x && y==mode->y && 
                    (mode->format.id==RGB565   ||
                     mode->format.id==BGR565   ||
                     mode->format.id==ARGB1555 ||
                     mode->format.id==ABGR1555 ||
                     mode->format.id==ARGB8888 ||
                     mode->format.id==ABGR8888 ||
                     mode->format.id==RGBA8888 ||
                     mode->format.id==BGRA8888 ||
                     mode->format.id==RGB888   ||
                     mode->format.id==BGR888)) return 1;

                // next mode
                mode=i.next();
            }
        }
        break;

        case VIRTUAL8:
        {
            // virtual 8bit mode
            while (mode)
            {
                // check if mode matches specs
                if (x==mode->x && y==mode->y && 
                    (mode->format.id==INDEX8 ||
                     mode->format.type==DIRECT && (mode->format.bits>=1 && mode->format.bits<=4))) return 1;

                // next mode
                mode=i.next();
            }
        }
        break;

        default:
        {
            // id=bits per pixel
            while (mode)
            {
                // check if mode matches specs
                if (x==mode->x && y==mode->y && id==mode->format.bits) return 1;

                // next mode
                mode=i.next();
            }
        }
    }

    // failure
    return 0;
}


int IDirectX::CheckDisplayMode(int x,int y,FORMAT const &format,int frequency)
{
    // get modelist
    List<MODE> *list=GetModeList();
    if (!list) return 0;

    // check modelist size
    if (list->size()==0) return 1;

    // search for matching mode in list
    List<MODE>::Iterator i=list->first();
    MODE *mode=i.current();
    while (mode)
    {
        // check if mode matches specs
        if (x==mode->x && y==mode->y && format==mode->format) return 1;

        // next mode
        mode=i.next();
    }

    // failure
    return 0;
}


void IDirectX::LeaveDisplayMode()
{
    // restore display mode
    if (lpDD2) lpDD2->RestoreDisplayMode();
    else if (lpDD) lpDD->RestoreDisplayMode();

    // leave exclusive mode
    LeaveExclusiveMode();
}







int IDirectX::EnterExclusiveMode()
{   
    //printf("IDirectX::EnterExclusiveMode\n");

    // get managed window
    HWND window=GetManagedWindow();
    if (!window)
    {
        //printf("no managed window found\n");
        return 0;
    }

    // enter directdraw exclusive mode
    HRESULT result;
    if (lpDD2) result=lpDD2->SetCooperativeLevel(window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    else if (lpDD) result=lpDD->SetCooperativeLevel(window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    else
    {
        //printf("no valid lpDD?\n");
        return 0;
    }
    
    // check result
    if (result!=DD_OK)
    {
        /*
        // debug
        switch (result)
        {
            case DDERR_EXCLUSIVEMODEALREADYSET: printf("DDERR_EXCLUSIVEMODEALREADYSET\n"); break;
            case DDERR_HWNDALREADYSET: printf("DDERR_HWNDALREADYSET\n"); break;
            case DDERR_HWNDSUBCLASSED: printf("DDERR_HWNDSUBCLASSED\n"); break;
            case DDERR_INVALIDOBJECT: printf("DDERR_INVALIDOBJECT\n"); break;
            case DDERR_INVALIDPARAMS: printf("DDERR_INVALIDPARAMS\n"); break;
            case DDERR_OUTOFMEMORY: printf("DDERR_OUTOFMEMORY\n"); break;
        }
        */

        // check for stupid dx bug...
        if (result==DDERR_HWNDALREADYSET)
        {
            // close old
            CloseDisplay();
    
            // reinitialize directdraw
            if (!InitDirectDraw()) 
            {
                //printf("InitDirectDraw failed\n");
                return 0;
            }

            // reattempt exclusive mode
            if (lpDD2) result=lpDD2->SetCooperativeLevel(window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
            else if (lpDD) result=lpDD->SetCooperativeLevel(window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
            else
            {
                //printf("failed to re-enter exclusive mode\n");
                return 0;
            }
        }

        /*
        // check result
        if (result!=DD_OK)
        {
            // debug
            switch (result)
            {
                case DDERR_EXCLUSIVEMODEALREADYSET: printf("DDERR_EXCLUSIVEMODEALREADYSET\n"); break;
                case DDERR_HWNDALREADYSET: printf("DDERR_HWNDALREADYSET\n"); break;
                case DDERR_HWNDSUBCLASSED: printf("DDERR_HWNDSUBCLASSED\n"); break;
                case DDERR_INVALIDOBJECT: printf("DDERR_INVALIDOBJECT\n"); break;
                case DDERR_INVALIDPARAMS: printf("DDERR_INVALIDPARAMS\n"); break;
                case DDERR_OUTOFMEMORY: printf("DDERR_OUTOFMEMORY\n"); break;
            }
            return 0;
        }
        */
    }

    // success
    //printf("IDirectX::EnterExclusiveMode ok\n");
    return 1;
}


void IDirectX::LeaveExclusiveMode()
{
    // debug
    //printf("IDirectX::LeaveExclusiveMode\n");

    // leave exclusive mode
    if (lpDD2) lpDD2->SetCooperativeLevel(NULL,DDSCL_NORMAL);
    else if (lpDD) lpDD->SetCooperativeLevel(NULL,DDSCL_NORMAL);

    //printf("IDirectX::LeaveExclusiveMode ok\n");
}







int IDirectX::InitDirectDraw()
{
    //printf("IDirectX::InitDirectDraw\n");

    // close old
    CloseDirectDraw();

    // load "ddraw.dll"
    DirectDrawInstance=(HMODULE)LoadLibrary("ddraw.dll");
    if (!DirectDrawInstance)
    {
        //printf("LoadLibrary failed\n");
        return 0;
    }

    // get directdraw function addresses
    DIRECTDRAWCREATE_FUNCTION DirectDrawCreateFunction = (DIRECTDRAWCREATE_FUNCTION) GetProcAddress(DirectDrawInstance,"DirectDrawCreate");    
    if (!DirectDrawCreateFunction)
    {
        //printf("GetProcAddress failed\n");
        return 0;
    }

    // contruct directdraw object
    HRESULT result=(*DirectDrawCreateFunction)(NULL,&lpDD,NULL);
    if (result!=DD_OK)
    {
        //printf("DirectDrawCreate failed\n");
        if (lpDD) lpDD->Release();
        lpDD=NULL;
        return 0;
    }

    // setup IID_IDirectDraw2
    GUID IID_IDirectDraw2;
    IID_IDirectDraw2.Data1=3014063072;
    IID_IDirectDraw2.Data2=11075;
    IID_IDirectDraw2.Data3=4559;
    IID_IDirectDraw2.Data4[0]=162;
    IID_IDirectDraw2.Data4[1]=222;
    IID_IDirectDraw2.Data4[2]=0;
    IID_IDirectDraw2.Data4[3]=170;
    IID_IDirectDraw2.Data4[4]=0;
    IID_IDirectDraw2.Data4[5]=185;
    IID_IDirectDraw2.Data4[6]=51;
    IID_IDirectDraw2.Data4[7]=86;

    // query IDirectDraw2 interface
    result=lpDD->QueryInterface(IID_IDirectDraw2,(LPVOID*)&lpDD2);
    if (result!=DD_OK) 
    {
        // failure
        //printf("failed to query lpDD2\n");
        lpDD2=NULL;    
    }

    // success
    //printf("IDirectX::InitDirectDraw ok\n");
    return 1;
}


void IDirectX::CloseDirectDraw()
{
    // close display
    CloseDisplay();

    // free surfaces
    FreeSurfaces();

    // free interface objects
    if (lpDD2) lpDD2->Release();
    if (lpDD)  lpDD->Release();

    // clear
    lpDD=NULL;
    lpDD2=NULL;    

    // release ddraw.dll
    if (DirectDrawInstance) FreeLibrary(DirectDrawInstance);
}







int IDirectX::CreateDisplayWindow(int width,int height,int output)
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
        // todo: fix mode changes fullscreen->fullscreen
    }

    // close old
    CloseDisplayWindow();

    // adjust window size
    AdjustWindowSize(width,height,output);

    // force active window
    ForceActiveWindow=1;

    // specific initialization
    if (output==WINDOWED)
    {
        // create windowed output window
        if (!CreateManagedWindow("PTC_DIRECTX_WINDOWED",WS_OVERLAPPEDWINDOW,SW_NORMAL,CW_USEDEFAULT,CW_USEDEFAULT,width,height)) return 0;
    }
    else if (output==FULLSCREEN)
    {
        // create fullscreen output window
        if (!CreateManagedWindow("PTC_DIRECTX_FULLSCREEN",WS_POPUP|WS_SYSMENU,SW_HIDE,0,0,0,0)) return 0;
    }
    else return 0;

    // success
    return 1;
}
                                            

int IDirectX::ResizeDisplayWindow(int width,int height,int output)
{
    // adjust window size
    AdjustWindowSize(width,height,output);

    // get managed window
    HWND window=GetManagedWindow();

    /*
    // check if resize is possible
    RECT rect;
    GetWindowRect(window,&rect);
    MODE mode=GetDisplayMode();
    if (rect.left+width>mode.x || rect.top+height>mode.y) return 0;
    */

    // resize managed window
    return ResizeManagedWindow(width,height);
}


void IDirectX::CloseDisplayWindow()
{
    // check if managed
    if (!ManagedWindow()) return;

    // close managed window
    CloseManagedWindow();
}







int IDirectX::InitPrimary()           
{
    //printf("IDirectX::InitPrimary\n");

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
    
    /*
    if (!result) printf("IDirectX::InitPrimary failed\n");
    else printf("IDirectX::InitPrimary ok\n");
    */

    // finished
    return result;
}


void IDirectX::ClosePrimary()
{
    // enter critical section
    EnterWindowCriticalSection();
    
    // close primary
    delete Primary;
    Primary=NULL;

    // leave critical section
    LeaveWindowCriticalSection();
}







int IDirectX::SetupModeList()
{
    // fail if modelist is already initialized
    ClearModeList();

    // enumerate directx modes
    if (lpDD2) if (lpDD2->EnumDisplayModes(0,NULL,(LPVOID)this,(LPDDENUMMODESCALLBACK)EnumDisplayModesCallback)==DD_OK) return 1;
    else if (lpDD) if (lpDD->EnumDisplayModes(0,NULL,(LPVOID)this,(LPDDENUMMODESCALLBACK)EnumDisplayModesCallback)==DD_OK) return 1;
    
    // failure
    return 0;
}


HRESULT CALLBACK IDirectX::EnumDisplayModesCallback(LPDDSURFACEDESC desc,IDirectX *owner)
{
    // check parmeters
    if (!desc || !owner) return DDENUMRET_OK;

    // setup mode
    MODE mode;
    memset(&mode,0,sizeof(mode));
    owner->GetName(mode.i);
    mode.x=desc->dwWidth;
    mode.y=desc->dwHeight;
    if (!owner->Translate(desc->ddpfPixelFormat,mode.format)) return DDENUMRET_OK;
    mode.layout=LINEAR;
    mode.output=FULLSCREEN;

    // add to list
    owner->AddMode(mode);
    return DDENUMRET_OK;
}







int IDirectX::Translate(DDPIXELFORMAT const &ddpf,FORMAT &format)
{
    // check format type
    if (ddpf.dwFlags==DDPF_RGB)
    {
        // direct RGB color
        int size=ddpf.dwRGBBitCount;

        // setup component masks
        uint r_mask=ddpf.dwRBitMask;
        uint g_mask=ddpf.dwGBitMask;
        uint b_mask=ddpf.dwBBitMask;
        uint a_mask=ddpf.dwRGBAlphaBitMask;

        // create format object from mask data
        FORMAT temp(r_mask,g_mask,b_mask,a_mask);

        // check size of color components
        if (temp.c1.size + temp.c2.size + temp.c3.size + temp.c4.size != (unsigned)format.bits)
        {
            // setup hidden alpha component
            a_mask=~(r_mask|g_mask|b_mask);

            // mask off from bits per pixel
            uint mask=(0xFFFFFFFF>>(32-size));
            a_mask&=mask;
        }

        // initialize from fixed up color masks
        return format.init(r_mask,g_mask,b_mask,a_mask);
    }
    else if (ddpf.dwFlags & DDPF_PALETTEINDEXED8)
    {
        // indexed 8bit color
        return format.init(INDEX8);
    }
    else return 0;
}


int IDirectX::Translate(FORMAT const &format,DDPIXELFORMAT &ddpf)
{
    // clear directdraw pixel format
    memset(&ddpf,sizeof(ddpf),0);
    
    // check format type
    if (format.type==DIRECT)
    {
        // setup ddpf for direct color
        ddpf.dwSize=sizeof(ddpf);
        ddpf.dwFlags=DDPF_RGB;

        // setup RGB component information
        ddpf.dwRGBBitCount=format.bits;
        ddpf.dwRBitMask=format.c1.mask;
        ddpf.dwGBitMask=format.c2.mask;
        ddpf.dwBBitMask=format.c3.mask;

        // done
        return 1;
    }
    else if (format.id==INDEX8)
    {
        // setup ddpf for indexed 8bit color
        ddpf.dwSize=sizeof(ddpf);
        ddpf.dwFlags=DDPF_PALETTEINDEXED8 | DDPF_RGB;

        // setup RGB component information
        ddpf.dwRGBBitCount=format.bits;
        ddpf.dwRBitMask=format.c1.mask;
        ddpf.dwGBitMask=format.c2.mask;
        ddpf.dwBBitMask=format.c3.mask;

        // done
        return 1;
    }

    // failure
    //printf("failed to translate FORMAT -> DDPIXELFORMAT\n");
    return 0;
}







int IDirectX::InitDisplayStart(int &x,int &y,int &output,int &frequency,int &layout)
{
    // debug
    //printf("IDirectX::InitDisplayStart\n");

    // fail if not managed
    if (!ManagedWindow()) 
    {
        //printf("no window found\n");
        return 0;
    }

    // no windowed output yet...
    if (output==WINDOWED) 
    {
        //printf("no windowed output yet!\n");
        return 0;
    }

    // fail on bad output parameter
    if (output!=DEFAULT && output!=FULLSCREEN && output!=WINDOWED) 
    {
        //printf("bad output parameter\n");
        return 0;
    }

    // default output is fullscreen
    if (output==DEFAULT) output=FULLSCREEN;

    // default frequency is UNKNOWN (?)
    if (frequency==DEFAULT) frequency=UNKNOWN;

    // check layout
    if (layout!=DEFAULT && layout!=LINEAR) 
    {
        //printf("bad layout parameter\n");
        return 0;
    }

    // todo: recycle window if already in a mode...

    // create display window
    if (!CreateDisplayWindow(x,y,output)) 
    {
        //printf("CreateDisplayWindow failed\n");
        return 0;
    }

    // success
    //printf("IDirectX::InitDisplayStart ok\n");
    return 1;
}


int IDirectX::InitDisplayFinish(int &x,int &y,FORMAT const &format,int &output,int &frequency,int &layout)
{
    // debug
    //printf("IDirectX::InitDisplayFinish\n");

    // setup data
    XResolution=x;
    YResolution=y;

    // initialize primary
    if (!InitPrimary()) 
    {
        //printf("InitPrimary failed\n");
        return 0;
    }

    // get primary surface pixel format
    Surface *primary=GetPrimary();
    FORMAT primary_format=primary->GetFormat();
    
    // check pixel format match
    if (format.ok() && format!=primary_format)
    {
        //printf("requested mode format does not match primary surface format!\n");
        return 0;
    }

    // get managed window
    HWND window=GetManagedWindow();
    if (!window) 
    {
        //printf("no managed window found\n");
        return 0;
    }

    // register window for interface lookup
    if (!RegisterWindow(window)) 
    {
        //printf("failed to register window\n");
        return 0;
    }

    // reallow window deactivation
    EnterWindowCriticalSection();
    ForceActiveWindow=0;
    LeaveWindowCriticalSection();

    // setup data
    XResolution=x;
    YResolution=y;
    Format=primary_format;
    Output=output;
    Frequency=frequency;

    // clear primary palette
    uint *data=(uint*)PrimaryPalette.Lock();
    if (data)
    {
        // clear palette to black
        for (int i=0; i<256; i++) data[i]=0;

        // unlock palette
        PrimaryPalette.Unlock();
    }

    // success
    //printf("IDirectX::InitDisplayFinish ok\n");
    return 1;
}


FORMAT IDirectX::GetDisplayFormat()
{
    // debug
    //printf("IDirectX::GetDisplayFormat\n");

    /*
    // setup directdraw surface descriptor
    DDSURFACEDESC desc;
    memset(&desc,0,sizeof(desc));
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DDSD_CAPS;
    desc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
        
    // create directdraw primary surface
    LPDIRECTDRAWSURFACE lpDDS=NULL;
    if (lpDD2) 
    {
        // lpDD2
        if (lpDD2->CreateSurface(&desc,&lpDDS,NULL)!=DD_OK) 
        {
            // failure
            //printf("lpDD2->CreateSurface failed\n");
            return 0;
        }
    }
    else if (lpDD)
    {
        // lpDD
        if (lpDD->CreateSurface(&desc,&lpDDS,NULL)!=DD_OK)
        {
            // failure
            printf("lpDD->CreateSurface failed\n");
            return 0;
        }
    }
    else
    {
        // failure
        printf("no valid lpDD object!\n");
        return 0;
    }

    // get pixel format
    DDPIXELFORMAT ddpf;
    memset(&ddpf,0,sizeof(ddpf));
    ddpf.dwSize=sizeof(ddpf);
    if (lpDDS->GetPixelFormat(&ddpf)!=DD_OK) 
    {
        // failure
        printf("lpDDS->GetPixelFormat failed\n");
        lpDDS->Release();
        return 0;
    }

    // translate to FORMAT
    FORMAT format;
    if (!Translate(ddpf,format)) 
    {
        // failure
        printf("format translation failed\n");
        lpDDS->Release();
        return 0;
    }

    // release surface
    lpDDS->Release();
    */

    // this function is now obsolete!
    FORMAT format=0;

    // success
    //printf("IDirectX::GetDisplayFormat ok\n");
    return format;
}







int IDirectX::RegisterWindow(HWND window)
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


int IDirectX::UnregisterWindow(HWND window)
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
            break;
        }
        
        // next
        current=iterator.next();
    }

    // leave critical section
    LeaveStaticCriticalSection();
    return result;
}


IDirectX* IDirectX::LookupInterface(HWND window)
{
    // enter critical section
    EnterStaticCriticalSection();

    // lookup interface via window handle
    IDirectX *i=NULL;
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







int IDirectX::RegisterWindowClasses()
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
    wc.lpszClassName = "PTC_DIRECTX_WINDOWED";
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
    wc.lpszClassName = "PTC_DIRECTX_FULLSCREEN";
    int result_fullscreen=RegisterClass(&wc);

    // check results
    if (!result_windowed || !result_fullscreen) return 0;
    else return 1;
}


void IDirectX::AdjustWindowSize(int &width,int &height,int output)
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


LRESULT CALLBACK IDirectX::WndProcCommon(IDirectX *i,HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // message handler
    if (i)
    {
        switch (message) 
        { 
            case WM_QUERYNEWPALETTE:
            {
                // enter palette critical section
                i->EnterPaletteCriticalSection();
                
                // setup palette

                // leave palette critical section
                i->LeavePaletteCriticalSection();
                break;
            }

            case WM_PALETTECHANGED:
            {
                // enter palette critical section
                i->EnterPaletteCriticalSection();

                // setup palette

                // leave palette critical section
                i->LeavePaletteCriticalSection();
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
    return 0;
}


LRESULT CALLBACK IDirectX::WndProcWindowed(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // lookup interface
    IDirectX *i=LookupInterface(hWnd);
        
    // enter window critical section
    if (i) i->EnterWindowCriticalSection();

    // locals
    int result=0;

    // common functionality
    result=WndProcCommon(i,hWnd,message,wParam,lParam);
    
    // leave window critical section                                       
    if (i) i->LeaveWindowCriticalSection();
    return result;
}


LRESULT CALLBACK IDirectX::WndProcFullscreen(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // lookup interface
    IDirectX *i=LookupInterface(hWnd);
        
    // enter window critical section
    if (i) i->EnterWindowCriticalSection();

    // locals
    int result=0;

    // message handler
    switch (message) 
    { 
        case WM_CREATE:
    
            //printf("IDirectX::WndProcFullscreen - WM_CREATE\n");

            // hide cursor
            SetCursor(NULL);
            break;

        case WM_PAINT:

            //printf("IDirectX::WndProcFullscreen - WM_PAINT\n");

            /*
            // check primary surface
            if (i && i->Primary && i->Primary->ok())
            {
                printf("restoring primary surface\n");

                // enter critical section
                i->EnterUpdateCriticalSection();

                printf("restoring primary surface\n");

                // restore primary surface
                if (i->Primary->Restore())
                {
                    printf("primary restore succeeded\n");

                    // todo: restore primary palette

                    // restore primary contents
                    if (i->PrimaryBackup)
                    {
                        printf("restoring contents of primary surface\n");

                        // bitblt backup to primary
                        if (!i->PrimaryBackup->BitBlt(*i->Primary))
                        {
                            // failure
                            printf("failed to bitblt backup->primary\n");
                        }

                        // delete primary backup
                        delete i->PrimaryBackup;
                        i->PrimaryBackup=NULL;
                    }
                }

                // leave critical section
                i->LeaveUpdateCriticalSection();
            }
            */

            // validate window rect
            ValidateRect(hWnd,NULL);
            break;

        case WM_SETCURSOR: 
            
            //printf("IDirectX::WndProcFullscreen - WM_SETCURSOR\n");

            // hide cursor
            SetCursor(NULL);
            break;
        
        case WM_ACTIVATE:

            //printf("IDirectX::WndProcFullscreen - WM_ACTIVATE\n");

            // handle focus switching
            if (LOWORD(wParam)==WA_ACTIVE || LOWORD(wParam)==WA_CLICKACTIVE)
            {
                //printf("window activation\n");

                // hide cursor
                SetCursor(NULL);

                // set window to foreground
                SetForegroundWindow(hWnd);
            }
            else
            {
                //printf("window deactivation\n");
                 
                // check active window force flag
                if (i && i->ForceActiveWindow)
                {
                    //printf("disabling window deactivation\n");
    
                    // disable window deactivation
                    SetActiveWindow(hWnd);
                    SetForegroundWindow(hWnd);
                }
                else
                {
                    // allow window deactivation
                    //printf("deactivating window\n");

                    // check
                    if (i)
                    {
                        /*
                        printf("backing up primary surface data\n");

                        // enter critical section
                        i->EnterUpdateCriticalSection();

                        // delete old primary backup
                        delete i->PrimaryBackup;

                        // create new primary backup surface
                        i->PrimaryBackup=new Surface(i,i->XResolution,i->YResolution,i->Format);

                        // check surfaces
                        if (i->Primary && i->PrimaryBackup && i->PrimaryBackup->ok())
                        {
                            printf("bitblt primary->backup\n");

                            // backup surface
                            if (i->Primary->BitBlt(*i->PrimaryBackup))
                            {
                                // failure
                                printf("bitblt failed\n");
                            }
                        }
                        else
                        {
                            // failure
                            if (i->Primary) printf("failed to allocate primary backup surface\n");
                            else printf("failed to backup primary. primary=NULL\n");
                        }

                        // leave critical section
                        i->LeaveUpdateCriticalSection();
                        */
                    }
                }

            }
            break;

        default:

            // common functionality
            result=WndProcCommon(i,hWnd,message,wParam,lParam);
    }
    
    // leave window critical section                                       
    if (i) i->LeaveWindowCriticalSection();
    return result;
}







void IDirectX::PrintGUID(GUID const &guid)
{
    // print GUID structure
    cout << "GUID data:\n";
    cout << "Data1=" << guid.Data1 << endl;
    cout << "Data2=" << guid.Data2 << endl;
    cout << "Data3=" << guid.Data3 << endl;
    cout << "Data4[0]=" << (int)guid.Data4[0] << endl;
    cout << "Data4[1]=" << (int)guid.Data4[1] << endl;
    cout << "Data4[2]=" << (int)guid.Data4[2] << endl;
    cout << "Data4[3]=" << (int)guid.Data4[3] << endl;
    cout << "Data4[4]=" << (int)guid.Data4[4] << endl;
    cout << "Data4[5]=" << (int)guid.Data4[5] << endl;
    cout << "Data4[6]=" << (int)guid.Data4[6] << endl;
    cout << "Data4[7]=" << (int)guid.Data4[7] << endl << endl;
}







#endif
