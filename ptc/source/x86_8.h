//////////////////////////////////////
// 8bit -> X conversion (intel x86) //
//////////////////////////////////////

#ifndef __PTC_X86_8BIT_H
#define __PTC_X86_8BIT_H

#include "raster.h"








#if defined(__8BIT__) && defined (__X86__)

// interface to external asm converters (do not call directly)
extern "C" void Convert8_4BYTE_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_3BYTE_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_2BYTE_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_1BYTE_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void AreaConvert8_4BYTE_X86(void *data,void *extra);
extern "C" void AreaConvert8_3BYTE_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert8_2BYTE_X86(RASTER::CONVERTER::AREA *data,void *extra);
extern "C" void AreaConvert8_1BYTE_X86(RASTER::CONVERTER::AREA *data,void *extra);


// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9                       
#pragma aux Convert8_4BYTE_X86 "_*"
#pragma aux Convert8_3BYTE_X86 "_*"
#pragma aux Convert8_2BYTE_X86 "_*"
#pragma aux Convert8_1BYTE_X86 "_*"
#pragma aux AreaConvert8_4BYTE_X86 "_*"
#pragma aux AreaConvert8_3BYTE_X86 "_*"
#pragma aux AreaConvert8_2BYTE_X86 "_*"
#pragma aux AreaConvert8_1BYTE_X86 "_*"
#endif

#endif








#endif
