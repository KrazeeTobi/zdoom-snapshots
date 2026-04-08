//////////////////////////////////////////
// copy conversion routines (intel x86) //
//////////////////////////////////////////

#ifndef __PTC_X86_COPY_H
#define __PTC_X86_COPY_H

#include "raster.h"







#ifdef __X86__

// interface to external asm converters (do not call directly)
extern "C" void ConvertCopy_X86(void *src,void *dest,uint bytes,void *extra);
extern "C" void AreaConvertCopy_X86(RASTER::CONVERTER::AREA *data,void *extra);

// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux ConvertCopy_X86 "_*"
#pragma aux AreaConvertCopy_X86 "_*"
#endif

#endif








#endif
