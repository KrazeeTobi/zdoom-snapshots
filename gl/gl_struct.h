#ifndef __GL_STRUCT
#define __GL_STRUCT

#include "doomtype.h"
#include "v_palette.h"
#include "tarray.h"
#include "gl_values.h"

typedef struct vertex_s vertex_t;
extern DWORD gl_boomcolormap;
extern DWORD gl_fixedcolormap;

struct GL_RECT
{
	float left,top;
	float width,height;


	void Offset(float xofs,float yofs)
	{
		left+=xofs;
		top+=yofs;
	}
	void Scale(float xfac,float yfac)
	{
		left*=xfac;
		width*=xfac;
		top*=yfac;
		height*=yfac;
	}
	void Scale(fixed_t xfac,fixed_t yfac)
	{
		Scale(xfac/(float)FRACUNIT,yfac/(float)FRACUNIT);
	}
};


struct GL_IRECT
{
	int left,top;
	int width,height;


	void Offset(int xofs,int yofs)
	{
		left+=xofs;
		top+=yofs;
	}
};


  // for internal use
struct FColormap
{
	PalEntry		LightColor;		// a is saturation (0 full, 31=b/w, other=custom colormap)
	PalEntry		FadeColor;		// a is fadedensity>>1

	void Clear()
	{
		LightColor=0xffffff;
		FadeColor=0;
	}

	void ClearColor()
	{
		LightColor.r=LightColor.g=LightColor.b=0xff;
	}


	void GetFixedColormap()
	{
		Clear();
		LightColor.a = gl_fixedcolormap<CM_LIMIT? gl_fixedcolormap:CM_DEFAULT;
	}

	FColormap & operator=(FDynamicColormap * from)
	{
		LightColor = from->Color;
		LightColor.a = gl_boomcolormap? gl_boomcolormap : from->Desaturate>>3;
		FadeColor = from->Fade;
		return * this;
	}
};

struct GLVertex
{
	float x,y,z;	// world coordinates
	float u,v;		// texture coordinates
	vertex_t * vt;	// real vertex
};

typedef struct
{
	float x1,x2;
	float z1,z2;
} GLSeg;


struct texcoord
{
	float u,v;
};


struct GLSkyInfo
{
	float x_offset[2];
	float y_offset;		// doubleskies don't have a y-offset
	FGLTexture * texture[2];
	int skytexno1;
	bool mirrored;
	bool doublesky;
	PalEntry fadecolor;	// if this isn't made part of the dome things will become more complicated when sky fog is used.

	bool operator==(const GLSkyInfo & inf)
	{
		return !memcmp(this, &inf, sizeof(*this));
	}
	bool operator!=(const GLSkyInfo & inf)
	{
		return !!memcmp(this, &inf, sizeof(*this));
	}
};

struct GLSectorStackInfo
{
	fixed_t deltax;
	fixed_t deltay;
	fixed_t deltaz;
	bool isupper;	
};


extern TArray<GLVertex> gl_vertices;


#endif