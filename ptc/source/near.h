//////////////////////////
// near memory routines //
//////////////////////////

#ifndef __PTC_NEAR_H
#define __PTC_NEAR_H

#include "misc.h"






// near pointer prototypes
inline void LockNearBase();
inline void UnlockNearBase();
inline void* AdjustNearPointer(void *pointer);
//inline uint MapRealPointer(void *pointer);






#if defined(__DJGPP__)


// djgpp implementation
#include <sys/nearptr.h>
#include <crt0.h>

extern int NearBaseLockCount;

inline void LockNearBase()
{
    // increment near lock count
    NearBaseLockCount++;

    // enable near pointers
    if (NearBaseLockCount==1) __djgpp_nearptr_enable(); 
}

inline void UnlockNearBase()
{
    // disable near pointers on last unlock
    if (NearBaseLockCount==1) __djgpp_nearptr_disable();

    // decrememnt lock count
    NearBaseLockCount--;
}

inline void* AdjustNearPointer(void *pointer)
{
    // adjust near pointer with conventional memory base
    return (void*)((uint)pointer+__djgpp_conventional_base);
}


#else


inline void LockNearBase()
{
    // nothing
}

inline void UnlockNearBase()
{
    // nothing
}

inline void* AdjustNearPointer(void *pointer)
{
    // no change
    return pointer;
}


#endif






// map realmode address (watcom c++)
#if defined(__WATCOMC__) || defined(__DJGPP__)
#define MapRealPointer(p)   (void*)((((uint)(p)>>12)&0xFFFF0)+((p)&0xFFFF))
#endif
/*
// map realmode address
inline uint MapRealPointer(void *pointer)
{
    return (void*)((((uint)(pointer)>>12)&0xFFFF0)+((pointer)&0xFFFF))
}
*/







#endif

