/////////////////////
// Amiga interface //
/////////////////////

#ifndef __PTC_IAMIGA_H
#define __PTC_IAMIGA_H







#ifdef __AMIGA__

// KLUDGE
extern struct ExecBase *
#ifdef __CONSTLIBBASEDECL__
__CONSTLIBBASEDECL__
#endif /* __CONSTLIBBASEDECL__ */
SysBase;

extern struct RTGMasterBase *RTGMasterBase;

// MAJOR FUCKING KLUDGE - FIX WHEN AWAKE!

#define TAG_USER   ((ULONG)(1L<<31))
#define TAG_DONE   0

#define smr_Dummy         TAG_USER
#define smr_MinWidth      (smr_Dummy + 0x01)
#define smr_MaxWidth      (smr_Dummy + 0x02)
#define smr_MinHeight     (smr_Dummy + 0x03)
#define smr_MaxHeight     (smr_Dummy + 0x04)
#define smr_ChunkySupport (smr_Dummy + 0x09)
#define smr_Buffers       (smr_Dummy + 0x0b)
#define smr_TitleText     (smr_Dummy + 0x18)

#define rtg_Dummy TAG_USER
#define rtg_Buffers (rtg_Dummy + 0x01)

#define grd_Dummy TAG_USER
#define grd_Width (grd_Dummy + 0x01)
#define grd_Height (grd_Dummy + 0x02)

typedef unsigned char UBYTE;
typedef unsigned long ULONG;
typedef unsigned short UWORD;
typedef ULONG Tag;

struct TagItem
{
    Tag	  ti_Tag;	/* identifies the type of data */
    ULONG ti_Data;	/* type-specific data	       */
};

// KLUDGE - Add all used prototypes here!

void CloseLibrary( struct Library *library );
struct Library *OpenLibrary( UBYTE *libName, unsigned long version );

void   CloseRtgScreen(struct RtgScreen *MyScreen);
void   FreeRtgScreenModeReq(struct ScreenReq *myreq);
void   GetRtgScreenData(struct RtgScreen *MyScreen, struct TagItem *taglist);
void   LoadRGBRtg(struct RtgScreen *MyScreen, void *Table);
void   *LockRtgScreen(struct RtgScreen *MyScreen);
struct RtgScreen * OpenRtgScreen(struct ScreenReq *sreq, struct TagItem *taglist);
struct ScreenReq * RtgScreenModeReq(struct TagItem *taglist);
void   UnlockRtgScreen(struct RtgScreen *MyScreen);

#define LP1NR(offs, name, t1, v1, r1, bt, bn)			\
({								\
   t1 _##name##_v1 = (v1);					\
   {								\
      register struct Library *const _##name##_bn __asm("a6") = (struct Library*)(bn); \
      register t1 _n1 __asm(#r1) = _##name##_v1;		\
      __asm volatile ("jsr a6@(-"#offs":W)"			\
      : /* no output */						\
      : "r" (_##name##_bn), "r"(_n1)				\
      : "d0", "d1", "a0", "a1", "fp0", "fp1", "cc", "memory");	\
   }								\
})

#define LP2NR(offs, name, t1, v1, r1, t2, v2, r2, bt, bn)	\
({								\
   t1 _##name##_v1 = (v1);					\
   t2 _##name##_v2 = (v2);					\
   {								\
      register struct Library *const _##name##_bn __asm("a6") = (struct Library*)(bn); \
      register t1 _n1 __asm(#r1) = _##name##_v1;		\
      register t2 _n2 __asm(#r2) = _##name##_v2;		\
      __asm volatile ("jsr a6@(-"#offs":W)"			\
      : /* no output */						\
      : "r" (_##name##_bn), "r"(_n1), "r"(_n2)			\
      : "d0", "d1", "a0", "a1", "fp0", "fp1", "cc", "memory");	\
   }								\
})

#define LP1(offs, rt, name, t1, v1, r1, bt, bn)			\
({								\
   t1 _##name##_v1 = (v1);					\
   {								\
      register rt _##name##_re __asm("d0");			\
      register struct Library *const _##name##_bn __asm("a6") = (struct Library*)(bn); \
      register t1 _n1 __asm(#r1) = _##name##_v1;		\
      __asm volatile ("jsr a6@(-"#offs":W)"			\
      : "=r" (_##name##_re)					\
      : "r" (_##name##_bn), "r"(_n1)				\
      : "d0", "d1", "a0", "a1", "fp0", "fp1", "cc", "memory");	\
      _##name##_re;						\
   }								\
})

#define LP2(offs, rt, name, t1, v1, r1, t2, v2, r2, bt, bn)	\
({								\
   t1 _##name##_v1 = (v1);					\
   t2 _##name##_v2 = (v2);					\
   {								\
      register rt _##name##_re __asm("d0");			\
      register struct Library *const _##name##_bn __asm("a6") = (struct Library*)(bn); \
      register t1 _n1 __asm(#r1) = _##name##_v1;		\
      register t2 _n2 __asm(#r2) = _##name##_v2;		\
      __asm volatile ("jsr a6@(-"#offs":W)"			\
      : "=r" (_##name##_re)					\
      : "r" (_##name##_bn), "r"(_n1), "r"(_n2)			\
      : "d0", "d1", "a0", "a1", "fp0", "fp1", "cc", "memory");	\
      _##name##_re;						\
   }								\
})

#define EXEC_BASE_NAME (*(struct ExecBase **)4)

#define CloseLibrary(library) \
	LP1NR(0x19e, CloseLibrary, struct Library *, library, a1, \
	, EXEC_BASE_NAME)

#define OpenLibrary(libName, version) \
	LP2(0x228, struct Library *, OpenLibrary, UBYTE *, libName, a1, unsigned long, version, d0, \
	, EXEC_BASE_NAME)

#ifndef RTGMASTER_BASE_NAME
#define RTGMASTER_BASE_NAME RTGMasterBase
#endif /* !RTGMASTER_BASE_NAME */

#define CloseRtgScreen(RtgScreen_) \
        LP1NR(0x24, CloseRtgScreen, struct RtgScreen *, RtgScreen_, a0, \
        , RTGMASTER_BASE_NAME)

#define FreeRtgScreenModeReq(ScreenReq_) \
        LP1NR(0x66, FreeRtgScreenModeReq, struct ScreenReq *, ScreenReq_, a0, \
        , RTGMASTER_BASE_NAME)

#define GetRtgScreenData(RtgScreen_, RtgTags) \
        LP2NR(0x48, GetRtgScreenData, struct RtgScreen *, RtgScreen_, a0, struct TagItem *, RtgTags, a1, \
        , RTGMASTER_BASE_NAME)

#define LoadRGBRtg(RtgScreen_, Table) \
        LP2NR(0x30, LoadRGBRtg, struct RtgScreen *, RtgScreen_, a0, void *, Table, a1, \
        , RTGMASTER_BASE_NAME)

#define LockRtgScreen(RtgScreen_) \
        LP1(0x36, void   *, LockRtgScreen, struct RtgScreen *, RtgScreen_, a0, \
        , RTGMASTER_BASE_NAME)

#define OpenRtgScreen(ScreenReq_, Tags) \
        LP2(0x1e, struct RtgScreen *, OpenRtgScreen, struct ScreenReq *, ScreenReq_, a0, struct TagItem *, Tags, a1, \
        , RTGMASTER_BASE_NAME)

#define RtgScreenModeReq(Tags) \
        LP1(0x60, struct ScreenReq *, RtgScreenModeReq, struct TagItem *, Tags, a0, \
        , RTGMASTER_BASE_NAME)

#define UnlockRtgScreen(RtgScreen_) \
        LP1NR(0x3c, UnlockRtgScreen, struct RtgScreen *, RtgScreen_, a0, \
        , RTGMASTER_BASE_NAME)

#endif

#include "idummy.h"
#include "isoft.h"
#include "block.h"

#ifdef __AMIGA__

class IAmiga : public ISoftware
{
    public:

        // setup
        IAmiga(WINDOW window=NULL);
        virtual ~IAmiga();

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

        // internal surface
        class SURFACE : public ISoftware::SURFACE
        {
            public:
            
                // setup
                SURFACE(IAmiga &i,int &width,int &height,FORMAT &format,int &type,int &orientation,int &advance,int &layout);
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
        int SetLinearMode(int x,int y,int id);
        
        // video memory management
        int InitVideoMemory();
        void ClearVideoMemory();
        void CloseVideoMemory();

        // primary surface management
        int InitPrimary();
        void ClosePrimary();

        // data
        uint XResolution;                   // x resolution of display
        uint YResolution;                   // y resolution of display
        FORMAT Format;                      // pixel format of display
        int Layout;                         // memory layout of display
        Surface *PrimarySurface;            // primary surface object
        void *LFB;                          // pointer to video memory
        unsigned LFBSize;                   // size of video memory

        // video memory manager
        MemoryManager VideoMemory;

        // status
        int Status;                         // status variable (1=OK, 0=INVALID)

        struct ScreenReq *ScreenReq;        // ptr to screenmode requester
        struct RtgScreen *RtgScreen;        // ptr to RTG screen

    // friend classes
    friend class SURFACE;
};

#else

class IAmiga : public IDummy
{
    public:

        // dummy constructor
        IAmiga(WINDOW window=NULL) { if (window); };
};

#endif








#endif
