/*
** gl_models.cpp
**
** General model handling code
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


TArray<FModel *> Models;
static TArray<FSpriteModelFrame> SpriteModelFrames;
static int * SpriteModelHash;
//TArray<FStateModelFrame> StateModelFrames;


//===========================================================================
//
// FindGFXFile
//
//===========================================================================

static int FindGFXFile(char * fn)
{
	char * dot = strrchr(fn, '.');
	if (!dot) dot=fn+strlen(fn);

	static const char * extensions[] = { ".PNG", ".JPG", ".TGA", ".PCX", NULL };

	for (const char ** extp=extensions; *extp; extp++)
	{
		strcpy(dot, *extp);
		int lump = Wads.CheckNumForFullName(fn);
		if (lump >= 0) 
		{
			return lump;
		}
	}
	return -1;
}


//===========================================================================
//
// LoadSkin
//
//===========================================================================

FTexture * LoadSkin(const char * path, const char * fn)
{
	char buffer[256];

	sprintf(buffer, "%s%s", path, fn);

	int texlump = FindGFXFile(buffer);
	if (texlump>=0)
	{
		FTexture * tex = new FHiresTexture("$$SKIN$$", 128, 128);

		FGLTexture * gtex = FGLTexture::ValidateTexture(tex);
		if (gtex) 
		{
			gtex->HiresLump = texlump;
		}
		return tex;
	}
	else 
	{
		return NULL;
	}
}

//===========================================================================
//
// ModelFrameHash
//
//===========================================================================

static int ModelFrameHash(FSpriteModelFrame * smf)
{
	const DWORD *table = GetCRCTable ();
	DWORD hash = 0xffffffff;

	const char * s = (const char *)(&smf->type);	// this uses type, sprite and frame for hashing
	const char * se= (const char *)(&smf->hashnext);

	for (; s<se; s++)
	{
		hash = CRC1 (hash, *s, table);
	}
	return hash ^ 0xffffffff;
}

//===========================================================================
//
// FindModel
//
//===========================================================================

static FModel * FindModel(const char * path, const char * modelfile)
{
	FModel * model;
	char fullname[256];
	int filepos;

	strcpy(fullname, path);
	FixPathSeperator(fullname);
	filepos=strlen(fullname);
	if (fullname[filepos-1]!='/') 
	{
		strcat(fullname, "/");
		filepos++;
	}

	strcpy(fullname+filepos, modelfile);

	int lump = Wads.CheckNumForFullName(fullname);

	if (lump<0)
	{
		Printf("FindModel: '%s' not found\n", fullname);
		return NULL;
	}

	for(int i = 0; i< (int)Models.Size(); i++)
	{
		if (!stricmp(fullname, Models[i]->filename)) return Models[i];
	}

	int len = Wads.LumpLength(lump);
	FMemLump lumpd = Wads.ReadLump(lump);
	char * buffer = (char*)lumpd.GetMem();

	if (!memcmp(buffer, "DMDM", 4))
	{
		model = new FDMDModel;
	}
	else if (!memcmp(buffer, "IDP2", 4))
	{
		model = new FMD2Model;
	}
	else if (!memcmp(buffer, "IDP3", 4))
	{
		model = new FMD3Model;
	}
	else
	{
		Printf("LoadModel: Unknown model format in '%s'\n", fullname);
		delete buffer;
		return NULL;
	}

	if (!model->Load(path, buffer, len))
	{
		delete model;
		delete buffer;
		return NULL;
	}
	model->filename = copystring(fullname);
	Models.Push(model);
	return model;
}

//===========================================================================
//
// gl_InitModels
//
//===========================================================================

void gl_InitModels()
{
	int Lump, lastLump;
	string path;
	int index;
	int i;

	FSpriteModelFrame smf;

	lastLump = 0;

	memset(&smf, 0, sizeof(smf));
	while ((Lump = Wads.FindLump("MODELDEF", &lastLump)) != -1)
	{
		SC_OpenLumpNum(Lump, "MODELDEF");
		while (SC_GetString())
		{
			if (SC_Compare("model"))
			{
				SC_MustGetString();
				memset(&smf, 0, sizeof(smf));
				smf.xscale=smf.yscale=smf.zscale=1.f;

				smf.type = TypeInfo::FindType(sc_String);
				if (!smf.type) SC_ScriptError("MODELDEF: Unknown actor type '%s'\n", sc_String);
				GetDefaultByType(smf.type)->hasmodel=true;
				SC_MustGetStringName("{");
				while (!SC_CheckString("}"))
				{
					SC_MustGetString();
					if (SC_Compare("path"))
					{
						SC_MustGetString();
						FixPathSeperator(sc_String);
						path = sc_String;
						if (path[path.Len()-1]!='/') path+='/';
					}
					else if (SC_Compare("model"))
					{
						SC_MustGetNumber();
						index=sc_Number;
						if (index<0 || index>=MAX_MODELS_PER_FRAME)
						{
							SC_ScriptError("Too many models in %s", smf.type->Name+1);
						}
						SC_MustGetString();
						smf.models[index] = FindModel(path.GetChars(), sc_String);
						if (!smf.models[index])
						{
							Printf("%s: model not found\n", sc_String);
						}
					}
					else if (SC_Compare("scale"))
					{
						SC_MustGetFloat();
						smf.xscale=sc_Float;
						SC_MustGetFloat();
						smf.yscale=sc_Float;
						SC_MustGetFloat();
						smf.zscale=sc_Float;
					}
					else if (SC_Compare("skin"))
					{
						SC_MustGetNumber();
						index=sc_Number;
						if (index<0 || index>=MAX_MODELS_PER_FRAME)
						{
							SC_ScriptError("Too many models in %s", smf.type->Name+1);
						}
						SC_MustGetString();
						if (SC_Compare(""))
						{
							smf.skins[index]=NULL;
						}
						else
						{
							smf.skins[index]=LoadSkin(path.GetChars(), sc_String);
						}
					}
					else if (SC_Compare("frameindex") || SC_Compare("frame"))
					{
						bool isframe=!!SC_Compare("frame");

						SC_MustGetString();
						smf.sprite = -1;
						for (i = 0; i < (int)sprites.Size (); ++i)
						{
							if (strncmp (sprites[i].name, sc_String, 4) == 0)
							{
								if (sprites[i].numframes==0)
								{
									//SC_ScriptError("Sprite %s has no frames", sc_String);
								}
								smf.sprite = i;
								break;
							}
						}
						if (smf.sprite==-1)
						{
							SC_ScriptError("Unknown sprite %s in model definition for %s", sc_String, smf.type->Name+1);
						}

						SC_MustGetString();
						string framechars = sc_String;

						SC_MustGetNumber();
						index=sc_Number;
						if (index<0 || index>=MAX_MODELS_PER_FRAME)
						{
							SC_ScriptError("Too many models in %s", smf.type->Name+1);
						}
						if (isframe)
						{
							SC_MustGetString();
							if (smf.models[index]!=NULL) smf.modelframes[index] = smf.models[index]->FindFrame(sc_String);
							else smf.modelframes[index] = -1;
						}
						else
						{
							SC_MustGetNumber();
							smf.modelframes[index] = sc_Number;
						}

						for(i=0; framechars[i]>0; i++)
						{
							char map[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
							char c = toupper(framechars[i])-'A';

							if (c<0 || c>=29)
							{
								SC_ScriptError("Invalid frame character %c found", c+'A');
							}
							if (map[c]) continue;
							smf.frame=c;
							SpriteModelFrames.Push(smf);
							map[c]=1;
						}
					}
				}
			}
		}
	}

	// create a hash table for quick access
	SpriteModelHash = new int[SpriteModelFrames.Size ()];
	memset(SpriteModelHash, 0xff, SpriteModelFrames.Size () * sizeof(int));

	for (i = 0; i < (int)SpriteModelFrames.Size (); i++)
	{
		int j = ModelFrameHash(&SpriteModelFrames[i]) % SpriteModelFrames.Size ();

		SpriteModelFrames[i].hashnext = SpriteModelHash[j];
		SpriteModelHash[j]=i;
	}
}


//===========================================================================
//
// gl_FindModelFrame
//
//===========================================================================

FSpriteModelFrame * gl_FindModelFrame(const TypeInfo * ti, int sprite, int frame)
{
	FSpriteModelFrame smf;

	if (GetDefaultByType(ti)->hasmodel)
	{
		memset(&smf, 0, sizeof(smf));
		smf.type=ti;
		smf.sprite=sprite;
		smf.frame=frame;

		int hash = SpriteModelHash[ModelFrameHash(&smf) % SpriteModelFrames.Size()];

		while (hash>=0)
		{
			FSpriteModelFrame * smff = &SpriteModelFrames[hash];
			if (smff->type==ti && smff->sprite==sprite && smff->frame==frame) return smff;
			hash=smff->hashnext;
		}
	}
	return NULL;
}



void gl_RenderModel(GLSprite * spr, int cm)
{
	FSpriteModelFrame * smf = spr->modelframe;


	// Setup transformation.
	gl.MatrixMode(GL_MODELVIEW);
	gl.PushMatrix();
	gl.DepthFunc(GL_LEQUAL);

	// Model space => World space
	gl.Translatef(spr->x, spr->y, spr->z );

	// Model rotation.
	gl.Rotatef(180.0f+spr->actor->angle*90.0f/ANGLE_90, 0, 1, 0);

	// Scaling and model space offset.
	gl.Scalef(	
		spr->actor->xscale/63.0f * smf->xscale,
		spr->actor->yscale/63.0f * smf->zscale,
		spr->actor->xscale/63.0f * smf->yscale);

	//gl.Translatef(smf->xoffset, smf->zoffset, smf->yoffset);


	for(int i=0; i<MAX_MODELS_PER_FRAME; i++)
	{
		FModel * mdl = smf->models[i];

		if (mdl!=NULL)
		{
			mdl->RenderFrame(smf->skins[i], smf->modelframes[i], cm);
		}
	}

	gl.MatrixMode(GL_MODELVIEW);
	gl.PopMatrix();
	gl.DepthFunc(GL_LESS);

}