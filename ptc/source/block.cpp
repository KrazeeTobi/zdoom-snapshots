//////////////////
// Memory block //
//////////////////
#include "block.h"
#include "manager.h"








MemoryBlock::MemoryBlock()
{
    // defaults
    LocalManager=NULL;
    Block=NULL;
}


MemoryBlock::MemoryBlock(MemoryManager &manager,uint size,int managed)
{
    // initialize
    LocalManager=&manager;
    Block=NULL;

    // allocate
    alloc(size,managed);
}


MemoryBlock::~MemoryBlock()
{
    // free block
    if (LocalManager && Block) LocalManager->FreeBlock(Block);
}







int MemoryBlock::alloc(uint size,int managed)
{
    // free block
    free();

    // allocate
    if (LocalManager) Block=LocalManager->RequestBlock(*this,size,managed);
    
    // return value
    if (Block) return 1;
    else return 0;
}


void MemoryBlock::free()
{
    // free block
    if (LocalManager && Block) LocalManager->FreeBlock(Block);
}







void* MemoryBlock::lock()
{
    // lock block
    if (LocalManager && Block) return LocalManager->LockBlock(Block);
    else return NULL;
}


void MemoryBlock::unlock()
{
    // unlock block
    if (LocalManager && Block) LocalManager->UnlockBlock(Block);
}


int MemoryBlock::count()
{
    // unlock block
    if (LocalManager && Block) return Block->count;
    else return 0;
}







int MemoryBlock::resize(uint size)
{
    // resize block
    if (LocalManager && Block) return LocalManager->ResizeBlock(Block,size);
    else return 0;
}







int MemoryBlock::ok()
{
    // return object status
    if (!LocalManager) return 0;
    else if (Block) return 1;
    else return 0;
}
