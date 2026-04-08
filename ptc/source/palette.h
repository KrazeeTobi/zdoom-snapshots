///////////////////
// palette class //
///////////////////

#ifndef __PTC_PALETTE_H
#define __PTC_PALETTE_H

#include "file.h"
#include "misc.h"
#include "raster.h"







class Palette
{
    // friends
    friend class Surface;

    public:

        // setup
        Palette();
        Palette(char filename[]);
        Palette(Palette const &other);
        ~Palette();
        
        // file operations
        int Load(char filename[]);
        int Save(char filename[]);

        // data access
        void* ReadOnly() const;
        void* Lock();
        void Unlock();
        int LockCount() const;

        // set and get palette data
        int Set(void *data);
        int Get(void *data) const;

        // color index table access
        void* GetIndexTable(FORMAT const &format,int alignment=DEFAULT);

        // operators
        int operator ==(Palette const &other) const;
        void operator =(Palette const &other);

        // state
        int ok() const;
        
    private:

        // palette data
        uint *Data;
        int DataLockCount;

        // index table struct
        struct INDEXTABLE
        {
            FORMAT Format;      // index pixel format
            int Alignment;      // byte align (4 byte align default)
            void *Data;         // index table data
        };

        // index table management
        void* CreateIndexTable(FORMAT const &format,int alignment);
        void ClearIndexTableList();

        // index table list
        List<INDEXTABLE> IndexTableList;

        // attach
        int Attach();
        int Detach();
        int AttachCount;
};









#endif
