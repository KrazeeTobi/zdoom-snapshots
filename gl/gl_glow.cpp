#include "gl_pch.h"


/*
** gl_glow.cpp
** Glowing flats like Doomsday
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

#include "w_wad.h"
#include "sc_man.h"

#include "gl/gl_texture.h"
#include "gl/gl_glow.h"
#include "gl/gl_functions.h"


bool * GlowingTextures;
int MaxGlowingTexture;	// TexMan.NumTextures can't be used because it may change after allocating the arrays here!
static PalEntry * GlowingColors;

void gl_ResetGlow()
{
	if (GlowingTextures) delete [] GlowingTextures;
	if (GlowingColors) delete [] GlowingColors;
	GlowingTextures=NULL;
	GlowingColors=NULL;
	MaxGlowingTexture=NULL;
}

//===========================================================================
// 
//	Reads a GLOWDEF lump and flags all glowing textures
//  This is done on a per-level basis
//
//===========================================================================
void gl_InitGlow(const char * lumpnm)
{
	int lump;

	if (!GlowingTextures)
	{
		MaxGlowingTexture=TexMan.NumTextures();
		GlowingTextures=new bool[MaxGlowingTexture];
		GlowingColors=new PalEntry[MaxGlowingTexture];
		memset(GlowingColors, 0, sizeof(PalEntry)*MaxGlowingTexture);
	}


	memset(GlowingTextures, 0, sizeof(bool)*MaxGlowingTexture);
	if (!lumpnm || *lumpnm==0) return;
	lump=Wads.CheckNumForName(lumpnm);
	if (lump==-1) return;


	SC_OpenLumpNum(lump,lumpnm);

	while (SC_GetString())
	{
		if (SC_Compare("FLATS"))
		{
			SC_MustGetStringName("{");
			while (!SC_CheckString("}"))
			{
				SC_MustGetString();
				int flump=TexMan.CheckForTexture(sc_String, FTexture::TEX_Flat,FTextureManager::TEXMAN_TryAny);
				if (flump!=-1 && flump<MaxGlowingTexture) 
				{
					GlowingTextures[flump]=true;
				}	 
			}
		}

		if (SC_Compare("WALLS"))
		{
			SC_MustGetStringName("{");
			while (!SC_CheckString("}"))
			{
				SC_MustGetString();
				int flump=TexMan.CheckForTexture(sc_String, FTexture::TEX_Wall,FTextureManager::TEXMAN_TryAny);
				if (flump!=-1 && flump<MaxGlowingTexture) 
				{
					GlowingTextures[flump]=true;
				}	 
			}
		}
	}
	SC_Close();
}


//===========================================================================
// 
//	Gets the average color of a texture for use as a glow color
//
//===========================================================================
void gl_GetGlowColor(unsigned int texno, float * data)
{
	if (texno<MaxGlowingTexture && GlowingColors)
	{
		if (GlowingColors[texno].a==0)
		{
			FGLTexture * tex = FGLTexture::ValidateTexture(texno);
			if (tex)
			{
				unsigned char * buffer = tex->CreateTexBuffer(CM_DEFAULT, 0);

				if (buffer)
				{
					GlowingColors[texno]=averageColor((unsigned long *) buffer, tex->GetWidth() * tex->GetHeight(), true);
					delete buffer;
					GlowingColors[texno].a=1;	// mark as processed
				}
			}
		}
		data[0]=GlowingColors[texno].r/255.0f;
		data[1]=GlowingColors[texno].g/255.0f;
		data[2]=GlowingColors[texno].b/255.0f;
	}
	else
	{
		data[0]=0;
		data[1]=0;
		data[2]=0;
	}
}


