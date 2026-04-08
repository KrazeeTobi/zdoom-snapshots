//////////////////////////////////////
// 8bit -> X conversion (intel MMX) //
//////////////////////////////////////

#ifndef __PTC_MMX_8BIT_H
#define __PTC_MMX_8BIT_H

#include "raster.h"








#if defined(__8BIT__) && defined (__MMX__)

// interface to external asm converters (do not call directly)
extern "C" void Convert8_4BYTE_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_3BYTE_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_2BYTE_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_1BYTE_MMX(void *src,void *dest,uint pixels,void *extra);


// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9                       
#pragma aux Convert8_4BYTE_MMX "_*"
#pragma aux Convert8_3BYTE_MMX "_*"
#pragma aux Convert8_2BYTE_MMX "_*"
#pragma aux Convert8_1BYTE_MMX "_*"
#endif

#endif








#endif
