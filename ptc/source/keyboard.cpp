/////////////////////
// keyboard buffer //
/////////////////////
#include "keyboard.h"






Keyboard::Keyboard(int size)
{
    // allocate buffer
    Buffer=new int[size];

    // setup data
    Size=size;
    Head=Buffer;
    Tail=Buffer;
}


Keyboard::~Keyboard()
{
    // delete buffer
    delete[] Buffer;
}





int Keyboard::insert(int key)
{
    // update head
    Head++;

    // loop back
    if (Head>=Buffer+Size) Head=Buffer;

    // check for tail
    if (Head==Tail) return 0;
    else
    {
        // insert key into buffer
        *Head=key;
        return 1;
    }
}





int Keyboard::check()
{
    // check for waiting keys
    return (Head!=Tail);
}


int Keyboard::get(int &key)
{
    // check for waiting keys
    if (Tail==Head) return 0;

    // get key
    key=*Tail;
    
    // update tail
    Tail++;

    // loop back
    if (Tail>=Buffer+Size) Tail=Buffer;

    // success
    return 1;
}
