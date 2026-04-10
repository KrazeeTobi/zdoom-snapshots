#include "gl_pch.h"
/*
** gl_portal.cpp
**   Generalized portal maintenance classes for skyboxes, horizons etc.
**   Requires a stencil buffer!
**
**---------------------------------------------------------------------------
** Copyright 2004-2005 Christoph Oelckers
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

#include "p_local.h"
#include "vectors.h"
#include "gl/gl_struct.h"
#include "gl/gl_portal.h"
#include "gl/gl_clipper.h"
#include "gl/gl_glow.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "gl/gl_intern.h"
#include "gl/gl_basic.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//
// General portal handling code
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CUSTOM_CVAR(Int, r_mirror_recursions,4,CVAR_GLOBALCONFIG|CVAR_ARCHIVE)
{
	if (self<0) self=0;
	if (self>10) self=10;
}
TArray<GLPortal *> GLPortal::portals;
int GLPortal::recursion;
int GLPortal::MirrorFlag;
int GLPortal::renderdepth;

line_t * GLPortal::mirrorline;
bool	 GLPortal::inupperstack;
bool	 GLPortal::inlowerstack;
bool	 GLPortal::inskybox;

//-----------------------------------------------------------------------------
//
// DrawPortalStencil
//
//-----------------------------------------------------------------------------
void GLPortal::DrawPortalStencil()
{
	for(int i=0;i<lines.Size();i++)
	{
		lines[i].DoRenderWall(false, NULL);

	}

	if (lines.Size() > 1)
	{
		// Cap the stencil at the top and bottom 
		// (cheap ass version - but the resulting polygons are far too small to bother! ;) )
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-256.0f,10000.0f,-256.0f);
		glVertex3f(-256.0f,10000.0f, 256.0f);
		glVertex3f( 256.0f,10000.0f, 256.0f);
		glVertex3f( 256.0f,10000.0f,-256.0f);
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-256.0f,-10000.0f,-256.0f);
		glVertex3f(-256.0f,-10000.0f, 256.0f);
		glVertex3f( 256.0f,-10000.0f, 256.0f);
		glVertex3f( 256.0f,-10000.0f,-256.0f);
		glEnd();
	}
}



//-----------------------------------------------------------------------------
//
// Start
//
//-----------------------------------------------------------------------------
void GLPortal::Start(bool usestencil)
{
	PortalAll.Start();
	if (usestencil)
	{
		// Create stencil 
		glStencilFunc(GL_EQUAL,recursion,~0);		// create stencil
		glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
		glColorMask(0,0,0,0);						// don't write to the graphics buffer
		glDepthMask(false);							// don't write to Z-buffer!
		glDisable(GL_TEXTURE_2D);					// Disable textures (important!)
		glColor3f(1,1,1);
		DrawPortalStencil();

		// Clear Z-buffer
		recursion++;
		
		glStencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
		glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// this stage doesn't modify the stencil
		glDepthMask(true);							// enable z-buffer again
		glDepthRange(1,1);
		glDepthFunc(GL_ALWAYS);
		DrawPortalStencil();
		
		// prepare for drawing to the stencil
		glEnable(GL_TEXTURE_2D);
		glDepthFunc(GL_LESS);
		glColorMask(1,1,1,1);
		glDepthRange(0,1);
		gl_StartDrawInfo(drawinfo);
	}
	PortalAll.Stop();

	// save viewpoint
	savedviewx=viewx;
	savedviewy=viewy;
	savedviewz=viewz;
	savedviewactor=viewactor;
	savedviewangle=viewangle;
	savedviewarea=in_area;
	mirrorline=NULL;
}


inline void GLPortal::ClearClipper()
{
	fixed_t angleOffset = viewangle - savedviewangle;

	clipper.Clear();

	static int call=0;

	// mask the drawable area in the clipper!
	/*
	clipper.AddClipRange(0,0xffffffff);
	for(int i=0;i<lines.Size();i++)
	{
		angle_t startAngle = R_PointToAngle2(savedviewx, savedviewy, 
												-FROM_MAP(lines[i].glseg.x2), FROM_MAP(lines[i].glseg.z2));

		angle_t endAngle = R_PointToAngle2(savedviewx, savedviewy, 
												-FROM_MAP(lines[i].glseg.x1), FROM_MAP(lines[i].glseg.z1));

		if (startAngle-endAngle>0) clipper.RemoveClipRange(startAngle + angleOffset, endAngle + angleOffset);
	}
	*/

	// and finally clip it to the visible area
	angle_t a1 = gl_FrustumAngle();
	if (a1<ANGLE_180) clipper.SafeAddClipRange(viewangle+a1, viewangle-a1);

}

//-----------------------------------------------------------------------------
//
// End
//
//-----------------------------------------------------------------------------
void GLPortal::End(bool usestencil)
{
	PortalAll.Start();
	if (usestencil)
	{
		gl_EndDrawInfo();

		// Restore the old view
		viewx=savedviewx;
		viewy=savedviewy;
		viewz=savedviewz;
		viewangle=savedviewangle;
		viewactor=savedviewactor;
		in_area=savedviewarea;
		gl_SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1));


		// first step: reset the depth buffer to max. depth
		glDepthRange(1,1);							// always
		glDepthFunc(GL_ALWAYS);						// write the farthest depth value
		glColorMask(0,0,0,0);						// no graphics
		glDisable(GL_TEXTURE_2D);					// Disable textures (important!)
		glColor3f(1,1,1);
		DrawPortalStencil();
		
		// second step: restore the depth buffer to the previous values and reset the stencil
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0,1);
		glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
		glStencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
		DrawPortalStencil();
		glDepthFunc(GL_LESS);


		glEnable(GL_TEXTURE_2D);
		glColorMask(1,1,1,1);
		recursion--;

		// restore old stencil op.
		glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
		glStencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
	}
	else
	{
		// only used for skies drawn as background - no skyboxes!
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0,1);
		glColorMask(0,0,0,0);						// no graphics
		glDisable(GL_TEXTURE_2D);					// Disable textures (important!)
		DrawPortalStencil();
		glEnable(GL_TEXTURE_2D);
		glColorMask(1,1,1,1);
		glDepthFunc(GL_LESS);
	}
	PortalAll.Stop();
}


//-----------------------------------------------------------------------------
//
// StartFrame
//
//-----------------------------------------------------------------------------
void GLPortal::StartFrame()
{
	GLPortal * p=NULL;
	portals.Push(p);
	if (renderdepth==0)
	{
		inskybox=inupperstack=inlowerstack=false;
		mirrorline=NULL;
	}
	renderdepth++;
}


//-----------------------------------------------------------------------------
//
// EndFrame
//
//-----------------------------------------------------------------------------
void GLPortal::EndFrame()
{
	GLPortal * p;

	while (portals.Pop(p) && p)
	{
		p->RenderPortal(true);
		delete p;
	}
	renderdepth--;
}


//-----------------------------------------------------------------------------
//
// EndFrame
//
//-----------------------------------------------------------------------------
bool GLPortal::RenderFirstSkyPortal()
{
	GLPortal * p;

	for(size_t i=portals.Size()-1;i>=0 && portals[i]!=NULL;i--)
	{
		p=portals[i];
		if (p->IsSky())
		{
			GLSkyInfo * sky = (GLSkyInfo *)p->GetSource();
			p->RenderPortal(false);
			portals.Delete(i);
			delete p;
			return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
//
// FindPortal
//
//-----------------------------------------------------------------------------
GLPortal * GLPortal::FindPortal(const void * src)
{
	int i=portals.Size()-1;

	while (i>=0 && portals[i] && portals[i]->GetSource()!=src) i--;
	return i>=0? portals[i]:NULL;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//
// Skybox Portal
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// GLSkyboxPortal::DrawContents
//
//-----------------------------------------------------------------------------
static int skyboxrecursion=0;
void GLSkyboxPortal::DrawContents()
{
	if (skyboxrecursion>=2) return;
	skyboxrecursion++;
	//Printf("Starting skybox %08x at %f, %f\n", origin, origin->x/65536.0f, origin->y/65536.0f);
	origin->flags|=MF_JUSTHIT;
	extralight = 0;

	glDisable(GL_DEPTH_CLAMP_NV);

	viewx = origin->x;
	viewy = origin->y;
	viewz = origin->z;

	// Don't let the viewpoint be too close to a floor or ceiling!
	fixed_t floorh = origin->Sector->floorplane.ZatPoint(origin->x, origin->y);
	fixed_t ceilh = origin->Sector->ceilingplane.ZatPoint(origin->x, origin->y);
	if (viewz<floorh+4*FRACUNIT) viewz=floorh+4*FRACUNIT;
	if (viewz>ceilh-4*FRACUNIT) viewz=ceilh-4*FRACUNIT;

	viewangle += origin->angle;

	viewactor = origin;

	validcount++;
	inskybox=true;
	gl_SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1));
	gl_SetViewArea();
	ClearClipper();
	gl_DrawScene();
	origin->flags&=~MF_JUSTHIT;
	inskybox=false;
	glEnable(GL_DEPTH_CLAMP_NV);
	//Printf("Finishing skybox %08x at %f, %f\n", origin, origin->x/65536.0f, origin->y/65536.0f);
	skyboxrecursion--;
}


//-----------------------------------------------------------------------------
//
// GLSectorStackPortal::DrawContents
//
//-----------------------------------------------------------------------------
void GLSectorStackPortal::DrawContents()
{
	viewx -= origin->deltax;
	viewy -= origin->deltay;
	viewz -= origin->deltaz;
	viewactor = NULL;

	validcount++;

	// avoid recursions!
	if (origin->isupper) inupperstack=true;
	else inlowerstack=true;

	gl_SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1));
	ClearClipper();
	gl_DrawScene();
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//
// Mirror Portal
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// R_EnterMirror
//
//-----------------------------------------------------------------------------
void GLMirrorPortal::DrawContents()
{
	if (renderdepth>r_mirror_recursions) return;

	mirrorline=linedef;
	angle_t startang = viewangle;
	fixed_t startx = viewx;
	fixed_t starty = viewy;

	vertex_t *v1 = mirrorline->v1;
	vertex_t *v2 = mirrorline->v2;

	// Reflect the current view behind the mirror.
	if (mirrorline->dx == 0)
	{ 
		// vertical mirror
		viewx = v1->x - startx + v1->x;
	}
	else if (mirrorline->dy == 0)
	{ 
		// horizontal mirror
		viewy = v1->y - starty + v1->y;
	}
	else
	{ 
		// any mirror--use floats to avoid integer overflow

		float dx = FIXED2FLOAT(v2->x - v1->x);
		float dy = FIXED2FLOAT(v2->y - v1->y);
		float x1 = FIXED2FLOAT(v1->x);
		float y1 = FIXED2FLOAT(v1->y);
		float x = FIXED2FLOAT(startx);
		float y = FIXED2FLOAT(starty);

		// the above two cases catch len == 0
		float r = ((x - x1)*dx + (y - y1)*dy) / (dx*dx + dy*dy);

		viewx = FLOAT2FIXED((x1 + r * dx)*2 - x);
		viewy = FLOAT2FIXED((y1 + r * dy)*2 - y);
	}
	viewangle = 2*R_PointToAngle2 (mirrorline->v1->x, mirrorline->v1->y,
										mirrorline->v2->x, mirrorline->v2->y) - startang;

	viewactor = NULL;

	validcount++;

	MirrorFlag++;
	gl_SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1));

	ClearClipper();
	angle_t a2=R_PointToAngle(mirrorline->v1->x, mirrorline->v1->y);
	angle_t a1=R_PointToAngle(mirrorline->v2->x, mirrorline->v2->y);
	clipper.SafeAddClipRange(a1,a2);

	gl_DrawScene();

	MirrorFlag--;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//
// Horizon Portal
// This one is more complicated to reduce the amount of data to be drawn
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Keep one global tesselator instance. There is no need to re-create this for
// each frame
//
//-----------------------------------------------------------------------------
typedef void (__stdcall *tessFunc)(void);

//-----------------------------------------------------------------------------
//
// Tesselatior callbacks
//
//-----------------------------------------------------------------------------
static void CALLBACK t_begin( GLenum type )
{
	glBegin(type);
}

static void CALLBACK t_error(GLenum error)
{
}

static void CALLBACK t_combine( GLdouble coords[3], vertex_t *vert[4], GLfloat w[4], void **dataOut )
{
	*dataOut = vert[0];
}

static void CALLBACK t_vertex( float *vert )
{
	//	Printf(LO_ERROR,"glVertex(%f, %f)\n", vert[0]*128, vert[1]*128);
	glTexCoord2f(vert[0]*2, -vert[1]*2);
	glVertex3f(-vert[0], vert[2], vert[1]);
}

static void CALLBACK t_end( void )
{
	glEnd();
}



//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
static const char edges[5][2]={
	{ BOXLEFT, BOXBOTTOM },
	{ BOXLEFT, BOXTOP },
	{ BOXRIGHT, BOXTOP },
	{ BOXRIGHT, BOXBOTTOM },
	{ BOXLEFT, BOXBOTTOM }
};
TArray<float> GLHorizonPortal::vtx;
GLUtesselator * GLHorizonPortal::tess;

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
inline void GLHorizonPortal::AddVertexToArray(float x, float y, float f)
{
	int index=vtx.Size();
	vtx.Resize(index+3);
	vtx[index]=x;
	vtx[index+1]=y;
	vtx[index+2]=f;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
int GLHorizonPortal::FindEdge(angle_t * edgeangles, angle_t angle)
{
	for(int i=0;i<4;i++)
	{
		//if (angle==edgeangles[i]) return -i-1;
		if (angle-edgeangles[i&3]>=0x80000000 && angle-edgeangles[(1+i)&3]<0x80000000) return i;
	}
	// should never happen!
	return 0;
}


inline void MakeDivline (fixed_t x1, fixed_t y1, fixed_t x2, fixed_t y2, divline_t *dl)
{
	dl->x = x1;
	dl->y = y1;
	dl->dx = x2-x1;
	dl->dy = y2-y1;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void GLHorizonPortal::AddIntersection(fixed_t * box, int edge, fixed_t ox, fixed_t oy, angle_t angle)
{
	fixed_t dx=finecosine[angle>>ANGLETOFINESHIFT];
	fixed_t dy=finesine[angle>>ANGLETOFINESHIFT];

	divline_t edgeline;
	divline_t viewvec={ox, oy, dx, dy};

	MakeDivline(box[edges[edge][0]], box[edges[edge][1]], box[edges[edge+1][0]], box[edges[edge+1][1]], &edgeline);

	fixed_t frac = P_InterceptVector(&edgeline, &viewvec);

	fixed_t px=edgeline.x+FixedMul(edgeline.dx, frac);
	fixed_t py=edgeline.y+FixedMul(edgeline.dy, frac);

	AddVertexToArray( TO_MAP(px)*2, TO_MAP(py)*2, 1.0f);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
int GLHorizonPortal::CreateGeometry()
{
	int i;
	int numoutervertices;
	fixed_t box[4];
	angle_t edgeangles[4];
	fixed_t hviewx=viewx/2;
	fixed_t hviewy=viewy/2;

	vtx.Clear();
	// use half the distance to avoid overflows
	box[BOXLEFT]  = hviewx - 16384*FRACUNIT;
	box[BOXRIGHT] = hviewx + 16384*FRACUNIT;
	box[BOXBOTTOM]= hviewy - 16384*FRACUNIT;
	box[BOXTOP]   = hviewy + 16384*FRACUNIT;

	edgeangles[0]=R_PointToAngle2(hviewx, hviewy, box[BOXLEFT]  ,box[BOXBOTTOM]);
	edgeangles[1]=R_PointToAngle2(hviewx, hviewy, box[BOXLEFT]  ,box[BOXTOP]   );
	edgeangles[2]=R_PointToAngle2(hviewx, hviewy, box[BOXRIGHT] ,box[BOXTOP]   );
	edgeangles[3]=R_PointToAngle2(hviewx, hviewy, box[BOXRIGHT] ,box[BOXBOTTOM]);

	angle_t clipangle = gl_FrustumAngle();
	if (clipangle>ANGLE_90) 
		clipangle = ANGLE_90;

	int rightedge=FindEdge(edgeangles, viewangle+clipangle);
	int leftedge=FindEdge(edgeangles, viewangle-clipangle);

	AddIntersection(box, rightedge, hviewx, hviewy, viewangle+clipangle);
	if (rightedge!=leftedge)
	{
		rightedge=(rightedge+1)&3;
		AddVertexToArray(TO_MAP(box[edges[rightedge][0]])*2.0f, TO_MAP(box[edges[rightedge][1]])*2.0f, 1.0f);
		if (rightedge!=leftedge)
		{
			AddVertexToArray(TO_MAP(box[edges[leftedge][0]])*2.0f, TO_MAP(box[edges[leftedge][1]])*2.0f, 1.0f);
		}
	}
	AddIntersection(box, leftedge, hviewx, hviewy, viewangle-clipangle);

	numoutervertices=vtx.Size()/3;

	int linec=lines.Size();

	for(i=0;i<lines.Size();i++)
	{
		lines[i].flag=0;
	}

	while (linec)
	{
		int index=-1;
		int cmp_angle=0x7fffffff;
		for(i=0;i<lines.Size();i++)
		{
			if (!lines[i].flag)
			{
				angle_t a1=R_PointToAngle(lines[i].vertexes[1]->x, lines[i].vertexes[1]->y) - (viewangle-clipangle);

				if ((signed)a1<cmp_angle)
				{
					index=i;
					cmp_angle=a1;
				}
				else if ((signed)a1==cmp_angle)
				{
					if (lines[i].vertexes[0]->x==lines[index].vertexes[1]->x &&
						lines[i].vertexes[0]->y==lines[index].vertexes[1]->y)
					{
						index=i;
						cmp_angle=a1;
					}
				}
			}
		}
		if (index==-1) break;
		lines[index].flag=1;
		linec--;
		AddVertexToArray(TO_MAP(lines[index].vertexes[1]->x), TO_MAP(lines[index].vertexes[1]->y), 0);
		AddVertexToArray(TO_MAP(lines[index].vertexes[0]->x), TO_MAP(lines[index].vertexes[0]->y), 0);

		do
		{
			for(i=0;i<lines.Size();i++)
			{
				if (!lines[i].flag && lines[i].vertexes[1]==lines[index].vertexes[0])
				{
					index=i;
					lines[i].flag=1;
					AddVertexToArray(TO_MAP(lines[index].vertexes[0]->x), TO_MAP(lines[index].vertexes[0]->y), 0);
					break;
				}
			}
		}
		while (i<lines.Size());
	}
	return numoutervertices;
}


void GLHorizonPortal::RenderGeometry()
{
	int i;
	int numoutervertices=CreateGeometry();
	GLSectorPlane * sp=&origin->plane;
	float z=(float)sp->texheight/MAP_SCALE;

	if (!tess) 
	{
		tess=gluNewTess();
		gluTessCallback(tess, GLU_TESS_BEGIN, (tessFunc)t_begin);
		gluTessCallback(tess, GLU_TESS_VERTEX, (tessFunc)t_vertex);
		gluTessCallback(tess, GLU_TESS_END, (tessFunc)t_end);
		gluTessCallback(tess, GLU_TESS_COMBINE,(tessFunc)t_combine);
		gluTessCallback(tess, GLU_TESS_ERROR, (tessFunc)t_error);
	}

	gluTessBeginPolygon(tess, NULL);
	gluTessBeginContour(tess);

	//Printf(LO_LOG, "Start Horizon\n");
	for(i=0;i<vtx.Size();i+=3)
	{
		double vt[3]={vtx[i], z, vtx[i+1]};

		//Printf(LO_LOG, "x=%f, y=%f\n", vtx[i]*128, vtx[i+1]*128); 
		vtx[i+2]=z;
		gluTessVertex(tess, vt, &vtx[i]);
	}
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);

	float vz=TO_MAP(viewz);
	float tz=(z-vz);///64.0f;

	// fill the gap between the polygon and the true horizon!
	glBegin(GL_TRIANGLE_STRIP);

	for(i=0;i<numoutervertices;i++)
	{
		glTexCoord2f((vtx[i*3]+vtx[i*3+1])*2, 0);
		glVertex3f(-vtx[i*3], z, vtx[i*3+1]);

		glTexCoord2f((vtx[i*3]+vtx[i*3+1])*2, tz);
		glVertex3f(-vtx[i*3], vz, vtx[i*3+1]);
	}
	glEnd();
}
//-----------------------------------------------------------------------------
//
// GLHorizonPortal::DrawContents
//
//-----------------------------------------------------------------------------
void GLHorizonPortal::DrawContents()
{
	GLSectorPlane * sp=&origin->plane;
	FGLTexture * gltexture;
	PalEntry color;
	int lightlevel;
	float z;
	player_t * player=&players[displayplayer];
	int i;

	gltexture=FGLTexture::ValidateTexture(sp->texture);
	if (!gltexture) return;




	gltexture->Bind(origin->colormap.LightColor.a);

	gl_SetPlaneTextureRotation(sp, gltexture);

	color = origin->colormap.LightColor;
	if (gltexture && gl_isGlowingTexture(sp->texture)) 
	{
		// glowing textures are always drawn full bright without color
		color=0xffffff;
		lightlevel=255;
	}
	else lightlevel=origin->lightlevel;

	gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), color, 1.0f);
	gl_SetFog(lightlevel, origin->colormap.FadeColor, STYLE_Normal);
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_ONE,GL_ZERO);

	RenderGeometry();
	if (gl_FrustumAngle()>=ANGLE_90)
	{
		// If we can look all around the back side must be processed, too.
		viewangle+=ANGLE_180;
		RenderGeometry();
	}

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}










/*
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//
// Horizon Portal
//
// This simply draws one huge polygon
//
// Originally I tried to minimize the amount of data to be drawn but there
// are 2 problems with it:
//
// 1. Setting this up mostly negates any performance gains.
// 2. It doesn't work with a 360° field of view (as when you are looking up.)
//
//
// So the brute force mechanism is just as good.
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// GLHorizonPortal::DrawContents
//
//-----------------------------------------------------------------------------
void GLHorizonPortal::DrawContents()
{
	GLSectorPlane * sp=&origin->plane;
	FGLTexture * gltexture;
	PalEntry color;
	int lightlevel;
	float z;
	player_t * player=&players[displayplayer];

	gltexture=FGLTexture::ValidateTexture(sp->texture);
	if (!gltexture) return;



	z=(float)sp->texheight/MAP_SCALE;

	gltexture->Bind(origin->colormap.LightColor.a);

	gl_SetPlaneTextureRotation(sp, gltexture);

	color = origin->colormap.LightColor;
	if (gltexture && gl_isGlowingTexture(sp->texture)) 
	{
		// glowing textures are always drawn full bright without color
		color=0xffffff;
		lightlevel=255;
	}
	else lightlevel=origin->lightlevel;

	gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), color, 1.0f);
	gl_SetFog(lightlevel, origin->colormap.FadeColor, STYLE_Normal);
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_ONE,GL_ZERO);

	glBegin(GL_TRIANGLE_FAN);

	// Draw to some far away boundary
	glTexCoord2f(0,0);
	glVertex3f(0,z,0);
	glTexCoord2f(1024.f, 1024.f);
	glVertex3f(-512.f, z, -512.f);
	glTexCoord2f(1024.f, -1024.f);
	glVertex3f(-512.f, z,  512.f);
	glTexCoord2f(-1024.f, -1024.f);
	glVertex3f( 512.f, z,  512.f);
	glTexCoord2f(-1024.f, 1024.f);
	glVertex3f( 512.f, z, -512.f);

	glTexCoord2f(1024.f, 1024.f);
	glVertex3f(-512.f, z, -512.f);

	glEnd();
		
	float vz=TO_MAP(viewz);
	float tz=(z-vz);///64.0f;

	// fill the gap between the polygon and the true horizon
	// Since I can't draw into infinity there can always be a
	// small gap

	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(1024.f, 0);
	glVertex3f(-512.f, z, -512.f);
	glTexCoord2f(1024.f, tz);
	glVertex3f(-512.f, vz, -512.f);

	glTexCoord2f(1024.f, 0);
	glVertex3f(-512.f, z,  512.f);
	glTexCoord2f(1024.f, tz);
	glVertex3f(-512.f, vz,  512.f);

	glTexCoord2f(-1024.f, 0);
	glVertex3f( 512.f, z,  512.f);
	glTexCoord2f(-1024.f, tz);
	glVertex3f( 512.f, vz,  512.f);

	glTexCoord2f(-1024.f, 0);
	glVertex3f( 512.f, z, -512.f);
	glTexCoord2f(-1024.f, tz);
	glVertex3f( 512.f, vz, -512.f);

	glEnd();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


*/