//////////////////////////////////////////
// copy conversion routines (intel MMX) //
//////////////////////////////////////////

#ifndef __PTC_MMX_COPY_H
#define __PTC_MMX_COPY_H

#include "raster.h"







#ifdef __MMX__

// interface to external asm converters (do not call directly)
extern "C" void ConvertCopy_MMX(void *src,void *dest,uint bytes,void *extra);


// watcom c++ interface
#ifdef __WATCOMC__
#pragma warning 601 9
#pragma aux ConvertCopy_MMX "_*"
#endif

#endif








#endif
