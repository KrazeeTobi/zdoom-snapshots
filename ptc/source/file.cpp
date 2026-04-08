////////////////
// File class //
////////////////
#include "file.h"








File::File()
{
    // defaults
    Defaults();
}


File::File(char *filename,int mode)
{
    // defaults
    Defaults();

    // open file
    open(filename,mode);
}


File::~File()
{
    // close
    close();
}







int File::open(char *filename,int mode)
{
#ifdef FILE_NON_BUFFERED

    // file open mode
    int openmode=0;

    // read/write mode
    if (mode&READ && (mode&WRITE)==0) openmode|=O_RDONLY;       // read only
    else if (mode&WRITE && (mode&READ)==0) openmode|=O_WRONLY;  // write only
    else openmode|=O_RDWR;                                      // read write

    // creation mode (append or not)
    if (mode&APPEND) openmode|=O_APPEND;            
    else if (mode&READ==0 && mode&WRITE) openmode|=O_CREAT|O_TRUNC;

    // file data mode (binary or text)
    if (mode&BINARY) openmode|=O_BINARY;
    if (mode&TEXT) openmode|=O_TEXT;   

    // open file   
    Handle=::open(filename,openmode);
    if (Handle!=-1) return 1;
    else
    {
        // failure!
        Defaults();
        return 0;
    }

#else

    // file open mode
    char openmode[256];
    memset(openmode,0,256);

    // creation mode
    if (mode&APPEND)
    {
        // append read/write mode
        if (mode&READ && (mode&WRITE)==0) strcat(openmode,"r");       // read only
        else if (mode&WRITE && (mode&READ)==0) strcat(openmode,"a");  // write only
        else strcat(openmode,"a+");                                   // read write
    }
    else
    {
        // no append read/write mode
        if (mode&READ && (mode&WRITE)==0) strcat(openmode,"r");       // read only
        else if (mode&WRITE && (mode&READ)==0) strcat(openmode,"w");  // write only
        else strcat(openmode,"r+");                                   // read write
    }

    // file data mode (binary or text)
    if (mode&BINARY) strcat(openmode,"b");
    if (mode&TEXT) strcat(openmode,"t");

    // open file   
    Handle=::fopen(filename,openmode);
    if (Handle) return 1;
    else
    {
        // failure!
        Defaults();
        return 0;
    }

#endif
}


void File::close()
{
    // close handle
    #ifdef FILE_NON_BUFFERED
    if (Handle) ::close(Handle);
    #else
    if (Handle) fclose(Handle);
    #endif

    // defaults
    Defaults();
}







int File::read(void *buffer,unsigned bytes)
{
#ifdef FILE_NON_BUFFERED

    // read from file
    if (Handle)
    {
        int result=::read(Handle,buffer,bytes);
        if (result==-1) return 0;
        else return result;
    }
    else return 0;

#else

    // read from file
    if (Handle) return fread(buffer,1,bytes,Handle);
    else return 0;

#endif
}


int File::write(void *buffer,unsigned bytes)
{
#ifdef FILE_NON_BUFFERED

    // write to file
    if (Handle)
    {
        int result=::write(Handle,buffer,bytes);
        if (result==-1) return 0;
        else return result;
    }
    else return 0;

#else

    // write to file
    if (Handle) return fwrite(buffer,1,bytes,Handle);
    else return 0;

#endif
}


int File::seek(int offset,int mode)
{
#ifdef FILE_NON_BUFFERED

    // seek in file
    if (Handle)
    {
        int result=::lseek(Handle,offset,mode);
        if (result==-1) return 0;
        else return result;
    }
    else return 0;

#else

    // seek in file
    if (Handle) return fseek(Handle,offset,mode);
    else return 0;

#endif
}


int File::tell()
{
#ifdef FILE_NON_BUFFERED

    // tell file pointer position
    if (Handle)
    {
        int result=::tell(Handle);
        if (result==-1) return 0;
        else return result;
    }
    else return 0;

#else

    // tell file pointer position
    if (Handle) return ftell(Handle);
    else return 0;

#endif
}


int File::eof()
{
#ifdef FILE_NON_BUFFERED

    // check for end of file
    if (Handle)
    {
        int result=::eof(Handle);
        if (result==-1) return 0;
        else return result;
    }
    else return 0;

#else

    // check for end of file
    if (Handle) return feof(Handle);
    else return 0;

#endif
}


int File::size()
{
    // return file size
    if (!Handle) return 0;
    else
    {
        int old=tell();
        seek(0,END);
        int location=tell();
        seek(old);
        return location;
    }
}







int File::ok() const
{
    // object status
    if (Handle) return 1;
    else return 0;
}







void File::Defaults()
{
    // defaults
    Handle=NULL;
}
