#include "gl_pch.h"
/*
** Hires texture management
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
#include "w_wad.h"
#include "m_png.h"
#include "r_draw.h"
#include "sbar.h"
#include "gi.h"
#include "cmdlib.h"
#include "sc_man.h"

#include "gl/gl_struct.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"

#include <il/il.h>
#include <il/ilu.h>

//==========================================================================
//
// This is just a dummy class to add hires textures to the texture manager
// If used by the software renderer it only makes sure that all the pointers
// point to valid data - but don't contain any actual texture information.
//
//==========================================================================

class FHiresTexture : public FTexture
{
	BYTE *Pixels;
	Span DummySpans[2];

public:
	FHiresTexture (const char * name, int w, int h);
	virtual ~FHiresTexture ();

	// Returns a single column of the texture
	virtual const BYTE *GetColumn (unsigned int column, const Span **spans_out);
	virtual const BYTE *GetPixels ();
	virtual void Unload ();
};


FHiresTexture::FHiresTexture (const char * name, int w, int h)
: Pixels(0)
{
	sprintf(Name, "%.8s", name);

	bMasked = true;
	Width = w;
	Height = h;
	CalcBitSize();
	DummySpans[0].TopOffset = 0;
	DummySpans[0].Length = Height;
	DummySpans[1].TopOffset = 0;
	DummySpans[1].Length = 0;

	ScaleX = ScaleY = 8;
	UseType = TEX_Override;
}

FHiresTexture::~FHiresTexture ()
{
	Unload ();
}

void FHiresTexture::Unload ()
{
	if (Pixels != NULL)
	{
		delete[] Pixels;
		Pixels = NULL;
	}
}

const BYTE *FHiresTexture::GetColumn (unsigned int column, const Span **spans_out)
{
	if (spans_out != NULL)
	{
		*spans_out = DummySpans;
	}
	return GetPixels();
}

const BYTE *FHiresTexture::GetPixels ()
{
	if (Pixels == NULL)
	{
		Pixels = new BYTE[Width*Height];
		memset(Pixels, 255, Width*Height);
	}
	return Pixels;
}

//==========================================================================
//
// Checks for the presence of a hires texture replacement
//
//==========================================================================
bool FGLTexture::CheckExternalFile()
{
	static const char * doom1texpath[]= {
		"./textures/doom/doom1/%s.%s", "./textures/doom/doom1/%s-ck.%s", 
			"./textures/doom/%s.%s", "./textures/doom/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * doom2texpath[]= {
		"./textures/doom/doom2/%s.%s", "./textures/doom/doom2/%s-ck.%s", 
			"./textures/doom/%s.%s", "./textures/doom/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * pluttexpath[]= {
		"./textures/doom/plut/%s.%s", "./textures/doom/plut/%s-ck.%s", 
			"./textures/doom/%s.%s", "./textures/doom/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * tnttexpath[]= {
		"./textures/doom/tnt/%s.%s", "./textures/doom/tnt/%s-ck.%s", 
			"./textures/doom/%s.%s", "./textures/doom/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * heretictexpath[]= {
		"./textures/heretic/%s.%s", "./textures/heretic/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * hexentexpath[]= {
		"./textures/hexen/%s.%s", "./textures/hexen/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * strifetexpath[]= {
		"./textures/strife/%s.%s", "./textures/strife/%s-ck.%s", "./textures/%s.%s", "./textures/%s-ck.%s", NULL
	};

	static const char * doom1flatpath[]= {
		"./flats/doom/doom1/%s.%s", "./textures/doom/doom1/flat-%s.%s", 
			"./flats/doom/%s.%s", "./textures/doom/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * doom2flatpath[]= {
		"./flats/doom/doom2/%s.%s", "./textures/doom/doom2/flat-%s.%s", 
			"./flats/doom/%s.%s", "./textures/doom/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * plutflatpath[]= {
		"./flats/doom/plut/%s.%s", "./textures/doom/plut/flat-%s.%s", 
			"./flats/doom/%s.%s", "./textures/doom/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * tntflatpath[]= {
		"./flats/doom/tnt/%s.%s", "./textures/doom/tnt/flat-%s.%s", 
			"./flats/doom/%s.%s", "./textures/doom/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * hereticflatpath[]= {
		"./flats/heretic/%s.%s", "./textures/heretic/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * hexenflatpath[]= {
		"./flats/hexen/%s.%s", "./textures/hexen/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * strifeflatpath[]= {
		"./flats/strife/%s.%s", "./textures/strife/flat-%s.%s", "./flats/%s.%s", "./textures/flat-%s.%s", NULL
	};

	static const char * doom1patchpath[]= {
		"./patches/doom/doom1/%s.%s", "./patches/doom/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * doom2patchpath[]= {
		"./patches/doom/doom2/%s.%s", "./patches/doom/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * plutpatchpath[]= {
		"./patches/doom/plut/%s.%s", "./patches/doom/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * tntpatchpath[]= {
		"./patches/doom/tnt/%s.%s", "./patches/doom/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * hereticpatchpath[]= {
		"./patches/heretic/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * hexenpatchpath[]= {
		"./patches/hexen/%s.%s", "./patches/%s.%s", NULL
	};

	static const char * strifepatchpath[]= {
		"./patches/strife/%s.%s", "./patches/%s.%s", NULL
	};

	char checkName[50];
	const char ** checklist;
	BYTE useType=tex->UseType;

	if (useType==FTexture::TEX_SkinSprite || useType==FTexture::TEX_Decal || useType==FTexture::TEX_FontChar)
	{
		return false;
	}

	bool ispatch = (useType==FTexture::TEX_MiscPatch || useType==FTexture::TEX_Sprite) ;

	// for patches this doesn't work yet
	if (ispatch) return false;

	switch (gameinfo.gametype)
	{
	case GAME_Doom:
		switch (gamemission)
		{
		case doom:
			checklist = ispatch ? doom1patchpath : useType==FTexture::TEX_Flat? doom1flatpath : doom1texpath;
			break;
		case doom2:
			checklist = ispatch ? doom2patchpath : useType==FTexture::TEX_Flat? doom2flatpath : doom2texpath;
			break;
		case pack_tnt:
			checklist = ispatch ? tntpatchpath : useType==FTexture::TEX_Flat? tntflatpath : tnttexpath;
			break;
		case pack_plut:
			checklist = ispatch ? plutpatchpath : useType==FTexture::TEX_Flat? plutflatpath : pluttexpath;
			break;
		default:
			return false;
		}
		break;

	case GAME_Heretic:
		checklist = ispatch ? hereticpatchpath : useType==FTexture::TEX_Flat? hereticflatpath : heretictexpath;
		break;
	case GAME_Hexen:
		checklist = ispatch ? hexenpatchpath : useType==FTexture::TEX_Flat? hexenflatpath : hexentexpath;
		break;
	case GAME_Strife:
		checklist = ispatch ?strifepatchpath : useType==FTexture::TEX_Flat? strifeflatpath : strifetexpath;
		break;
	default:
		return false;
	}

	while (*checklist)
	{
		static const char * extensions[] = { "PNG", "JPG", "TGA", "PCX", NULL };

		for (const char ** extp=extensions; *extp; extp++)
		{
			sprintf(checkName, *checklist, tex->Name, *extp);
			if (_access(checkName, 0) == 0) 
			{
				hirespath=copystring(checkName);
				return true;
			}
		}
		checklist++;
	}
	return false;
}


//==========================================================================
//
// Loads a hires texture
//
//==========================================================================
unsigned char *FGLTexture::LoadFile(const char *fileName, int *width, int *height, int cm)
{
	unsigned char *buffer = NULL;
	byte *data;
	ILuint imgID;
	int imgSize;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ilLoad(IL_TYPE_UNKNOWN, (const ILstring)fileName);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	*width = ilGetInteger(IL_IMAGE_WIDTH);
	*height = ilGetInteger(IL_IMAGE_HEIGHT);
	imgSize = *width * *height;
	buffer = new unsigned char[4*imgSize];
	data = ilGetData();

	if (strstr(fileName, "-ck."))
	{
		// This is a crappy Doomsday color keyed image
		// We have to remove the key manually. :(
		DWORD * dwdata=(DWORD*)data;
		for(int i=0;i<imgSize;i++)
		{
			if (dwdata[i]==0xffffff00 || dwdata[i]==0xffff00ff) dwdata[i]=0;
		}
	}

	// Since I have to copy the image anyway I'll do the 
	// palette manipulation in the same step.
	ModifyPalette((PalEntry*)buffer, (PalEntry*)data, cm, imgSize, true);

	ilDeleteImages(1, &imgID);
	return buffer;
}


//==========================================================================
//
// Loads a hires texture from a lump
//
//==========================================================================
unsigned char * FGLTexture::LoadFromLump(int lumpNum, int *width, int *height, int cm)
{
	unsigned char *buffer = NULL;
	byte *data;
	ILuint imgID;
	int imgSize;
	FMemLump memLump;

	memLump = Wads.ReadLump(lumpNum);

	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ilLoadL(IL_TYPE_UNKNOWN, memLump.GetMem(), Wads.LumpLength(lumpNum));

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	*width = ilGetInteger(IL_IMAGE_WIDTH);
	*height = ilGetInteger(IL_IMAGE_HEIGHT);
	imgSize = *width * *height;
	buffer = new unsigned char[imgSize*4];
	data = ilGetData();

	// Since I have to copy the image anyway I'll do the 
	// palette manipulation in the same step.
	ModifyPalette((PalEntry*)buffer, (PalEntry*)data, cm, imgSize, true);

	ilDeleteImages(1, &imgID);

	return buffer;
}

//==========================================================================
//
// Checks for the presence of a hires texture replacement and loads it
//
//==========================================================================
unsigned char *FGLTexture::LoadHiresTexture(int *width, int *height,int cm)
{
	if (HiresLump>=0)
	{
		// This is an internally defined hires texture.
		return LoadFromLump(HiresLump, width, height, cm);
	}

	if (HiresLump==-1)
	{
		HiresLump = CheckExternalFile()? -2:-3;
	}
	if (hirespath != NULL)
	{
		unsigned char * buffer = LoadFile(hirespath, width, height, cm);
		if (buffer) return buffer;

		// don't try again
		HiresLump=-3;
		delete [] hirespath;
		hirespath=NULL;
	}
	return NULL;
}


//==========================================================================
//
// Adds all hires texture definitions.
//
//==========================================================================

void FGLTexture::LoadHiresTextures()
{
	int remapLump, lastLump;
	char src[9];
	bool is32bit;
	int width, height;
	int type,mode;

	lastLump = 0;
	src[8] = '\0';

	while ((remapLump = Wads.FindLump("HIRESTEX", &lastLump)) != -1)
	{
		SC_OpenLumpNum(remapLump, "HIRESTEX");
		while (SC_GetString())
		{
			if (SC_Compare("remap")) // remap an existing texture
			{
				SC_MustGetString();
				
				// allow selection by type
				if (SC_Compare("wall")) type=FTexture::TEX_Wall, mode=FTextureManager::TEXMAN_Overridable;
				else if (SC_Compare("flat")) type=FTexture::TEX_Flat, mode=FTextureManager::TEXMAN_Overridable;
				else if (SC_Compare("sprite")) type=FTexture::TEX_Sprite, mode=0;
				else type = FTexture::TEX_Any;
				
				sc_String[8]=0;

				int tex = TexMan.CheckForTexture(sc_String, type, mode);

				SC_MustGetString();
				sc_String[8]=0;

				if (tex>0) 
				{
					FTexture * texx = TexMan[tex];
					FGLTexture * gtex = FGLTexture::ValidateTexture(texx);
					if (gtex) 
					{
						gtex->HiresLump = Wads.CheckNumForName(sc_String);
						if (!gtex->HiresLump) Printf("Unable to set hires texture for '%s': %s not found\n", texx->Name, sc_String);
					}
				}
			}
			else if (SC_Compare("define")) // define a new "fake" texture
			{
				SC_GetString();
				memcpy(src, sc_String, 8);

				int lumpnum = Wads.CheckNumForName(sc_String);

				SC_GetString();
				is32bit = !!SC_Compare("force32bit");
				if (!is32bit) SC_UnGet();

				SC_GetNumber();
				width = sc_Number;
				SC_GetNumber();
				height = sc_Number;

				if (lumpnum>=0)
				{
					int oldtex = TexMan.CheckForTexture(src, FTexture::TEX_Override);
					FTexture * tex = new FHiresTexture(src, width, height);

					if (oldtex>=0) TexMan.ReplaceTexture(oldtex, tex, true);
					else TexMan.AddTexture(tex);

					FGLTexture * gtex = FGLTexture::ValidateTexture(tex);
					if (gtex) 
					{
						gtex->HiresLump = lumpnum;
					}
				}				
				//else Printf("Unable to define hires texture '%s'\n", tex->Name);
			}
		}
		SC_Close();
	}
}
	