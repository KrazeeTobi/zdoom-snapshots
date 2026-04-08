/////////////////////////////////////
// 8bit -> X conversion (ansi c++) //
/////////////////////////////////////

#ifndef __CPP_8BIT_H
#define __CPP_8BIT_H

#include "raster.h"








#if defined(__8BIT__)

// ansi c++ converters (may be called directly)
void Convert8_4BYTE_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert8_3BYTE_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert8_2BYTE_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert8_1BYTE_CPP(void *src,void *dest,uint pixels,void *extra);

#endif









#endif
