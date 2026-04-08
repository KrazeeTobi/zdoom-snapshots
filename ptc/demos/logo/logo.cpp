//////////////
// PTC logo //
//////////////
#include "ptc.h"
                  





// light info
struct LIGHT
{
    float x;           // x displacement
    float y;           // y displacement
};


// cluster info
struct CLUSTER
{
    float x;           // current x position
    float y;           // current y position
    float cx;          // central x location
    float cy;          // central y location
    float width;       // width of ellipse
    float height;      // height of ellipse
    LIGHT r;           // red light info
    LIGHT g;           // green light info
    LIGHT b;           // blue light info
};


// fast light info
struct FASTLIGHT
{
    int x;             // x displacement
    int y;             // y displacement
};


// fast cluster info
struct FASTCLUSTER
{
    int x;             // current x position
    int y;             // current y position
    FASTLIGHT r;       // red light info
    FASTLIGHT g;       // green light info
    FASTLIGHT b;       // blue light info
};






int GetHeight(int x,int y,uchar *src)
{
    // clip coords
    if (x<0) return 0;
    if (y<0) return 0;
    if (x>255) return 0;
    if (y>119) return 0;

    // calculate offset
    return src[y*256+x];
}


int InitBump(uchar *bump)
{
    // create temporary 8bit buffer for map load
    uchar *src=new uchar[256*120];
    if (!src) return 0;

    // load bump map from file
    File file("bump.raw");
    if (file.read(src,256*120)!=256*120)
    {
        // failure
        delete[] src;
        return 0;
    }

    // convert to environment x,y array
    for (int y=0; y<120; y++)
    {
        for (int x=0; x<256; x++)
        {
            // setup the normal from source heightmap
            int nx = GetHeight(x-1,y,src) - GetHeight(x+1,y,src) + 128;
            int ny = GetHeight(x,y-1,src) - GetHeight(x,y+1,src) + 128;

            // clip
            if (nx<0) nx=0;
            if (ny<0) ny=0;
            if (nx>255) nx=255;
            if (ny>255) ny=255;

            // store environment x,y
            bump[2*(y*256+x)+0] = nx;
            bump[2*(y*256+x)+1] = ny;
        }
    }

    // success
    delete[] src;
    return 1;
}


void InitLight(char *light)
{
    // calc lighting envmap
    for (int y=0; y<256; y++)
    {
        for (int x=0; x<256; x++)
        {
            // setup normal
            float nx = (float)(x-128) / 128.0f;
            float ny = (float)(y-128) / 128.0f;
            float nz = 1.0f - (float)sqrt( (nx*nx + ny*ny) );
            if (nz<0) nz=0;

            // phong illumination
            uint i = (uint) ( nz*110 + nz*nz*110 );
            if (i>255) i=255;
            light[y*256+x]=(uchar)i;
        }
    }
}


void DrawLogo(FASTCLUSTER cluster[],uchar *buffer,int pitch,uchar *bump,uchar *light,uchar *clip)
{
    // setup offset
    int offset=0;
    uchar *dest=buffer+pitch*40+32*4;

    // render bump map
    for (int y=0; y<120; y++)
    {
        for (int x=0; x<256; x++)
        {
            // get normal from bump map
            int nx = bump[offset*2]   + x;
            int ny = bump[offset*2+1] + y;

            // setup normal coords
            int nx0 = nx - cluster[0].x;
            int ny0 = ny - cluster[0].y;

            // red component normal coords
            int r_nx0 = nx0 + cluster[0].r.x;
            int r_ny0 = ny0 + cluster[0].r.y;

            // green component normal coords
            int g_nx0 = nx0 + cluster[0].g.x;
            int g_ny0 = ny0 + cluster[0].g.y;

            // blue component normal coords
            int b_nx0 = nx0 + cluster[0].b.x;
            int b_ny0 = ny0 + cluster[0].b.y;

            // clip coords
            r_nx0 = clip[r_nx0];
            r_ny0 = clip[r_ny0];
            g_nx0 = clip[g_nx0];
            g_ny0 = clip[g_ny0];
            b_nx0 = clip[b_nx0];
            b_ny0 = clip[b_ny0];

            // read components 
            uchar r = light[ r_nx0 + (r_ny0<<8) ];
            uchar g = light[ g_nx0 + (g_ny0<<8) ];
            uchar b = light[ b_nx0 + (b_ny0<<8) ];

            // store color
            uint *image=(uint*)dest;
            *image=RGB32(r,g,b);
                        
            // next
            offset++;
            dest+=4;
        }

        // next line
        dest+=pitch-1024;
    }
}







#ifdef __WIN32__

int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{
    // initialize ptc (win32)
    PTC ptc(320,200);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("could not initialize");
        return 1;
    }

    // set title
    ptc.SetTitle("ptc logo");

#else

int main(int argc,char *argv[])
{
    // initialize ptc (dos)
    PTC ptc(320,200,argc,argv);
    if (!ptc.ok())
    {
        // fallback to virtual 32bit
        if (!ptc.Init(320,200))
        {
            // failure
            ptc.Error("could not initialize");
            return 1;
        }
    }

#endif

    // create main drawing surface
    Surface surface(ptc,320,200,ARGB8888);
                
    // setup bump buffer
    static uchar bump_buffer[256*120*2];
    if (!InitBump(bump_buffer))
    {
        // failure
        ptc.Error("could not load bump map");
        return 0;
    }

    // create light surface (unbound)
    Surface light(256,256,GREY8,SYSTEM,TOPDOWN,0);
    void *buffer=light.Lock();
    if (!buffer)
    {
        // failure
        ptc.Error("could not lock light buffer");
        return 1;
    }
    InitLight((char*)buffer);
    light.Unlock();

    // setup clip array
    static uchar clip[256*8];
    memset(clip,0,256*8);
    for (int i=0; i<256; i++) clip[256*4-128+i]=i;

    // setup clusters
    CLUSTER cluster[1];
    cluster[0].cx = (float) ( 128 + random(20) - 10 );
    cluster[0].cy = (float) (  60 + random(20) - 10 );

    // setup time
    float t  = 0.00f;
    float dt = 0.04f;      // adjust this if too fast or slow...

    // main loop
    while (t<8.2)
    {
        // exit on keypress
        if (ptc.kbhit()) return 0;
        
        // motion parameters
        float p = t * 0.6f;
        float q = (float)pow(p,1.60);

        // setup cluster ellipse coefficients
        cluster[0].width  = 120 + 225 * (float)pow( ( cos(q*0.4 + 0.14) + 1.0 ) / 2.0 , 0.7);
        cluster[0].height = 100 + 175 * (float)pow( ( cos(q*0.4 + 0.14) + 1.0 ) / 2.0 , 0.7);

        // setup point position 
        cluster[0].x = cluster[0].cx + cluster[0].width  * (float)cos(q*1.2 + 0.14);
        cluster[0].y = cluster[0].cy + cluster[0].height * (float)sin(q*1.2 + 0.14);

        // cluster light displacements
        cluster[0].r.x = 44 * (float)sin(p*1.5 + 1.5);
        cluster[0].r.y = 28 * (float)sin(p*1.5 + 1.5);
        cluster[0].g.x = 42 * (float)sin(p*2.2 + 3.5);
        cluster[0].g.y = 32 * (float)sin(p*2.2 + 3.5);
        cluster[0].b.x = 39 * (float)sin(p*1.2 + 0.6);
        cluster[0].b.y = 31 * (float)sin(p*1.2 + 0.6);

        // cluster -> fast cluster
        FASTCLUSTER fast[1];
        fast[0].x   = (int)cluster[0].x;
        fast[0].y   = (int)cluster[0].y;
        fast[0].r.x = (int)cluster[0].r.x;
        fast[0].r.y = (int)cluster[0].r.y;
        fast[0].g.x = (int)cluster[0].g.x;
        fast[0].g.y = (int)cluster[0].g.y;
        fast[0].b.x = (int)cluster[0].b.x;
        fast[0].b.y = (int)cluster[0].b.y;

        // lock surfaces
        uchar *surface_buffer=(uchar*)surface.Lock();
        uchar *light_buffer=(uchar*)light.Lock();

        // draw bump logo to surface
        DrawLogo(fast,surface_buffer,surface.GetPitch(),bump_buffer,light_buffer,clip+256*4-128);

        // unlock surfaces
        surface.Unlock();
        light.Unlock();

        // update display
        surface.Update();

        // slide off screen at end...
        if (t>7.3)
        {
            cluster[0].cx-=dt*30;
            cluster[0].cy+=dt*195;
        }

        // time
        t+=dt;
    }
    return 0;
}
