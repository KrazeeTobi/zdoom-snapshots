////////////////////
// bitblt example //
////////////////////
#include "ptc.h"





// image tile
struct TILE
{
    float x;               // current x position
    float y;               // current y position
    float cx;              // central x position
    float cy;              // central y position
    float dx;              // x movement phase shift
    float dy;              // y movement phase shift
    float sx;              // x movement amplitude multiplier
    float sy;              // y movement applitude multiplier
    float tx;              // x movement time multiplier
    float ty;              // y movement time multiplier
    Surface *surface;      // tile surface
};





int main(int argc,char *argv[])
{
    // initialize from command line (ie. "bitblt RGB565")
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
    
    // load image
    Surface image(ptc,"image.tga");
    image.Convert(ARGB8888);
    if (!image.ok())
    {   
        // failure
        ptc.Error("could not load image");
        return 1;
    }

    // locals
    int i=0;
    int x=0;
    int y=0;

    // initialize surface tiles
    TILE Tile[40];
    for (i=0,y=0; y<200; y+=40)
    {
        for (x=0; x<320; x+=40)
        {
            // create tile surface
            Tile[i].surface=new Surface(ptc,40,40,ARGB8888);
            if (!Tile[i].surface || !Tile[i].surface->ok())
            {
                // failure
                ptc.Error("failed to create tile surface");
                return 1;
            }

            // bitblt section of image to tile surface
            image.BitBlt(*Tile[i].surface,RECTANGLE(x,y,x+40,y+40),RECTANGLE(0,0,40,40));
        
            // setup tile parameters
            Tile[i].cx = x;
            Tile[i].cy = y;
            Tile[i].dx = random(1000) / 200.0f + 50.0f;
            Tile[i].dy = random(1000) / 200.0f + 50.0f;
            Tile[i].sx = random(1000) / 500.0f + 0.5f;
            Tile[i].sy = random(1000) / 700.0f + 0.5f;
            Tile[i].tx = random(1000) / 500.0f + 1.0f;
            Tile[i].ty = random(1000) / 500.0f + 1.0f;

            // next tile
            i++;
        }
    }

    // create drawing surface
    Surface surface(ptc,320,200,ARGB8888);
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not create surface");
        return 1;
    }

    // time
    float t  = 0.0f;
    float dt = 0.01;      // adjust for speed

    // main loop
    while (!ptc.kbhit())
    {
        // update tile positions
        for (i=0; i<40; i++)
        {
            // tile energy
            float energy = 50.0;
        
            // calculate current tile positions
            Tile[i].x = Tile[i].cx + ( energy * Tile[i].sx * sin( t*Tile[i].tx + Tile[i].dx ) ); 
            Tile[i].y = Tile[i].cy + ( energy * Tile[i].sy * sin( t*Tile[i].ty + Tile[i].dy ) ); 

            // draw tile on surface
            Tile[i].surface->BitBlt(surface,(int)Tile[i].x,(int)Tile[i].y);
        }

        // update to display
        surface.Update();

        // get surface data
        FORMAT format=surface.GetFormat();
        int bytes=surface.GetWidth()*format.bytes;
        int pitch=surface.GetPitch();
        int height=surface.GetHeight();
        
        // lock surface
        char *buffer=(char*)surface.Lock();
        if (!buffer) 
        {
            // failure
            ptc.Error("could not lock surface");
            return 1;
        }
        
        // clear surface to black        
        for (y=0; y<height; y++) memset(buffer+y*pitch,0,bytes);
        
        // unlock surface
        surface.Unlock();

        // time
        t+=dt;
    }
    return 0;
}
