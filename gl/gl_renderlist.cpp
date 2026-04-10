#include "gl_pch.h"
/*
** gl_renderlist.cpp
** Render lists
**
**---------------------------------------------------------------------------
** Copyright 2005 Christoph Oelckers
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

#include "m_crc32.h"
#include "gl/gl_struct.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_portal.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_basic.h"
#include "gl/gl_functions.h"

#if 0
static TArray<GLRenderVertex> RenderVertices(100);
static TArray<GLPoly> RenderPolys;
static FreeList<GLPolyPropertyDesc> props;

void gl_SetArrays()
{
	void * lastaddr=NULL;

	//if (lastaddr!=&RenderVertices[0])
	{
		lastaddr=&RenderVertices[0];
		glTexCoordPointer(2,GL_FLOAT,sizeof(GLRenderVertex),&RenderVertices[0].u);
		glVertexPointer(3,GL_FLOAT,sizeof(GLRenderVertex),&RenderVertices[0].x);
		glColorPointer(4,GL_BYTE, sizeof(GLRenderVertex),&RenderVertices[0].r);
		glEnableClientState(GL_COLOR_ARRAY);
	}
}

void gl_ResetArrays()
{
	// temporary!
	glTexCoordPointer(2,GL_FLOAT,sizeof(GLVertex),&gl_vertices[0].u);
	glVertexPointer(3,GL_FLOAT,sizeof(GLVertex),&gl_vertices[0].x);
	glDisableClientState(GL_COLOR_ARRAY);
}

static unsigned int HashKey (const char *s)
{
	DWORD key = 0xffffffff;
	const DWORD *table = GetCRCTable ();
	int len=GLPolyPropertyDesc::HashLen();

	while (len > 0)
	{
		key = CRC1 (key, tolower(*s++), table);
		--len;
	}
	return key % PROPHASH_SIZE;
}


void GLPolyRenderList::AddPoly(int poly, GLPolyPropertyDesc * prop)
{
	int hashcode = HashKey((const char*)prop);

	GLPolyPropertyDesc * list = PolyPropHash[hashcode];

	while (list)
	{
		if (!memcmp(list, prop, GLPolyPropertyDesc::HashLen())) break;
	}
	if (!list)
	{
		list = props.GetNew();
		*list = *prop;
		list->hashnext = PolyPropHash[hashcode];
		PolyPropHash[hashcode] = list;

		list->first = list->last = poly;
	}
	else
	{
		list->last = RenderPolys[list->last].next = poly;
	}
	RenderPolys[poly].next = -1;
}


void GLPolyRenderList::AddWall(GLWall * w)
{
	float r,g,b;

	if (w->seg->frontsector->e->lightlist.Size()==0) 
	{
		// simple w without light change
		int v_index = RenderVertices.Reserve(4);
		int p_index = RenderPolys.Reserve(1);
		GLPolyPropertyDesc prop;

		RenderVertices[v_index+0].Set(w->glseg.x1, w->ybottom[0], w->glseg.z1, w->lolft.u, w->lolft.v);
		RenderVertices[v_index+1].Set(w->glseg.x1, w->ytop[0],    w->glseg.z1, w->uplft.u, w->uplft.v);
		RenderVertices[v_index+2].Set(w->glseg.x2, w->ytop[1],    w->glseg.z2, w->uprgt.u, w->uprgt.v);
		RenderVertices[v_index+3].Set(w->glseg.x2, w->ybottom[0], w->glseg.z2, w->lorgt.u, w->lorgt.v);

		gl_GetLightColor(w->lightlevel+(extralight<<LIGHTSEGSHIFT), w->Colormap.LightColor, &r, &g, &b);
		RenderVertices[v_index+0].SetColor(r/255,g/255,b/255);
		RenderVertices[v_index+1].SetColor(r/255,g/255,b/255);
		RenderVertices[v_index+2].SetColor(r/255,g/255,b/255);
		RenderVertices[v_index+3].SetColor(r/255,g/255,b/255);

		RenderPolys[p_index].type = GLDIT_WALL;
		RenderPolys[p_index].firstvertex = v_index;
		RenderPolys[p_index].numvertices = 4;

		prop.cm = w->Colormap.LightColor.a;
		prop.Fog = w->Colormap.FadeColor;
		prop.Fog.a = gl_GetFogDensity(w->lightlevel, w->Colormap.FadeColor);
		prop.flags = 0;
		prop.translation=0;
		prop.tex=w->gltexture;

		AddPoly(p_index, &prop);
	}
	else 
	{
		// the wall contains different lights
	}

}


inline void GLPoly::Draw()
{
	glDrawArrays(GL_TRIANGLE_FAN, firstvertex, numvertices);
}

void GLPolyPropertyDesc::Draw()
{
	if (Fog.a > 0) 
	{
		GLfloat FogColor[4]={Fog.r/255.0f,Fog.g/255.0f,Fog.b/255.0f,0.0f};
		glFogfv(GL_FOG_COLOR,FogColor);
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY,Fog.a/500.0f);
	}
	else glDisable(GL_FOG);

	if (tex) tex->Bind(cm);
	else glDisable(GL_TEXTURE_2D);

	int polyindex = first;
	while (polyindex>=0)
	{
		RenderPolys[polyindex].Draw();
		polyindex = RenderPolys[polyindex].next;
	}
	if (!tex) glDisable(GL_TEXTURE_2D);
}

void GLPolyRenderList::Draw()
{
	gl_SetArrays();

	for (int i=0;i<PROPHASH_SIZE;i++)
	{
		GLPolyPropertyDesc * prop = PolyPropHash[i];
		while (prop)
		{
			prop->Draw();
			GLPolyPropertyDesc * p = prop->hashnext;
			props.Release(prop);
			prop=p;
		}
		PolyPropHash[i]=NULL;
	}

	gl_ResetArrays();
}
#endif