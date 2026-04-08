///////////////////////////////////////
// 32bit -> X conversion (intel x86) //
///////////////////////////////////////

#ifndef __PTC_X86_32BIT_H
#define __PTC_X86_32BIT_H

#include "raster.h"








#if defined(__32BIT__) && defined(__X86__)

// interface to external asm converters (do not call directly!)
extern "C" void Convert32_ABGR8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGBA8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGRA8888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGR888_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB565_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_BGR565_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_ARGB1555_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_ABGR1555_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_GREY8_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_RGB332_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void AreaConvert32_ABGR8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_RGBA8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_BGRA8888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_RGB888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_BGR888_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_RGB565_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_BGR565_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_ARGB1555_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_ABGR1555_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_GREY8_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert32_RGB332_X86(RASTER::CONVERTER::AREA *data,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux Convert32_ABGR8888_X86 "_*"
#pragma aux Convert32_RGBA8888_X86 "_*"
#pragma aux Convert32_BGRA8888_X86 "_*"
#pragma aux Convert32_RGB888_X86   "_*"
#pragma aux Convert32_BGR888_X86   "_*"
#pragma aux Convert32_RGB565_X86   "_*"
#pragma aux Convert32_BGR565_X86   "_*"
#pragma aux Convert32_ARGB1555_X86 "_*"
#pragma aux Convert32_ABGR1555_X86 "_*"
#pragma aux Convert32_GREY8_X86    "_*"
#pragma aux Convert32_RGB332_X86   "_*"
#pragma aux AreaConvert32_ABGR8888_X86 "_*"
#pragma aux AreaConvert32_RGBA8888_X86 "_*"
#pragma aux AreaConvert32_BGRA8888_X86 "_*"
#pragma aux AreaConvert32_RGB888_X86   "_*"
#pragma aux AreaConvert32_BGR888_X86   "_*"
#pragma aux AreaConvert32_RGB565_X86   "_*"
#pragma aux AreaConvert32_BGR565_X86   "_*"
#pragma aux AreaConvert32_ARGB1555_X86 "_*"
#pragma aux AreaConvert32_ABGR1555_X86 "_*"
#pragma aux AreaConvert32_GREY8_X86    "_*"
#pragma aux AreaConvert32_RGB332_X86   "_*"
#endif

#endif








#endif
