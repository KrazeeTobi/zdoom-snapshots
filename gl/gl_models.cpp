/*
** gl_models.cpp
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

#define MD2_MAGIC			0x32504449
#define DMD_MAGIC			0x4D444D44
#define NUMVERTEXNORMALS	162

static float   avertexnormals[NUMVERTEXNORMALS][3] = {
#include "tab_anorms.h"
};

static const float rModelAspectMod = 1 / 1.2f;	//.833334f;
enum { VX, VY, VZ };

TArray<FModel *> Models;
static TArray<FSpriteModelFrame> SpriteModelFrames;
static int * SpriteModelHash;
//TArray<FStateModelFrame> StateModelFrames;


//===========================================================================
//
// UnpackVector
//  Packed: pppppppy yyyyyyyy. Yaw is on the XY plane.
//
//===========================================================================

static void UnpackVector(unsigned short packed, float vec[3])
{
	float   yaw = (packed & 511) / 512.0f * 2 * PI;
	float   pitch = ((packed >> 9) / 127.0f - 0.5f) * PI;
	float   cosp = (float) cos(pitch);

	vec[VX] = (float) cos(yaw) * cosp;
	vec[VY] = (float) sin(yaw) * cosp;
	vec[VZ] = (float) sin(pitch);
}

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

static FTexture * LoadSkin(const char * path, const char * fn)
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
// FDMDModel::Load
//
//===========================================================================

bool FDMDModel::Load(const char * path, const char * buffer, int length)
{
	struct dmd_chunk_t
	{
		int             type;
		int             length;		   // Next chunk follows...
	};

#pragma pack(1)
	struct dmd_packedVertex_t
	{
		byte            vertex[3];
		unsigned short  normal;		   // Yaw and pitch.
	};

	struct dmd_packedFrame_t
	{
		float           scale[3];
		float           translate[3];
		char            name[16];
		dmd_packedVertex_t vertices[1];
	} ;
#pragma pack()

	// Chunk types.
	enum 
	{
		DMC_END,					   // Must be the last chunk.
		DMC_INFO					   // Required; will be expected to exist.
	};

	dmd_chunk_t * chunk = (dmd_chunk_t*)(buffer+12);
	char   *temp;
	ModelFrame *frame;
	int     i, k, c;
	FTriangle *triangles[MAX_LODS];
	int     axis[3] = { 0, 2, 1 };

	int fileoffset=12+sizeof(dmd_chunk_t);

	chunk->type = LONG(chunk->type);
	while(chunk->type != DMC_END)
	{
		switch (chunk->type)
		{
		case DMC_INFO:			// Standard DMD information chunk.
			memcpy(&info, buffer + fileoffset, LONG(chunk->length));
			info.skinWidth = LONG(info.skinWidth);
			info.skinHeight = LONG(info.skinHeight);
			info.frameSize = LONG(info.frameSize);
			info.numSkins = LONG(info.numSkins);
			info.numVertices = LONG(info.numVertices);
			info.numTexCoords = LONG(info.numTexCoords);
			info.numFrames = LONG(info.numFrames);
			info.numLODs = LONG(info.numLODs);
			info.offsetSkins = LONG(info.offsetSkins);
			info.offsetTexCoords = LONG(info.offsetTexCoords);
			info.offsetFrames = LONG(info.offsetFrames);
			info.offsetLODs = LONG(info.offsetLODs);
			info.offsetEnd = LONG(info.offsetEnd);
			fileoffset += chunk->length;
			break;

		default:
			// Just skip all unknown chunks.
			fileoffset += chunk->length;
			break;
		}
		// Read the next chunk header.
		chunk = (dmd_chunk_t*)(buffer+fileoffset);
		chunk->type = LONG(chunk->type);
		fileoffset += sizeof(dmd_chunk_t);
	}

	// Allocate and load in the data.
	skins = new DMDSkin[info.numSkins];

	for(i = 0; i < info.numSkins; i++)
	{
		skins[i].name = copystring(buffer + info.offsetSkins + i*64);
		skins[i].texture = LoadSkin(path, skins[i].name);
	}


	temp = (char*)buffer + info.offsetFrames;
	frames = new ModelFrame[info.numFrames];

	for(i = 0, frame = frames; i < info.numFrames; i++, frame++)
	{
		dmd_packedFrame_t *pfr = (dmd_packedFrame_t *) (temp + info.frameSize * i);
		dmd_packedVertex_t *pVtx;

		memcpy(frame->name, pfr->name, sizeof(pfr->name));
		frame->vertices = new FModelVertex[info.numVertices];
		frame->normals = new FModelVertex[info.numVertices];

		// Translate each vertex.
		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
		{
			UnpackVector(USHORT(pVtx->normal), frame->normals[k].xyz);
			for(c = 0; c < 3; c++)
			{
				frame->vertices[k].xyz[axis[c]] =
					pVtx->vertex[c] * FLOAT(pfr->scale[c]) + 
					FLOAT(pfr->translate[c]);
			}
			// Aspect undo.
			frame->vertices[k].xyz[1] *= rModelAspectMod;
		}
	}

	memcpy(lodInfo, buffer+info.offsetLODs, info.numLODs * sizeof(DMDLoDInfo));
	for(i = 0; i < info.numLODs; i++)
	{
		lodInfo[i].numTriangles = LONG(lodInfo[i].numTriangles);
		lodInfo[i].numGlCommands = LONG(lodInfo[i].numGlCommands);
		lodInfo[i].offsetTriangles = LONG(lodInfo[i].offsetTriangles);
		lodInfo[i].offsetGlCommands = LONG(lodInfo[i].offsetGlCommands);

		triangles[i] = (FTriangle*)(buffer + lodInfo[i].offsetTriangles);

		lods[i].glCommands = new int[lodInfo[i].numGlCommands];
		memcpy(lods[i].glCommands, buffer + lodInfo[i].offsetGlCommands, sizeof(int) * lodInfo[i].numGlCommands);
	}

	// Determine vertex usage at each LOD level.
	vertexUsage = new char[info.numVertices];
	memset(vertexUsage, 0, info.numVertices);

	for(i = 0; i < info.numLODs; i++)
		for(k = 0; k < lodInfo[i].numTriangles; k++)
			for(c = 0; c < 3; c++)
				vertexUsage[SHORT(triangles[i][k].vertexIndices[c])] |= 1 << i;

	loaded=true;
	return true;
}


FDMDModel::~FDMDModel()
{
	// clean up
}

//===========================================================================
//
// FDMDModel::FindFrame
//
//===========================================================================
int FDMDModel::FindFrame(const char * name)
{
	for (int i=0;i<info.numFrames;i++)
	{
		if (!stricmp(name, frames[i].name)) return i;
	}
	return -1;
}

//===========================================================================
//
// FMD2Model::Load
//
//===========================================================================

bool FMD2Model::Load(const char * path, const char * buffer, int length)
{
	// Internal data structures of MD2 files - only used during loading!
	struct md2_header_t
	{
		int             magic;
		int             version;
		int             skinWidth;
		int             skinHeight;
		int             frameSize;
		int             numSkins;
		int             numVertices;
		int             numTexCoords;
		int             numTriangles;
		int             numGlCommands;
		int             numFrames;
		int             offsetSkins;
		int             offsetTexCoords;
		int             offsetTriangles;
		int             offsetFrames;
		int             offsetGlCommands;
		int             offsetEnd;
	} ;

	struct md2_triangleVertex_t
	{
		byte            vertex[3];
		byte            lightNormalIndex;
	};

	struct md2_packedFrame_t
	{
		float           scale[3];
		float           translate[3];
		char            name[16];
		md2_triangleVertex_t vertices[1];
	};

	md2_header_t * md2header = (md2_header_t *)buffer;
	ModelFrame *frame;
	byte   *md2_frames;
	int     i, k, c;
	int     axis[3] = { 0, 2, 1 };

	// Convert it to DMD.
	header.magic = MD2_MAGIC;
	header.version = 8;
	header.flags = 0;
	vertexUsage = NULL;
	info.skinWidth = LONG(md2header->skinWidth);
	info.skinHeight = LONG(md2header->skinHeight);
	info.frameSize = LONG(md2header->frameSize);
	info.numLODs = 1;
	info.numSkins = LONG(md2header->numSkins);
	info.numTexCoords = LONG(md2header->numTexCoords);
	info.numVertices = LONG(md2header->numVertices);
	info.numFrames = LONG(md2header->numFrames);
	info.offsetSkins = LONG(md2header->offsetSkins);
	info.offsetTexCoords = LONG(md2header->offsetTexCoords);
	info.offsetFrames = LONG(md2header->offsetFrames);
	info.offsetLODs = LONG(md2header->offsetEnd);	// Doesn't exist.
	lodInfo[0].numTriangles = LONG(md2header->numTriangles);
	lodInfo[0].numGlCommands = LONG(md2header->numGlCommands);
	lodInfo[0].offsetTriangles = LONG(md2header->offsetTriangles);
	lodInfo[0].offsetGlCommands = LONG(md2header->offsetGlCommands);
	info.offsetEnd = LONG(md2header->offsetEnd);

	if (info.offsetFrames + info.frameSize * info.numFrames > length)
	{
		Printf("LoadModel: Model '%s' file too short\n");
		return false;
	}

	// The frames need to be unpacked.
	md2_frames = (byte*)buffer + info.offsetFrames;

	frames = new ModelFrame[info.numFrames];

	for(i = 0, frame = frames; i < info.numFrames; i++, frame++)
	{
		md2_packedFrame_t *pfr = (md2_packedFrame_t *) (md2_frames + info.frameSize * i);
		md2_triangleVertex_t *pVtx;

		memcpy(frame->name, pfr->name, sizeof(pfr->name));
		frame->vertices = new FModelVertex[info.numVertices];
		frame->normals = new FModelVertex[info.numVertices];

		// Translate each vertex.
		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
		{
			memcpy(frame->normals[k].xyz,
				avertexnormals[pVtx->lightNormalIndex], sizeof(float) * 3);

			for(c = 0; c < 3; c++)
			{
				frame->vertices[k].xyz[axis[c]] =
					(pVtx->vertex[c] * pfr->scale[c] + pfr->translate[c])/MAP_COEFF;
			}
			// Aspect ratio adjustment (1.33 -> 1.6.)
			frame->vertices[k].xyz[VY] *= rModelAspectMod;
		}
	}


	lods[0].glCommands = new int[lodInfo[0].numGlCommands];
	memcpy(lods[0].glCommands, buffer + lodInfo[0].offsetGlCommands, sizeof(int) * lodInfo[0].numGlCommands);
		
	skins = new DMDSkin[info.numSkins];

	for(i = 0; i < info.numSkins; i++)
	{
		skins[i].name = copystring(buffer + info.offsetSkins + i*64);
		skins[i].texture = LoadSkin(path, skins[i].name);
	}
	loaded=true;
	return true;
}

FMD2Model::~FMD2Model()
{
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
		bool res = model->Load(path, buffer, len);
		if (!res)
		{
			delete model;
			delete buffer;
			return NULL;
		}
	}
	else if (!memcmp(buffer, "IDP2", 4))
	{
		model = new FMD2Model;
		bool res = model->Load(path, buffer, len);
		if (!res)
		{
			delete model;
			delete buffer;
			return NULL;
		}
	}
	else
	{
		Printf("LoadModel: Unknown model format in '%s'\n", fullname);
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
				GetDefaultByType(smf.type)->hasmodel=true;
				if (!smf.type) SC_ScriptError("MODELDEF: Unknown actor type '%s'\n", sc_String);
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
					else if (SC_Compare("frameindex"))
					{
						SC_MustGetString();
						smf.sprite = -1;
						for (i = 0; i < (int)sprites.Size (); ++i)
						{
							if (strncmp (sprites[i].name, sc_String, 4) == 0)
							{
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
						SC_MustGetNumber();
						smf.modelframes[index] = sc_Number;

						for(i=0; framechars[i]>0; i++)
						{
							char map[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
							char c = toupper(framechars[i])-'A';

							if (c<0 || c>=29 || map[c]) continue;
							smf.frame=c;
							SpriteModelFrames.Push(smf);
							map[c]=1;
						}
					}
					else if (SC_Compare("frame"))
					{
						SC_MustGetString();

						smf.sprite = -1;
						for (i = 0; i < (int)sprites.Size (); ++i)
						{
							if (strncmp (sprites[i].name, sc_String, 4) == 0)
							{
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

						SC_MustGetString();
						
						if (smf.models[index]!=NULL) smf.modelframes[index] = smf.models[index]->FindFrame(sc_String);
						else smf.modelframes[index] = -1;


						for(i=0; framechars[i]>0; i++)
						{
							char map[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
							char c = toupper(framechars[i])-'A';

							if (c<0 || c>=29 || map[c]) continue;
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

		int hash = ModelFrameHash(&smf) % SpriteModelFrames.Size();

		while (hash>=0)
		{
			FSpriteModelFrame * smff = &SpriteModelFrames[SpriteModelHash[hash]];
			if (smff->type==ti && smff->sprite==sprite && smff->frame==frame) return smff;
			hash=smff->hashnext;
		}
	}
	return NULL;
}

//===========================================================================
//
// Render a set of GL commands using the given data.
//
//===========================================================================
static void RenderGLCommands(void *glCommands, unsigned int numVertices,FModelVertex * vertices)
{
	char   *pos;
	FGLCommandVertex * v;
	int     count;

	for(pos = (char*)glCommands; *pos;)
	{
		count = *(int *) pos;
		pos += 4;

		// The type of primitive depends on the sign.
		gl.Begin(count > 0 ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN);
		count = abs(count);

		while(count--)
		{
			v = (FGLCommandVertex *) pos;
			pos += sizeof(FGLCommandVertex);

			gl.TexCoord2fv(&v->s);
			gl.Vertex3fv((float*)&vertices[v->index]);
		}

		gl.End();
	}
}


void FDMDModel::RenderFrame(FTexture * skin, int frameno, int cm)
{
	int activeLod;

	if (frameno>=info.numFrames) return;

	ModelFrame * frame = &frames[frameno];
	//int mainFlags = mf->flags;



	gl.MatrixMode(GL_TEXTURE);
	gl.PushMatrix();

	if (!skin)
	{
		if (info.numSkins==0) return;
		skin = skins[0].texture;
		if (!skin) return;
	}

	FGLTexture * tex = FGLTexture::ValidateTexture(skin);

	tex->BindPatch(cm);

	// scale texture coordinates to the next largest power of 2 size
	if (!(gl.flags&RFL_NPOT_TEXTURE))
	{
		float w=skin->GetWidth();
		float h=skin->GetHeight();
		float scale_x=   w / (1<<(quickertoint(ceil(log10(w)/log10(2.0f))))); 
		float scale_y=   h / (1<<(quickertoint(ceil(log10(h)/log10(2.0f))))); 
		gl.Scalef(scale_x, scale_y, 1.0f);
	}


	// Now we can draw.
	int numVerts = info.numVertices;

	// Determine the suitable LOD.
	/*
	if(info.numLODs > 1 && rend_model_lod != 0)
	{
		float   lodFactor = rend_model_lod * screen->Width() / 640.0f / (currentFoV / 90.0f);
		if(lodFactor) lodFactor = 1 / lodFactor;

		// Determine the LOD we will be using.
		activeLod = (int) (lodFactor * spr->distance);
		if(activeLod < 0) activeLod = 0;
		if(activeLod >= mdl->info.numLODs) activeLod = mdl->info.numLODs - 1;
		vertexUsage = mdl->vertexUsage;
	}
	else
	*/
	{
		activeLod = 0;
	}


	RenderGLCommands(lods[activeLod].glCommands, numVerts, frame->vertices/*, modelColors, NULL*/);

	// We're done!
	gl.PopMatrix();
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