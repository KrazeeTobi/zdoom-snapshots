///////////////////////////
// matrox native support //
///////////////////////////
#include "matrox.h"

#ifdef __MATROX__





 /*----------------------01-13-98 09:19am---------------------------
  * Submissives Matrox Mystique Low-Resolution gfx-mode hack...
  * (works only for 640x480 and 640x400 modes, but in any color-depth)
  *
  *  exclusively written for for gaffer's PTC-library.
  *
  *               (c) 1998 Nils Pipenbrinck aka Submissive /CTS
  * -----------------------------------------------------------------*/





MATROX::MATROX()
{
    // defaults
    defaults();
    
    // initialize
    if (mystique_init()) status=1;
}


MATROX::~MATROX()
{
    // close
    mystique_close();
}





int MATROX::zoom()
{
    // check if already zoomed
    if (zoom_active) return 1;
    
    // check ports
    if (!ports_mapped) return 0;
  
    // disable sequencer
    disable_sequencer();
    
    // Zoom X Enable (DAC Control)
    mga_ports[0x3c00] = 0x38;
    mga_ports[0x3c0a] = 1;
  
    // Zoom Y Enable (CRTC-0 Control)
    mga_ports[0x1fd4] = 0x09;
    uint val= mga_ports[0x1fd5];
    mga_ports[0x1fd4] = 0x09;
    mga_ports[0x1fd5] = (uchar) ( (val&0xE0) + ((val&0x1F)+1) );
    
    // enable sequencer
    enable_sequencer();
    
    // success
    zoom_active=1;
    return 1;
}


int MATROX::unzoom()
{
    // check if zoomed
    if (!zoom_active)  return 1;
  
    // check ports
    if (!ports_mapped) return 0;

    // disable sequencer
    disable_sequencer();
  
    // Zoom X Disable (DAC Control)
    mga_ports[0x3c00] = 0x38;
    mga_ports[0x3c0a] = 0;
    
    // Zoom Y Disable (CRTC-9 Control)
    mga_ports[0x1fd4] = 0x09;
    uint val= mga_ports[0x1fd5];
    mga_ports[0x1fd4] = 0x09;
    mga_ports[0x1fd5] = (uchar) ( (val&0xE0) + ((val&0x1F)-1) );
    
    // enable sequencer
    enable_sequencer();
    
    // success
    zoom_active=0;
    return 1;
}





int MATROX::ok()
{
    // status
    return status;
}





int MATROX::mystique_init()
{
    // check for pci-bios
    if (!PCI_Installed()) return 0;
  
    // scan pci-bus for mystique card
    if ((mystique_id=find_mystique())==-1) return 0;

    // map the mystique memory mapped ports
    mga_ports=(uchar*)dpmi.MapPhysicalAddress((PCI_ReadDword((uchar)mystique_id,0x10) & 0xfffffff0),16*1024);
    if (!mga_ports) return 0;
  
    // return code based on the dpmi-return code...
    return (ports_mapped = (mga_ports!=0));
}


void MATROX::mystique_close()
{
    // leave the system in a defined state.
    if (zoom_active) unzoom();
    
    // unmap the ports
    if (ports_mapped) dpmi.FreePhysicalMapping((uint)mga_ports);
}


int MATROX::find_mystique()
{
    // search for mystique
    return PCI_FindDevice(0x102b,0x051a);
}


void MATROX::enable_sequencer()
{
    // wait for retrace
    while ( (inp(0x3da) & 8) ) {}
    while (!(inp(0x3da) & 8) ) {}

    // enable sequencer
    mga_ports[0x1fc4]=1;
    uint val = mga_ports[0x1fc5];
    mga_ports[0x1fc4]=1;
    mga_ports[0x1fc5]= (uchar)(val&~32);
}


void MATROX::disable_sequencer()
{
    // wait for retrace
    while ( (inp(0x3da) & 8) ) {}
    while (!(inp(0x3da) & 8) ) {}

    // disable sequencer
    mga_ports[0x1fc4] = 1;
    uint val = mga_ports[0x1fc5];
    mga_ports[0x1fc4] = 1;
    mga_ports[0x1fc5] = (uchar) (val|32);
}





int MATROX::PCI_Installed(void)
{
    // check for pci
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x0000b101;
    if (!dpmi.int86(0x1a,&regs,&regs)) return 0;
    else return (regs.edx==0x20494350);
}


int MATROX::PCI_FindDevice(int id1,int id2)
{
    // get pci device number from id1 and id2
    for (int i=0; i<32; i++) 
    {
        // check for match
        if (( (PCI_ReadDword((uchar)i,0) & 0xffff) == id1 ) && ((PCI_ReadDword((uchar)i,0) >> 16) == id2)) return i;
    }

    // failure!
    return -1;
}


uint MATROX::PCI_ReadDword(uchar id,uint offset)
{
    // read pci dword
    DPMI::REGS regs;
    memset(&regs,0,sizeof(regs));
    regs.eax=0x0000b10a;
    regs.ebx=id<<3;
    regs.edi=offset;
    dpmi.int86(0x1a,&regs,&regs);
    return regs.ecx;
}





void MATROX::defaults()
{
    // defaults
    mga_ports=NULL;
    mystique_id=0;
    ports_mapped=0;
    zoom_active=0;
    status=0;
}






#endif
