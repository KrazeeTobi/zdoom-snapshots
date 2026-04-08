//////////////////////////////////////
// 16bit -> X conversion (ansi c++) //
//////////////////////////////////////

#ifndef __PTC_CPP_16BIT_H
#define __PTC_CPP_16BIT_H

#include "raster.h"








#if defined(__16BIT__)

// ansi c++ converters (may be called directly)
void Convert16_ARGB8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_ABGR8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_RGBA8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_BGRA8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_RGB888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_BGR888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_BGR565_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_ARGB1555_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_ABGR1555_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_GREY8_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert16_RGB332_CPP(void *src,void *dest,uint pixels,void *extra);

#endif








#endif
