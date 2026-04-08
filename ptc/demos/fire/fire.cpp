/////////////////
// fire effect //
/////////////////
#include "ptc.h"
                  




int SetupPalette(Palette &palette)
{
    // lock palette data
    uint *data=(uint*)palette.Lock();
    if (!data) return 1;
    
    // black to red
    int i=0;
    int c=0;
    while (i<64)
    {
        data[i]=RGB32(c,0,0);
        c+=4;
        i++;
    }

    // red to yellow
    c=0;
    while (i<128)
    {
        data[i]=RGB32(255,c,0);
        c+=4;
        i++;
    }

    // yellow to white
    c=0;
    while (i<128)
    {
        data[i]=RGB32(255,255,c);
        c+=4;
        i++;
    }

    // white
    while (i<256) 
    {
        data[i]=RGB32(255,255,255);
        i++;
    }

    // unlock palette
    palette.Unlock();
    return 1;
}




#ifdef __WIN32__

// windowed application (win32)
int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{

#else

// console application (dos/unix)
int main(int argc,char *argv[])
{

#endif

    // initialize ptc
    PTC ptc(320,200,VIRTUAL8);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("could not initialize");
        return 1;
    }

    // set title
    ptc.SetTitle("fire");

    // create main drawing surface
    Surface surface(ptc,320,210,INDEX8);
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not create surface");
        return 1;
    }
        
    // setup palette
    Palette palette;
    if (!SetupPalette(palette))
    {
        // failure
        ptc.Error("failed to setup palette");
        return 1;
    }

    // set surface palette
    surface.SetPalette(palette);

    // set display palette if required
    FORMAT format=ptc.GetFormat();
    if (format.type==INDEXED && format.model!=GREYSCALE) ptc.SetPalette(palette);

    // setup surface data
    int pitch=surface.GetPitch();
    int width=surface.GetWidth();        // note: width must be divisible by 4
    int height=surface.GetHeight();

    // flame data
    int state=0;
    float intensity=0.0f;

    // main loop
    while (1)
    {
        // exit on keypress
        if (ptc.kbhit()) state=2;

        // state machine
        switch (state)
        {
            case 0: // raise flame
                    intensity+=0.007f;
                    if (intensity>0.8f) state=1;
                    break;

            case 1: // constant flame
                    break;

            case 2: // lower flame
                    intensity-=0.005f;
                    if (intensity<0.0f) return 0;
        }

        // lock surface
        uchar *buffer=(uchar*)surface.Lock();
        if (!buffer) 
        {
            // failure
            ptc.Error("could not lock surface");
            return 1;
        }

        // main flame loop
        uchar *line=buffer;
        for (int y=0; y<height-4; y+=2)
        {
            // current pixel
            uchar *pixel=line;

            // special case: first pixel on line
            uchar *p=pixel+(pitch<<1);
            uint top=*p;
            top+=*(p+width-1);
            top+=*(p+1);
            uint bottom=*(pixel+(pitch<<2));
            uint c1=(top+bottom)>>2;
            if (c1>1) c1--;
            uint c2=(c1+bottom)>>1;
            *pixel=(uchar)c1;
            *(pixel+pitch)=(uchar)c2;
            pixel++;

            // main line loop
            for (int x=1; x<width-1; x++)
            {
                // sum top pixels
                uchar *p=pixel+(pitch<<1);
                uint top=*p;
                top+=*(p-1);
                top+=*(p+1);

                // bottom pixel
                uint bottom=*(pixel+(pitch<<2));

                // combine pixels
                uint c1=(top+bottom)>>2;
                if (c1>1) c1--;

                // interpolate
                uint c2=(c1+bottom)>>1;

                // store pixels
                *pixel=(uchar)c1;
                *(pixel+pitch)=(uchar)c2;

                // next pixel
                pixel++;
            }

            // special case last pixel on line
            p=pixel+(pitch<<1);
            top=*p;
            top+=*(p-1);
            top+=*(p-width+1);
            bottom=*(pixel+(pitch<<2));
            c1=(top+bottom)>>2;
            if (c1>1) c1--;
            c2=(c1+bottom)>>1;
            *pixel=(uchar)c1;
            *(pixel+pitch)=(uchar)c2;

            // next line
            line+=pitch*2;
        }

        // flame generator bar
        uchar *generator=buffer+pitch*(height-4);
        for (int x=0; x<width; x+=4)
        {
            // random 4x4 color blocks
            int c=random((int)(255.0f*intensity));
            *(generator+0)=c;
            *(generator+1)=c;
            *(generator+2)=c;
            *(generator+3)=c;
            *(generator+pitch+0)=c;
            *(generator+pitch+1)=c;
            *(generator+pitch+2)=c;
            *(generator+pitch+3)=c;
            *(generator+pitch*2+0)=c;
            *(generator+pitch*2+1)=c;
            *(generator+pitch*2+2)=c;
            *(generator+pitch*2+3)=c;
            *(generator+pitch*3+0)=c;
            *(generator+pitch*3+1)=c;
            *(generator+pitch*3+2)=c;
            *(generator+pitch*3+3)=c;

            // next block
            generator+=4;
        }

        // unlock surface
        surface.Unlock();

        // update display
        surface.Update(RECTANGLE(0,0,320,200));
    }
    return 0;
}
