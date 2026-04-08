////////////////////
// DPMI interface //
////////////////////

#ifndef __PTC_DPMI_H
#define __PTC_DPMI_H

#include "lang.h"
#include "misc.h"
#include "config.h"


#ifdef __DPMI__










class DPMI
{
    public:

        // DPMI registers
        #pragma pack(1)
        struct REGS
        {
            uint edi      PACKED;
            uint esi      PACKED;
            uint ebp      PACKED;
            uint reserved PACKED;
            uint ebx      PACKED;
            uint edx      PACKED;
            uint ecx      PACKED;
            uint eax      PACKED;
            ushort flags  PACKED;
            ushort es     PACKED;
            ushort ds     PACKED;
            ushort fs     PACKED;
            ushort gs     PACKED;
            ushort ip     PACKED;
            ushort cs     PACKED;
            ushort sp     PACKED;
            ushort ss     PACKED;
        };
        #pragma pack()

        // DPMI info
        #pragma pack(1)
        struct INFO
        {
            uchar major     PACKED;
            uchar minor     PACKED;
            ushort flags    PACKED;
            uchar processor PACKED;
            uchar masterPIC PACKED;
            uchar slavePIC  PACKED;
        };
        #pragma pack()
        
        // setup
        inline DPMI();
        inline ~DPMI();

        // high level dpmi interface
        inline int GetDosMemory(ushort *segment,ushort *selector,uint bytes=4096); 

        // low level dpmi interface
        inline static int GetVersion(INFO *version);
        inline static int AllocateSelector(ushort *selector);
        inline static int FreeSelector(ushort selector);
        inline static int SegmentToSelector(ushort segment,ushort *selector);
        inline static int GetSelectorBase(ushort selector,uint *base);
        inline static int SetSelectorBase(ushort selector,uint base);
        inline static int SetSelectorLimit(ushort selector,uint limit);
        inline static int SetSelectorRights(ushort selector,ushort rights);
        inline static int AllocateDosMemory(ushort *segment,ushort *selector,uint bytes);
        inline static int FreeDosMemory(ushort selector);
        inline static int ResizeDosMemory(ushort selector,uint bytes);
        inline static void* MapPhysicalAddress(uint physical,uint size);
        inline static int FreePhysicalMapping(uint linear);
        inline static int int86(uchar interrupt_no,DPMI::REGS *in,DPMI::REGS *out);

        // status
        inline int ok();

    private:

        // static dos memory
        static int InitDosMemory(int bytes);
        static void CloseDosMemory();
        static int DosMemoryCount;
        static ushort DosMemorySegment;
        static ushort DosMemorySelector;
        static uint DosMemorySize;

        // status
        int Status;
};






// includes
#include <dos.h>
#include "far.h"
#include <string.h>

#ifdef __DJGPP__
#include <dpmi.h>
#endif







inline DPMI::DPMI()
{
    // initialize dos memory
    if (!InitDosMemory(10*1024)) Status=0;
    else Status=1;
}


inline DPMI::~DPMI()
{
    // close dos memory
    CloseDosMemory();
}






inline int DPMI::GetDosMemory(ushort *segment,ushort *selector,uint size)
{
    if (size>DosMemorySize)
    {
        // resize static block?
        return 0;
    }
         
    // return static dos memory block
    *segment=DosMemorySegment;
    *selector=DosMemorySelector;
    return 1;
}






inline int DPMI::GetVersion(DPMI::INFO *version)
{
#ifdef __DJGPP__

    if (__dpmi_get_version((__dpmi_version_ret*)version)!=-1) return 1;
    else return 0;

#else

    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=0x400;
    ::int386(0x31,&regs,&regs);
    version->major=regs.h.ah;
    version->minor=regs.h.al;
    version->flags=regs.w.bx;
    version->processor=regs.h.cl;
    version->masterPIC=regs.h.dh;
    version->slavePIC=regs.h.dl;
    return !regs.w.cflag;

#endif
}


inline int DPMI::AllocateSelector(ushort *selector)
{     
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=0;
    regs.w.cx=1;
    ::int386(0x31,&regs,&regs);
    *selector=regs.w.ax;
    return !regs.w.cflag;
}


inline int DPMI::FreeSelector(ushort selector)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=1;
    regs.w.bx=selector;
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;
}


inline int DPMI::SegmentToSelector(ushort segment,ushort *selector)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=2;
    regs.w.bx=segment;
    ::int386(0x31,&regs,&regs);
    *selector=regs.w.ax;
    return !regs.w.cflag;
}


inline int DPMI::GetSelectorBase(ushort selector,uint *base)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=6;
    regs.w.bx=selector;
    ::int386(0x31,&regs,&regs);
    *base=((uint)regs.w.cx<<16)+regs.w.dx;
    return !regs.w.cflag;
}


inline int DPMI::SetSelectorBase(ushort selector,uint base)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax = 7;
    regs.w.bx = selector;
    regs.w.cx = (ushort)(base>>16);
    regs.w.dx = (ushort)(base&0xFFFF);
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;
}


inline int DPMI::SetSelectorLimit(ushort selector,uint limit)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax = 8;
    regs.w.bx = selector;
    regs.w.cx = (ushort)(limit>>16);
    regs.w.dx = (ushort)(limit&0xFFFF);
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;
}


inline int DPMI::SetSelectorRights(ushort selector,ushort rights)
{
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=9;
    regs.w.bx=selector;
    regs.w.cx=rights;
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;
}


inline int DPMI::AllocateDosMemory(ushort *segment,ushort *selector,uint bytes)
{
/*
#ifdef __DJGPP__

    int sel;
    if (__dpmi_allocate_dos_memory((bytes+16)>>4,&sel)!=-1)
    {
        *selector=sel;
        segment=
        return 1;
    }
    else return 0;

#else
*/
    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax = 0x100;
    regs.w.bx = (ushort)((bytes+16)>>4);
    ::int386(0x31,&regs,&regs);
    if (regs.w.cflag==0)
    {
        *segment=regs.w.ax;
        *selector=regs.w.dx;
        return 1;
    }
    else return 0;

//#endif
}


inline int DPMI::FreeDosMemory(ushort selector)
{
#ifdef __DJGPP__

    if (__dpmi_free_dos_memory(selector)!=-1) return 1;
    else return 0;

#else

    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=0x101;
    regs.w.dx=selector;
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;

#endif
}


inline int DPMI::ResizeDosMemory(ushort selector,uint bytes)
{
#ifdef __DJGPP__

    if (__dpmi_resize_dos_memory(selector,(bytes+16)>>4,NULL)!=-1) return 1;
    else return 0;

#else

    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax=0x102;
    regs.w.bx=selector;
    regs.w.dx=(ushort)((bytes+16)>>4);
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;

#endif
}


inline void* DPMI::MapPhysicalAddress(uint physical,uint size)
{
#ifdef __DJGPP__

    __dpmi_meminfo info;
    info.size=size;
    info.address=physical;    
    if (__dpmi_physical_address_mapping(&info)!=-1) return (void*)info.address;
    else return NULL;

#else

    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax = 0x800;
    regs.w.bx = (ushort)(physical>>16);
    regs.w.cx = (ushort)(physical&0xFFFF);
    regs.w.si = (ushort)(size>>16);
    regs.w.di = (ushort)(size&0xFFFF);
    ::int386(0x31,&regs,&regs);
    if (regs.w.cflag==0) return (void*) ( ((uint)regs.w.bx<<16) + regs.w.cx );
    else return NULL;

#endif
}


inline int DPMI::FreePhysicalMapping(uint linear)
{
#ifdef __DJGPP__

    __dpmi_meminfo info;
    info.size=0;
    info.address=linear;
    if (__dpmi_free_physical_address_mapping(&info)!=-1) return 1;
    else return 0;

#else

    ::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.w.ax = 0x801;
    regs.w.bx = (ushort)(linear>>16);
    regs.w.cx = (ushort)(linear&0xFFFF);
    ::int386(0x31,&regs,&regs);
    return !regs.w.cflag;

#endif
}


inline int DPMI::int86(uchar interrupt_no,DPMI::REGS *in,DPMI::REGS *out)
{
#ifdef __DJGPP__

    if (__dpmi_int(interrupt_no,(__dpmi_regs*)in)!=-1)
    {
        memcpy(out,in,sizeof(DPMI::REGS));
        return 1;
    }
    else return 0;

#else

    ::REGS r;
    ::SREGS s;
    memset(&r,0,sizeof(::REGS));
    memset(&s,0,sizeof(::SREGS));
    r.w.ax  = 0x300;
    r.w.bx  = interrupt_no;
    r.w.cx  = 0;
    s.es    = FP_SEG(in);
    r.x.edi = FP_OFF(in);
    ::int386x(0x31,&r,&r,&s);
    far_memcpy(out,s.es,r.x.edi,sizeof(DPMI::REGS));
    if (r.w.cflag) return 1;
    else return 0;

#endif
}





inline int DPMI::ok()
{
    return Status;
}







#endif

#endif
