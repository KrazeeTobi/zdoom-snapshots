/*
** gl_models_md3.cpp
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
** 4. When not used as part of GZDoom or a GZDoom derivative, this code will be
**    covered by the terms of the GNU Lesser General Public License as published
**    by the Free Software Foundation; either version 2.1 of the License, or (at
**    your option) any later version.
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

#include "gl_pch.h"
#include "w_wad.h"
#include "cmdlib.h"
#include "sc_man.h"
#include "m_crc32.h"
#include "gl_models.h"
#include "gl_texture.h"
#include "gl_values.h"
#include "gl_renderstruct.h"

#define MAX_QPATH 64



static void UnpackVector(unsigned short packed, float & nx, float & ny, float & nz)
{
	// decode the lat/lng normal to a 3 float normal
	double lat = ( packed >> 8 ) & 0xff;
	double lng = ( packed & 0xff );
	lat *= PI/128;
	lng *= PI/128;

	nx = cos(lat) * sin(lng);
	ny = sin(lat) * sin(lng);
	nz = cos(lng);
}



bool FMD3Model::Load(const char * path, const char * buffer, int length)
{
	#pragma pack(4)
	struct md3_header_t
	{
		DWORD Magic;
		DWORD Version;
		char Name[MAX_QPATH];
		DWORD Flags;
		DWORD Num_Frames;
		DWORD Num_Tags;
		DWORD Num_Surfaces;
		DWORD Num_Skins;
		DWORD Ofs_Frames;
		DWORD Ofs_Tags;
		DWORD Ofs_Surfaces;
		DWORD Ofs_Eof;
	};

	struct md3_surface_t
	{
		DWORD Magic;
		char Name[MAX_QPATH];
		DWORD Flags;
		DWORD Num_Frames;
		DWORD Num_Shaders;
		DWORD Num_Verts;
		DWORD Num_Triangles;
		DWORD Ofs_Triangles;
		DWORD Ofs_Shaders;
		DWORD Ofs_Texcoord;
		DWORD Ofs_XYZNormal;
		DWORD Ofs_End;
	};

	struct md3_triangle_t
	{
		DWORD vt_index[3];
	};

	struct md3_shader_t
	{
		char Name[MAX_QPATH];
		DWORD index;
	};

	struct md3_texcoord_t
	{
		float s,t;
	};

	struct md3_vertex_t
	{
		short x,z,y,n;
	};

	struct md3_frame_t
	{
		float min_Bounds[3];
		float max_Bounds[3];
		float localorigin[3];
		float radius;
		char Name[16];
	};
	#pragma pack()

	md3_header_t * hdr=(md3_header_t *)buffer;

	numFrames = LONG(hdr->Num_Frames);
	numTags = LONG(hdr->Num_Tags);
	numSurfaces = LONG(hdr->Num_Surfaces);

	md3_frame_t * frm = (md3_frame_t*)(buffer + LONG(hdr->Ofs_Frames));

	frames = new MD3Frame[numFrames];
	for(int i=0;i<numFrames;i++)
	{
		strncpy(frames[i].Name, frm[i].Name, 16);
		for(int j=0;j<3;j++) frames[i].origin[j] = frm[i].localorigin[j];
	}

	md3_surface_t * surf = (md3_surface_t*)(buffer + LONG(hdr->Ofs_Surfaces));

	surfaces = new MD3Surface[numSurfaces];
	for(int i=0;i<numSurfaces;i++)
	{
		MD3Surface * s = &surfaces[i];
		md3_surface_t * ss = &surf[i];

		s->numSkins = LONG(ss->Num_Shaders);
		s->numTriangles = LONG(ss->Num_Triangles);
		s->numVertices = LONG(ss->Num_Verts);

		// copy triangle indices
		md3_triangle_t * tris = (md3_triangle_t*)(((char*)ss)+LONG(ss->Ofs_Triangles));
		s->tris = new MD3Triangle[s->numTriangles];

		for(int i=0;i<s->numTriangles;i++) for (int j=0;j<3;j++)
		{
			s->tris[i].VertIndex[j]=LONG(tris[i].vt_index[j]);
		}

		// copy shaders (skins)
		md3_shader_t * shader = (md3_shader_t*)(((char*)ss)+LONG(ss->Ofs_Shaders));
		s->skins = new FTexture *[s->numSkins];

		for(int i=0;i<s->numSkins;i++)
		{
			s->skins[i] = LoadSkin(path, shader[i].Name);
		}

		// Load texture coordinates
		md3_texcoord_t * tc = (md3_texcoord_t*)(((char*)ss)+LONG(ss->Ofs_Texcoord));
		s->texcoords = new MD3TexCoord[s->numVertices];

		for(int i=0;i<s->numVertices;i++)
		{
			s->texcoords[i].s = tc[i].s;
			s->texcoords[i].t = tc[i].t;
		}

		// Load vertices and texture coordinates
		md3_vertex_t * vt = (md3_vertex_t*)(((char*)ss)+LONG(ss->Ofs_XYZNormal));
		s->vertices = new MD3Vertex[s->numVertices * numFrames];

		for(int i=0;i<s->numVertices * numFrames;i++)
		{
			s->vertices[i].x = SHORT(vt[i].x)/64.f / MAP_COEFF;
			s->vertices[i].y = SHORT(vt[i].y)/64.f / MAP_COEFF * rModelAspectMod;
			s->vertices[i].z = SHORT(vt[i].z)/64.f / MAP_COEFF;
			UnpackVector( SHORT(vt[i].n), s->vertices[i].nx, s->vertices[i].ny, s->vertices[i].nz);
		}
	}
	return true;
}

int FMD3Model::FindFrame(const char * name)
{
	for (int i=0;i<numFrames;i++)
	{
		if (!stricmp(name, frames[i].Name)) return i;
	}
	return -1;
}

void FMD3Model::RenderTriangles(MD3Surface * surf, MD3Vertex * vert)
{
	gl.Begin(GL_TRIANGLES);
	for(int i=0; i<surf->numTriangles;i++)
	{
		for(int j=0;j<3;j++)
		{
			int x = surf->tris[i].VertIndex[j];

			gl.TexCoord2fv(&surf->texcoords[x].s);
			gl.Vertex3fv(&vert[x].x);
		}
	}
	gl.End();
}

void FMD3Model::RenderFrame(FTexture * skin, int frameno, int cm)
{
	if (frameno>=numFrames) return;

	MD3Frame * frame = &frames[frameno];

	// I can't confirm correctness of this because no model I have tested uses this information
	// gl.MatrixMode(GL_MODELVIEW);
	// gl.Translatef(frame->origin[0], frame->origin[1], frame->origin[2]);

	for(int i=0;i<numSurfaces;i++)
	{
		MD3Surface * surf = &surfaces[i];

		if (!skin)
		{
			if (surf->numSkins==0) return;
			skin = surf->skins[0];
			if (!skin) return;
		}

		FGLTexture * tex = FGLTexture::ValidateTexture(skin);

		tex->Bind(cm);
		RenderTriangles(surf, surf->vertices + frameno * surf->numVertices);
	}
}


