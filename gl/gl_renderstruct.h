/*
** gl_renderstruct.h
** Defines the structures used to store data to be rendered
**
**---------------------------------------------------------------------------
** Copyright 2002-2005 Christoph Oelckers
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/


#ifndef __GL_RENDERSTRUCT_H
#define __GL_RENDERSTRUCT_H

#include "r_local.h"
#include "tarray.h"
#include "templates.h"

#include "gl/gl_values.h"
#include "gl/gl_struct.h"

struct F3DFloor;
struct model_t;
class FGLTexture;


int GetFloorLight (const sector_t *sec);
int GetCeilingLight (const sector_t *sec);


struct GLDrawList;
class ADynamicLight;

struct GLSectorPlane
{
	int texture;
	secplane_t plane;
	fixed_t texheight;
	fixed_t xoffs,  yoffs;
	fixed_t	xscale, yscale;
	angle_t	angle;

	void GetFromSector(sector_t * sec, bool ceiling)
	{
		if (ceiling)
		{
			texture = sec->ceilingpic;
			plane = sec->ceilingplane;
			texheight = sec->ceilingtexz;
			xoffs = sec->ceiling_xoffs;
			yoffs = sec->ceiling_yoffs + sec->base_ceiling_yoffs;
			xscale = sec->ceiling_xscale;
			yscale = sec->ceiling_yscale;
			angle = sec->ceiling_angle + sec->base_ceiling_angle;
		}
		else
		{
			texture = sec->floorpic;
			plane = sec->floorplane;
			texheight = sec->floortexz;
			xoffs = sec->floor_xoffs;
			yoffs = sec->floor_yoffs + sec->base_floor_yoffs;
			xscale = sec->floor_xscale;
			yscale = sec->floor_yscale;
			angle = sec->floor_angle + sec->base_floor_angle;
		}
	}
};

struct GLHorizonInfo
{
	GLSectorPlane plane;
	int lightlevel;
	FColormap colormap;
};



class GLWall
{
	enum
	{
		GLWF_CLAMPX,
		GLWF_CLAMPY,
		GLWF_SKYHACK
	};

public:
	friend struct GLDrawList;
	friend class GLPortal;

	GLSeg glseg;
	vertex_t * vertexes[2];				// required for polygon splitting
	float ytop[2],ybottom[2];
	texcoord uplft, uprgt, lolft, lorgt;
	float alpha;
	FGLTexture *gltexture;


	FColormap Colormap;
	
	fixed_t viewdistance;
	byte lightlevel;
	byte type;
	byte RenderStyle;
	byte flags;

	union
	{
		// it's either one of them but never more!
		AActor * skybox;			// for skyboxes
		GLSkyInfo * sky;			// for normal sky
		GLHorizonInfo * horizon;	// for horizon information
		GLSectorStackInfo * stack;	// for sector stacks
		secplane_t * planemirror;	// for plane mirrors
	};


	int topflat,bottomflat;

	// these are not the same as ytop and ybottom!!!
	float yceil[2];
	float yfloor[2];

public:
	seg_t * seg;			// this gives the easiest access to all other structs involved
	subsector_t * sub;		// For polyobjects
private:

	void PutWall(bool translucent);

	bool PrepareLight(texcoord * tcs, ADynamicLight * light);
	void DoRenderWall(bool textured, float * color2, ADynamicLight * light=NULL);
	void DoRenderGlowingPoly(bool textured=true, bool dolight=true, ADynamicLight * light=NULL);
	int Intersection(GL_RECT * rc,GLWall * result);

	void FloodPlane(int pass);

	void MirrorPlane(secplane_t * plane, bool ceiling);
	void SkyTexture(int sky1,ASkyViewpoint * skyboxx, bool ceiling);

	void SkyNormal(sector_t * fs,vertex_t * v1,vertex_t * v2);
	void SkyTop(seg_t * seg,sector_t * fs,sector_t * bs,vertex_t * v1,vertex_t * v2);
	void SkyBottom(seg_t * seg,sector_t * fs,sector_t * bs,vertex_t * v1,vertex_t * v2);
	void SplitWall(sector_t * frontsector, bool translucent);
	bool RenderMirror();
	void LightPass();
	void RenderTwoSidedWall(int pass);
	void RenderOneSidedWall(int pass);
	void SetHorizon(vertex_t * ul, vertex_t * ur, vertex_t * ll, vertex_t * lr);
	bool DoHorizon(seg_t * seg,sector_t * fs, vertex_t * v1,vertex_t * v2);

	bool SetWallCoordinates(seg_t * seg, int ceilingrefheight,
							int topleft,int topright, int bottomleft,int bottomright, int texoffset);

	void DoTexture(int type,seg_t * seg,int peg,
						   int ceilingrefheight,int floorrefheight,
						   int CeilingHeightstart,int CeilingHeightend,
						   int FloorHeightstart,int FloorHeightend,
						   int v_offset);

	void DoMidTexture(seg_t * seg, bool drawfogsheet,
					  sector_t * realfront, sector_t * realback,
					  fixed_t fch1, fixed_t fch2, fixed_t ffh1, fixed_t ffh2,
					  fixed_t bch1, fixed_t bch2, fixed_t bfh1, fixed_t bfh2);

	void BuildFFBlock(seg_t * seg, F3DFloor * rover,
					  fixed_t ff_topleft, fixed_t ff_topright, 
					  fixed_t ff_bottomleft, fixed_t ff_bottomright);
	void InverseFloors(seg_t * seg, sector_t * frontsector,
					   fixed_t topleft, fixed_t topright, 
					   fixed_t bottomleft, fixed_t bottomright);
	void ClipFFloors(seg_t * seg, F3DFloor * ffloor, sector_t * frontsector,
					fixed_t topleft, fixed_t topright, 
					fixed_t bottomleft, fixed_t bottomright);
	void DoFFloorBlocks(seg_t * seg, sector_t * frontsector, sector_t * backsector,
					  fixed_t fch1, fixed_t fch2, fixed_t ffh1, fixed_t ffh2,
					  fixed_t bch1, fixed_t bch2, fixed_t bfh1, fixed_t bfh2);

	void RenderFogSheet();

	void DrawDecal(ADecal *actor, seg_t *seg, sector_t *frontSector, sector_t *backSector);
	void DoDrawDecals(ADecal * decal, seg_t * seg);
	void ProcessOneDecal(seg_t *seg, ADecal * decal, float leftxfrac,float rightxfrac);
	void ProcessDecals(seg_t *seg, float leftxfrac,float rightxfrac);

public:

	void Process(seg_t *seg, sector_t * frontsector, sector_t * backsector, subsector_t * polysub);
	void ProcessLowerMiniseg(seg_t *seg, sector_t * frontsector, sector_t * backsector);
	void Draw(int pass);

	float PointOnSide(float x,float y)
	{
		return (y-glseg.z1)*(glseg.x2-glseg.x1)-(x-glseg.x1)*(glseg.z2-glseg.z1);
	}

	// Lines start-end and fdiv must intersect.
	double CalcIntersectionVertex(GLWall * w2)
	{
		float ax = glseg.x1, ay=glseg.z1;
		float bx = glseg.x2, by=glseg.z2;
		float cx = w2->glseg.x1, cy=w2->glseg.z1;
		float dx = w2->glseg.x2, dy=w2->glseg.z2;
		return ((ay-cy)*(dx-cx)-(ax-cx)*(dy-cy)) / ((bx-ax)*(dy-cy)-(by-ay)*(dx-cx));
	}

};

struct gl_subsectordata;
struct FSpriteModelFrame;

class GLFlat
{
public:
	friend struct GLDrawList;

	sector_t * sector;
	gl_subsectordata * sub;	// only used for translucent planes
	float z; // the z position of the flat (height)
	FGLTexture *gltexture;

	FColormap Colormap;	// light and fog

	float alpha;
	GLSectorPlane plane;
	short lightlevel;
	bool use_fog;
	bool ceiling;
	byte renderflags;

	void DrawSubsector(gl_subsectordata * glsub);
	void DrawSubsectorLights(gl_subsectordata * glsub);

	void PutFlat(bool translucent);
	void Process(sector_t * sector, bool whichplane, bool notexture);
	void ProcessSector(sector_t * frontsector, subsector_t * sub);
	void Draw(int pass);
};


struct particle_t;

class GLSprite
{
public:
	friend struct GLDrawList;
	friend void Mod_RenderModel(GLSprite * spr, model_t * mdl, int framenumber);

	unsigned char RenderStyle;
	byte lightlevel;
	byte foglevel;
	PalEntry ThingColor;	// thing's own color
	FColormap Colormap;
	FSpriteModelFrame * modelframe;

	int translation;
	fixed_t scale;
	float x,y,z;	// needed for sorting!

	float ul,ur;
	float vt,vb;
	float x1,y1,z1;
	float x2,y2,z2;

	FGLTexture *gltexture;
	float trans;
	AActor * actor;
	particle_t * particle;

	void SplitSprite(sector_t * frontsector, bool translucent);
	void SetLowerParam();

public:

	void Draw(int pass);
	void PutSprite(bool translucent);
	void Process(AActor* thing,sector_t * sector);
	void ProcessParticle (particle_t *particle, sector_t *sector);//, int shade, int fakeside)
	void SetThingColor(PalEntry);

};


/*
struct GLPolyVertex
{
	float x,y,z;
	float u,v;
	float r,g,b,a;
};

class GLPoly
{
	int vertexstart;
	int vertexcount;

	FLightNode * lighthead;	// all lights that may affect this polygon
	Plane p;				// plane representation of this polygon

	GLPoly * left;			// This creates a tree sorted by texture
	GLPoly * right;
	GLPoly * next;			// next is the chain of polys with the same texture
	GLPoly * last;			// last points to the last element of this chain
	GLPoly * linear;		// a linear list of all polys
	GLPoly * linearlast;	// This is used to render the non-textured passes
};

*/


// unfortunately this struct must not contain pointers because
// the arrays may be reallocated!
struct GLDrawItem
{
	GLDrawItemType rendertype;
	int index;

	GLDrawItem(GLDrawItemType _rendertype,int _index) : rendertype(_rendertype),index(_index) {}
};

struct SortNode
{
	int itemindex;
	SortNode * parent;
	SortNode * next;		// unsorted successor
	SortNode * left;		// left side of this node
	SortNode * equal;		// equal to this node
	SortNode * right;		// right side of this node


	void UnlinkFromChain();
	void Link(SortNode * hook);
	void AddToEqual(SortNode * newnode);
	void AddToLeft (SortNode * newnode);
	void AddToRight(SortNode * newnode);
};

struct GLDrawList
{
//private:
	TArray<GLWall> walls;
	TArray<GLFlat> flats;
	TArray<GLSprite> sprites;
	TArray<GLDrawItem> drawitems;
	int SortNodeStart;
	SortNode * sorted;

public:
	GLDrawList()
	{
		next=NULL;
		SortNodeStart=-1;
		sorted=NULL;
	}

	~GLDrawList()
	{
		Reset();
	}

	void AddWall(GLWall * wall);
	void AddFlat(GLFlat * flat);
	void AddSprite(GLSprite * sprite);
	void Reset();
	void Sort();


	void MakeSortList();
	SortNode * FindSortPlane(SortNode * head);
	SortNode * FindSortWall(SortNode * head);
	void SortPlaneIntoPlane(SortNode * head,SortNode * sort);
	void SortWallIntoPlane(SortNode * head,SortNode * sort);
	void SortSpriteIntoPlane(SortNode * head,SortNode * sort);
	void SortWallIntoWall(SortNode * head,SortNode * sort);
	void SortSpriteIntoWall(SortNode * head,SortNode * sort);
	int CompareSprites(SortNode * a,SortNode * b);
	SortNode * SortSpriteList(SortNode * head);
	SortNode * DoSort(SortNode * head);
	void DoDraw(int pass, int index);
	void DoDrawSorted(SortNode * node);
	void DrawSorted();
	void Draw(int pass);

	GLDrawList * next;
} ;

extern GLDrawList * gl_drawlist;

void gl_StartDrawInfo(GLDrawList * di);
void gl_EndDrawInfo();



#endif