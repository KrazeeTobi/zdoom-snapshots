///////////////////////////////////////
// 16bit -> X conversion (intel MMX) //
///////////////////////////////////////

#ifndef __PTC_MMX_16BIT_H
#define __PTC_MMX_16BIT_H

#include "raster.h"








#if defined(__16BIT__) && defined(__MMX__)

// interface to external asm converters (do not call directly)
extern "C" void Convert16_ARGB8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ABGR8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGBA8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGRA8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGB888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGR888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGR565_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ARGB1555_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ABGR1555_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_GREY8_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGB332_MMX(void *src,void *dest,uint pixels,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9 
#pragma aux Convert16_ARGB8888_MMX "_*"
#pragma aux Convert16_ABGR8888_MMX "_*"
#pragma aux Convert16_RGBA8888_MMX "_*"
#pragma aux Convert16_BGRA8888_MMX "_*"
#pragma aux Convert16_RGB888_MMX   "_*"  
#pragma aux Convert16_BGR888_MMX   "_*"  
#pragma aux Convert16_BGR565_MMX   "_*"
#pragma aux Convert16_ARGB1555_MMX "_*"
#pragma aux Convert16_ABGR1555_MMX "_*"
#pragma aux Convert16_GREY8_MMX    "_*"
#pragma aux Convert16_RGB332_MMX   "_*"
#endif

#endif








#endif
