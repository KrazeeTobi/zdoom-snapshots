/////////////////////////////////////
// 8bit -> X conversion (ansi c++) //
/////////////////////////////////////
#include "cpp_8.h"

#if defined(__8BIT__)







void Convert8_4BYTE_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // check pixel count
    if (!pixels) return;

    // setup table
    uint *table=(uint*)extra;
    if (!table) return;

    // setup pointers
    uchar *p=(uchar*)src;
    uint *q=(uint*)dest;

    // main loop
    while (pixels--)
    {
        *q=table[*p];
        p++;
        q++;
    }
}


void Convert8_3BYTE_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // setup data pointers
    uchar *p=(uchar*)src;
    uchar *q=(uchar*)dest;

    // setup table
    uint *table=(uint*)extra;
    if (!table) return;

    // setup main loop count
    uint count=pixels>>2;
    if (!count) return;
    
    // setup tail count
    uint tail=pixels&3;

    // main loop
    while (count--)
    {
        // read color dwords
        uint color1 = table[*(p+0)];
        uint color2 = table[*(p+1)];
        uint color3 = table[*(p+2)];
        uint color4 = table[*(p+3)];

        // convert and pack into three dwords
        uint dword1 = color1 + (color2<<24);
        uint dword2 = ((color2>>8) & 0xFFFF) + (color3<<16);
        uint dword3 = ((color3>>16) & 0xFF)  + (color4<<8);

        // store
        uint *d=(uint*)q;
        *(d+0)=dword1;
        *(d+1)=dword2;
        *(d+2)=dword3;

        // next
        p+=4;
        q+=12;
    }

    // tail loop
    if (tail)
    {
        while (tail--)
        {
            // source color
            uint color=table[*p];

            // unpack r,g,b
            uint r = (color&0x00FF0000) >> 16;
            uint g = (color&0x0000FF00) >> 8;
            uint b = (color&0x000000FF);

#ifdef __LITTLE_ENDIAN__

            // store (little endian)
            *(q+0) = b;
            *(q+1) = g;
            *(q+2) = r;

#elif __BIG_ENDIAN__

            // store (big endian)
            *(q+0) = r;
            *(q+1) = g;
            *(q+2) = b;

#else

            // no endian information is configured!
            #error processor endianness is not configured!

#endif

            // next pixel
            p++;
            q+=3;
        }
    }
}


void Convert8_2BYTE_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // check pixel count
    if (!pixels) return;

    // setup table
    uint *table=(uint*)extra;
    if (!table) return;

    // setup pointers
    uchar *p=(uchar*)src;
    ushort *q=(ushort*)dest;

    // main loop
    while (pixels--)
    {
        *q=(ushort)table[*p];
        p++;
        q++;
    }
}


void Convert8_1BYTE_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // check pixel count
    if (!pixels) return;

    // setup table
    uchar *table=(uchar*)extra;
    if (!table) return;

    // setup pointers
    uchar *p=(uchar*)src;
    uchar *q=(uchar*)dest;

    // main loop
    while (pixels--)
    {
        *q=table[*p];
        p++;
        q++;
    }
}


#endif
