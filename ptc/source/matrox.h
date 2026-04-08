///////////////////////////
// matrox native support //
///////////////////////////

#ifndef __PTC_MATROX_H
#define __PTC_MATROX_H

#include "misc.h"
#include "dpmi.h"

#ifdef __MATROX__






class MATROX
{
    public:

        // setup
        MATROX();
        ~MATROX();

        // interface
        int zoom();
        int unzoom();

        // status
        int ok();

    private:

        // internal routines
        int mystique_init();
        void mystique_close();
        int find_mystique();
        void enable_sequencer();
        void disable_sequencer();

        // pci routines
        int PCI_Installed();
        int PCI_FindDevice(int id1,int id2);
        uint PCI_ReadDword(uchar id,uint offset);

        // defaults
        void defaults();

        // data
        uchar *mga_ports;
        int mystique_id;
        int ports_mapped;
        int zoom_active;

        // status variable
        int status;

        // dpmi object
        DPMI dpmi;
};






#endif

#endif
