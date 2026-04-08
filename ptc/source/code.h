////////////////
// Code Block //
////////////////

#ifndef __PTC_CODE_H
#define __PTC_CODE_H

// include files
#include "misc.h"
#include "config.h"





#if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)


// registers
enum regs 
{ 
    EAX,AX,AH,AL,
    EBX,BX,BH,BL,
    ECX,CX,CH,CL,
    EDX,DX,DH,DL,
    ESI,
    EDI,
    EBP,
    ESP 
};


class Code
{
    public:

        // setup
        Code();
        ~Code();

        // code emission
        void move(int dest,int src);
        void load(int reg,int bytes);
        void store(int reg,int bytes);
        void shift(int reg,int count);
        void push(int reg);
        void pop(int reg);
        void push();
        void pop();
        void neg(int reg);
        void add(int reg,int value);
        void inc(int reg);
        void dec(int reg);
        void and(int reg,int value);
        void or(int dest,int src);
        void jnz(void *address);
        void ret();

        // code info
        void* start();
        void* current();

        // control
        void execute();
        void clear();
        void print();
        
        // status
        int ok();

    private:

        // emission
        void emit(unsigned char value);
        void emit(unsigned short value);
        void emit(unsigned int value);

        // code block data
        unsigned char *CodeBlock;
        unsigned char *CurrentCode;
};


#endif





#endif
