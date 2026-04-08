//////////////////////////////////
// prometheus truecolor globals //
//////////////////////////////////

#ifndef __PTC_GLOBALS_H
#define __PTC_GLOBALS_H

#include "misc.h"






// default constant
const DEFAULT=-1;

// unknown constant
const UNKNOWN=0;

// memory layouts
const LINEAR = 10;
const BANKED = 11;
const PLANAR = 12;   // FAKEMODE also
const MIPMAP = 13;

// surface orientations
const TOPDOWN  = 20;
const BOTTOMUP = 21;

// surface types 
const SYSTEM    = 30;
const VIDEO     = 31;
const TEXTURE   = 32;
const OFFSCREEN = 33;

// output types
const FULLSCREEN = 40;
const WINDOWED   = 41;

// coordinate systems
const ABSOLUTE   = 50;
const NORMALIZED = 51;


// format ids
const ARGB8888    = 1000;
const ABGR8888    = 1001;
const BGRA8888    = 1002;
const RGBA8888    = 1003;
const RGB888      = 1004;
const BGR888      = 1005;
const RGB565      = 1006;
const BGR565      = 1007;
const ARGB1555    = 1008;
const ABGR1555    = 1009;
const INDEX8      = 1010;
const FAKEMODE1A  = 1011; 
const FAKEMODE1B  = 1012;
const FAKEMODE1C  = 1013;
const FAKEMODE2A  = 1014;
const FAKEMODE2B  = 1015;
const FAKEMODE2C  = 1016;
const FAKEMODE3A  = 1017;
const FAKEMODE3B  = 1018;
const FAKEMODE3C  = 1019;
const GREY8       = 1020;
const RGB332      = 1021;
const YACbCr7144  = 1022;
const YCbCr422    = 1023;
const YCbCr643    = 1024;
const YCbCr433    = 1025;

// format id limits
const FORMATBASE  = 1000;
const FORMATMAX   = 1025;

// helpers
const RGB555    = ARGB1555;
const BGR555    = ABGR1555;
const FAKEMODE1 = FAKEMODE1A;
const FAKEMODE2 = FAKEMODE2A;
const FAKEMODE3 = FAKEMODE3C;      


// fuzzy modesets
const FUZZY8  = FORMATMAX+100;
const FUZZY16 = FORMATMAX+101;
const FUZZY32 = FORMATMAX+102;


// virtual modesets (fuzzy)
const VIRTUAL8  = FUZZY8;
const VIRTUAL16 = FUZZY16;
const VIRTUAL32 = FUZZY32;


// format types
const DIRECT   = FORMATMAX+200;
const INDEXED  = FORMATMAX+201; 
const FAKEMODE = FORMATMAX+202;


// color models
const RGBA      = FORMATMAX+300;
const CMYA      = FORMATMAX+301;
const CMYK      = FORMATMAX+302;
const HSIA      = FORMATMAX+303;
const HLSA      = FORMATMAX+304;
const YIQA      = FORMATMAX+305;
const YUVA      = FORMATMAX+306;
const YCbCrA    = FORMATMAX+307;
const GREYSCALE = FORMATMAX+308;


// american spellings
const GRAYSCALE = GREYSCALE;
const GRAY8     = GREY8;


// surface lock
const LOCKBUSY = 1;






#endif
