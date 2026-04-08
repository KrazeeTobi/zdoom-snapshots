////////////////////
// VESA interface //
////////////////////

#ifndef __PTC_IVESA_H
#define __PTC_IVESA_H

#include "idummy.h"
#include "isoft.h"
#include "block.h"
#include "dpmi.h"
#include "far.h"
#include "near.h"
#include "matrox.h"








#ifdef __VESA__

class IVESA : public ISoftware
{
    public:

        // setup
        IVESA(WINDOW window=NULL);
        virtual ~IVESA();

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

        // internal vesa surface
        class SURFACE : public ISoftware::SURFACE
        {
            public:
            
                // setup
                SURFACE(IVESA &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
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

        // VBE information block
        #pragma pack(1)
        struct VBEINFO
        {
            char    VESASignature[4]     PACKED;    // 'VESA' 4 byte signature
            short   Version              PACKED;    // VBE version number
            long    OemStringPtr         PACKED;    // Pointer to OEM string
            long    Capabilities         PACKED;    // Capabilities of video card
            long    VideoModePtr         PACKED;    // Pointer to supported modes
            short   TotalMemory          PACKED;    // Number of 64kb memory blocks
            // VBE 2.0 extensions
            short   OemSoftwareRev       PACKED;    // OEM Software revision number
            long    OemVendorNamePtr     PACKED;    // Pointer to Vendor Name string
            long    OemProductNamePtr    PACKED;    // Pointer to Product Name string
            long    OemProductRevPtr     PACKED;    // Pointer to Product Revision str
            char    reserved[222]        PACKED;    // Pad to 256 byte block size
            char    OemDATA[256]         PACKED;    // Scratch pad for OEM data
        };

        // VESA mode information block
        struct MODEINFO
        {
            short   ModeAttributes       PACKED;    // Mode attributes
            char    WinAAttributes       PACKED;    // Window A attributes
            char    WinBAttributes       PACKED;    // Window B attributes
            short   WinGranularity       PACKED;    // Window granularity in k
            short   WinSize              PACKED;    // Window size in k
            short   WinASegment          PACKED;    // Window A segment
            short   WinBSegment          PACKED;    // Window B segment
            long    WinFuncPtr           PACKED;    // Pointer to window function
            short   BytesPerScanLine     PACKED;    // Bytes per scanline
            short   XResolution          PACKED;    // Horizontal resolution
            short   YResolution          PACKED;    // Vertical resolution
            char    XCharSize            PACKED;    // Character cell width
            char    YCharSize            PACKED;    // Character cell height
            char    NumberOfPlanes       PACKED;    // Number of memory planes
            char    BitsPerPixel         PACKED;    // Bits per pixel
            char    NumberOfBanks        PACKED;    // Number of CGA style banks
            char    MemoryModel          PACKED;    // Memory model type
            char    BankSize             PACKED;    // Size of CGA style banks
            char    NumberOfImagePages   PACKED;    // Number of images pages
            char    reserved1            PACKED;    // Reserved
            char    RedMaskSize          PACKED;    // Size of direct color red mask
            char    RedFieldPosition     PACKED;    // Bit posn of lsb of red mask
            char    GreenMaskSize        PACKED;    // Size of direct color green mask
            char    GreenFieldPosition   PACKED;    // Bit posn of lsb of green mask
            char    BlueMaskSize         PACKED;    // Size of direct color blue mask
            char    BlueFieldPosition    PACKED;    // Bit posn of lsb of blue mask
            char    AlphaMaskSize        PACKED;    // Size of direct color alpha mask
            char    AlphaFieldPosition   PACKED;    // Bit posn of lsb of alpha mask
            char    DirectColorModeInfo  PACKED;    // Direct color mode attributes
            // VBE 2.0 extensions
            long    PhysBasePtr          PACKED;    // Physical address for linear buf
            long    OffScreenMemOffset   PACKED;    // Pointer to start of offscreen mem
            short   OffScreenMemSize     PACKED;    // Amount of offscreen mem in 1K's
            char    reserved2[206]       PACKED;    // Pad to 256 byte block size
        };
        #pragma pack()

        // mode attributes constants               
        enum MODEATTRIBS { ModeAvailable = 0x0001,  // Mode is available
                           ModeColor     = 0x0008,  // Mode is a color video mode
                           ModeGraphics  = 0x0010,  // Mode is a graphics mode
                           ModeNonBanked = 0x0040,  // Banked mode is not supported
                           ModeLinear    = 0x0080}; // Linear mode supported

        // internal mode setting
        int SetMode(int mode);
        int SetMode(int x,int y,int bits,int layout);
        int SetMode(int x,int y,FORMAT const &format,int layout);
        int SetGREY8(int x,int y,int layout);
        int SetRGB332(int x,int y,int layout);

        // mode number retrieval
        int GetModeNumber(int x,int y,int bits,int layout);
        int GetModeNumber(int x,int y,FORMAT const &format,int layout);

        // mode information retrieval
        MODE GetMode(MODEINFO const &modeinfo);
        FORMAT GetFormat(MODEINFO const &modeinfo);
        int GetModeInfo(int mode,MODEINFO *modeinfo);
        int GetModeInfo(int x,int y,int bits,MODEINFO *modeinfo);
        int GetModeInfo(int x,int y,FORMAT const &format,MODEINFO *modeinfo);

        // internal vesa routines
        int SetWindowPosition(int position);
        int SetDisplayStart(int x,int y,int vrt=1);
        int GetDisplayStart(int &x,int &y);
        int SetDACWidth(int width);
        int GetDACWidth();
        int InitInfo();
        int Error(int eax);

        // linear frame buffer management
        int InitLFB();
        void FreeLFB();

        // video memory management
        int InitVideoMemory();
        void CloseVideoMemory();

        // primary surface management
        int InitPrimary();
        void ClosePrimary();

        // miscellaneous
        int AddModes(List<MODE> &list,MODEINFO const &modeinfo);
        void AddMode(List<MODE> &list,MODE const &mode);

        // emulation
        int Save(char filename[]);
        int Load(char filename[]);

        // interface data
        FORMAT Format;                      // pixel format of display
        int Layout;                         // layout of display
        int NeedRestore;                    // need to restore text mode (yes/no)
        WINDOW Window;                     // output window (required under win32)
        Surface *PrimarySurface;            // primary surface

        // vesa data
        void *LFB;                          // address of linear frame buffer
        VBEINFO Info;                       // vesa bios information
        MODEINFO ModeInfo;                  // current vesa mode information
        MODEINFO *ModeInfoList;             // mode information list storage (for emulation only)
        short ModeList[512];                // list of available mode numbers
        int DisplayStartX;                  // display start x
        int DisplayStartY;                  // display start y
        int DACWidth;                       // DAC width (palette modes)

        // memory managers
        MemoryManager VideoMemory;
        MemoryManager SystemMemory;

        // dpmi object
        #ifdef __DOS32__
        DPMI dpmi;
        #endif

        // matrox object
        #ifdef __MATROX__
        MATROX *Matrox;
        int NativeMatrox;
        #endif

        // status
        int Status;                         // status variable (1=OK, 0=INVALID)


    // friend classes
    friend class SURFACE;
};

#else

class IVESA : public IDummy
{
    public:

        // dummy object
        IVESA(WINDOW window=NULL) { if (window); };
};

#endif








#endif
