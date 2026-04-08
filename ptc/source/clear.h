///////////////////
// memory clears //
///////////////////

#ifndef __PTC_CLEAR_H
#define __PTC_CLEAR_H

#include "misc.h"







inline void Clear_1BYTE(void *dest,void *data,uint pixels)
{
    // clear (1 byte per pixel)
    memset(dest,*(char*)data,pixels);
}


inline void Clear_2BYTE(void *dest,void *data,uint pixels)
{
    // clear (2 bytes per pixel)
    ushort color=*(ushort*)data;
    ushort *p=(ushort*)dest;
    ushort *pmax=p+pixels;
    while (p<pmax)
    {
        *p=color;
        p++;
    }
}

inline void Clear_3BYTE(void *dest,void *data,uint pixels)
{
    // clear (3 bytes per pixel)

    // use parms to advoid warning...
    if (dest || data || pixels);
}                               


inline void Clear_4BYTE(void *dest,void *data,uint pixels)
{
    // clear (4 bytes per pixel)
    uint color=*(uint*)data;
    uint *p=(uint*)dest;
    uint *pmax=p+pixels;
    while (p<pmax)
    {
        *p=color;
        p++;
    }
}


                





#endif
