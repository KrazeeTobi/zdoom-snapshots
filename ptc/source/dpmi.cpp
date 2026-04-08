////////////////////
// DPMI interface //
////////////////////
#include "dpmi.h"

#ifdef __DPMI__






// global dpmi object
DPMI global_dpmi_object;

// static data
ushort DPMI::DosMemorySegment=NULL;
ushort DPMI::DosMemorySelector=NULL;
uint DPMI::DosMemorySize=0;
int DPMI::DosMemoryCount=0;






// initialize dos memory
int DPMI::InitDosMemory(int bytes)
{
    // increment reference count
    DosMemoryCount++;

    // initialize if (1)
    if (DosMemoryCount==1) 
    {
        if (!AllocateDosMemory(&DosMemorySegment,&DosMemorySelector,bytes)) return 0;
        else DosMemorySize=bytes;
        return 1;
    }
    else return 1;
}

void DPMI::CloseDosMemory()
{
    // close if dec to zero
    if (DosMemoryCount==1)
    {
        if (DosMemorySelector)
        {
            FreeDosMemory(DosMemorySelector);
            DosMemorySize=0;
            DosMemorySegment=NULL;
            DosMemorySelector=NULL;
        }
    }

    // decrement reference count
    if (DosMemoryCount>0) DosMemoryCount--;
}







#endif
