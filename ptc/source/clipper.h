////////////////////
// clipper object //
////////////////////

#ifndef __PTC_CLIPPER_H
#define __PTC_CLIPPER_H

#include "misc.h"
#include "rect.h"
#include "classes.h"






class Clipper
{
    // friend classes
    friend class Surface;

    public:
        
        // constructors
        Clipper();
        Clipper(RECTANGLE const &src,RECTANGLE const &dest);

        // clipping
        int SourceClip(int &x,int &y) const;
        int SourceClip(RECTANGLE &rect) const;
        int DestinationClip(int &x,int &y) const;
        int DestinationClip(RECTANGLE &rect) const;

        // set clipping rectangles
        void Set(RECTANGLE const &src,RECTANGLE const &dest);
        void SetSource(RECTANGLE const &rect);
        void SetDestination(RECTANGLE const &rect);

        // get clipping rectangles
        RECTANGLE GetSource();
        RECTANGLE GetDestination();

    private:

        // attach
        int Attach();
        int Detach();
        int AttachCount;

        // clipping rectangles
        RECTANGLE Source,Destination;
};










#endif
