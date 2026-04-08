/////////////////////
// rectangle class //
/////////////////////

#ifndef __PTC_RECTANGLE_H
#define __PTC_RECTANGLE_H

#include "misc.h"
#include "globals.h"
#include <math.h>
#include <stdlib.h>




                              


class RECTANGLE
{
    public:

        // construct
        inline RECTANGLE();
        inline RECTANGLE(int x1,int y1,int x2,int y2);
        
        // fix rectangle
        inline void fix();

        // cool stuff
        inline int width() const;
        inline int height() const;
        inline int area() const;

        // operators
        inline int operator ==(RECTANGLE const &other) const;
        inline int operator !=(RECTANGLE const &other) const;

        // data
        int x1,y1,x2,y2;
};








inline RECTANGLE::RECTANGLE()
{
    // defaults
    x1=0;
    y1=0;
    x2=0;
    y2=0;
}


inline RECTANGLE::RECTANGLE(int ix1,int iy1,int ix2,int iy2)
{
    // init
    x1=ix1;
    y1=iy1;
    x2=ix2;
    y2=iy2;
}







inline void RECTANGLE::fix()
{
    // fix x coords so that x1 is leftmost
    int temp;
    if (x1>x2)
    {
        temp=x1;
        x1=x2;
        x2=temp;
    }

    // fix y coords so that y1 is topmost
    if (y1>y2)
    {
        temp=y1;
        y1=y2;
        y2=temp;
    }
}







inline int RECTANGLE::width() const
{
    // rect width
    return x2-x1;
}


inline int RECTANGLE::height() const
{
    // rect height
    return y2-y1;
}


inline int RECTANGLE::area() const
{
    // rect area
    return abs((x2-x1)*(y2-y1));
}







inline int RECTANGLE::operator ==(RECTANGLE const &other) const
{
    if (x1!=other.x1 || y1!=other.y1 || x2!=other.x2 || y2!=other.y2) return 0;
    else return 1;
}


inline int RECTANGLE::operator !=(RECTANGLE const &other) const
{
    return !(*this==other);
}










#endif
