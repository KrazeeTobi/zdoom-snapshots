///////////////////////////////////////
// 32bit -> X conversion (intel MMX) //
///////////////////////////////////////

#ifndef __PTC_MMX_32BIT_H
#define __PTC_MMX_32BIT_H

#include "raster.h"








#if defined(__32BIT__) && defined(__MMX__)

// interface to external asm converters (do not call directly!)
extern "C" void Convert32_ABGR8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGBA8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGRA8888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGR888_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB565_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGR565_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_ARGB1555_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_ABGR1555_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_GREY8_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB332_MMX(void *src,void *dest,uint pixels,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux Convert32_ABGR8888_MMX "_*"
#pragma aux Convert32_RGBA8888_MMX "_*"
#pragma aux Convert32_BGRA8888_MMX "_*"
#pragma aux Convert32_RGB888_MMX   "_*"
#pragma aux Convert32_BGR888_MMX   "_*"
#pragma aux Convert32_RGB565_MMX   "_*"
#pragma aux Convert32_BGR565_MMX   "_*"
#pragma aux Convert32_ARGB1555_MMX "_*"
#pragma aux Convert32_ABGR1555_MMX "_*"
#pragma aux Convert32_GREY8_MMX    "_*"
#pragma aux Convert32_RGB332_MMX   "_*"
#endif

#endif








#endif
