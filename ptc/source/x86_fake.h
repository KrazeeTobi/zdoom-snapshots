//////////////////////////////////////////
// X -> FAKEMODE conversion (intel x86) //
//////////////////////////////////////////

#ifndef __PTC_X86_FAKEMODE_H
#define __PTC_X86_FAKEMODE_H

#include "raster.h"







#if defined(__FAKEMODE__) && defined(__X86__)

// interface to external x86 asm converters (do not call directly!)
extern "C" void Convert32_FAKEMODE1A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE1B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE1C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE2C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert32_FAKEMODE3C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE1C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE2C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert16_FAKEMODE3C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE1C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE2C_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3A_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3B_X86(void *src,void *dest,uint pixels,void *extra);
extern "C" void Convert8_FAKEMODE3C_X86(void *src,void *dest,uint pixels,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux Convert32_FAKEMODE1A_X86 "_*"
#pragma aux Convert32_FAKEMODE1B_X86 "_*"
#pragma aux Convert32_FAKEMODE1C_X86 "_*"
#pragma aux Convert32_FAKEMODE2A_X86 "_*"
#pragma aux Convert32_FAKEMODE2B_X86 "_*"
#pragma aux Convert32_FAKEMODE2C_X86 "_*"
#pragma aux Convert32_FAKEMODE3A_X86 "_*"
#pragma aux Convert32_FAKEMODE3B_X86 "_*"
#pragma aux Convert32_FAKEMODE3C_X86 "_*"
#pragma aux Convert16_FAKEMODE1A_X86 "_*"
#pragma aux Convert16_FAKEMODE1B_X86 "_*"
#pragma aux Convert16_FAKEMODE1C_X86 "_*"
#pragma aux Convert16_FAKEMODE2A_X86 "_*"
#pragma aux Convert16_FAKEMODE2B_X86 "_*"
#pragma aux Convert16_FAKEMODE2C_X86 "_*"
#pragma aux Convert16_FAKEMODE3A_X86 "_*"
#pragma aux Convert16_FAKEMODE3B_X86 "_*"
#pragma aux Convert16_FAKEMODE3C_X86 "_*"
#pragma aux Convert8_FAKEMODE1A_X86  "_*"
#pragma aux Convert8_FAKEMODE1B_X86  "_*"
#pragma aux Convert8_FAKEMODE1C_X86  "_*"
#pragma aux Convert8_FAKEMODE2A_X86  "_*"
#pragma aux Convert8_FAKEMODE2B_X86  "_*"
#pragma aux Convert8_FAKEMODE2C_X86  "_*"
#pragma aux Convert8_FAKEMODE3A_X86  "_*"
#pragma aux Convert8_FAKEMODE3B_X86  "_*"
#pragma aux Convert8_FAKEMODE3C_X86  "_*"
#endif

#endif








#endif
