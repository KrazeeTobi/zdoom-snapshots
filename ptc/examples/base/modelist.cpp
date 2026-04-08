//////////////////////////
// ptc modelist example //
//////////////////////////
#include "ptc.h"
#include <iostream.h>
#include <iomanip.h>





int main()
{
    // init
    PTC ptc;

    // get ptc modelist
    List<MODE> list;
    ptc.GetModeList(list);

    // display modelist
    cout << "PTC modelist:" << endl << endl;
    List<MODE>::Iterator iterator=list.first();
    MODE *mode=iterator.current();
    while (mode)
    {
        // print mode interface name
        cout << setw(8) << mode->i;

        // print mode resolution
        cout << " " 
             << setw(4) << mode->x 
             << " x " 
             << setiosflags(ios::left) 
             << setw(4) << mode->y
             << resetiosflags(ios::left);

        // print bits per pixel
        cout << " " << setw(2) << mode->format.bits << "bit";
        
        // print pixel format 
        cout << "  " << setw(10);
        switch (mode->format.id)
        {
            case ARGB8888:   cout << "ARGB8888";   break;
            case ABGR8888:   cout << "ABGR8888";   break;
            case BGRA8888:   cout << "BGRA8888";   break;
            case RGBA8888:   cout << "RGBA8888";   break;
            case RGB888:     cout << "RGB888";     break;
            case BGR888:     cout << "BGR888";     break;
            case RGB565:     cout << "RGB565";     break;
            case BGR565:     cout << "BGR565";     break;
            case ARGB1555:   cout << "ARGB1555";   break;
            case ABGR1555:   cout << "ABGR1555";   break;
            case INDEX8:     cout << "INDEX8";     break;
            case FAKEMODE1A: cout << "FAKEMODE1A"; break;
            case FAKEMODE1B: cout << "FAKEMODE1B"; break;
            case FAKEMODE1C: cout << "FAKEMODE1C"; break;
            case FAKEMODE2A: cout << "FAKEMODE2A"; break;
            case FAKEMODE2B: cout << "FAKEMODE2B"; break;
            case FAKEMODE2C: cout << "FAKEMODE2C"; break;
            case FAKEMODE3A: cout << "FAKEMODE3A"; break;
            case FAKEMODE3B: cout << "FAKEMODE3B"; break;
            case FAKEMODE3C: cout << "FAKEMODE3C"; break;
            case GREY8:      cout << "GREY8";      break;
            case RGB332:     cout << "RGB332";     break;
            default:         cout << "(custom)";   break;
        }

        // print format type
        cout << "  " << setw(8);
        switch (mode->format.type)
        {
            case DIRECT:   cout << "DIRECT";   break;
            case INDEXED:  cout << "INDEXED";  break;
            case FAKEMODE: cout << "FAKEMODE"; break;
            default:       cout << "--------"; break;
        }

        // print memory layout
        cout << "  " << setw(8);
        switch (mode->layout)
        {
            case LINEAR:   cout << "LINEAR";   break;
            case BANKED:   cout << "BANKED";   break;
            case PLANAR:   cout << "PLANAR";   break;
            case FAKEMODE: cout << "FAKEMODE"; break;
            default:       cout << "--------"; break;
        }
        
        // print output type
        cout << "  " << setw(10);
        switch (mode->output)
        {
            case WINDOWED:   cout << "WINDOWED";   break;
            case FULLSCREEN: cout << "FULLSCREEN"; break;
            default:         cout << "----------"; break;
        }

		// print mode frequency
		cout << "  " << setw(3);
		switch (mode->frequency)
        {
            case UNKNOWN: cout << "---Hz"; break;
            default:      cout << mode->frequency << "Hz";
        }

        // go to next mode
        mode=iterator.next();
        cout << endl;
    }
    return 0;
}
