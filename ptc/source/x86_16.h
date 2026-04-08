///////////////////////////////////////
// 16bit -> X conversion (intel x86) //
///////////////////////////////////////

#ifndef __PTC_X86_16BIT_H
#define __PTC_X86_16BIT_H

#include "raster.h"








#if defined(__16BIT__) && defined(__X86__)

// interface to external asm converters (do not call directly)
extern "C" void Convert16_ARGB8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ABGR8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGBA8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGRA8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGB888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGR888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_BGR565_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ARGB1555_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_ABGR1555_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_GREY8_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_RGB332_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void AreaConvert16_ARGB8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_ABGR8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_RGBA8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_BGRA8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_RGB888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_BGR888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_BGR565_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_ARGB1555_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_ABGR1555_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_GREY8_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert16_RGB332_X86(RASTER::CONVERTER::AREA *data,void *extra);

// lookup tables
#ifndef __CYGNUS__
extern "C" uint Convert16_ARGB8888_LUT_X86[512];
extern "C" uint Convert16_ABGR8888_LUT_X86[512];
extern "C" uint Convert16_RGBA8888_LUT_X86[512];
extern "C" uint Convert16_BGRA8888_LUT_X86[512];
#endif

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9 
#pragma aux Convert16_ARGB8888_X86 "_*"
#pragma aux Convert16_ABGR8888_X86 "_*"
#pragma aux Convert16_RGBA8888_X86 "_*"
#pragma aux Convert16_BGRA8888_X86 "_*"
#pragma aux Convert16_RGB888_X86   "_*"  
#pragma aux Convert16_BGR888_X86   "_*"  
#pragma aux Convert16_BGR565_X86   "_*"
#pragma aux Convert16_ARGB1555_X86 "_*"
#pragma aux Convert16_ABGR1555_X86 "_*"
#pragma aux Convert16_GREY8_X86    "_*"
#pragma aux Convert16_RGB332_X86   "_*"
#pragma aux AreaConvert16_ARGB8888_X86 "_*"
#pragma aux AreaConvert16_ABGR8888_X86 "_*"
#pragma aux AreaConvert16_RGBA8888_X86 "_*"
#pragma aux AreaConvert16_BGRA8888_X86 "_*"
#pragma aux AreaConvert16_RGB888_X86   "_*"  
#pragma aux AreaConvert16_BGR888_X86   "_*"  
#pragma aux AreaConvert16_BGR565_X86   "_*"
#pragma aux AreaConvert16_ARGB1555_X86 "_*"
#pragma aux AreaConvert16_ABGR1555_X86 "_*"
#pragma aux AreaConvert16_GREY8_X86    "_*"
#pragma aux AreaConvert16_RGB332_X86   "_*"
#endif

#endif








#endif
