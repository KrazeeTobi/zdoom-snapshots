//////////////////////////////////////
// 32bit -> X conversion (ansi c++) //
//////////////////////////////////////
#include "cpp_32.h"

#if defined(__32BIT__)







// 32bit ARGB8888 -> 32bit X template converter
template <class T,
          int r_position,int r_size,
          int g_position,int g_size,
          int b_position,int b_size>
class Convert32_X
{
    public:
      
        // convert function
        inline void Convert(void *src,void *dest,uint pixels)
        {
            // fail on zero pixel count
            if (!pixels) return;
            
            // setup component source base
            const int r_base=16+8-r_size;
            const int g_base=8+8-g_size;
            const int b_base=0+8-b_size;

            // setup component mask base
            int r_maskbase=r_base;
            int g_maskbase=g_base;
            int b_maskbase=b_base;
            if (r_maskbase<16) r_maskbase=16;
            if (g_maskbase<8) g_maskbase=8;
            if (b_maskbase<0) b_maskbase=0;

            // setup component mask size
            const int r_masksize=16+8-r_maskbase;
            const int g_masksize=8+8-g_maskbase;
            const int b_masksize=0+8-b_maskbase;

            // setup component masks
            const uint r_mask=((1<<r_masksize)-1) << r_maskbase;
            const uint g_mask=((1<<g_masksize)-1) << g_maskbase;
            const uint b_mask=((1<<b_masksize)-1) << b_maskbase;
        
            // setup component shifts (+ = shiftleft, - = shiftright)
            const int r_shift=r_position-r_base;
            const int g_shift=g_position-g_base;
            const int b_shift=b_position-b_base;

            // setup data pointers
            uint *p=(uint*)src;
            T *q=(T*)dest;

            // main loop
            while (pixels--)
            {
                // read in color data
                uint color_r=*p;
                uint color_g=color_r;
                uint color_b=color_r;

                // mask components
                color_r&=r_mask;
                color_g&=g_mask;
                color_b&=b_mask;

                // shift red component into position
                if (r_shift>=0) color_r<<=r_shift;
                else color_r>>=(-r_shift);

                // shift green component into position
                if (g_shift>=0) color_g<<=g_shift;
                else color_g>>=(-g_shift);

                // shift blue component into position
                if (b_shift>=0) color_b<<=b_shift;
                else color_b>>=(-b_shift);

                // combine components
                uint color=color_r+color_g+color_b;

                // store
                *q=(T)color;

                // next pixel
                p++;
                q++;
            }
        }
};





void Convert32_ABGR8888_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<uint,0,8,8,8,16,8> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_RGBA8888_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<uint,24,8,16,8,8,8> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_BGRA8888_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<uint,8,8,16,8,24,8> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_RGB888_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // setup data pointers
    uint *p=(uint*)src;
    uchar *q=(uchar*)dest;

    // setup main loop count
    uint count=pixels>>2;
    if (!count) return;
    
    // setup tail count
    uint tail=pixels&3;

    // main loop
    while (count--)
    {
        // read color dwords
        uint color1 = *(p+0);
        uint color2 = *(p+1);
        uint color3 = *(p+2);
        uint color4 = *(p+3);

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
            uint color=*p;

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


void Convert32_BGR888_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // setup data pointers
    uint *p=(uint*)src;
    uchar *q=(uchar*)dest;

    // setup main loop count
    uint count=pixels>>2;
    if (!count) return;
    
    // setup tail count
    uint tail=pixels&3;

    // main loop
    while (count--)
    {
        // read color dwords
        uint color1 = *(p+0);
        uint color2 = *(p+1);
        uint color3 = *(p+2);
        uint color4 = *(p+3);

        // convert and pack into three dwords
        uint dword1 = ((color1&0x00FF0000)>>16) + (color1&0x0000FF00) + ((color1&0x000000FF)<<16) + ((color2&0x00FF0000)<<8);
        uint dword2 = ((color2&0x0000FF00)>>8) + ((color2&0x000000FF)<<8) + (color3&0x00FF0000) + ((color3&0x0000FF00)<<16);
        uint dword3 = (color3&0x000000FF) + ((color4&0x00FF0000)>>8) + ((color4&0x0000FF00)<<8) + ((color4&0x000000FF)<<24);

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
            uint color=*p;

            // unpack r,g,b
            uint r = (color&0x00FF0000) >> 16;
            uint g = (color&0x0000FF00) >> 8;
            uint b = (color&0x000000FF);

#ifdef __LITTLE_ENDIAN__

            // store (little endian)
            *(q+0) = r;
            *(q+1) = g;
            *(q+2) = b;

#elif __BIG_ENDIAN__

            // store (big endian)
            *(q+0) = b;
            *(q+1) = g;
            *(q+2) = r;

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


void Convert32_RGB565_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<ushort,11,5,5,6,0,5> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_BGR565_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<ushort,0,5,5,6,11,5> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_ARGB1555_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<ushort,10,5,5,5,0,5> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_ABGR1555_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<ushort,0,5,5,5,10,5> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}


void Convert32_GREY8_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // zero pixel count check
    if (!pixels) return;
                       
    // setup data pointers
    uint *p=(uint*)src;
    uchar *q=(uchar*)dest;

    // convert to greyscale
    while (pixels--)
    {
        // source color
        uint color=*p;

        // unpack r,g,b
        uint r = (color&0x00FF0000) >> 16;
        uint g = (color&0x0000FF00) >> 8;
        uint b = (color&0x000000FF);

        // combine into new color
        color = (r>>2) + (g>>1) + (b>>2);

        // store
        *q=(uchar)color;

        // next pixel
        p++;
        q++;
    }
}


void Convert32_RGB332_CPP(void *src,void *dest,uint pixels,void *extra)
{
    // instantiate template class
    Convert32_X<uchar,5,3,2,3,0,2> convert;
    
    // perform conversion
    convert.Convert(src,dest,pixels);

    // advoid warnings
    if (extra);
}









#endif
