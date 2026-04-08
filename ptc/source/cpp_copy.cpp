/////////////////////////////////////////
// copy conversion routines (ansi c++) //
/////////////////////////////////////////
#include "cpp_copy.h"






void ConvertCopy_CPP(void *src,void *dest,uint bytes,void *extra)
{
    // advoid warnings
    if (extra);

    // copy
    memcpy(dest,src,bytes);
}
