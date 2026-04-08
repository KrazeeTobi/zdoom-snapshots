//////////////////////////////////////////
// X -> FAKEMODE conversion (intel MMX) //
//////////////////////////////////////////

#ifndef __PTC_MMX_FAKEMODE_H
#define __PTC_MMX_FAKEMODE_H

#include "raster.h"







#if defined(__FAKEMODE__) && defined(__MMX__)

// interface to external x86 asm converters (do not call directly!)
extern "C" void Convert32_FAKEMODE1A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE1B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE1C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2C_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3A_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3B_MMX(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3C_MMX(void *src,void *dest,uint pixels,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux Convert32_FAKEMODE1A_MMX "_*"
#pragma aux Convert32_FAKEMODE1B_MMX "_*"
#pragma aux Convert32_FAKEMODE1C_MMX "_*"
#pragma aux Convert32_FAKEMODE2A_MMX "_*"
#pragma aux Convert32_FAKEMODE2B_MMX "_*"
#pragma aux Convert32_FAKEMODE2C_MMX "_*"
#pragma aux Convert32_FAKEMODE3A_MMX "_*"
#pragma aux Convert32_FAKEMODE3B_MMX "_*"
#pragma aux Convert32_FAKEMODE3C_MMX "_*"
#pragma aux Convert16_FAKEMODE1A_MMX "_*"
#pragma aux Convert16_FAKEMODE1B_MMX "_*"
#pragma aux Convert16_FAKEMODE1C_MMX "_*"
#pragma aux Convert16_FAKEMODE2A_MMX "_*"
#pragma aux Convert16_FAKEMODE2B_MMX "_*"
#pragma aux Convert16_FAKEMODE2C_MMX "_*"
#pragma aux Convert16_FAKEMODE3A_MMX "_*"
#pragma aux Convert16_FAKEMODE3B_MMX "_*"
#pragma aux Convert16_FAKEMODE3C_MMX "_*"
#pragma aux Convert8_FAKEMODE1A_MMX  "_*"
#pragma aux Convert8_FAKEMODE1B_MMX  "_*"
#pragma aux Convert8_FAKEMODE1C_MMX  "_*"
#pragma aux Convert8_FAKEMODE2A_MMX  "_*"
#pragma aux Convert8_FAKEMODE2B_MMX  "_*"
#pragma aux Convert8_FAKEMODE2C_MMX  "_*"
#pragma aux Convert8_FAKEMODE3A_MMX  "_*"
#pragma aux Convert8_FAKEMODE3B_MMX  "_*"
#pragma aux Convert8_FAKEMODE3C_MMX  "_*"
#endif

#endif








#endif
