////////////////
// Code Block //
////////////////

// include files
#include "code.h"
#include <iostream.h>


#if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)


// internals
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>





Code::Code()
{
    // setup current code
    CodeBlock=(unsigned char*)VirtualAlloc(NULL,4069,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    CurrentCode=CodeBlock;
    //cout << "CodeBlock=" << (void*)CodeBlock << endl;
}


Code::~Code()
{
    // cleanup
    VirtualFree(CodeBlock,0,MEM_RELEASE);
    //cout << "freeing code block\n";
}





void Code::move(int dest,int src)
{
    // "mov dest,src"
    if (dest==EBP && src==ECX)
    {
        // "mov ebp,ecx"
        emit((unsigned char)0x89);
        emit((unsigned char)0xCD);
    }
}


void Code::load(int reg,int bytes)
{
    // check bytes
    if (bytes==4)
    {
        // "mov reg,[esi]"
        emit((unsigned char)0x8B);

        // register
        switch (reg)
        {
            case EAX: emit((unsigned char)0x06); break;
            case EBX: emit((unsigned char)0x1E); break;
            case ECX: emit((unsigned char)0x0E); break;
            case EDX: emit((unsigned char)0x16); break;
        }
    }
    else
    {
        // todo !
    }
}


void Code::store(int reg,int bytes)
{
    // check bytes
    if (bytes==4)
    {
        // store 4 bytes
        if (reg==EAX)
        {
            // "mov [edi],eax"
            emit((unsigned char)0x89); 
            emit((unsigned char)0x07);
        }
    }
    else if (bytes==3)
    {
        // store 3 bytes
        if (reg==EAX)
        {       
            // "mov [edi],al; mov [edi+1],ah; shr eax,16; mov [edi+2],al"
            //printf("store 3 bytes!\n");
            emit((unsigned char)0x88);
            emit((unsigned char)0x07);
            emit((unsigned char)0x88);
            emit((unsigned char)0x67);
            emit((unsigned char)0x01);
            emit((unsigned char)0xC1);
            emit((unsigned char)0xE8);
            emit((unsigned char)0x10);
            emit((unsigned char)0x88);
            emit((unsigned char)0x47);
            emit((unsigned char)0x02);          // note: this *modifies* EAX !
        }
    }
    else if (bytes==2)
    {
        // store 2 bytes 
        if (reg==EAX || reg==AX)
        {
            // "mov [edi],ax"
            emit((unsigned char)0x66); 
            emit((unsigned char)0x89); 
            emit((unsigned char)0x07);
        }
    }
    else if (bytes==1)
    {
        // store 1 byte
        if (reg==EAX || reg==AX || reg==AL)
        {
            // "mov [edi],al"
            emit((unsigned char)0x88);
            emit((unsigned char)0x07);
        }
        else if (reg==AH)
        {
            // "mov [edi],ah"
            emit((unsigned char)0x88); 
            emit((unsigned char)0x27);
        }
    }

    /*
    // "mov [edi],reg"
    switch (reg)
    {
        case EAX: emit((unsigned char)0x89); emit((unsigned char)0x07); break; 
        case AX:  emit((unsigned char)0x66); emit((unsigned char)0x89); emit((unsigned char)0x07); break;
        case AH:  emit((unsigned char)0x88); emit((unsigned char)0x27); break;
        case AL:  emit((unsigned char)0x88); emit((unsigned char)0x07); break;  
    }
    */
}


void Code::shift(int reg,int count)         // note: shift = shr
{
    // "shr/shl reg,count"
    if (count==0) return;
    else if (count==1)
    {
        // "shr reg,1"
        emit((unsigned char)0xD1);
        switch (reg)
        {
            case EAX: emit((unsigned char)0xE8); break;
            case EBX: emit((unsigned char)0xEB); break;
            case ECX: emit((unsigned char)0xE9); break;
            case EDX: emit((unsigned char)0xEA); break;
        }
    }
    else if (count==-1)
    {
        // "shl reg,1"
        emit((unsigned char)0xD1);
        switch (reg)
        {
            case EAX: emit((unsigned char)0xE0); break;
            case EBX: emit((unsigned char)0xE3); break;
            case ECX: emit((unsigned char)0xE1); break;
            case EDX: emit((unsigned char)0xE2); break;
        }
    }
    else if (count>1)
    {
        // "shr reg,count"
        emit((unsigned char)0xC1);
        switch (reg)
        {
            case EAX: emit((unsigned char)0xE8); break;
            case EBX: emit((unsigned char)0xEB); break;
            case ECX: emit((unsigned char)0xE9); break;
            case EDX: emit((unsigned char)0xEA); break;
        }
        emit((unsigned char)count);
    }
    else if (count<1)
    {
        // "shl reg,count"
        emit((unsigned char)0xC1);
        switch (reg)
        {
            case EAX: emit((unsigned char)0xE0); break;
            case EBX: emit((unsigned char)0xE3); break;
            case ECX: emit((unsigned char)0xE1); break;
            case EDX: emit((unsigned char)0xE2); break;
        }
        emit((unsigned char)-count);
    }
}


void Code::push(int reg)
{
    // "push reg"
    if (reg==EBP) emit((unsigned char)0x55);
}


void Code::pop(int reg)
{
    // "pop reg"
    if (reg==EBP) emit((unsigned char)0x5D);
}


void Code::push()
{
    // "pusha"
    emit((unsigned char)0x60);
}


void Code::pop()
{
    // "popa"
    emit((unsigned char)0x61);
}


void Code::neg(int reg)
{
    // "neg reg"
    if (reg==EBP)
    {
        // "neg ebp"
        emit((unsigned char)0xF7);
        emit((unsigned char)0xDD);
    }
}


void Code::add(int reg,int value)
{
    // "add reg,value"
    if (reg==ESI)
    {
        // "add esi,value"
        emit((unsigned char)0x81);
        emit((unsigned char)0xC6);
        emit((unsigned int)value);
    }
    else if (reg==EDI)
    {
        // "add edi,value"
        emit((unsigned char)0x81);
        emit((unsigned char)0xC7);
        emit((unsigned int)value);
    }
}


void Code::inc(int reg)
{
    // "inc reg"
    if (reg==EBP) 
    {
        // "inc ebp"
        emit((unsigned char)0x25);
        emit((unsigned int)0xFFFFFFFF);
    }
}
    
    
void Code::dec(int reg)
{
    // "dec reg"
    if (reg==EBP) emit((unsigned char)0x4D);
}
    
    
void Code::and(int reg,int value)
{
    // "and reg,value"
    switch (reg)
    {
        case EAX: emit((unsigned char)0x25); emit((unsigned int)value); break;
        case EBX: emit((unsigned char)0x81); emit((unsigned char)0xE3); emit((unsigned int)value); break;
        case ECX: emit((unsigned char)0x81); emit((unsigned char)0xE1); emit((unsigned int)value); break;
        case EDX: emit((unsigned char)0x81); emit((unsigned char)0xE2); emit((unsigned int)value); break;
    }
}


void Code::or(int dest,int src)
{
    // "or reg,value"
    if (dest==EAX && src==EBX)
    {
        // "or eax,ebx"
        emit((unsigned char)0x09); 
        emit((unsigned char)0xD8);
    }
    else if (dest==ECX && src==EDX)
    {
        // "or ecx,edx"
        emit((unsigned char)0x09); 
        emit((unsigned char)0xD1);
    }
    else if (dest==EAX && src==ECX) 
    {
        // "or eax,ecx"
        emit((unsigned char)0x09); 
        emit((unsigned char)0xC8);
    }
}


void Code::jnz(void *address)
{
    // calculate jump offset
    int offset = (int)address-(int)CurrentCode;

    // check for short jump
    int near_offset=offset-2;
    if (near_offset>=-127 && near_offset<=127)
    {

        // near jump
        emit((unsigned char)0x75);
        emit((unsigned char)near_offset);
    }
    else
    {
        // "jz" exit loop
        emit((unsigned char)0x74);
        emit((unsigned char)5);

        // "jmp" to address
        emit((unsigned char)0xE9);
        emit((unsigned int)(offset-7));
    }
}


void Code::ret()
{
    // "ret"
    emit((unsigned char)0xC3);
}





void* Code::start()
{
    // code start address
    return (void*)CodeBlock;
}


void* Code::current()
{
    // current code address
    return (void*)CurrentCode;
}





void Code::execute()
{
    // execute code
    void (*function)() = (void(*)()) (void*)&CodeBlock[0];
    function();
}


void Code::clear()
{
    // clear block
    CurrentCode=CodeBlock;
}


void Code::print()
{
    // print
    unsigned char *code=CodeBlock;
    while (code<CurrentCode)
    {
        printf("%.2X,",(unsigned int)*code);
        code++;
    }
    printf("\n");
}
        




int Code::ok()
{
    // status
    return 1;
}





void Code::emit(unsigned char value)
{
    // emit byte
    *CurrentCode=value;
    CurrentCode++;
}


void Code::emit(unsigned short value)
{
    // emit word
    unsigned short* code=(unsigned short*)CurrentCode;
    *code=value;
    CurrentCode+=2;
}


void Code::emit(unsigned int value)
{
    // emit dword
    unsigned int* code=(unsigned int*)CurrentCode;
    *code=value;
    CurrentCode+=4;
}





#endif
