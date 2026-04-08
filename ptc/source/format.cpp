////////////////////////
// pixel format class //
////////////////////////
#include "format.h"








FORMAT::FORMAT()
{
    // defaults
    clear();
}


FORMAT::FORMAT(int format_id)
{
    // defaults
    clear();
    
    // initialize
    init(format_id);
}


FORMAT::FORMAT(char format_id[])
{
    // defaults
    clear();

    // initialize
    init(format_id);
}


FORMAT::FORMAT(uint c1_mask,uint c2_mask,uint c3_mask,uint c4_mask,int imodel,int ibits)
{
    // defaults
    clear();
    
    // initialize
    init(c1_mask,c2_mask,c3_mask,c4_mask,imodel,ibits);
}


FORMAT::FORMAT(uint c1_position,uint c1_size,
               uint c2_position,uint c2_size,
               uint c3_position,uint c3_size,
               uint c4_position,uint c4_size,int imodel,int ibits)
{
    // defaults
    clear();
    
    // initialize
    init(c1_position,c1_size,c2_position,c2_size,c3_position,c3_size,c4_position,c4_size,imodel,ibits);
}






int FORMAT::init(int format_id)
{
    // init from id
    switch (format_id)
    {
        case ARGB8888: id=ARGB8888;
                       type=DIRECT;
                       model=RGBA;
                       bits=32;
                       bytes=4;     
                       c1.position=16;
                       c2.position=8;
                       c3.position=0;
                       c4.position=24;
                       c1.size=8;
                       c2.size=8;
                       c3.size=8;           
                       c4.size=8;
                       setup();
                       return 1;

        case ABGR8888: id=ABGR8888;
                       type=DIRECT;
                       model=RGBA;
                       bits=32;
                       bytes=4;     
                       c1.position=0;
                       c2.position=8;
                       c3.position=16;
                       c4.position=24;
                       c1.size=8;
                       c2.size=8;
                       c3.size=8;           
                       c4.size=8;
                       setup();
                       return 1;

        case BGRA8888: id=BGRA8888;
                       type=DIRECT;
                       model=RGBA;
                       bits=32;
                       bytes=4;     
                       c1.position=8;
                       c2.position=16;
                       c3.position=24;
                       c4.position=0;
                       c1.size=8;
                       c2.size=8;
                       c3.size=8;           
                       c4.size=8;
                       setup();
                       return 1;

        case RGBA8888: id=RGBA8888;
                       type=DIRECT;
                       model=RGBA;
                       bits=32;
                       bytes=4;     
                       c1.position=24;
                       c2.position=16;
                       c3.position=8;
                       c4.position=0;
                       c1.size=8;
                       c2.size=8;
                       c3.size=8;           
                       c4.size=8;
                       setup();
                       return 1;

        case RGB888: id=RGB888;
                     type=DIRECT;
                     model=RGBA;
                     bits=24;
                     bytes=3;     
                     c1.position=16;
                     c2.position=8;
                     c3.position=0;
                     c4.position=0;
                     c1.size=8;
                     c2.size=8;
                     c3.size=8;           
                     c4.size=0;
                     setup();
                     return 1;

        case BGR888: id=BGR888;
                     type=DIRECT;
                     model=RGBA;
                     bits=24;
                     bytes=3;     
                     c1.position=0;
                     c2.position=8;
                     c3.position=16;
                     c4.position=0;
                     c1.size=8;
                     c2.size=8;
                     c3.size=8;           
                     c4.size=0;
                     setup();
                     return 1;

        case RGB565: id=RGB565;
                     type=DIRECT;
                     model=RGBA;
                     bits=16;
                     bytes=2;     
                     c1.position=11;
                     c2.position=5;
                     c3.position=0;
                     c4.position=0;
                     c1.size=5;
                     c2.size=6;
                     c3.size=5;           
                     c4.size=0;
                     setup();
                     return 1;

        case BGR565: id=BGR565;
                     type=DIRECT;
                     model=RGBA;
                     bits=16;
                     bytes=2;     
                     c1.position=0;
                     c2.position=5;
                     c3.position=11;
                     c4.position=0;
                     c1.size=5;
                     c2.size=6;
                     c3.size=5;           
                     c4.size=0;
                     setup();
                     return 1;

        case ARGB1555: id=ARGB1555;
                       type=DIRECT;
                       model=RGBA;
                       bits=16;
                       bytes=2;     
                       c1.position=10;
                       c2.position=5;
                       c3.position=0;
                       c4.position=15;
                       c1.size=5;
                       c2.size=5;
                       c3.size=5;           
                       c4.size=1;
                       setup();
                       return 1;

        case ABGR1555: id=ABGR1555;
                       type=DIRECT;
                       model=RGBA;
                       bits=16;
                       bytes=2;     
                       c1.position=0;
                       c2.position=5;
                       c3.position=10;
                       c4.position=15;
                       c1.size=5;
                       c2.size=5;
                       c3.size=5;           
                       c4.size=1;
                       setup();
                       return 1;

        case INDEX8: id=INDEX8;
                     type=INDEXED;
                     model=0;
                     bits=8;
                     bytes=1;     
                     c1.position=0;
                     c2.position=0;
                     c3.position=0;
                     c4.position=0;
                     c1.size=0;
                     c2.size=0;
                     c3.size=0;           
                     c4.size=0;
                     setup();
                     return 1;

        case FAKEMODE1A: id=FAKEMODE1A;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=18;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;
                        
        case FAKEMODE1B: id=FAKEMODE1B;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=18;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;
                        
        case FAKEMODE1C: id=FAKEMODE1C;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=18;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;
        
        case FAKEMODE2A: id=FAKEMODE2A;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=14;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case FAKEMODE2B: id=FAKEMODE2B;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=14;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case FAKEMODE2C: id=FAKEMODE2C;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=14;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case FAKEMODE3A: id=FAKEMODE3A;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=12;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case FAKEMODE3B: id=FAKEMODE3B;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=12;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case FAKEMODE3C: id=FAKEMODE3C;
                         type=FAKEMODE;
                         model=RGBA;
                         bits=12;
                         bytes=0;     
                         c1.position=0;
                         c2.position=0;
                         c3.position=0;
                         c4.position=0;
                         c1.size=0;
                         c2.size=0;
                         c3.size=0;           
                         c4.size=0;
                         setup();
                         return 1;

        case GREY8: id=GREY8;
                    type=INDEXED;
                    model=GREYSCALE;
                    bits=8;
                    bytes=1;     
                    c1.position=0;
                    c2.position=0;
                    c3.position=0;
                    c4.position=0;
                    c1.size=0;
                    c2.size=0;
                    c3.size=0;           
                    c4.size=0;
                    setup();
                    return 1;

        case RGB332: id=RGB332;
                     type=DIRECT;
                     model=RGBA;
                     bits=8;
                     bytes=1;     
                     c1.position=5;
                     c2.position=2;
                     c3.position=0;
                     c4.position=0;
                     c1.size=3;
                     c2.size=3;
                     c3.size=2;           
                     c4.size=0;
                     setup();
                     return 1;
    }

    // failure
    return 0;
}


int FORMAT::init(char format_id[])
{
    // initialize from format id string
         if (stricmp(format_id,"ARGB8888")==0)   return init(ARGB8888);
    else if (stricmp(format_id,"ABGR8888")==0)   return init(ABGR8888);
    else if (stricmp(format_id,"BGRA8888")==0)   return init(BGRA8888);
    else if (stricmp(format_id,"RGBA8888")==0)   return init(RGBA8888);
    else if (stricmp(format_id,"RGB888")==0)     return init(RGB888);
    else if (stricmp(format_id,"BGR888")==0)     return init(BGR888);
    else if (stricmp(format_id,"RGB565")==0)     return init(RGB565);
    else if (stricmp(format_id,"BGR565")==0)     return init(BGR565);
    else if (stricmp(format_id,"RGB555")==0)     return init(RGB555);
    else if (stricmp(format_id,"BGR555")==0)     return init(BGR555);
    else if (stricmp(format_id,"ARGB1555")==0)   return init(ARGB1555);
    else if (stricmp(format_id,"ABGR1555")==0)   return init(ABGR1555);
    else if (stricmp(format_id,"RGB332")==0)     return init(RGB332);
    else if (stricmp(format_id,"INDEX8")==0)     return init(INDEX8);
    else if (stricmp(format_id,"GREY8")==0)      return init(GREY8);
    else if (stricmp(format_id,"GRAY8")==0)      return init(GRAY8);
    else if (stricmp(format_id,"FAKEMODE1A")==0) return init(FAKEMODE1A);
    else if (stricmp(format_id,"FAKEMODE1B")==0) return init(FAKEMODE1B);
    else if (stricmp(format_id,"FAKEMODE1C")==0) return init(FAKEMODE1C);
    else if (stricmp(format_id,"FAKEMODE2A")==0) return init(FAKEMODE2A);
    else if (stricmp(format_id,"FAKEMODE2B")==0) return init(FAKEMODE2B);
    else if (stricmp(format_id,"FAKEMODE2C")==0) return init(FAKEMODE2C);
    else if (stricmp(format_id,"FAKEMODE3A")==0) return init(FAKEMODE3A);
    else if (stricmp(format_id,"FAKEMODE3B")==0) return init(FAKEMODE3B);
    else if (stricmp(format_id,"FAKEMODE3C")==0) return init(FAKEMODE3C);
    else if (stricmp(format_id,"FAKEMODE1")==0)  return init(FAKEMODE1);
    else if (stricmp(format_id,"FAKEMODE2")==0)  return init(FAKEMODE2);
    else if (stricmp(format_id,"FAKEMODE3")==0)  return init(FAKEMODE3);
    else return 0;
}


int FORMAT::init(uint c1_mask,uint c2_mask,uint c3_mask,uint c4_mask,int imodel,int ibits)
{
    // calculate component information from masks
    FIELDINFO field1=getfieldinfo(c1_mask);
    FIELDINFO field2=getfieldinfo(c2_mask);
    FIELDINFO field3=getfieldinfo(c3_mask);
    FIELDINFO field4=getfieldinfo(c4_mask);
   
    // initialize with component info
    return init(field1.position,field1.size,
                field2.position,field2.size,
                field3.position,field3.size,
                field4.position,field4.size,imodel,ibits);
}


int FORMAT::init(uint c1_position,uint c1_size,
                 uint c2_position,uint c2_size,
                 uint c3_position,uint c3_size,
                 uint c4_position,uint c4_size,int imodel,int ibits)
{
    // check color model
    if (imodel==RGBA)
    {
        // setup format struct
        if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==8 &&
            c1_position==16 && c2_position==8 && c3_position==0 && c4_position==24)
        {
            // ARGB8888 (32bit)
            return init(ARGB8888);
        }
        else if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==8 &&
                 c1_position==0 && c2_position==8 && c3_position==16 && c4_position==24)
        {
            // ABGR8888 (32bit)
            return init(ABGR8888);
        }
        else if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==8 &&
                 c1_position==24 && c2_position==16 && c3_position==8 && c4_position==0)
        {
            // RGBA8888 (32bit)
            return init(RGBA8888);
        }
        else if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==8 &&
                 c1_position==8 && c2_position==16 && c3_position==24 && c4_position==0)
        {
            // BGRA8888 (32bit)
            return init(BGRA8888);
        }
        else if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==0 &&
                 c1_position==16 && c2_position==8 && c3_position==0 && c4_position==0)
        {
            // RGB888 (24bit)
            return init(RGB888);
        }
        else if (c1_size==8 && c2_size==8 && c3_size==8 && c4_size==0 &&
                 c1_position==0 && c2_position==8 && c3_position==16 && c4_position==0)
        {
            // BGR888 (24bit)
            return init(BGR888);
        }
        else if (c1_size==5 && c2_size==6 && c3_size==5 && c4_size==0 && 
                 c1_position==11 && c2_position==5 && c3_position==0 && c4_position==0)
        {
            // RGB565 (16bit)
            return init(RGB565);
        }
        else if (c1_size==5 && c2_size==6 && c3_size==5 && c4_size==0 && 
                 c1_position==0 && c2_position==5 && c3_position==11 && c4_position==0)
        {
            // BGR565 (16bit)
            return init(BGR565);
        }   
        else if (c1_size==5 && c2_size==5 && c3_size==5 && c4_size==1 &&
                 c1_position==10 && c2_position==5 && c3_position==0 && c4_position==15)
        {
            // ARGB1555 ("15bit")
            return init(ARGB1555);
        }
        else if (c1_size==5 && c2_size==5 && c3_size==5 && c4_size==1 &&
                 c1_position==0 && c2_position==5 && c3_position==10 && c4_position==15)
        {
            // ABGR1555 ("15bit")
            return init(ABGR1555);
        }
        else if (c1_size==3 && c2_size==3 && c3_size==2 && c4_size==0 &&
                 c1_position==5 && c2_position==2 && c3_position==0 && c4_position==0)
        {
            // RGB332 (8bit)
            return init(RGB332);
        }
    }

    // custom format
    id=0;
    type=DIRECT;
    model=imodel;
    c1.position=c1_position;
    c2.position=c2_position;
    c3.position=c3_position;
    c4.position=c4_position;
    c1.size=c1_size;
    c2.size=c2_size;
    c3.size=c3_size;
    c4.size=c4_size;
    if (ibits==DEFAULT) bits=c1.size+c2.size+c3.size+c4.size; 
    else bits=ibits;
    bytes=(int)ceil(bits/8.0);
    bits=bytes*8;
    setup();
    return 1;
}






void FORMAT::clear()
{
    id=0;
    type=0;
    model=0;
    bits=0;
    bytes=0;
    c1.position=0;
    c1.size=0;
    c1.mask=0;
    c1.shift=0;
    c2.position=0;
    c2.size=0;
    c2.mask=0;
    c2.shift=0;
    c3.position=0;
    c3.size=0;           
    c3.mask=0;
    c3.shift=0;           
    c4.position=0;             
    c4.size=0;
    c4.mask=0;             
    c4.shift=0;
}







int FORMAT::pack(COLOR const &color,void *data) const
{
    // advoid warnings...
    if (color.mode || data) return 1;
    return 0;
}

    
int FORMAT::pack(uchar ic1,uchar ic2,uchar ic3,uchar ic4,void *data,int imodel) const
{
    if (model==imodel)
    {
        // identical color models

        // setup color components
        uint color_c1=(uint)ic1;
        uint color_c2=(uint)ic2;
        uint color_c3=(uint)ic3;
        uint color_c4=(uint)ic4;

        // shift component1 into position and mask
        if (c1.shift>=0) color_c1<<=c1.shift;
        else color_c1>>=-c1.shift;
        color_c1&=c1.mask;
                           
        // shift component2 into position and mask
        if (c2.shift>=0) color_c2<<=c2.shift;
        else color_c2>>=-c2.shift;
        color_c2&=c2.mask;

        // shift component3 into position and mask
        if (c3.shift>=0) color_c3<<=c3.shift;
        else color_c3>>=-c3.shift;
        color_c3&=c3.mask;

        // shift component4 into position and mask
        if (c4.shift>=0) color_c4<<=c4.shift;
        else color_c4>>=-c4.shift;
        color_c4&=c4.mask;

        // combine components
        uint color=color_c1+color_c2+color_c3+color_c4;
    
        // copy to data
        uchar *p=(uchar*)&color;//+(4-bytes);      // note: need endian fix here
        uchar *q=(uchar*)data;
        uchar *pmax=p+bytes;
        while (p<pmax)
        {
            *q=*p;
            p++; 
            q++;
        }

        // TODO: handle 32bit+ per pixel cases + endian fix
        return 1;
    }
    else return 0;
}


int FORMAT::unpack(void *data,COLOR &color,int mode,int imodel) const
{
    // advoid warnings...
    if (data || color.mode || mode || imodel) return 1;
    return 0;
}


int FORMAT::unpack(void *data,uchar &ic1,uchar &ic2,uchar &ic3,uchar &ic4,int imodel)
{
    // temp: handle model
    if (imodel!=model) return 0;

    // copy data to color
    uint color=0;
    uchar *p=(uchar*)data;
    uchar *q=(uchar*)&color;      // note: need endian fix here
    uchar *pmax=p+bytes;
    while (p<pmax)
    {
        *q=*p;
        p++; 
        q++;
    }

    // setup components
    uint color_c1=color & c1.mask;
    uint color_c2=color & c2.mask;
    uint color_c3=color & c3.mask;
    uint color_c4=color & c4.mask;

    // shift component 1 into position
    if (c1.shift<0) color_c1<<=-c1.shift;
    else color_c1>>=c1.shift;

    // shift component 2 into position
    if (c2.shift<0) color_c2<<=-c2.shift;
    else color_c2>>=c2.shift;

    // shift component 3 into position
    if (c3.shift<0) color_c3<<=-c3.shift;
    else color_c3>>=c3.shift;

    // shift component 4 into position
    if (c4.shift<0) color_c4<<=-c4.shift;
    else color_c4>>=c4.shift;

    // extend components if needed
    if (c1.size<8) color_c1 = (color_c1 * ((1<<c1.size)-1) / 255) << (8-c1.size);
    if (c2.size<8) color_c2 = (color_c2 * ((1<<c2.size)-1) / 255) << (8-c2.size);
    if (c3.size<8) color_c3 = (color_c3 * ((1<<c3.size)-1) / 255) << (8-c3.size);
    if (c4.size<8) color_c4 = (color_c4 * ((1<<c4.size)-1) / 255) << (8-c4.size);

    // assign
    ic1=(uchar)color_c1;
    ic2=(uchar)color_c2;
    ic3=(uchar)color_c3;
    ic4=(uchar)color_c4;
    return 1;
}






int FORMAT::ok() const
{
    if (id>=FORMATBASE && id<=FORMATMAX) return 1;
    else
    {
        // no format id - check data
        if (type==DIRECT)
        {
            // check color model
            if (model!=RGBA && model!=CMYA && model!=CMYK && model!=HSIA && model!=HLSA &&
                model!=YIQA && model!=YUVA && model!=YCbCrA)
            {
                //cout << "bad color model" << endl;
                return 0;
            }

            // check truecolor format
            if (c1.size+c2.size+c3.size+c4.size==0 || bits==0 || bytes==0) 
            {
                //cout << "bad size, bits, bytes" << endl;
                return 0;
            }
            else return 1;
        }
        else if (type==INDEXED)
        {
            // check indexed format
            if (bits==0 || bytes==0) return 0;
            else return 1;
        }
        else if (type==FAKEMODE)
        {
            // check fakemode
            if (!bits) return 0;      // should i?
            return 1;
        }
        else
        {
            //cout << "huh?" << endl;
            return 0;
        }
    }
}







void FORMAT::operator =(const int format_id)
{
    // initialize with format id
    init(format_id);
}


void FORMAT::operator =(FORMAT const &other)
{
    // fail if identical assign
    if (&other==this) return;

    // assign
    id=other.id;
    type=other.type;
    bits=other.bits;
    bytes=other.bytes;
    model=other.model;
    c1=other.c1;
    c2=other.c2;
    c3=other.c3;
    c4=other.c4;
}


int FORMAT::operator ==(FORMAT const &other) const
{
    // check for id match
    if (id==other.id && id>=FORMATBASE && id<=FORMATMAX) return 1;

    // check data for match
    if (type!=other.type || model!=other.model || bits!=other.bits || bytes!=other.bytes) return 0;

    // check bits per pixel
    if (bits!=other.bits) return 0;

    // check components
    if (c1.position!=other.c1.position || c1.size!=other.c1.size || c1.mask!=other.c1.mask || c1.shift!=other.c1.shift ||
        c2.position!=other.c2.position || c2.size!=other.c2.size || c2.mask!=other.c2.mask || c2.shift!=other.c2.shift ||
        c3.position!=other.c3.position || c3.size!=other.c3.size || c3.mask!=other.c3.mask || c3.shift!=other.c3.shift ||
        c4.position!=other.c4.position || c4.size!=other.c4.size || c4.mask!=other.c4.mask || c4.shift!=other.c4.shift) return 0;
    else return 1;
}

        
int FORMAT::operator !=(FORMAT const &other) const
{
    return !(*this==other);
}







void FORMAT::setup()
{
    // component 1
    if (c1.size>0 && c1.size<=8)
    {
        // setup info
        uint c1_base=8-c1.size;
        c1.shift=c1.position-c1_base;
        c1.mask=((1<<c1.size)-1) << c1.position;
    }
    else
    {
        // clear info
        c1.mask=0;
        c1.shift=0;
    }

    // component two
    if (c2.size>0 && c2.size<=8)
    {
        // setup info
        uint c2_base=8-c2.size;
        c2.shift=c2.position-c2_base;
        c2.mask=((1<<c2.size)-1) << c2.position;
    }
    else
    {
        // clear info
        c2.mask=0;
        c2.shift=0;
    }
    
    // component three
    if (c3.size>0 && c3.size<=8)
    {
        // setup info
        uint c3_base=8-c3.size;
        c3.shift=c3.position-c3_base;
        c3.mask=((1<<c3.size)-1) << c3.position;
    }
    else
    {
        // clear info
        c3.mask=0;
        c3.shift=0;
    }

    // component four
    if (c4.size>0 && c4.size<=8)
    {
        // setup info
        uint c4_base=8-c4.size;
        c4.shift=c4.position-c4_base;
        c4.mask=((1<<c4.size)-1) << c4.position;
    }
    else
    {
        // clear info
        c4.mask=0;
        c4.shift=0;
    }
}








FORMAT::FIELDINFO FORMAT::getfieldinfo(uint mask)
{
    // field info 
    FIELDINFO info;

    // setup mask
    info.mask=mask;
    info.shift=0;

    // calculate bit position of field
    int position=0;
    while (position<32)
    {
        int position_mask=1<<position;
        if (mask & position_mask)
        {
            // found base of component - find size
            int size=1;
            while (position+size<32)
            {
                position_mask=1<<(position+size);
                if (!(mask & position_mask)) break;
                size++;
            }

            // end of component - return fieldinfo
            info.size=size;
            info.position=position;
            return info;
        }
        
        // next position
        position++;
    } 

    // failure
    info.size=0;
    info.position=0;
    return info;
}







