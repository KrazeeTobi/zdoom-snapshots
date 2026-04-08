///////////////////////
// SVGALIB interface //
///////////////////////
#include "isvgalib.h"
#include "surface.h"

#ifdef __SVGALIB__

#include <vga.h>

ISVGALIB::ISVGALIB(WINDOW /*window*/)
  {
  // defaults
  XResolution = 0;
  YResolution = 0;
  Layout = 0;
  PrimarySurface = NULL;

  // initialize svgalib
  Status = (vga_init() == 0);

  // set up LFB
  LFB = 0;
  LFBSize = 0;
  }

ISVGALIB::~ISVGALIB()
  {
  // return to textmode if needed
  vga_setmode(TEXT);

  // todo: restore font (stop xterm font corruption)
  
  // close primary surface
  ClosePrimary();

  // close video memory
  CloseVideoMemory();
  }

Interface::INFO ISVGALIB::GetInfo()
  {
  // return info
  INFO info;
  memset(&info, 0, sizeof(info));
  return info;
  }

int ISVGALIB::GetModeList(List<MODE> &modelist)
  {
  // clear list
  modelist.free();

  // add video modes to list
  AddMode(modelist, 320, 200, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 240, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 400, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 360, 480, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, GREY8, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, GREY8, FULLSCREEN, LINEAR);

  AddMode(modelist, 320, 200, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 240, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 400, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 360, 480, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, INDEX8, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, INDEX8, FULLSCREEN, LINEAR);

  AddMode(modelist, 320, 200, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 240, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 320, 400, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 360, 480, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, RGB332, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, RGB332, FULLSCREEN, LINEAR);

  AddMode(modelist, 320, 200, ARGB1555, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, ARGB1555, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, ARGB1555, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, ARGB1555, FULLSCREEN, LINEAR);

  AddMode(modelist, 320, 200, RGB565, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, RGB565, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, RGB565, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, RGB565, FULLSCREEN, LINEAR);

  AddMode(modelist, 320, 200, RGB888, FULLSCREEN, LINEAR);
  AddMode(modelist, 640, 480, RGB888, FULLSCREEN, LINEAR);
  AddMode(modelist, 800, 600, RGB888, FULLSCREEN, LINEAR);
  AddMode(modelist, 1024, 768, RGB888, FULLSCREEN, LINEAR);

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

int ISVGALIB::SetMode(const MODE &mode)
  {
  // check mode interface name
  char name[8];
  GetName(name);
  if (stricmp(mode.i, name) != 0) return 0;

  // interface name ok - set mode
  return SetMode(mode.x, mode.y, mode.format, mode.output, mode.frequency, mode.layout);
  }

int ISVGALIB::SetMode(int x, int y, int id, int output, int frequency, int layout)
  {
  // fail on invalid output parameter
  if (output != FULLSCREEN && output != DEFAULT) return 0;

  // fail on invalid frequency parameter
  if (frequency != DEFAULT && frequency != UNKNOWN) return 0;

  // FAKEMODE to FAKEMODE2A
  if (id == FAKEMODE) id=FAKEMODE2A;

  // fail on default (x,y)
  if (x == DEFAULT || y == DEFAULT) return 0;

  // DEFAULT layout -> LINEAR
  //if (layout == DEFAULT) layout = LINEAR;

  // DIRECT -> VIRTUAL32
  if (id == DIRECT) id = VIRTUAL32;

  // INDEXED -> INDEX8
  if (id == INDEXED) id = VIRTUAL8;

  switch(id)
    {
    case VIRTUAL32:
      // VIRTUAL 32bit mode set
      if (SetMode(x,y,FORMAT(ARGB8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGBA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(BGRA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(ABGR8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGB888),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR888),layout))     return 1;
      if (SetMode(x,y,FORMAT(RGB565),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR565),layout))     return 1;
      if (SetMode(x,y,FORMAT(ARGB1555),layout))   return 1;
      if (SetMode(x,y,FORMAT(ABGR1555),layout))   return 1;
      if (SetMode(x,y,FORMAT(FAKEMODE2A),layout)) return 1;
      break;
    case VIRTUAL16:
      // VIRTUAL 16bit mode set
      if (SetMode(x,y,FORMAT(RGB565),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR565),layout))     return 1;
      if (SetMode(x,y,FORMAT(ARGB1555),layout))   return 1;
      if (SetMode(x,y,FORMAT(ABGR1555),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGB888),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR888),layout))     return 1;
      if (SetMode(x,y,FORMAT(ARGB8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGBA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(BGRA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(ABGR8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(FAKEMODE2A),layout)) return 1;
      break;
    case VIRTUAL8:
      // VIRTUAL 8bit mode set
      if (SetMode(x,y,FORMAT(INDEX8),layout))     return 1;
      if (SetMode(x,y,FORMAT(ARGB8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGBA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(BGRA8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(ABGR8888),layout))   return 1;
      if (SetMode(x,y,FORMAT(RGB888),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR888),layout))     return 1;
      if (SetMode(x,y,FORMAT(RGB565),layout))     return 1;
      if (SetMode(x,y,FORMAT(BGR565),layout))     return 1;
      if (SetMode(x,y,FORMAT(ARGB1555),layout))   return 1;
      if (SetMode(x,y,FORMAT(FAKEMODE2A),layout)) return 1;
      break;
    default:
      // attempt modeset from format id
      if (SetMode(x,y,FORMAT(id),layout)) return 1;

      // try modeset from bits per pixel
      if (SetMode(x,y,id,layout)) return 1;
    }

  return 0;
  }

int ISVGALIB::SetMode(int x, int y, const FORMAT &format, int output, int frequency, int layout)
  {
  // fail on invalid output parameter
  if (output != FULLSCREEN && output != DEFAULT) return 0;

  // fail on invalid frequency parameter
  if (frequency != DEFAULT && frequency != UNKNOWN) return 0;

  // fail on default (x, y)
  if (x == DEFAULT || y == DEFAULT) return 0;

  // DEFAULT layout -> LINEAR
  if (layout == DEFAULT) layout = LINEAR;

  // set mode
  return SetMode(x,y,format,layout);
  }

MODE ISVGALIB::GetMode()
  {
  // get mode info
  MODE mode;
  GetName(mode.i);
  mode.x = XResolution;
  mode.y = YResolution;
  mode.format = Format;
  mode.output = FULLSCREEN;
  mode.frequency = UNKNOWN;
  mode.layout = Layout;
  return mode;
  }

int ISVGALIB::SetPalette(Palette &palette)
  {
  // only set palette when indexed format
  if (Format.type != INDEXED) return 0;

  // access palette data
  uint* data_argb = (uint*)palette.ReadOnly();
  if (!data_argb) return 0;

  // shift palette data down to 6bits per primary & pack into RGB
  int data_rgb[768];
  int* p = data_rgb;
  uint* q = data_argb;
  uint* qmax = q+256;
  while (q<qmax)
    {
    uchar r, g, b;
    UnpackRGB32(*q, r, g, b);
    *(p+0) = (uchar)(r>>2);
    *(p+1) = (uchar)(g>>2);
    *(p+2) = (uchar)(b>>2);
    p += 3;
    q++;
    }

  vga_setpalvec(0, 256, data_rgb);
  return 1;
  }

int ISVGALIB::GetPalette(Palette &palette)
  {
  // only set palette when indexed format
  if (Format.type != INDEXED) return 0;

  // access palette palette data
  uint* data_argb = (uint*)palette.Lock();
  if (!data_argb) return 0;

  // fetch palette
  int data_rgb[768];
  vga_getpalvec(0, 256, data_rgb);

  // shift palette back up to 8 bits per component and pack into ARGB8888
  int*  p = data_rgb;
  uint* q = data_argb;
  uint* qstop = q+256;
  while (q<qstop)
    {
    *q = RGB32((uchar)(*(p+1)<<2), (uchar)(*(p+1)<<2), (uchar)(*(p+2)<<2));
    p += 3;
    q++;
    }

  // unlock
  palette.Unlock();
  return 1;
  }

int ISVGALIB::WaitForRetrace()
  {
  // wait for vertical retrace
  vga_waitretrace();
  return 1;
  }

int ISVGALIB::SetPrimary(Surface& /*surface*/)
  {
  return 0;
  }

Surface* ISVGALIB::GetPrimary()
  {
  return PrimarySurface;
  }

int ISVGALIB::SetOrigin(int /*x*/, int /*y*/)
  {
  return 0;
  }

int ISVGALIB::GetOrigin(int& x, int& y)
  {
  x = 0;
  y = 0;
  return 0;
  }

int ISVGALIB::GetTotalVideoMemory()
  {
  return LFBSize;
  }

int ISVGALIB::GetFreeVideoMemory()
  {
  return 0;
  }

int ISVGALIB::CompactVideoMemory()
  {
  return 0;
  }

int ISVGALIB::getch()
  {
  return vga_getch();
  }

int ISVGALIB::kbhit()
  {
  return vga_getkey();
  }

void ISVGALIB::GetName(char name[]) const
  {
  strcpy(name, "SVGALIB");
  }

int ISVGALIB::GetXResolution() const
  {
  return XResolution;
  }

int ISVGALIB::GetYResolution() const
  {
  return YResolution;
  }

int ISVGALIB::GetBitsPerPixel() const
  {
  return Format.bits;
  }

int ISVGALIB::GetBytesPerPixel() const
  {
  return Format.bytes;
  }
	    
int ISVGALIB::GetOutput() const
  {
  return FULLSCREEN;
  }

int ISVGALIB::GetFrequency() const
  {
  return UNKNOWN;
  }

int ISVGALIB::GetLayout() const
  {
  return Layout;
  }

FORMAT ISVGALIB::GetFormat() const
  {
  return Format;
  }
	    
WINDOW ISVGALIB::GetWindow() const
  {
  return NULL;
  }

int ISVGALIB::ok() const
  {
  return Status;
  }

Interface::SURFACE* ISVGALIB::RequestSurface(int &width, int &height, FORMAT &format, int &type, int &orientation, int &advance, int &layout)
  {
  // create native surface
  SURFACE* surface = new SURFACE(*this, width, height, format, type, orientation, advance, layout);
  if (surface && surface->ok()) return surface;
  else delete surface;

  // fallback to software
  return ISoftware::RequestSurface(width, height, format, type, orientation, advance, layout);
  }

int ISVGALIB::SetMode(int x, int y, const FORMAT &format, int layout)
  {
  // default layout
  if (layout==DEFAULT)
  {
      if (format.type==FAKEMODE) layout=FAKEMODE;
      else layout=LINEAR;
  }

  // special format mode sets      
  if (format.id == GREY8 && SetGREY8(x, y, layout)) return 1;
  if (format.id == RGB332 && SetRGB332(x, y, layout)) return 1;
  if (format.type == FAKEMODE && SetFakeMode(x,y,format.id,layout)) return 1;

  // standard set mode
  if (!SetMode(GetMode(x, y, format.id), layout)) return 0;

  // initialize
  Format.init(format.id);
  XResolution = x;
  YResolution = y;
  Layout = layout;

  // initialize video memory
  if (!InitVideoMemory()) return 0;

  // init primary surface
  if (!InitPrimary()) return 0;

  // success
  return 1;
  }

int ISVGALIB::SetGREY8(int x, int y, int layout)
  {
  // enter mode
  if (!SetMode(GetMode(x, y, INDEX8), layout)) return 0;

  // set greyscale palette
  for (int c = 0; c<256; c++)
    {
    vga_setpalette(c, c>>2, c>>2, c>>2);
    }       
   
  // initialize
  Format.init(GREY8);
  XResolution = x;
  YResolution = y;
  Layout = layout;

  // initialize video memory
  if (!InitVideoMemory()) return 0;

  // init primary surface
  if (!InitPrimary()) return 0;

  // success
  return 1;
  }

int ISVGALIB::SetRGB332(int x, int y, int layout)
  {
  // enter mode 
  if (!SetMode(GetMode(x, y, INDEX8), layout)) return 0;

  // setup RGB332 palette
  Palette palette;
  uint* data = (uint*)palette.Lock();
  for (int i = 0; i<256; i++)
    {
    float r = (float)((i&0xE0)>>5) * (float)255.0 / (float)7.0;
    float g = (float)((i&0x1C)>>2) * (float)255.0 / (float)7.0;
    float b = (float) (i&0x03)     * (float)255.0 / (float)3.0;
    data[i] = RGB32((uchar)r, (uchar)g, (uchar)b);
    }
  palette.Unlock();

  // set RGB332 palette (fudge format)
  Format.init(INDEX8);
  if (!SetPalette(palette)) return 0;

  // initialize data
  Format.init(RGB332);
  XResolution = x;
  YResolution = y;
  Layout = layout;

  // initialize video memory
  if (!InitVideoMemory()) return 0;

  // init primary surface
  if (!InitPrimary()) return 0;

  // success
  return 1;
  }


int ISVGALIB::SetFakeMode(int x,int y,int id,int layout)
{
#ifdef __FAKEMODE__

    // check layout
    /*
    if (layout!=DEFAULT && layout!=FAKEMODE)
    {
        printf("bad layout\n");
        return 0;
    }
    */

    // check resolution
    if (x!=320 || y!=200) return 0;

    // attempt direct fakemode svgalib modesets
    if (id==FAKEMODE1A || id==FAKEMODE1B || id==FAKEMODE1C)
    {
        // initialize 320x600 via svgalib
        if (!SetMode(GetMode(320,600,INDEX8),LINEAR))
        {
            // svgalib failure: tweak 320x600 manually
            if (!SetMode(GetMode(320,200,INDEX8),FAKEMODE)) return 0;

            // clear VGA palette
            Palette palette;
            Format.init(INDEX8);
            SetPalette(palette);

            // clear memory
            ClearMemory();
    
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
        }
    }
    else if (id==FAKEMODE2A || id==FAKEMODE2B || id==FAKEMODE2C ||
             id==FAKEMODE3A || id==FAKEMODE3B || id==FAKEMODE3C)
    {
        // initialize 320x400 via svgalib
        if (!SetMode(GetMode(320,400,INDEX8),FAKEMODE))
        {
            // svgalib failure: tweak 320x400 manually from mode13h
            if (!SetMode(GetMode(320,200,INDEX8),LINEAR)) return 0;

            // clear VGA palette
            Palette palette;
            Format.init(INDEX8);
            SetPalette(palette);

            // clear memory
            ClearMemory();
    
            // enter fakemode
            if (id==FAKEMODE2A || id==FAKEMODE2B || id==FAKEMODE2C ||
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
        }
    }
    else
    {
        // invalid id
        return 0;
    }
    
    // set fakemode palette
    if (id==FAKEMODE1A || id==FAKEMODE1B || id==FAKEMODE1C ||
        id==FAKEMODE3A || id==FAKEMODE3B || id==FAKEMODE3C)
    {
        // setup palette for FAKEMODE1x and FAKEMODE3x
        Palette palette;
        uint *data=(uint*)palette.Lock();
        if (!data) return 0;
        int c=0;
        for (c=0; c<64; c++) data[c]     = RGB32(c<<2,0,0);
        for (c=0; c<64; c++) data[c+64]  = RGB32(0,c<<2,0);
        for (c=0; c<64; c++) data[c+128] = RGB32(0,0,c<<2);
        for (c=0; c<64; c++) data[c+192] = RGB32(c<<2,c<<2,c<<2);
        palette.Unlock();
        Format.init(INDEX8);
        if (!SetPalette(palette)) return 0;
    }
    else if (id==FAKEMODE2A || id==FAKEMODE2B || id==FAKEMODE2C)
    {
        // setup palette for FAKEMODE2x
        Palette palette;
        uint *data=(uint*)palette.Lock();
        if (!data) return 0;
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

        // set palette
        Format.init(INDEX8);
        if (!SetPalette(palette)) return 0;
    }

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

#else 

    // failure
    return 0;

#endif
}


int ISVGALIB::SetMode(int mode, int layout)
  {
  if (mode == -1) return 0;
  if (vga_setmode(mode) == -1) return 0;
  //if (!LFB)
    {
    if (mode == G320x200x256 || layout!=LINEAR)
      {
      LFB = vga_getgraphmem();
      LFBSize = 0xFFFF;
      }
    else
      {
      LFBSize = vga_setlinearaddressing();
      if (LFBSize == -1) return 0;
      LFB = vga_getgraphmem();           // todo: fix this function to 
      }                                  // properly handle linear, banked, vga
    }                                    // modes properly
  return 1; 
  }

int ISVGALIB::GetMode(int x, int y, int id)
  {
  if (x == 320 && y == 200 && id == INDEX8)  return G320x200x256;
  if (x == 320 && y == 240 && id == INDEX8)  return G320x240x256;
  if (x == 320 && y == 400 && id == INDEX8)  return G320x400x256;
  if (x == 360 && y == 480 && id == INDEX8)  return G360x480x256;
  if (x == 640 && y == 480 && id == INDEX8)  return G640x480x256;
  if (x == 800 && y == 600 && id == INDEX8)  return G800x600x256;
  if (x == 1024 && y == 768 && id == INDEX8) return G1024x768x256;
  
  if (x == 320 && y == 200 && id == ARGB1555)  return G320x200x32K;
  if (x == 640 && y == 480 && id == ARGB1555)  return G640x480x32K;
  if (x == 800 && y == 600 && id == ARGB1555)  return G800x600x32K;
  if (x == 1024 && y == 768 && id == ARGB1555) return G1024x768x32K;

  if (x == 320 && y == 200 && id == RGB565)  return G320x200x64K;
  if (x == 640 && y == 480 && id == RGB565)  return G640x480x64K;
  if (x == 800 && y == 600 && id == RGB565)  return G800x600x64K;
  if (x == 1024 && y == 768 && id == RGB565) return G1024x768x64K;

  if (x == 320 && y == 200 && id == RGB888)  return G320x200x16M;
  if (x == 640 && y == 480 && id == RGB888)  return G640x480x16M;
  if (x == 800 && y == 600 && id == RGB888)  return G800x600x16M;
  if (x == 1024 && y == 768 && id == RGB888) return G1024x768x16M;

  // extension (320x600 8bit for FAKEMODE1x)
  #ifdef G320x600x256
  if (x == 320 && y == 600 && id == INDEX8) return G320x600x256;
  #endif
  
  // no mode found
  return -1;
  }

void ISVGALIB::ClearMemory()
{
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
}

int ISVGALIB::InitVideoMemory()
  {
  // close old manager
  CloseVideoMemory();

  // initialize video memory manager
  VideoMemory.Init(LFB, LFBSize);
  if (!VideoMemory.ok()) return 0;
  else return 1;
  }

void ISVGALIB::CloseVideoMemory()
  {
  // free all video surfaces
  List<Surface>::Iterator iterator = SurfaceList.first();

  Surface* current = iterator.current();
  while (current)
    {
    if (current->Type == VIDEO) current->Free();
    current = iterator.next();
    }

  // free all video memory
  VideoMemory.Close();
  }

int ISVGALIB::InitPrimary()
  {
  // close old primary
  ClosePrimary();

  // initialize new video memory surface
  PrimarySurface = new Surface(this, XResolution, YResolution, Format, VIDEO, DEFAULT, DEFAULT, Layout);

  if (!PrimarySurface) return 0;

  if (!PrimarySurface->ok())
    {
    // failure
    ClosePrimary();
    return 0;
    }

  // success
  return 1;
  }

void ISVGALIB::ClosePrimary()
  {
  // free primary surface
  delete PrimarySurface;
  PrimarySurface = NULL;
  }
  
int ISVGALIB::AddMode(List<MODE> &modelist, int x, int y, const FORMAT &format, int output, int layout)
  {
  // add mode to list
  MODE* mode = new MODE;
  GetName(mode->i);
  mode->x = x;
  mode->y = y;
  mode->format = format;
  mode->output = output;
  mode->frequency = UNKNOWN;
  mode->layout = layout;
  return modelist.add(mode);
  }

ISVGALIB::SURFACE::SURFACE(ISVGALIB &i, int &width, int &height, FORMAT &format, int &type, int &orientation, int &advance, int &layout)
  {
  // defaults
  Block = NULL;

  // check parameters
  if ((width <= 0 || height <= 0) || !format.ok() || (type != VIDEO && type != OFFSCREEN) ||
      (orientation != TOPDOWN && orientation != BOTTOMUP && orientation != DEFAULT) || 
      (advance != DEFAULT && advance<0) || (layout != LINEAR && layout != FAKEMODE && layout != DEFAULT)) return;

  // setup new orientation
  int new_orientation = orientation;
  if (new_orientation == DEFAULT) new_orientation = TOPDOWN;
	
  // setup new advance
  int new_advance = advance;
  if (new_advance == DEFAULT) new_advance = 0;

  // setup new layout
  int new_layout = layout;

  // format type
  if (format.type == FAKEMODE)
    {
    // TOPDOWN orientation only
    if (new_orientation != TOPDOWN) return;

    // default layout is FAKEMODE
    if (new_layout != DEFAULT && new_layout != FAKEMODE) return;
    else new_layout = FAKEMODE;

    // fail on non-zero advance
    if (new_advance != 0) return;

    // allocate video memory block (take entire 64k)
    Block = new MemoryBlock(i.VideoMemory, 0xFFFF);
    }                                           
  else
    {
    // default layout is LINEAR
    if (new_layout != DEFAULT && new_layout != LINEAR) return;
    else new_layout = LINEAR;

    // linear only
    if (new_layout != LINEAR) return;

    // calculate size of surface memory
    uint size = width*height*format.bytes + height*new_advance;
    if (!size) return;

    // allocate video memory block
    Block = new MemoryBlock(i.VideoMemory, size);
    }

  // check block
  if (!Block || !Block->ok())
    {
    delete Block;
    Block = NULL;
    return;
    }

  // setup data
  type = VIDEO;
  orientation = new_orientation;
  advance = new_advance;
  layout = new_layout;
	
  // setup lock offset
  if (orientation == TOPDOWN) LockOffset = 0;
  else LockOffset = (width*format.bytes+advance)*(height-1);
  }

ISVGALIB::SURFACE::~SURFACE()
  {
  delete Block;
  }

void* ISVGALIB::SURFACE::Lock(int /*wait*/)
  {
  // lock + offset (orientation)
  return ((char*)Block->lock()) + LockOffset;
  }

void ISVGALIB::SURFACE::Unlock()
  {
  // unlock
  Block->unlock();
  }

int ISVGALIB::SURFACE::LockCount()
  {
  // surface lock count
  return Block->count();
  }

int ISVGALIB::SURFACE::Lockable()
  {
  // lockable
  return 1;
  }

int ISVGALIB::SURFACE::Restore()
  {
  // no restore
  return 0;
  }

int ISVGALIB::SURFACE::NativeType()
  {
  // native type
  return NATIVE_UNAVAILABLE;
  }

void* ISVGALIB::SURFACE::GetNative()
  {
  // return native
  return NULL;
  }

int ISVGALIB::SURFACE::ok()
  {
  // status
  if (!Block) return 0;
  else return Block->ok();
  }

#endif
