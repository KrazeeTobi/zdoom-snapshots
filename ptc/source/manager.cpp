////////////////////
// Memory manager //
////////////////////
#include "manager.h"
#include "block.h"







MemoryManager::MemoryManager()
{
    // defaults
    Memory=NULL;
    MemorySize=0;
}


MemoryManager::MemoryManager(void *memory,uint size)
{
    // defaults
    Memory=NULL;
    MemorySize=0;

    // initialize
    Init(memory,size);
}


MemoryManager::~MemoryManager()
{
    // close
    Close();
}







int MemoryManager::Init(void *memory,uint size)
{
    // close
    Close();

    // initialize
    Memory=memory;
    MemorySize=size;

    // initialize free block list
    BLOCK *free=new BLOCK;
    free->owner=NULL;
    free->address=Memory;
    free->size=MemorySize;
    free->count=0;
    free->managed=0;
    FreeBlockList.add(free);
    return 1;
}


void MemoryManager::Close()
{
    // free all standalone memory blocks + invalidate
    List<BLOCK>::Iterator i=StandaloneList.first();
    BLOCK *current=i.current();
    while (current)               
    {
        free(current->address);
        current->owner->LocalManager=NULL;
        current->owner->Block=NULL;
        current=i.next();
    }

    // invalidate all managed memory blocks
    List<BLOCK>::Iterator j=BlockList.first();
    current=j.current();
    while (current)               
    {
        current->owner->LocalManager=NULL;
        current->owner->Block=NULL;
        current=j.next();
    }

    // empty lists
    StandaloneList.free();
    BlockList.free();
    FreeBlockList.free();

    // defaults
    Memory=NULL;
    MemorySize=0;
}







int MemoryManager::Compact()
{
    // compact memory blocks (not yet)
    return 0;
}







int MemoryManager::GetSize()
{
    return MemorySize;
}


int GetFree()
{
    // not yet
    return 0;
}


int MemoryManager::ok() const
{
    if (Memory && MemorySize==0) return 0;
    else return 1;
}







MemoryManager::BLOCK* MemoryManager::RequestBlock(MemoryBlock &owner,uint size,int managed)
{
    // check block request type
    if (managed)
    {
        // managed block request
        BLOCK *block=new BLOCK;
        if (!block) return NULL;                    // how to check!

        // scan for first free block large enough
        List<BLOCK>::Iterator iterator=FreeBlockList.first();
        BLOCK *current=iterator.current();
        while (current)
        {
            if (current->size>=size)
            {
                // found suitable block
                block->owner=&owner;
                block->address=current->address;
                block->size=size;
                block->count=0;
                block->managed=1;

                // add new block to list
                if (!BlockList.add(block))
                {
                    // failure
                    delete block;
                    return NULL;
                }
            
                // resize free block
                current->owner=NULL;
                current->address=(uchar*)block->address+block->size;
                current->size=current->size-size;
                current->count=0;
                current->managed=0;

                // check free block size (remove if zero)
                if (current->size==0) FreeBlockList.free(current);

                // return block to user
                return block;
            }
            current=iterator.next();
        }
        
        // failure
        delete block;
        return NULL;
    }
    else
    {
        // standalone block request
        BLOCK *block=new BLOCK;
        if (!block) return NULL;                    // how to check!
        block->owner=&owner;
        block->address=(char*)malloc(size);
        if (!block->address)
        {
            // failure
            delete block;
            return NULL;
        }
        block->size=size;
        block->count=0;
        block->managed=0;

        // add new block to list
        if (!StandaloneList.add(block))
        {
            // failure
            free(block->address);
            delete block;
            return NULL;
        }

        // success
        return block;
    }
}


void* MemoryManager::LockBlock(BLOCK *block)
{
    // increment lock count
    block->count++;

    // return buffer
    return block->address;
}


void MemoryManager::UnlockBlock(BLOCK *block)
{
    // decrement lock count
    if (block->count) block->count--;
}


int MemoryManager::ResizeBlock(BLOCK *block,uint size)
{
    // free block then reallocate
    if (block || size);
    return 0;
}


void MemoryManager::FreeBlock(BLOCK *block)
{
    // check if address is in range
    if (block->managed)
    {
        // invalidate owner block
        if (block->owner)
        {
            block->owner->LocalManager=NULL;
            block->owner->Block=NULL;
        }

        // free block from list
        BlockList.remove(block);

        // add to free list
        block->owner=NULL;
        block->count=0;
        FreeBlockList.add(block);

        // todo: merge adjacent free blocks
    }
    else
    {
        // invalidate owner block
        if (block->owner)
        {
            block->owner->LocalManager=NULL;
            block->owner->Block=NULL;
        }

        // free memory
        free(block->address);

        // free block from managed list
        StandaloneList.free(block);
    }
}
