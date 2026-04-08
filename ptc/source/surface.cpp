///////////////////
// Surface class //
///////////////////
#include "surface.h"
#include "isoft.h"
#include "idummy.h"






// statics
#ifndef __DJGPP__
ISoftware Surface::SoftwareInterface;
#endif
Palette Surface::DummyPalette;
IDummy Surface::DummyInterface;
IDummy::SURFACE Surface::DummySurface;


// disgusting DJGPP work around
#ifdef __DJGPP__
ISoftware *DJGPP_SUCKS_SoftwareInterface=NULL;
int DJGPP_SUCKS_SoftwareInterfaceCount=0;
#define SoftwareInterface (*DJGPP_SUCKS_SoftwareInterface)
#endif







Surface::Surface()
{
    // init software
    InitSoftware();

    // defaults
    Defaults();
}


Surface::~Surface()
{
    // free
    Free();

    // close software
    CloseSoftware();

    // invalidate
    Invalidate();
}







Surface::Surface(int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{                    
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(width,height,format,type,orientation,advance,layout);
}


Surface::Surface(void *memory,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(memory,width,height,format,type,orientation,advance,layout);
}


Surface::Surface(char filename[],int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Load(filename,type,orientation,advance,layout);
}







Surface::Surface(Interface *i,int type)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(i,type);
}


Surface::Surface(Interface *i,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(i,width,height,format,type,orientation,advance,layout);
}


Surface::Surface(Interface *i,char filename[],int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Load(i,filename,type,orientation,advance,layout);
}







int Surface::Load(char filename[],int type,int orientation,int advance,int layout)
{
    // load image
    return Load(&SoftwareInterface,filename,type,orientation,advance,layout);
}


int Surface::Load(Interface *i,char filename[],int type,int orientation,int advance,int layout)
{
    // advoid warnings....
    if (i || type || orientation || advance || layout);

    // only zero advance or DEFAULT
    if (advance!=0 && advance!=DEFAULT) return 0;

    // only DEFAULT or LINEAR layout
    if (layout!=DEFAULT && layout!=LINEAR) return 0;
    
    // load image
    Image image(filename,Image::READ);
    if (!image.ok()) return 0;

    // get image information
    FORMAT image_format;
    int image_width=0;
    int image_height=0;
    int image_palette=0;
    if (!image.info(image_width,image_height,image_format,image_palette)) return 0;

    // create surface
    if (!Init(i,image_width,image_height,image_format,type,orientation,advance,layout)) return 0;

    // lock surface
    void *buffer=Lock();
    if (!buffer)
    {
        Free();
        return 0;
    }

    // load image data and palette (if exists)
    if (image_palette)
    {
        // create palette
        Palette *palette=new Palette;
        if (!palette || !palette->ok())
        {
            Free();
            return 0;
        }

        // lock palette data
        void *palette_data=palette->Lock();
        if (!palette_data)
        {
            Free();
            return 0;
        }

        // load image data and palette
        if (!image.load(buffer,Orientation,Pitch,palette_data)) 
        {
            Free();
            return 0;
        }

        // unlock palette
        palette->Unlock();

        // attach palette
        if (!AttachPalette(palette))
        {
            Free();
            return 0;
        }
    }
    else
    {
        // load image data (no palette)
        if (!image.load(buffer,Orientation,Pitch,NULL)) 
        {
            Free();
            return 0;
        }
    }

    // unlock
    Unlock();
    return 1;
}


int Surface::Save(char filename,char *options)
{
    // save image
    if (filename || options);
    return 0;
}


int Surface::Save(char filename,FORMAT const &format,char *options)               // add orientation?
{
    // save image
    if (filename || format.ok() || options);
    return 0;
}







int Surface::Bind(Interface *i)
{
    // check interface
    if (!i) return 0;

    // check if already bound
    if (LocalInterface==i) return 1;

    // check surface
    if (!ok()) return 0;

    // fail on primary surface
    if (Primary) return 0;

    // create temporary surface
    Surface temp(i,Width,Height,Format,Type,Orientation,Advance,Layout);
    if (!temp.ok()) return 0;

    // bitblt to temporary surface
    if (!BitBlt(temp)) return 0;

    // detach surfaces
    LocalInterface->Detach(this);
    temp.LocalInterface->Detach(&temp);

    // switch internal surfaces
    Interface::SURFACE *surface=InternalSurface;
    InternalSurface=temp.InternalSurface;
    temp.InternalSurface=surface;

    // switch interfaces
    i=LocalInterface;
    LocalInterface=temp.LocalInterface;
    temp.LocalInterface=i;

    // attach surfaces
    LocalInterface->Attach(this);
    temp.LocalInterface->Attach(&temp);

    // done
    return 1;
}


int Surface::Unbind()
{
    // unbind (bind to SoftwareInterface)
    return Bind((Interface*)&SoftwareInterface);
}







void* Surface::Lock(int wait)
{
    // lock surface
    return InternalSurface->Lock(wait);
}


void Surface::Unlock()
{
    // unlock surface
    InternalSurface->Unlock();
}


int Surface::LockCount() const
{
    // surface lock count
    return InternalSurface->LockCount();
}


int Surface::Lockable() const
{
    // surface lockability
    return InternalSurface->Lockable();
}


int Surface::Restore()
{
    // check if needed
    if (ok()) return 0;

    // try to restore internal surface
    if (InternalSurface->Restore()) return 1;

    // reallocate internal surface
    return Init(LocalInterface,Width,Height,Format,Type,Orientation,Advance,Layout);
}







int Surface::Clear(COLOR const &color)
{
    // clear entire surface to a color
    return InternalSurface->Clear(*this,color);
}


int Surface::Clear(RECTANGLE const &rect,COLOR const &color)
{
    // clear area on surface to a color
    return InternalSurface->Clear(*this,rect,color);
}







int Surface::Update()
{
    // update surface
    return Update(LocalInterface);
}


int Surface::Update(RECTANGLE const &rect)
{
    // update surface
    return Update(LocalInterface,rect);
}


int Surface::Update(RECTANGLE const &src,RECTANGLE const &dest)
{
    // update surface
    return Update(LocalInterface,src,dest);
}







int Surface::Update(Interface *i)
{
    // check interface
    if (!i) return 0;

    // get primary surface
    Surface *primary=i->GetPrimary();
    if (!primary) return 0;

    // bitblt to primary surface
    return BitBlt(*primary);
}


int Surface::Update(Interface *i,RECTANGLE const &rect)
{
    // check interface
    if (!i) return 0;

    // get primary surface
    Surface *primary=i->GetPrimary();
    if (!primary) return 0;

    // bitblt to primary surface
    return BitBlt(*primary,rect,rect);
}


int Surface::Update(Interface *i,RECTANGLE const &src,RECTANGLE const &dest)
{
    // check interface
    if (!i) return 0;

    // get primary surface
    Surface *primary=i->GetPrimary();
    if (!primary) return 0;

    // bitblt to primary surface
    return BitBlt(*primary,src,dest);
}







int Surface::BitBlt(Surface &dest,EFFECTS const *effects)
{
    // check for bitblt complications...
    if (Width!=dest.Width || Height!=dest.Height || Advance!=0 || dest.Advance!=0 || Orientation!=dest.Orientation)
    {
        // clipped bitblt required
        RECTANGLE rect(0,0,Width,Height);
        return BitBlt(dest,rect,rect,effects);
    }

    // TODO: check clippers for complications
    
    // setup table
    void *table=NULL;
    if (Format.type==INDEXED) table=LocalPalette->GetIndexTable(dest.Format);

    // bitblt between internal surface objects
    if (dest.InternalSurface->BitBlt(*this,dest,effects,table)) return 1;
    else return InternalSurface->BitBlt(*this,dest,effects,table);
}                                                                                 


int Surface::BitBlt(Surface &dest,int x,int y,EFFECTS const *effects)
{
    // setup rectangles
    RECTANGLE src_rect(0,0,Width,Height);
    RECTANGLE dest_rect(x,y,x+Width,y+Height);

    // bitblt
    return BitBlt(dest,src_rect,dest_rect,effects);
}


int Surface::BitBlt(Surface &dest,RECTANGLE const &src_rect,RECTANGLE const &dest_rect,EFFECTS const *effects)
{
    // clip rectangles
    RECTANGLE src_clipped=src_rect;
    RECTANGLE dest_clipped=dest_rect;                      
    if (!Clip(*this,src_clipped,dest,dest_clipped)) return 1;

    // setup table
    void *table=NULL;
    if (Format.type==INDEXED) table=LocalPalette->GetIndexTable(dest.Format);

    // bitblt between internal surface objects
    if (dest.InternalSurface->BitBlt(*this,src_clipped,dest,dest_clipped,effects,table)) return 1;
    else return InternalSurface->BitBlt(*this,src_clipped,dest,dest_clipped,effects,table);
}







int Surface::StretchBlt(Surface &dest,EFFECTS const *effects)
{   
    // todo: perform clipping

    // setup table
    void *table=NULL;
    if (Format.type==INDEXED) table=LocalPalette->GetIndexTable(dest.Format);

    // bitblt between internal surface objects
    if (dest.InternalSurface->StretchBlt(*this,dest,effects,table)) return 1;
    else return InternalSurface->StretchBlt(*this,dest,effects,table);
}


int Surface::StretchBlt(Surface &dest,RECTANGLE const &src_rect,RECTANGLE const &dest_rect,EFFECTS const *effects)
{
    // todo: perform clipping

    // setup table
    void *table=NULL;
    if (Format.type==INDEXED) table=LocalPalette->GetIndexTable(dest.Format);

    // bitblt between internal surface objects
    if (dest.InternalSurface->StretchBlt(*this,src_rect,dest,dest_rect,effects,table)) return 1;
    else return InternalSurface->StretchBlt(*this,src_rect,dest,dest_rect,effects,table);
}







int Surface::Relocate(int type,int orientation,int advance,int layout)
{
    // check surface
    if (!ok()) return 0;

    // fail on primary
    if (Primary) return 0;

    // check surface
    if (!ok()) return 0;

    // fail on primary surface
    if (Primary) return 0;

    // create temporary surface
    Surface temp(LocalInterface,Width,Height,Format,type,orientation,advance,layout);
    if (!temp.ok()) return 0;

    // bitblt to temporary surface
    if (!BitBlt(temp)) return 0;

    // detach surfaces
    LocalInterface->Detach(this);
    temp.LocalInterface->Detach(&temp);

    // switch internal surfaces
    Interface::SURFACE *surface=InternalSurface;
    InternalSurface=temp.InternalSurface;
    temp.InternalSurface=surface;

    // switch interfaces
    Interface *i=LocalInterface;
    LocalInterface=temp.LocalInterface;
    temp.LocalInterface=i;

    // attach surfaces
    LocalInterface->Attach(this);
    temp.LocalInterface->Attach(&temp);

    // setup data
    Type=temp.Type;
    Orientation=temp.Orientation;
    Advance=temp.Advance;
    Layout=temp.Layout;
    Pitch=temp.Pitch;

    // done
    return 1;
}


int Surface::Orientate(int orientation)
{
    // check surface
    if (!ok()) return 0;

    // fail on primary
    if (Primary) return 0;

    // check orientation
    if (Orientation==orientation) return 1;

    // create temporary surface
    Surface temp(LocalInterface,Width,Height,Format,Type,orientation,Advance,Layout);
    if (!temp.ok()) return 0;

    // bitblt from current to temporary surface
    if (!this->BitBlt(temp)) return 0;

    // switch internal surfaces
    Interface::SURFACE *handle=InternalSurface;
    InternalSurface=temp.InternalSurface;
    temp.InternalSurface=handle;

    // setup data
    Pitch=temp.Pitch;
    Orientation=temp.Orientation;

    // done
    return 1;
}


int Surface::Convert(FORMAT const &format)              
{
    // check surface
    if (!ok()) return 0;

    // fail if primary
    if (Primary) return 0;

    // check if identical formats
    if (format==Format) return 1;

    // create temporary surface of destination format
    Surface temp(LocalInterface,Width,Height,format,Type,Orientation,Advance,Layout);
    if (!temp.ok()) return 0;

    // bitblt from current to temporary surface
    if (!BitBlt(temp)) return 0;

    // switch internal surfaces
    Interface::SURFACE *handle=InternalSurface;
    InternalSurface=temp.InternalSurface;
    temp.InternalSurface=handle;

    // switch palette objects
    Palette *palette=LocalPalette;
    LocalPalette=temp.LocalPalette;
    temp.LocalPalette=palette;

    // setup data
    Width=temp.Width;
    Height=temp.Height;
    Type=temp.Type;
    Layout=temp.Layout;
    Orientation=temp.Orientation;
    Advance=temp.Advance;
    Pitch=temp.Pitch;
    Primary=temp.Primary;
    Format=temp.Format;

    // done
    return 1;
}


int Surface::Resize(int width,int height)
{
    // not yet
    if (width || height);
    return 0;
}


int Surface::Resample(int width,int height)
{
    // not yet
    if (width || height);
    return 0;
}


int Surface::Crop(RECTANGLE const &rect)
{
    // not yet
    if (rect.x1);
    return 0;
}







int Surface::SetPalette(Palette const &palette)
{
    // check palette
    if (!palette.ok()) return 0;

    // only accept palette operations in indexed formats
    if (Format.type!=INDEXED) return 0;
    
    // allocate new palette
    Palette *new_palette=new Palette;
    if (!new_palette) return 0;

    // assign palette
    *new_palette=palette;
    
    // attach new palette
    if (!AttachPalette(new_palette))
    {
        delete new_palette;
        return 0;
    }

    // if primary surface set interface palette
    if (Primary) return LocalInterface->SetPalette(*new_palette);
    else return 1;
}


Palette* Surface::GetPalette()
{
    // only accept palette operations in indexed formats
    if (Format.type!=INDEXED) return NULL;

    // primary surface
    if (Primary)
    {
        // update local palette to match interface palette
        if (LocalPalette!=&DummyPalette) LocalInterface->GetPalette(*LocalPalette);
    }

    // return local palette
    if (LocalPalette==&DummyPalette) return NULL;
    else return LocalPalette;
}


int Surface::AttachPalette(Palette *palette)
{
    // check surface
    if (!ok()) return 0;

    // indexed formats only
    if (Format.type!=INDEXED) return 0;

    // check palette
    if (!palette || !palette->ok()) return 0;

    // check if already attached
    if (palette==LocalPalette) return 1;

    // detach palette
    DetachPalette();

    // attach palette
    LocalPalette=palette;
    LocalPalette->Attach();
    return 1;
}


void Surface::DetachPalette()
{
    // check for valid palette
    if (LocalPalette==&DummyPalette) return;

    // detach palette
    if (!LocalPalette->Detach())
    {
        // delete palette
        delete LocalPalette;
    }

    // dummy palette
    LocalPalette=&DummyPalette;
}


void Surface::DefaultPalette()
{
    // initialize palette
    if (Format.model==GREYSCALE)
    {
        // setup greyscale palette
        Palette palette;
        uint* data=(uint*)palette.Lock();
        if (!data) return;
        for (int i=0; i<256; i++) data[i]=RGB32((uchar)i,(uchar)i,(uchar)i);
        palette.Unlock();
        SetPalette(palette);
    }
    
    // todo: Palette::Default(format) ??
}








int Surface::SetClipper(Clipper const &clipper)
{
    // allocate new clipper
    Clipper *new_clipper=new Clipper;
    if (!new_clipper) return 0;

    // assign clipper
    *new_clipper=clipper;
    
    // attach new clipper
    if (!AttachClipper(new_clipper))
    {
        delete new_clipper;
        return 0;
    }
    else return 1;
}


Clipper* Surface::GetClipper()
{
    // return clipper
    return LocalClipper;
}


int Surface::AttachClipper(Clipper *clipper)
{
    // check surface
    if (!ok()) return 0;

    // check clipper
    if (!clipper) return 0;

    // check if already attached
    if (clipper==LocalClipper) return 1;

    // detach clipper
    DetachClipper();

    // attach clipper
    LocalClipper=clipper;
    LocalClipper->Attach();
    return 1;
}


void Surface::DetachClipper()
{
    // check for valid clipper
    if (!LocalClipper) return;

    // detach clipper
    if (!LocalClipper->Detach())
    {
        // delete clipper
        delete LocalClipper;
    }

    // clear clipper
    LocalClipper=NULL;
}


void Surface::DefaultClipper()
{
    // initialize default clipper
    RECTANGLE rect(0,0,Width,Height);
    Clipper clipper(rect,rect);
    SetClipper(clipper);
}








int Surface::SetPrimary()
{
    // set surface as primary
    return LocalInterface->SetPrimary(*this);
}


int Surface::IsPrimary() const
{
    // primary status
    return Primary;
}


int Surface::SetOrigin(int x,int y)
{
    // set surface origin
    if (Primary) return LocalInterface->SetOrigin(x,y);
    else return 0;
}


int Surface::GetOrigin(int &x,int &y) const
{
    // get surface origin
    if (Primary) return LocalInterface->GetOrigin(x,y);
    else return 0;
}







int Surface::NativeType()
{
    // return native surface type
    return InternalSurface->NativeType();
}


void* Surface::GetNative()
{
    // return native surface handle
    return InternalSurface->GetNative();
}


        
        
        
        
        
int Surface::GetWidth() const
{
    return Width;
}
 

int Surface::GetHeight() const
{
    return Height;
}


int Surface::GetType() const
{
    return Type;
}          


int Surface::GetOrientation() const
{
    return Orientation;
}


int Surface::GetLayout() const
{
    return Layout;
}


int Surface::GetAdvance() const
{
    return Advance;
}


int Surface::GetPitch() const
{
    return Pitch;
}


int Surface::GetArea() const
{
    return Width*Height;
}


int Surface::GetBitsPerPixel() const
{
    return Format.bits;
}


int Surface::GetBytesPerPixel() const
{
    return Format.bytes;
}


FORMAT Surface::GetFormat() const
{
    return Format;
}


RECTANGLE Surface::GetDimensions() const
{
    return RECTANGLE(0,0,Width,Height);
}


Interface* Surface::GetInterface() const
{
#ifndef __DJGPP__
    if (LocalInterface!=(Interface*)&SoftwareInterface && LocalInterface!=(Interface*)&DummyInterface) return LocalInterface;
#else
    if (LocalInterface!=(Interface*)DJGPP_SUCKS_SoftwareInterface && LocalInterface!=(Interface*)&DummyInterface) return LocalInterface;
#endif
    else return NULL;
}







int Surface::ok() const
{
    // surface status
    return InternalSurface->ok();
}

        
        
        
        
            
        
void Surface::operator =(Surface const &other)
{
    // initialize surface matching attributes of other surface
    if (!Init(other.LocalInterface,other.Width,other.Height,other.Format,other.Type,other.Orientation,other.Advance,other.Layout)) return;
}







Surface::Surface(PTC const &ptc,int type)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(ptc,type);
}


Surface::Surface(PTC const &ptc,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Init(ptc,width,height,format,type,orientation,advance,layout);
}


Surface::Surface(PTC const &ptc,char filename[],int type,int orientation,int advance,int layout)
{
    // init software
    InitSoftware();

    // defaults
    Defaults();

    // initialize
    Load(ptc.GetInterface(),filename,type,orientation,advance,layout);
}                    


int Surface::Init(PTC const &ptc,int type)
{
    // initialize
    return Init(ptc.GetInterface(),type);
}


int Surface::Init(PTC const &ptc,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // initialize
    return Init(ptc.GetInterface(),width,height,format,type,orientation,advance,layout);
}


int Surface::Load(PTC const &ptc,char filename[],int type,int orientation,int advance,int layout)
{
    // initialize
    return Load(ptc.GetInterface(),filename,type,orientation,advance,layout);
}


int Surface::Bind(PTC const &ptc)
{
    return Bind(ptc.GetInterface());
}


int Surface::Update(PTC const &ptc)
{
    return Update(ptc.GetInterface());
}


int Surface::Update(PTC const &ptc,RECTANGLE const &src)
{
    return Update(ptc.GetInterface(),src);
}


int Surface::Update(PTC const &ptc,RECTANGLE const &src,RECTANGLE const &dest)
{
    return Update(ptc.GetInterface(),src,dest);
}







int Surface::Init(int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // initialize
    return Init(&SoftwareInterface,width,height,format,type,orientation,advance,layout);
}


int Surface::Init(void *memory,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // how to handle?
    if (memory || width || height || format.ok() || type || orientation || advance || layout);
    return 0;
}


int Surface::Init(Interface *i,int type)
{
    // initialize
    if (i) return Init(i,i->GetXResolution(),i->GetYResolution(),i->GetFormat(),type,DEFAULT,DEFAULT,DEFAULT);
    else
    {
        // failure
        Free();
        return 0;
    }
}


int Surface::Init(Interface *i,int width,int height,FORMAT const &format,int type,int orientation,int advance,int layout)
{
    // fail on invalid interface
    if (!i) return 0;

    // check width and height
    if (width<=0 || height<=0) 
    {
        //printf("bad width or height\n");
        return 0;
    }

    // check format
    /*
    if (!format.ok()) 
    {
        printf("bad pixel format\n");
        return 0;
    }
    */

    // check advance
    if (advance<0 && advance!=DEFAULT) 
    {
        //printf("invalid advance\n");
        return 0;
    }

    // surface data
    int new_width=width;
    int new_height=height;
    int new_type=type;
    int new_orientation=orientation;
    int new_advance=advance;
    int new_layout=layout;
    FORMAT new_format=format;

    // request an internal surface object from interface
    Interface::SURFACE *surface=i->RequestSurface(new_width,new_height,new_format,new_type,new_orientation,new_advance,new_layout);
    if (!surface) 
    {
        //printf("RequestSurface failed\n");
        return 0;
    }

    // check new surface data against requested
    if (width!=DEFAULT && new_width!=width ||
        height!=DEFAULT && new_height!=height ||
        orientation!=DEFAULT && new_orientation!=orientation ||
        advance!=DEFAULT && new_advance!=advance ||
        layout!=DEFAULT && new_layout!=layout)          // note: removed new_format check for DX
    {
        // failure
        //printf("surface data mismatch!\n");
        delete surface;
        return 0;
    }

    // check surface type
    if (type==OFFSCREEN && (new_type!=SYSTEM && new_type!=VIDEO))
    {
        //printf("bad surface type\n");
        delete surface;
        return 0;
    }
    else if (type!=DEFAULT && new_type!=type)
    {
        //printf("bad surface type\n");
        delete surface;
        return 0;
    }

    // free
    Free();

    // detach/attach interface
    if (LocalInterface!=i)
    {
        LocalInterface->Detach(this);
        i->Attach(this);
    }

    // setup local interface
    LocalInterface=i;

    // setup internal surface
    InternalSurface=surface;

    // setup data
    Width=new_width;
    Height=new_height;
    Format=new_format;
    Type=new_type;
    Orientation=new_orientation;
    Advance=new_advance;
    Layout=new_layout;

    // setup pitch
    if (Orientation==TOPDOWN) Pitch=Width*Format.bytes+Advance;
    else Pitch=-(Width*Format.bytes+Advance);

    // setup default palette
    DefaultPalette();
    
    // setup default clipper
    DefaultClipper();

    // success
    return 1;
}


void Surface::Free()
{
    // detatch clipper
    DetachClipper();

    // detach palette
    DetachPalette();

    // free internal surface
    if (InternalSurface!=(Interface::SURFACE*)&DummySurface)
    {
        delete InternalSurface;
        InternalSurface=&DummySurface;
    }
}


void Surface::Invalidate()
{
    // detach from interface
    LocalInterface->Detach(this);

    // dummy interface
    LocalInterface=(Interface*)&DummyInterface;
}







void Surface::Defaults()
{
    // data defaults
    Width=0;
    Height=0;
    Type=0;
    Orientation=0;
    Advance=0;
    Layout=0;
    Pitch=0;
    Primary=0;
    LocalClipper=NULL;
    LocalPalette=&DummyPalette;
    LocalInterface=&DummyInterface;
    InternalSurface=&DummySurface;
}







void Surface::InitSoftware()
{
#ifdef __DJGPP__

    // check count...
    if (!DJGPP_SUCKS_SoftwareInterfaceCount)
    {
        // create software interface
        DJGPP_SUCKS_SoftwareInterface=new ISoftware;
    }

    // increment count
    DJGPP_SUCKS_SoftwareInterfaceCount++;

#endif
}


void Surface::CloseSoftware()
{
#ifdef __DJGPP__

    // check count
    if (DJGPP_SUCKS_SoftwareInterfaceCount==1)
    {
        // delete software interface
        delete DJGPP_SUCKS_SoftwareInterface;
    }

    // decrement count         
    DJGPP_SUCKS_SoftwareInterfaceCount--;

#endif
}







int Clip(Surface const &src,Surface const &dest,RECTANGLE &rect)
{
    // fix rect
    rect.fix();

    // source clip
    if (src.LocalClipper && !src.LocalClipper->SourceClip(rect)) return 0;

    // destination clip
    if (dest.LocalClipper && !dest.LocalClipper->DestinationClip(rect)) return 0;

    // success
    return 1;
}


int Clip(Surface const &src,RECTANGLE &src_rect,Surface const &dest,RECTANGLE &dest_rect)
{
    // clipper case
    if (src.LocalClipper && dest.LocalClipper)
    {
        // setup preclip rects
        RECTANGLE src_preclip=src_rect;
        RECTANGLE dest_preclip=dest_rect;
        src_preclip.fix();
        dest_preclip.fix();

        // clip source rectangle -> src_postclip
        RECTANGLE src_postclip=src_preclip;
        if (src.LocalClipper) if (!src.LocalClipper->SourceClip(src_postclip)) return 0;
        int dx=(int)(src_postclip.x1-src_preclip.x1);
        int dy=(int)(src_postclip.y1-src_preclip.y1);

        // adjust dest rectangle with src x1,y1 clip adjust
        dest_preclip.x1+=dx;
        dest_preclip.y1+=dy;
        if (dest_preclip.x1>dest_preclip.x2 || dest_preclip.y1>dest_preclip.y2) return 0;

        // clip dest rectangle -> dest_postclip
        RECTANGLE dest_postclip=dest_preclip;
        if (dest.LocalClipper) if (!dest.LocalClipper->DestinationClip(dest_postclip)) return 0;
        dx=(int)(dest_postclip.x1-dest_preclip.x1);
        dy=(int)(dest_postclip.y1-dest_preclip.y1);

        // adjust src post clip rect with dest x1,y1 adjust
        src_postclip.x1+=dx;
        src_postclip.y1+=dy;
        if (src_postclip.x1>src_postclip.x2 || src_postclip.y1>src_postclip.y2) return 0;

        // get rectangle widths & heights
        int src_width=(int)src_postclip.width();
        int src_height=(int)src_postclip.height();
        int dest_width=(int)dest_postclip.width();
        int dest_height=(int)dest_postclip.height();

        // final width
        int width;
        if (src_width<dest_width) width=src_width;
        else width=dest_width;
    
        // final height
        int height;
        if (src_height<dest_height) height=src_height;
        else height=dest_height;

        // setup output rectangles
        src_rect.x1=src_postclip.x1;
        src_rect.y1=src_postclip.y1;
        src_rect.x2=src_postclip.x1+width;
        src_rect.y2=src_postclip.y1+height;
        dest_rect.x1=dest_postclip.x1;
        dest_rect.y1=dest_postclip.y1;
        dest_rect.x2=dest_postclip.x1+width;
        dest_rect.y2=dest_postclip.y1+height;
        return 1;
    }
    else if (src.LocalClipper)
    {
        // source only clip
        return src.LocalClipper->SourceClip(src_rect);
    }
    else if (dest.LocalClipper)
    {
        // destination only clip
        return dest.LocalClipper->DestinationClip(dest_rect);
    }
    else
    {
        // no clipping
        return 1;
    }
}
