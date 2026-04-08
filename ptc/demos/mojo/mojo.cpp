// nasty code by alex "statix" evans for ptc. (c) copyright alex evans 1998
// time... 02.00 am on 13/1/98.
// have fun
// it's my take on some classic light mask effect
// it's raytracing through properly modelled fog with occlusion, multiple
// shadow rays cast back to the light for each pixel ray, and erm, its
// s l o w... but it looks nice don't it?

// oh and fresnel fall off... or something

// UNTESTED! ok?

// define inv for interesting fx (not)

#include "mojo.h"


#define SC 12
//#define INV

#define MINSEGSIZE 2.5f
#define NSEG 5

UBYTE *MaskMap;

class Ray
{
public:
    FVector mPosn;
    FVector mDir;
    Ray(const FVector &p,const FVector &d) {mPosn=p;mDir=d;mDir.Normalise();};
};

class VLight
{
public:
    float mAng;
    FVector mPosn;
    FVector mTarget;
    FMatrix mAxis;
    FVector mCol;

    FVector p,p2,d;    // temp space

    VLight(const FVector &col);
    void Move(const FVector &p) {mPosn=p;Update();};
    void MoveT(const FVector &p) {mTarget=p;Update();};
    void Update() {mAxis.Row[2]=(mTarget-mPosn);mAxis.Normalise();};
    FVector Light(const Ray *ray);
    float CalcLight(float t);

};



VLight::VLight(const FVector &col)
{
    mCol=col*0.9f;
    mAng=2.8f;
    mPosn=FVector(0.0f,0.0f,20.0f);
    mTarget=FVector(0.0f,0.0f,0.1f);
    mAxis.MakeID();
    Update();
}

float VLight::CalcLight(float t)
{
    // trace line to bitmap from mPosn to p2
    if (!((mPosn.Z>0)^(p2.Z>0)))
    {
	// fresnel fall off...
	return p.Z/p.MagnitudeSq();
    }
    float f=-(mPosn.Z)/(p2.Z-mPosn.Z);
    int x=160+int(SC*((p2.X-mPosn.X)*f+mPosn.X));
#ifndef INV
    if (x<0 || x>319) return p.Z/p.MagnitudeSq();;
    int y=100+int(SC*((p2.Y-mPosn.Y)*f+mPosn.Y));
    if (y<0 || y>199) return p.Z/p.MagnitudeSq();;
    int c=MaskMap[y*320+x];
#else
    if (x<0 || x>319) return 0;
    int y=100+int(SC*((p2.Y-mPosn.Y)*f+mPosn.Y));
    if (y<0 || y>199) return 0;
    int c=255-MaskMap[y*320+x];
#endif
    if (c==0) return 0;
    return (c*(1/255.0f))*p.Z/p.MagnitudeSq();

}



FVector VLight::Light(const Ray *ray)
{
    float f=0;

    p2=ray->mPosn;
    p=mAxis*(ray->mPosn-mPosn);
    d=mAxis*ray->mDir;
    float A=(d.X*d.X+d.Y*d.Y);
    float B=2.f*(d.X*p.X+d.Y*p.Y)-mAng*(d.Z);
    float C=(p.X*p.X+p.Y*p.Y)-mAng*(p.Z);
    float D=B*B-4*A*C;
    if (D<=0) return FVector(0,0,0);
    D=float(sqrt(D));
    A*=2;
    float t1=(-B-D)/A;
    float t2=(-B+D)/A;
    int frc=255;
    float t3=-ray->mPosn.Z/ray->mDir.Z;
    if (t2<=0) return FVector(0,0,0);
    if (t1<0) t1=0;
    if (t3>0)
    {
	// clip to bitmap plane
	FVector pp=ray->mPosn+ray->mDir*t3;
	int x=160+int(SC*pp.X);
#ifndef INV
	if (x>=0 && x<=319)
	{
	    int y=100+int(SC*pp.Y);
	    if (y>=0 && y<=199)
	    {
		//return FVector(0,0,1);
		frc=MaskMap[y*320+x];
		if (frc<1)
		{
		    if (t1>t3) t1=t3;
		    if (t2>t3) t2=t3;
		}
	    }
	    else t3=t2;
	} else t3=t2;
#else
	if (x>=0 && x<=319)
	{
	    int y=100+int(SC*pp.Y);
	    if (y>=0 && y<=199 && MaskMap[y*320+x]<128)
	    {
		t3=t2;
	    }
	}
	if (t1>t3) t1=t3;
	if (t2>t3) t2=t3;
#endif
    }
    if (t1>=t2) return FVector(0,0,0);
    float l1,l2;
    float fr=frc/255.0f;
    l1=CalcLight(t1);if (t1>t3) l1*=fr;
    int q=NSEG;
    float t=t1;
    float h=(t2-t1)/NSEG;
    if (h<MINSEGSIZE) h=MINSEGSIZE;
    while (t<t3 && q>0 && t<t2)
    {
	t+=h;
	if (t>t2) {h-=t2-t;t=t2;q=0;} else q--;
	float h=(t-t1);
	p+=d*h;
	p2+=ray->mDir*h;
	float l2=CalcLight(t);
	f+=(l1+l2)*h;
	l1=l2;
	t1=t;
    }
    while (q>0 && t<t2)
    {
	t+=h;
	if (t>t2) {h-=t2-t;t=t2;q=0;} else q--;
	p+=d*h;
	p2+=ray->mDir*h;
	float l2=CalcLight(t);if (t>t3) l2*=fr;
	f+=(l1+l2)*h;
	l1=l2;
	t1=t;
    }
    return mCol*f;
}

inline int CLIPC(float f)
{
    int a=int(f*255.0);
    return (a<0)?0:(a>255)?255:a;
}

UWORD frandtab[65536];

void initfrand()
{

    memset(frandtab,0,sizeof(frandtab));
    int s=1;
    for (int c1=1;c1<65536;c1++)
    {
	frandtab[c1]=s;
	s=(((s>>4)^(s>>13)^(s>>15))&1)+(s<<1);
    }
}

int inline frand()
{
    static UWORD seed=54;
    return frandtab[seed++];
}

float vlightt;


void VLightPart(PTC *ptc, Surface *surface)
{
    initfrand();
    VLight *vl=new VLight(FVector(0.1f,0.4f,1.0f));
    VLight *vl2=new VLight(FVector(1.0f,0.5f,0.2f));
    vl->Move(FVector(0,0,20));
    vl2->Move(FVector(0,6,30));

    FVector camposn(7,0.5,-10);
    FMatrix camaxis;
    camaxis.MakeID();
    camaxis.Row[2]=FVector(0,0,0)-camposn;camaxis.Normalise();
    float camf=100.f;

    MaskMap=new UBYTE[320*200];
    memset(MaskMap,0,320*200);

	/////////////////
	// load picture.raw

    File file("picture.raw",File::READ|File::BINARY);
    file.read(MaskMap,320*200);
    file.close();

    // build the order of the squares
    int c1=0;
    int order[10*19][2];
    for (c1=0;c1<10*19;c1++)
    {
	order[c1][0]=(c1%19);
	order[c1][1]=(c1/19)+1;
    }
    // swap them around
    for (c1=0;c1<10000;c1++)
    {
	int t;
	int c2=rand()%190;
	int c3=(rand()+1+c2)%190;
	t=order[c2][0];order[c2][0]=order[c3][0];order[c3][0]=t;
	t=order[c2][1];order[c2][1]=order[c3][1];order[c3][1]=t;
    }

    vlightt=0;


	///////////////////
	// main loop

    while (!ptc->kbhit())
    {
        // lock surface
    	char *screenbuf=(char*)surface->Lock();
        if (!screenbuf) return;

        // get surface pitch
    	int pitch=surface->GetPitch();

				vlightt+=0.75;   // this is the speed set! machine indep? haha!

	float t=13.0f-0.1822f*vlightt;
	float cz=1.0f-0.01f*vlightt;
	//vl->Move(FVector(sin(t)*5,cos(t*-0.675+4543)*5,15));
	//vl->MoveT(FVector(0,0,-15));
	vl->Move(FVector(t,0,22));
	vl2->Move(FVector(-t,-7,28));

	camposn=FVector(cz*4.0f+9.0f,cz,-t/7-13);
	camaxis.Row[2]=FVector(0,0,0)-camposn;camaxis.Normalise();


	UBYTE idx[200/16][320/16];
	memset(idx,25,sizeof(idx));

	ULONG oc,c;
	// swap them around
	for (c1=0;c1<100;c1++)
	{
	    int t;
	    int c2=rand()%190;
	    int c3=(rand()+1+c2)%190;
	    t=order[c2][0];order[c2][0]=order[c3][0];order[c3][0]=t;
	    t=order[c2][1];order[c2][1]=order[c3][1];order[c3][1]=t;
	}
	for (int zz=0;zz<190;zz++)
	{
	    int xx=order[zz][0];
	    int yy=order[zz][1];
	    int i=0;

	    int c2=idx[yy][xx]>>1;
	    for (c1=0;c1<c2;c1++)
	    {
		int a=frand()&255;
		int x=xx*16+(a&15)+6+4;
		int y=yy*16+(a>>4)+6;

		FVector col(0,0,0);
		Ray ray(camposn,camaxis.Row[2]*camf+camaxis.Row[0]*float(x-160)+camaxis.Row[1]*float(y-100));
		col+=vl->Light(&ray);
		col+=vl2->Light(&ray);

		c=(CLIPC(col.R)<<16)+(CLIPC(col.G)<<8)+(CLIPC(col.B));
				char *d=screenbuf+x*4+y*pitch;
		i+=abs((c&255)-(d[321]&255))+abs((c>>16)-(d[321]>>16));
		if (c1) i+=abs((c&255)-(oc&255))+abs((c>>16)-(oc>>16));
		oc=c;


    #define DARKEN(x) ((x>>1)&0x7f7f7f)
		ULONG c2=DARKEN(c);
		((ULONG*)d)[1]=DARKEN(((ULONG*)d)[1])+c2;
		((ULONG*)d)[2]=DARKEN(((ULONG*)d)[2])+c2;
		d+=pitch;
		((ULONG*)d)[0]=DARKEN(((ULONG*)d)[0])+c2;
		((ULONG*)d)[1]=c;
		((ULONG*)d)[2]=c;
		((ULONG*)d)[3]=DARKEN(((ULONG*)d)[3])+c2;         
		d+=pitch;
		((ULONG*)d)[0]=DARKEN(((ULONG*)d)[0])+c2;
		((ULONG*)d)[1]=c;
		((ULONG*)d)[2]=c;
		((ULONG*)d)[3]=DARKEN(((ULONG*)d)[3])+c2;         
		d+=pitch;
		((ULONG*)d)[1]=DARKEN(((ULONG*)d)[1])+c2;
		((ULONG*)d)[2]=DARKEN(((ULONG*)d)[2])+c2;




	    }
	    i*=5;
	    i/=3*idx[yy][xx];
	    if (i<2) i=2;
	    if (i>256) i=256;
	    idx[yy][xx]=i;

        // unlock surface
        surface->Unlock();

	    if ((zz%95)==0)
			{
				// copy up to the screen
				surface->Update();
			}
	}

	}

    delete [] MaskMap;
    delete vl;
    delete vl2;

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
    ptc.SetTitle("mojo by statix");

#else

int main(int argc,char *argv[])
{
    // initialize ptc (dos)
    PTC ptc(320,200,argc,argv);
    if (!ptc.ok())
    {
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
    if (!surface.ok())
    {
        // failure
        ptc.Error("could not create surface");
        return 1;
    }

     VLightPart(&ptc,&surface);
     ptc.Close();
     printf("mojo by alex \"statix\" evans\nto be used as an example of bad coding and good ptc\nno responsibility taken for this!\nenjoy ptc! it's great\n\n-statix 13/1/98");
     return 0;

}
