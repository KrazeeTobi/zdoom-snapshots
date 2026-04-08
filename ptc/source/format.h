////////////////////////
// pixel format class //
////////////////////////

#ifndef __PTC_FORMAT_H
#define __PTC_FORMAT_H

#include "misc.h"
#include "lang.h"
#include "color.h"
#include "globals.h"
#include <math.h>
#include <iostream.h>




  


class FORMAT
{
    public:

        // constructors
        FORMAT();
        FORMAT(int id);
        FORMAT(char id[]);
        FORMAT(uint c1_mask,uint c2_mask,uint c3_mask,uint c4_mask=0,
               int model=RGBA,int bits=DEFAULT); 
        FORMAT(uint c1_position,uint c1_size,
               uint c2_position,uint c2_size,
               uint c3_position,uint c3_size,
               uint c4_position=0,uint c4_size=0,
               int model=RGBA,int bits=DEFAULT);

        // initialize
        int init(int id);
        int init(char id[]);
        int init(uint c1_mask,uint c2_mask,uint c3_mask,uint c4_mask=0,
                 int model=RGBA,int bits=DEFAULT); 
        int init(uint c1_position,uint c1_size,
                 uint c2_position,uint c2_size,
                 uint c3_position,uint c3_size,
                 uint c4_position=0,uint c4_size=0,
                 int model=RGBA,int bits=DEFAULT);

        // clear format
        void clear();

        // color packing/unpacking
        int pack(COLOR const &color,void *data) const;
        int pack(uchar c1,uchar c2,uchar c3,uchar c4,void *data,int model=RGBA) const;
        int unpack(void *data,COLOR &color,int mode=ABSOLUTE,int model=RGBA) const;
        int unpack(void *data,uchar &c1,uchar &c2,uchar &c3,uchar &c4,int model=RGBA);

        // operators
        void operator =(int const id);
        void operator =(FORMAT const &other);
        int operator ==(FORMAT const &other) const;
        int operator !=(FORMAT const &other) const;

        // format status
        int ok() const;

        // data
        int id;                         // format id
        int type;                       // format type
        int model;                      // color model
        int bits;                       // bits per pixel
        int bytes;                      // bytes per pixel

        // field information struct
        struct FIELDINFO
        {
            uint position;              // bit position of LSB
            uint size;                  // size in bits
            uint mask;                  // field mask
            int shift;                  // shift magnitude to position (for uchar pack only - may be negative!)
        };
        
        // component information
        FIELDINFO c1;                   // |R|C|C|H|H|Y|Y| Y|
        FIELDINFO c2;                   // |G|M|M|S|L|I|U|Cb|
        FIELDINFO c3;                   // |B|Y|Y|I|S|Q|V|Cr|
        FIELDINFO c4;                   // |A|A|K|A|A|A|A| A|

    private:

        // setup masks and other data
        void setup();

        // get field information from mask
        FIELDINFO getfieldinfo(uint mask);
};










#endif
