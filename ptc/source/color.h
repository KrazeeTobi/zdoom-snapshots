//////////////////
// color object //
//////////////////

#ifndef __PTC_COLOR_H
#define __PTC_COLOR_H

#include "misc.h"
#include "globals.h"








class COLOR
{
    public:

        // constructors
        COLOR(float i,int mode=ABSOLUTE);
        COLOR(float c1,float c2,float c3,float c4=0,int mode=ABSOLUTE,int model=RGBA);

        // format type
        int type;
        
        // color model
        int model;

        // operation mode
        int mode;

        // operators
        inline int operator ==(COLOR const &other) const;
        inline int operator !=(COLOR const &other) const;

        // color components
        float c1;                   // |R|C|C|H|H|Y|Y| Y|
        float c2;                   // |G|M|M|S|L|I|U|Cb|
        float c3;                   // |G|Y|Y|I|S|Q|V|Cr|
        union 
        {
            float c4;               // |A|A|K|A|A|A|A| A|
            float index;            // color index
        };
};









inline int COLOR::operator ==(COLOR const &other) const
{
    // must have matching modes
    if (mode!=other.mode) return 0;

    // check for mismatch
    if (type==DIRECT && model==other.model)
    {
        // check direct color components
        if (c1!=other.c1 || c2!=other.c2 || c3!=other.c3 || c4!=other.c4) return 0;   
    }
    else if (type==INDEXED && index!=other.index) return 0;

    // equal
    return 1;
}


inline int COLOR::operator !=(COLOR const &other) const
{
    return !(*this==other);
}











// pack (r,g,b) into 32bit color integer
inline uint RGB32(uchar r,uchar g,uchar b)
{
    uint color=r<<16;
    color+=g<<8;
    color+=b;
    return color;
}

// pack (r,g,b,a) to 32bit color integer
inline uint RGBA32(uchar r,uchar g,uchar b,uchar a)
{
    uint color=a<<24;
    color+=r<<16;
    color+=g<<8;
    color+=b;
    return color;
}







// unpack 32bit color into r,g,b,a components
inline void UnpackRGB32(uint color,uchar &r,uchar &g,uchar &b)
{
    uchar *color_uchar=(uchar*)&color;
    r=color_uchar[2];
    g=color_uchar[1];
    b=color_uchar[0];
}

// unpack RGBA32 color integer into RGBA components
inline void UnpackRGBA32(uint color,uchar &r,uchar &g,uchar &b,uchar &a)
{
    uchar *color_uchar=(uchar*)&color;
    r=color_uchar[2];
    g=color_uchar[1];
    b=color_uchar[0];
    a=color_uchar[3];
}







// pack r,g,b into 16bit color (RGB565)
inline ushort RGB16(uchar r,uchar g,uchar b)
{
    ushort color;
    color  = (ushort) ( ( (ushort)r & (ushort)0xF8 ) << 8 );
    color += (ushort) ( ( (ushort)g & (ushort)0xFC ) << 3 );
    color += (ushort) ( ( (ushort)b )                >> 3 );
    return color;
}


// unpack 16bit color into r,g,b components
inline void UnpackRGB16(ushort color,uchar &r,uchar &g,uchar &b)
{
    r = (uchar) ( (color>>8) & (ushort)0xF8 );
    g = (uchar) ( (color>>3) & (ushort)0xFC );
    b = (uchar) ( (color<<3) & (ushort)0xF8 );
}








#endif
