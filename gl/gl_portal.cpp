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
void GLSkyboxPortal::DrawContents()
{
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
	glTexCoord2f(10240.f, 10240.f);
	glVertex3f(-5120.f, z, -5120.f);
	glTexCoord2f(10240.f, -10240.f);
	glVertex3f(-5120.f, z,  5120.f);
	glTexCoord2f(-10240.f, -10240.f);
	glVertex3f( 5120.f, z,  5120.f);
	glTexCoord2f(-10240.f, 10240.f);
	glVertex3f( 5120.f, z, -5120.f);

	glEnd();
		
	float vz=TO_MAP(viewz);
	float tz=(z-vz);///64.0f;

	// fill the gap between the polygon and the true horizon
	// Since I can't draw into infinity there can always be a
	// small gap

	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(10240.f, 0);
	glVertex3f(-5120.f, z, -5120.f);
	glTexCoord2f(10240.f, tz);
	glVertex3f(-5120.f, vz, -5120.f);

	glTexCoord2f(10240.f, 0);
	glVertex3f(-5120.f, z,  5120.f);
	glTexCoord2f(10240.f, tz);
	glVertex3f(-5120.f, vz,  5120.f);

	glTexCoord2f(-10240.f, 0);
	glVertex3f( 5120.f, z,  5120.f);
	glTexCoord2f(-10240.f, tz);
	glVertex3f( 5120.f, vz,  5120.f);

	glTexCoord2f(-10240.f, 0);
	glVertex3f( 5120.f, z, -5120.f);
	glTexCoord2f(-10240.f, tz);
	glVertex3f( 5120.f, vz, -5120.f);

	glEnd();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


