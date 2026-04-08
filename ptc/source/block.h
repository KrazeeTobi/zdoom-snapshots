//////////////////
// Memory block //
//////////////////

#ifndef __PTC_BLOCK_H
#define __PTC_BLOCK_H

#include "manager.h"







class MemoryBlock
{
    // friends
    friend class MemoryManager;

    public:

        // setup
        MemoryBlock();
        MemoryBlock(MemoryManager &manager,uint size,int managed=1);
        ~MemoryBlock();

        // management
        int alloc(uint size,int managed);
        void free();

        // memory access
        void* lock();
        void unlock();
        int count();

        // interface
        int resize(uint size);

        // object status
        int ok();

    private:

        // memory manager
        MemoryManager *LocalManager;

        // manager block
        MemoryManager::BLOCK *Block;
};







#endif
