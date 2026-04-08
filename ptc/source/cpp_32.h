//////////////////////////////////////
// 32bit -> X conversion (ansi c++) //
//////////////////////////////////////

#ifndef __PTC_CPP_32BIT_H
#define __PTC_CPP_32BIT_H

#include "raster.h"








#if defined(__32BIT__)

// ansi c++ converters (may be called directly)
void Convert32_ABGR8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_RGBA8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_BGRA8888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_RGB888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_BGR888_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_RGB565_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_BGR565_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_ARGB1555_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_ABGR1555_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_GREY8_CPP(void *src,void *dest,uint pixels,void *extra);
void Convert32_RGB332_CPP(void *src,void *dest,uint pixels,void *extra);

#endif








#endif
