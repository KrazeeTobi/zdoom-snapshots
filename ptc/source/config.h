////////////////////////////////////////
// prometheus truecolor configuration //
////////////////////////////////////////

#ifndef __PTC_CONFIG_H
#define __PTC_CONFIG_H

#include "lang.h"






#if defined(__AMIGA__)

    // processor
    #define __PPC__

    // interfaces
    #define __AMIGA__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__


#elif defined(__DOS32__)

    // processor
    #define __X86__

    // interfaces
    #define __VGA__
    #define __VESA__

    // modules
    #define __DPMI__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__
    #define __FAKEMODE__


#elif defined(__WIN32__)

    // processor
    #define __X86__

    // interfaces
    #define __GDI__
    #define __DIRECTX__

    // options
    #define __VFW__
    #define __SMC__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__


#elif defined(__LINUX__)

    // processor
    #define __X86__

    // gnu compiler
    #define __GNU__

    // interfaces
    #define __SVGALIB__
    #define __X11__
    #define __GGI__

    // options
    #define __MITSHM__
    #define __FAKEMODE__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__

#else

    // configure!
    #error you must configure the target platform!

#endif







#if defined(__X86__)

    // intel little endian
    #define __LITTLE_ENDIAN__
    #undef  __BIG_ENDIAN__    


#elif defined(__PPC__)

    // motorola big endian
    #define __BIG_ENDIAN__
    #undef  __LITTLE_ENDIAN__    

#else

    // configure!
    #error you must configure the processor endianness!

#endif






// Cygnus GNU-Win32 cannot handle DirectX yet
#ifdef __CYGNUS__
#undef __DIRECTX__
#endif







#endif
