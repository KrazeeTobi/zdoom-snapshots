///////////////////
// thread module //
///////////////////

#ifndef __PTC_THREAD_H
#define __PTC_THREAD_H

#include "misc.h"
#include "config.h"







#if defined(__VISUALC__) || ( defined(__WIN32__) && defined(__WATCOMC__) )

    // use "_beginthread"
    #include <process.h>
    #define __THREAD_BEGINTHREAD__

    #if defined(__VISUALC__)

    // Visual C++ "beginthread"
    inline unsigned long beginthread( void( __cdecl *start_address )( void * ), unsigned stack_size, void *arglist )
    {
        return _beginthread(start_address,stack_size,arglist);
    }

    #elif defined(__WATCOMC__) && (__WATCOMC__>=1100)

    // Watcom C++ 11.0 "beginthread"
    typedef void (*THREAD_FUNCTION)(void*);
    inline unsigned long beginthread( THREAD_FUNCTION function, unsigned stack_size, void *arglist )
    {
        return _beginthread(function,stack_size,arglist);
    }

    #elif defined(__WATCOMC__) && (__WATCOMC__<1100)

    // Watcom C++ 10.x "beginthread"
    typedef void (*THREAD_FUNCTION)(void*);
    inline unsigned long beginthread( THREAD_FUNCTION function, unsigned stack_size, void *arglist )
    {
        return _beginthread(function,NULL,stack_size,arglist);
    }

    #endif

#elif defined(__WIN32__)

    // use "CreateThread"
    #define __THREAD_CREATETHREAD__
    typedef HANDLE CREATETHREAD_HANDLE;

#endif





// fixup "INFINITE"
#ifdef __WIN32__
const THREAD_INFINITE=INFINITE;
#undef INFINITE
const INFINITE=THREAD_INFINITE;
#else
const INFINITE=0xFFFFFFFF;
#endif








class Thread
{
    public:

        // Thread::HANDLE
        #if defined(__THREAD_BEGINTHREAD__)

            // "_beginthread" thread handle
            typedef uint HANDLE;
    
        #elif defined(__THREAD_CREATETHREAD__)

            // "CreateThread" thread handle
            typedef CREATETHREAD_HANDLE HANDLE;

        #else
    
            // no threads
            typedef uint HANDLE;

        #endif


        // Thread::FUNCTION
        #if defined(__THREAD_BEGINTHREAD__)

            // "_beginthread" thread function
            #if defined(__VISUALC__)
                typedef void (__cdecl*FUNCTION)(void*);
            #else
                typedef void (*FUNCTION)(void*);
            #endif
    
        #elif defined(__THREAD_CREATETHREAD__)

            // "CreateThread" thread function
            typedef void (*FUNCTION)(void*);

        #else
        
            // no thread support
            typedef void* FUNCTION;

        #endif

        // timeout
        enum TIMEOUT 
        { 
            CHECK    = 0,
            INFINITE = ::INFINITE
        };

    public:

        // setup
        inline Thread();
        inline Thread(FUNCTION function,void *data,int stack=DEFAULT);
        inline ~Thread();

        // wait for thread exit
        inline void WaitForExit(int timeout=INFINITE);

        // thread handle
        inline HANDLE GetHandle();

        // status
        inline int ok();

    private:

        // data
        HANDLE Handle;
};








inline Thread::Thread()
{
    // defaults
    Handle=NULL;
}



inline Thread::Thread(Thread::FUNCTION function,void *data,int stack)
{
#if defined(__VISUALC__)

    // default visual c++ stack size
    if (stack==DEFAULT) stack=0;

#elif defined(__WATCOMC__)

    // default watcom c++ stack size
    if (stack==DEFAULT) stack=256*1024;        // too big?
    
#elif defined(__THREAD_CREATETHREAD__)
    
    // default CreateThread stack size
    if (stack==DEFAULT) stack=0;

#endif

#if defined(__THREAD_BEGINTHREAD__)

    // beginthread
    Handle=beginthread(function,stack,data);

#elif defined(__THREAD_CREATETHREAD__)

    // CreateThread
    DWORD id;
    Handle=CreateThread(NULL,stack,(LPTHREAD_START_ROUTINE)function,data,0,&id);

#else

    // no threads
    Handle=NULL;

    // advoid warnings
    if (function || data || stack);

#endif
}



inline Thread::~Thread()
{
#if defined(__THREAD_BEGINTHREAD__)

    // endthread?
    Handle=NULL;

#elif defined(__THREAD_CREATETHREAD__)

    // CreateThread
    if (Handle) CloseHandle(Handle);

#else

    // no threads
    Handle=NULL;

#endif
}








inline void Thread::WaitForExit(int timeout)
{
#if defined(__THREAD_BEGINTHREAD__) || defined(__THREAD_CREATETHREAD__)

    // wait for exit
    WaitForSingleObject((::HANDLE)Handle,timeout);
    Handle=NULL;

#else

    // advoid warnings
    if (timeout);

#endif
}







inline Thread::HANDLE Thread::GetHandle()
{
    // thread handle
    return Handle;
}



    
    
    

inline int Thread::ok()
{
    // return status
    if (Handle) return 1;
    else return 0;
}








#endif
