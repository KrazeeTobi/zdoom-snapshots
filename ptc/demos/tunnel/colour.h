////////////////////////////////////////////
// Colour Ramp class                      //
// Copyright (c) 1998 by BLACKAXE / KoLOr //
////////////////////////////////////////////

#ifndef __COLOUR_H
#define __COLOUR_H

#include "ptc.h"
#include <math.h>
#include <stdlib.h>






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ColourRamp class
// builds a table which contains shade values for a 8bit texture
// as paremeters we pass the palette of the surface we want to built a shadetable of, and then it builds 256 shades (128 to
// white and 128 to black) which can be index using the GetColour(color, shade) function

class ColourRamp 
{
    public:
        
        // constructor
        inline ColourRamp(Palette &palette);
        
        // lookup shaded colour
        inline ushort GetColour(int index,int shade);

    protected:

        // data
        ushort *Table;
};





inline ColourRamp::ColourRamp(Palette &palette)
{
    // allocate color table
    Table = new ushort[256*256];

    // lock palette
    uint *data=(uint*)palette.Lock();
    if (!data) return;

    // initialize color table
    for(int i=0; i<256; i++) 
    {
        // unpack palette color integer
        int r = (data[i]>>16) & 0xFF;
        int g = (data[i]>>8)  & 0xFF;
        int b = (data[i]>>0)  & 0xFF;

        // original color shade
        Table[i*256+128] = RGB16(r,g,b);

        // calculate shades to white
        float sr=(float)r;
        float sg=(float)g;
        float sb=(float)b;
        float dr = (float)(256 - r) / 128.0f;
        float dg = (float)(256 - g) / 128.0f;
        float db = (float)(256 - b) / 128.0f;
        for(int j=0; j<128; j++)
        {
            sr += dr; 
            sg += dg;
            sb += db;
            Table[i*256+j+129] = RGB16((int)sr,(int)sg,(int)sb);
        }

        // calculate shades to black
        sr = 0.0f;
        sg = 0.0f;
        sb = 0.0f;
        dr = (float)(r) / 128.0f;
        dg = (float)(g) / 128.0f;
        db = (float)(b) / 128.0f;
        for(int k=0; k<128; k++) 
        {
            Table[i*256+k] = RGB16((int)sr,(int)sg,(int)sb);
            sr += dr; 
            sg += dg;
            sb += db;
        }
    }

    // unlock palette
    palette.Unlock();
}


inline ushort ColourRamp::GetColour(int index,int shade)
{
    // lookup shaded colour
    return Table[(index<<8)+shade];
}







#endif
