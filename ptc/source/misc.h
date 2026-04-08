/////////////////////////
// miscellaneous stuff //
/////////////////////////

#ifndef __PTC_MISC_H
#define __PTC_MISC_H

#include "lang.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>





// ptc version
#define __PTC__ 072




// nice types
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;




// NULL if needed
#ifndef NULL                                       
#define NULL 0
#endif




#ifndef __BORLANDC__

// randomize
#ifndef randomize
#define randomize() srand((uint)time(NULL))
#endif

// randoms
#ifndef random
inline int random(int max)
{
    float n=(float)rand()/((float)(RAND_MAX));
    int val=(int)floor(n*max);
    if (val>=max) val=max-1;
    return val;
}
#endif

#endif




// inp | outp for VC++
#ifdef __VISUALC__
#define outp  _outp
#define outpw _outpw
#define outpd _outpd
#define inp  _inp
#define inpw _inpw
#define inpd _inpd
#endif

// inp | outp fo DJGPP
#if defined(__DJGPP__) 
#include <pc.h>
#endif

// inp | outp for LINUX
#if defined(__LINUX__)

    inline void outp(int port,int value) 
    { 
        __asm__ volatile ("outb %0,%1" 
                : :"a" ((unsigned char) value), "d"((unsigned short) port)); 
    }

    inline void outpw(int port,int value)
    {
        __asm__ volatile ("outw %0,%1"
	            : :"a" ((unsigned short) value), "d"((unsigned short) port));
    }

    inline void outpd(int port,int value)
    {
        __asm__ volatile ("outl %0,%1"
                : :"a" ((unsigned long)value), "d" ((unsigned short) port));
    }

    inline int inp(int port) 
    {
        unsigned char value;
        __asm__ volatile ("inb %1,%0"
		      :"=a" (value)
		      :"d"((unsigned short) port));
        return value;
    }
    
    inline int inpw(int port)
    {
        unsigned short value;
        __asm__ volatile ("inw %1,%0"
		          :"=a" (value)
		          :"d"((unsigned short) port));
        return value;
    }

    inline int inpd(int port)
    {
        unsigned int value;
        __asm__ volatile("inl %1,%0" :
                   	     "=a" (value) :
                         "d" ((unsigned short)port));
        return value;
    }
    
#endif




// console input
#if !defined(__LINUX__) && !defined(__CYGNUS__) && !defined(__AMIGA__)
#include <conio.h>
#endif



// include memory routines
#if defined(__VISUALC__) || defined(__GNU__) || defined(__LINUX__) || defined(__AMIGA__)
#include <memory.h>
#include <string.h>
#endif

#ifdef __WATCOMC__
#include <mem.h>
#include <string.h>
#endif



// include windows.h for windows platform
#ifdef __WIN32__
#include <windows.h>
#endif




// win32 delay
#ifdef __WIN32__
inline void delay(int time) { Sleep(time); }
#endif




// fixup icky kbhit #defines in DJGPP
#ifdef __DJGPP__
    #if kbhit == _conio_kbhit
    #undef kbhit
    inline int kbhit() { return _conio_kbhit(); }
    #endif
#endif


// no kbhit
#ifdef __CYGNUS__
    inline int kbhit() { return 0; }
    inline int getch() { return getchar(); }
#endif

// no kbhit
#ifdef __LINUX__
    inline int kbhit() { return 0; }
    inline int getch() { return getchar(); }
#endif

// no kbhit
#ifdef __AMIGA__
    inline int kbhit() { return 0; }
    inline int getch() { return getchar(); }
#endif



// define "PACKED"
#ifndef PACKED
    #if defined (__DJGPP__)
        #define PACKED __attribute__ ((packed))        
    #else
        #define PACKED 
    #endif
#endif



// stricmp
#if defined(__LINUX__) || defined(__CYGNUS__) || defined(__AMIGA__)
    #define stricmp strcasecmp
#endif





// undefine
#undef ABSOLUTE
#undef HALFTONE





// WINDOW
#ifdef __WIN32__

    // win32 HWND
    #define WINDOW HWND

#elif defined(__X11__)

    // X11 Window
    #define WINDOW Window

#else
    
    // dummy window
    struct STUPIDWINDOW { int bah; };
    typedef STUPIDWINDOW* WINDOW;

#endif









#endif
