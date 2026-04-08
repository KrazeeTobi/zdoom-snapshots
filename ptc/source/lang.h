////////////////////////////
// setup compiler defines //
////////////////////////////

#ifndef __PTC_LANG_H
#define __PTC_LANG_H






// COMPILERS

    // __VISUALC__
    #if (_MSC_VER>=1000)
    #define __VISUALC__
    #endif

    // __BORLANDC__

    // __WATCOMC__
    
    // __DJGPP__

    // __CYGNUS__
    #ifdef __CYGWIN32__ 
    #define __CYGNUS__
    #endif

    // __GNU__
    #if defined(__DJGPP__) || defined(__CYGNUS__)
    #define __GNU__
    #endif


// PLATFORMS

    // __WIN32__
    #if defined(WIN32) || defined(_WIN32) || defined(WINDOWS) || defined(__NT__)
    #define __WIN32__
    #endif

    // __LINUX__

    // __DOS32__
    #if !defined(__WIN32__) && !defined(__LINUX__)
    #define __DOS32__
    #endif








#endif
