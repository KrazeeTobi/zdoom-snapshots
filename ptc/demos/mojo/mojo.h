#include "ptc.h"
#include "coords.h"


typedef unsigned char        UBYTE;
typedef signed char          SBYTE;
typedef unsigned char        BYTE;
typedef unsigned short int   UWORD;
typedef signed short int     SWORD;
typedef unsigned short int   WORD;
typedef signed int           SLONG;
typedef unsigned int         UDWORD;
typedef signed int           SDWORD;

#ifndef __WIN32__
typedef unsigned int         ULONG;
typedef unsigned int         LONG;
typedef unsigned int         DWORD;
#endif

#define CHAR char
#define VOID void

#ifndef PI
#define PI 3.14159265493
#endif

#define SQR(x) ((x)*(x))

