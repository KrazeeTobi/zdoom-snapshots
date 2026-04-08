/////////////////////
// keyboard buffer //
/////////////////////

#ifndef __KEYBOARD_H
#define __KEYBOARD_H






class Keyboard
{
    public:

        // setup
        Keyboard(int size=1024);
        ~Keyboard();

        // interface
        int insert(int key);

        // routines
        int check();
        int get(int &key);

    private:

        // data
        int Size;           // size of keyboard buffer (in integers)
        int *Buffer;        // keyboard buffer
        int *Head;          // keyboard buffer head
        int *Tail;          // keyboard buffer tail
};






#endif
