/////////////////////////
// far memory routines //
/////////////////////////

#ifndef __PTC_FAR_H
#define __PTC_FAR_H

#include "misc.h"







// prototypes
inline void far_memcpy(uint dest_selector,uint dest_offset,uint src_selector,uint src_offset,uint bytes);
inline void far_memcpy(void const *dest,uint src_selector,uint src_offset,uint bytes);
inline void far_memcpy(uint dest_selector,uint dest_offset,void const *src,uint bytes);
inline void far_memset(uint dest_selector,uint dest_offset,int value,uint bytes);







#if defined(__WATCOMC__)

// watcom implementation
#include <i86.h>
#include <mem.h>

inline void far_memcpy(uint dest_selector,uint dest_offset,uint src_selector,uint src_offset,uint bytes)
{
    // far to far
    _fmemcpy(MK_FP(dest_selector,dest_offset),MK_FP(src_selector,src_offset),bytes);
}

inline void far_memcpy(void const *dest,uint src_selector,uint src_offset,uint bytes)
{
    // far to near
    far_memcpy(FP_SEG((void far*)dest),FP_OFF((void far*)dest),src_selector,src_offset,bytes);
}

inline void far_memcpy(uint dest_selector,uint dest_offset,void const *src,uint bytes)
{
    // near to far
    far_memcpy(dest_selector,dest_offset,FP_SEG((void far*)src),FP_OFF((void far*)src),bytes);
}

inline void far_memset(uint dest_selector,uint dest_offset,int value,uint bytes)
{
    // far memset
    _fmemset(MK_FP(dest_selector,dest_offset),value,bytes);
}



#elif defined(__VISUALC__)



// visual c++ implementation

inline void far_memcpy(uint dest_selector,uint dest_offset,uint src_selector,uint src_offset,uint bytes)
{
    // far to far
}

inline void far_memcpy(void const *dest,uint src_selector,uint src_offset,uint bytes)
{
    // far to near
}

inline void far_memcpy(uint dest_selector,uint dest_offset,void const *src,uint bytes)
{
    // near to far
}

inline void far_memset(uint dest_selector,uint dest_offset,int value,uint bytes)
{
    // far memset
}



#elif defined(__DJGPP__)


// djgpp implementation
#include <go32.h>

inline void far_memcpy(uint dest_selector,uint dest_offset,uint src_selector,uint src_offset,uint bytes)
{
    // far to far                                          
    movedata(src_selector,src_offset,dest_selector,dest_offset,bytes);
}

inline void far_memcpy(void const *dest,uint src_selector,uint src_offset,uint bytes)
{                                                                                     
    // far to near                    
    movedata(src_selector,src_offset,(uint)_my_ds(),(uint)dest,bytes);
}

inline void far_memcpy(uint dest_selector,uint dest_offset,void const *src,uint bytes)
{
    // near to far
    movedata((uint)_my_ds(),(uint)src,dest_selector,dest_offset,bytes);
}

inline void far_memset(uint dest_selector,uint dest_offset,int value,uint bytes)
{
    // far memset (not yet)
    if (dest_selector || dest_offset || value || bytes);
}


#else


inline void far_memcpy(uint dest_selector,uint dest_offset,uint src_selector,uint src_offset,uint bytes)
{
    // far to far
}

inline void far_memcpy(void const *dest,uint src_selector,uint src_offset,uint bytes)
{
    // far to near
}

inline void far_memcpy(uint dest_selector,uint dest_offset,void const *src,uint bytes)
{
    // near to far
}

inline void far_memset(uint dest_selector,uint dest_offset,int value,uint bytes)
{
    // far memset
}


#endif







#endif
