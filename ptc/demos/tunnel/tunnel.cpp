////////////////////////////////////////////
// Free-Directional Tunnel PTC example    //
// Copyright (c) 1998 by BLACKAXE / KoLOr //
// Adapted for PTC by Gaffer/Xtatic 1998  //
////////////////////////////////////////////

#include "ptc.h"
#include "matrix.h"
#include "vector.h"
#include "colour.h"







class Tunnel
{
    public:

        // setup
        Tunnel();
        Tunnel(char texture[],float radius);
        ~Tunnel();

        // update tunnel
        int Update(float dt);

        // draw tunnel
        int Draw(Surface &surface);

        // status
        int ok();

    protected:

        // internals
        Matrix CalculateRotation();
        void InitFrame();
        void InterpolateBlock(int x,int y,int pitch,ushort *start,uchar *texture);
        void Defaults();

        // data
        float AngleX;
        float AngleY;
        float AngleZ;
        float Radius;
        float Position;

        // grid item structure
        struct GridItem 
        {
            long u;     // texel u coordinate
            long v;     // texel v coordinate
            long c;     // texel lighting
        };

        // grid array
        GridItem Grid[41][26];

        // texture surface
        Surface *Texture;

        // texture shade table
        ColourRamp *ShadeTable;

        // status
        int Status;
};







Tunnel::Tunnel()
{
    // defaults
    Defaults();
}


Tunnel::Tunnel(char texture[],float radius)
{
    // defaults
    Defaults();

    // setup radius
    Radius=radius;

    // load texture map to linear surface block
    Texture=new Surface("texture.tga",SYSTEM,TOPDOWN,0,LINEAR);
    if (!Texture->ok()) return;

    // check texture attributes: must be 256x256x8bit
    int width=Texture->GetWidth();
    int height=Texture->GetHeight();
    FORMAT format=Texture->GetFormat();
    if (width!=256 || height!=256 || format.id!=INDEX8) return;

    // get texture palette
    Palette *palette=Texture->GetPalette();
    if (!palette) return;

    // setup shade table
    ShadeTable=new ColourRamp(*palette);
    
    // success
    Status=1;
}


Tunnel::~Tunnel()
{
    // free objects
    delete Texture;
    delete ShadeTable;
}


int Tunnel::Update(float dt)
{
    // check status
    if (!Status) return 0;

    // todo: handle dt
    if (dt);

    // update motion
    Position -= 50.0f * dt;
    if (Position<-50.0) 
    {
        // rotate view
        AngleX += 1.6f * dt;
        AngleY -= 2.5f * dt;
        AngleZ += 0.9f * dt;
    }

    // done
    return 1;
}


int Tunnel::Draw(Surface &surface)
{
    // check status
    if (!Status) return 0;

    // initialize
    InitFrame();
  
    // lock surface
    uchar *buffer=(uchar*)surface.Lock();
    if (!buffer) return 0;
    
    // lock texture
    uchar *texture=(uchar*)Texture->Lock();
    if (!texture) return 0;

    // get surface pitch
    int pitch = surface.GetPitch();

    // interpolate grid blocks to surface
    for(int y=0; y<25; y++) 
    {
        for(int x=0; x<40; x++)
        {
            // calculate address of topleft block corner
            ushort *block = (ushort*)(buffer + (y*8)*pitch + x*8*2);

            // render interpolated 8x8 block
            InterpolateBlock(x,y,pitch,block,texture);
        }
    }   

    // unlock texture
    Texture->Unlock();

    // unlock surface
    surface.Unlock();

    // done
    return 1;
}


int Tunnel::ok()
{
    // status
    return Status;
}


Matrix Tunnel::CalculateRotation()
{    
    // setup x rotation matrix
    Matrix XRotation;
    XRotation.Set(1,1, cos(AngleX));
    XRotation.Set(1,2, sin(AngleX));
    XRotation.Set(2,1,-sin(AngleX));
    XRotation.Set(2,2, cos(AngleX));

    // setup y rotation matrix
    Matrix YRotation;
    YRotation.Set(0,0, cos(AngleY));
    YRotation.Set(0,2,-sin(AngleY));
    YRotation.Set(2,0, sin(AngleY));
    YRotation.Set(2,2, cos(AngleY));

    // setup z rotation matrix
    Matrix ZRotation;
    ZRotation.Set(0,0, cos(AngleZ));
    ZRotation.Set(0,1, sin(AngleZ));
    ZRotation.Set(1,0,-sin(AngleZ));
    ZRotation.Set(1,1, cos(AngleZ));

    // concatenate rotations
    return (XRotation*YRotation*ZRotation);    
}



void Tunnel::InitFrame()
{
    // setup rotation matrix
    Matrix RotationMatrix = CalculateRotation();

    // setup direction vectors
    Vector Dir1(-160, -100, 128),
           Dir2( 160, -100, 128),
           Dir3(-160,  100, 128),
           Dir4( 160,  100, 128);
    Dir1 *= RotationMatrix; Dir1.Normalize();
    Dir2 *= RotationMatrix; Dir2.Normalize();
    Dir3 *= RotationMatrix; Dir3.Normalize();
    Dir4 *= RotationMatrix; Dir4.Normalize();

    // vectors needed to calculate vertical interpolation of the Direction vector
    Vector DeltaLeftY  = (1.0 / 26.0)*(Dir3 - Dir1);
    Vector DeltaRightY = (1.0 / 26.0)*(Dir4 - Dir2);   
    Vector StartLeftY  = Dir1;
    Vector StartRightY = Dir2;
    Vector Origin(0,0,-128);

    // grid y loop
    for(int i=0; i<26; i++) 
    {
        // setup vectors
        Vector DeltaX = (1.0 / 41.0)*(StartRightY - StartLeftY);
        Vector Direction = StartLeftY;

        // grid x loop
        for(int j=0; j < 41; j++) 
        {
            // calculate intersection with cylinder through grid (x,y)
            float a = Direction.GetX()*Direction.GetX() + Direction.GetY()*Direction.GetY();
            float b = 2*(Origin.GetX()*Direction.GetX() + Origin.GetY()*Direction.GetY());
            float c = Origin.GetX()*Origin.GetX() + Origin.GetY()*Origin.GetY() - Radius*Radius;
            float delta = b*b - 4*a*c;
            float t = (-b - sqrt(delta))/(2*a);
            Vector Intersection = Origin + t*Direction;

            // calculate mapping and depth cue    
            Grid[j][i].u = (int)(fabs(Intersection.GetZ()+Position*8)*0.2*256.0);
            Grid[j][i].v = (int)(fabs(atan2(Intersection.GetY(), Intersection.GetX())*65536/PI));
            Grid[j][i].c = 32768 - (int)(fabs((Intersection.GetZ() - Origin.GetZ()))*0.08*256.0);
            if (Grid[j][i].c < 0) Grid[j][i].c = 0;

            // interpolate direction along x
            Direction = Direction + DeltaX;        
        }
        
        // interpolate direction along y
        StartLeftY  = StartLeftY  + DeltaLeftY;
        StartRightY = StartRightY + DeltaRightY;
    }        
}


void Tunnel::InterpolateBlock(int x,int y,int pitch,ushort *start,uchar *texture)
{ 
    // setup line pointer
    uchar *line=(uchar*)start;
    
    // deltas and starts for left side
    int ydul = (Grid[x][y+1].u - Grid[x][y].u) << 5;
    int ydvl = (Grid[x][y+1].v - Grid[x][y].v) << 5;
    int ydcl = (Grid[x][y+1].c - Grid[x][y].c) << 5;
    int yul  = Grid[x][y].u << 8;
    int yvl  = Grid[x][y].v << 8;
    int ycl  = Grid[x][y].c << 8;
   
    // deltas and starts for right side
    int ydur = (Grid[x+1][y+1].u - Grid[x+1][y].u) << 5;
    int ydvr = (Grid[x+1][y+1].v - Grid[x+1][y].v) << 5;
    int ydcr = (Grid[x+1][y+1].c - Grid[x+1][y].c) << 5;
    int yur  = Grid[x+1][y].u << 8;
    int yvr  = Grid[x+1][y].v << 8;
    int ycr  = Grid[x+1][y].c << 8;

    // loop 8 lines
    for(int i=0; i < 8; i++) 
    { 
        // deltas and start values for the lines
        int xdu = (yur - yul) >> 3;
        int xu  = yul;
        int xdv = (yvr - yvl) >> 3;
        int xv  = yvl;
        int xdc = (ycr - ycl) >> 3;
        int xc  = ycl;

        // loop 8 pixels
        ushort *pixel=(ushort*)line;
        for(int j=0; j < 8; j++) 
        {
            // lookup shaded texel
            *pixel++ = ShadeTable->GetColour(texture[((xv >> 8) & 0xff00)+((xu & 0xff0000) >> 16)], xc >> 16);
            
            // next pixel
            xu += xdu;
            xv += xdv;
            xc += xdc;
        }
  
        // next line
        line+=pitch;
                       
        // interpolate
        yul += ydul;
        yvl += ydvl;
        ycl += ydcl;
        yur += ydur;
        yvr += ydvr;
        ycr += ydcr;
    }
}


void Tunnel::Defaults()
{
    // defaults
    AngleX=0.0f;
    AngleY=0.0f;
    AngleZ=0.0f;
    Radius=0.0f;
    Position=0.0f;
    Texture=NULL;
    ShadeTable=NULL;
    Status=0;
}







#ifdef __WIN32__

int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{
    // initialize ptc (win32)
    PTC ptc(320,200,VIRTUAL16);
    if (!ptc.ok())
    {
        // failure
        ptc.Error("could not initialize");
        return 1;
    }

    // set title
    ptc.SetTitle("tunnel");

#else

int main(int argc,char *argv[])
{
    // initialize ptc (dos)
    PTC ptc(320,200,argc,argv);
    if (!ptc.ok())
    {
        // fallback to virtual 16 bit
        if (!ptc.Init(320,200,VIRTUAL16))
        {
            // failure
            ptc.Error("could not initialize");
            return 1;
        }
    }

#endif

    // create surface
    Surface surface(ptc,320,200,RGB565);
    if (!surface.ok())
    {
        ptc.Error("could not create surface");
        return 1;
    }

    // create tunnel
    Tunnel tunnel("texture.tga",255.0f);
    if (!tunnel.ok())
    {
        // failure
        ptc.Error("could not create tunnel");
        return 1;
    }

    // time
    float t=0.0f;
    float dt=0.01f;

    // main loop
    while (!ptc.kbhit())
    {
        // update tunnel
        tunnel.Update(dt);

        // draw tunnel;
        tunnel.Draw(surface);
        
        // update display
        surface.Update();

        // time
        t+=dt;
    }
    
    // close ptc
    ptc.Close();

    // print greets
    cout << "Free-Directional Tunnel Example" << endl;
    cout << "Code by BlackAxe / KoLOr" << endl;
    cout << "Texture by RayTrayza / KoLOr" << endl << endl;
    cout << "Greets must fly out to: Raytrayza, Noize, Shiva, Siriuz, Salice, LordChaos, " << endl;
    cout << "                        DrZeng, Climax, KB, Cirion, HooDoo, mcc, Brioche, Magic" << endl;
    cout << "                        Lure, Wog, Assign, Kyp," << endl;
    cout << "                        and all the dudes i forgot right now as always" << endl;   
    return 0;    
}
