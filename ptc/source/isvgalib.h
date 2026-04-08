///////////////////////
// SVGALIB interface //
///////////////////////

#ifndef __PTC_ISVGALIB_H
#define __PTC_ISVGALIB_H

#include "idummy.h"
#include "isoft.h"
#include "block.h"

#ifdef __SVGALIB__

class ISVGALIB : public ISoftware
  {
  public:

    // setup
    ISVGALIB(WINDOW window=NULL);
    virtual ~ISVGALIB();

    // interface information
    virtual Interface::INFO GetInfo();
    virtual int GetModeList(List<MODE> &modelist);

    // display mode routines
    virtual int SetMode(MODE const &info);
    virtual int SetMode(int x,int y,int id,int output,int frequency,int layout);
    virtual int SetMode(int x,int y,FORMAT const &format,int output,int frequency,int layout);
    virtual MODE GetMode();

    // palette routines
    virtual int SetPalette(Palette &palette);
    virtual int GetPalette(Palette &palette);
        
    // hardware functions
    virtual int WaitForRetrace();
        
    // primary surface operations
    virtual int SetPrimary(Surface &surface);
    virtual Surface* GetPrimary();
    virtual int SetOrigin(int x,int y);
    virtual int GetOrigin(int &x,int &y);

    // video memory management
    virtual int GetTotalVideoMemory();
    virtual int GetFreeVideoMemory();
    virtual int CompactVideoMemory();

    // console routines
    virtual int getch();
    virtual int kbhit();

    // data access
    virtual void GetName(char name[]) const;
    virtual int GetXResolution() const;
    virtual int GetYResolution() const;
    virtual int GetBitsPerPixel() const;
    virtual int GetBytesPerPixel() const;
    virtual int GetOutput() const;
    virtual int GetFrequency() const;
    virtual int GetLayout() const;
    virtual FORMAT GetFormat() const;
    virtual WINDOW GetWindow() const;

    // object state
    virtual int ok() const;

  protected:

    // internal surface management
    virtual Interface::SURFACE* RequestSurface(int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);

    // internal svgalib surface
    class SURFACE : public ISoftware::SURFACE
      {
      public:
            
	// setup
	SURFACE(ISVGALIB &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
	virtual ~SURFACE();

	// surface memory
	virtual void* Lock(int wait);
	virtual void Unlock();
	virtual int LockCount();
	virtual int Lockable();
	virtual int Restore();

	// native access
	virtual int NativeType();
	virtual void* GetNative();

	// status
	virtual int ok();

      private:
	// data
	MemoryBlock *Block;
	int LockOffset;
      };

  private:

    // internal mode setting
    int SetMode(int x,int y,FORMAT const &format,int layout);
    int SetGREY8(int x,int y,int layout);
    int SetRGB332(int x,int y,int layout);
    int SetFakeMode(int x,int y,int id,int layout);
    int SetMode(int mode, int layout);
    int GetMode(int x, int y, int id);

    // vga memory clear
    void ClearMemory();

    // video memory management
    int InitVideoMemory();
    void CloseVideoMemory();

    // primary surface management
    int InitPrimary();
    void ClosePrimary();

    // modelist helper
    int AddMode(List<MODE> &modelist,int x,int y,FORMAT const &format,int output,int layout);
        
    // data
    uint XResolution;                   // x resolution of display
    uint YResolution;                   // y resolution of display
    FORMAT Format;                      // pixel format of display
    int Layout;                         // memory layout of display
    Surface* PrimarySurface;            // primary surface object
    void* LFB;                          // pointer to video memory
    int LFBSize;                        // size of video memory

    // video memory manager
    MemoryManager VideoMemory;

    // status
    int Status;                         // status variable (1=OK, 0=INVALID)

    // friend classes
    friend class SURFACE;
  };

#else

class ISVGALIB : public IDummy
  {
  public:

    // dummy constructor
    ISVGALIB(WINDOW window=NULL) { if (window); };
  };

#endif

#endif
