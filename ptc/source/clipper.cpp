////////////////////
// clipper object //
////////////////////
#include "clipper.h"








Clipper::Clipper()
{
    // default
    AttachCount=0;
}


Clipper::Clipper(RECTANGLE const &src,RECTANGLE const &dest)
{
    // setup
    Set(src,dest);
    AttachCount=0;
}





int Clipper::SourceClip(int &x,int &y) const
{
    // return 0 if (x,y) is outside clipping rect
    if (x<Source.x1 || x>Source.x2 || y<Source.y1 || y>Source.y2) return 0;
    else return 1;
}
 

int Clipper::SourceClip(RECTANGLE &rect) const
{
    // if total rejection return 0
    if (rect.x2<Source.x1 || rect.x1>Source.x2 || rect.y2<Source.y1 || rect.y1>Source.y2) return 0;

    // clip rectangle (x1,y1,x2,y2) to clipping rect
    if (rect.x1<Source.x1) rect.x1=Source.x1;
    if (rect.y1<Source.y1) rect.y1=Source.y1;
    if (rect.x2>Source.x2) rect.x2=Source.x2;
    if (rect.y2>Source.y2) rect.y2=Source.y2;
    return 1;
}


int Clipper::DestinationClip(int &x,int &y) const
{
    // return 0 if (x,y) is outside clipping rect
    if (x<Destination.x1 || x>Destination.x2 || y<Destination.y1 || y>Destination.y2) return 0;
    else return 1;
}


int Clipper::DestinationClip(RECTANGLE &rect) const
{
    // if total rejection return 0
    if (rect.x2<Destination.x1 || rect.x1>Destination.x2 || rect.y2<Destination.y1 || rect.y1>Destination.y2) return 0;

    // clip rectangle (x1,y1,x2,y2) to clipping rect
    if (rect.x1<Destination.x1) rect.x1=Destination.x1;
    if (rect.y1<Destination.y1) rect.y1=Destination.y1;
    if (rect.x2>Destination.x2) rect.x2=Destination.x2;
    if (rect.y2>Destination.y2) rect.y2=Destination.y2;
    return 1;
}






void Clipper::Set(RECTANGLE const &src,RECTANGLE const &dest)
{
    // set clipping rectangles
    Source=src;
    Source.fix();
    Destination=dest;
    Destination.fix();
}


void Clipper::SetSource(RECTANGLE const &rect)
{
    // set source rectangle
    Source=rect;
    Source.fix();
}


void Clipper::SetDestination(RECTANGLE const &rect)
{
    // set destination rectangle
    Destination=rect;
    Destination.fix();
}






RECTANGLE Clipper::GetSource()
{
    // return source rectangle
    return Source;
}


RECTANGLE Clipper::GetDestination()
{
    // return destination rectangle
    return Destination;
}


        
 

       
int Clipper::Attach()
{
    // increment attach count
    return ++AttachCount;
}


int Clipper::Detach()
{
    // decrement attach count
    if (AttachCount) AttachCount--;
    return AttachCount;
}
