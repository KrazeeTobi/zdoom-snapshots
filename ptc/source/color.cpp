/////////////////
// color class //
/////////////////
#include "color.h"








COLOR::COLOR(float i,int imode)
{
    index=i;
    mode=imode;
    type=INDEXED;
    model=UNKNOWN;
}


COLOR::COLOR(float ic1,float ic2,float ic3,float ic4,int imode,int imodel)
{
    type=DIRECT;
    mode=imode;
    model=imodel;
    c1=ic1;
    c2=ic2;
    c3=ic3;
    c4=ic4;
}
