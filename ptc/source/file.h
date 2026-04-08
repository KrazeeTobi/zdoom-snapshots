////////////////
// File class //
////////////////

#ifndef __PTC_FILE_H
#define __PTC_FILE_H

#include "list.h"
#include <stdio.h>
#include <fcntl.h>






// configuration
#if defined(__WATCOMC__) && __WATCOMC__<1100
#define FILE_NON_BUFFERED
#include <io.h>
#endif

                   
                  

           
class File
{
    public:

        // file mode
        enum MODE
        {
            READ=1,
            WRITE=2,
            APPEND=4,
            TEXT=8,
            BINARY=16
        };

        // seek origin
        enum SEEK
        {
            START=SEEK_SET,
            END=SEEK_END,
            CURRENT=SEEK_CUR
        };

    public:

        // setup
        File();
        File(char *filename,int mode=READ|WRITE|BINARY);
        ~File();

        // open and close
        int open(char *filename,int mode=READ|WRITE|BINARY);
        void close();

        // file operations
        int read(void *buffer,unsigned bytes);
        int write(void *buffer,unsigned bytes);
        int seek(int offset,int mode=START);
        int tell();
        int eof();
        int size();

        // status
        int ok() const;

    protected:

        // defaults
        void Defaults();

        // file handle
        #ifdef FILE_NON_BUFFERED
        int Handle;
        #else
        FILE *Handle;
        #endif
};










#endif
