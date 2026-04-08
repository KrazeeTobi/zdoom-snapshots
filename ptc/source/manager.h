////////////////////
// Memory manager //
////////////////////

#ifndef __PTC_MANAGER_H
#define __PTC_MANAGER_H

#include "misc.h"
#include "list.h"






class MemoryManager
{
    // friend classes
    friend class MemoryBlock;

    public:

        // setup
        MemoryManager();
        MemoryManager(void *memory,uint size);
        ~MemoryManager();

        // intialize and close
        int Init(void *memory,uint size);
        void Close();

        // interface
        int Compact();

        // information
        int GetSize();
        int GetFree();

        // object status
        int ok() const;

    private:

        // memory block
        struct BLOCK
        {
            MemoryBlock *owner;          // owner object
            void *address;               // address of memory
            uint size;                   // size of memory
            int count;                   // lock count
            int managed;                 // managed status
        };

        // block manipulation
        BLOCK* RequestBlock(MemoryBlock &owner,uint size,int managed=1);
        void* LockBlock(BLOCK *block);
        void UnlockBlock(BLOCK *block);
        int ResizeBlock(BLOCK *block,uint size);
        void FreeBlock(BLOCK *block);

        // list of standalone memory blocks                        
        List<BLOCK> StandaloneList;                                // is this sorta tracking necessary?

        // list of managed memory blocks
        List<BLOCK> BlockList;

        // list of free memory blocks
        List<BLOCK> FreeBlockList;

        // memory to be managed
        void *Memory;
        uint MemorySize;
};









#endif
