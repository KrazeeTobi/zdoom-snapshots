#include "gl_pch.h"

//===========================================================================
//
// One rather messy source file with code from Doomsday so the models in 
// Silent Steel can be properly displayed.
//
// Don't expect this to be of much use. ;)
//
//===========================================================================
#include "w_wad.h"
#include "sc_man.h"

#include "gl/gl_renderstruct.h"
#include "gl/gl_functions.h"
#include "gl/models.h"
#include "gl/gl_texture.h"
#include "gl/gltexture.h"


float   avertexnormals[NUMVERTEXNORMALS][3] = {
#include "tab_anorms.h"
};


TArray<model_t*> modellist;
float   rModelAspectMod = 1 / 1.2f;	//.833334f;
model_t ** spritemodels;
int * modelskins;


void R_ClearModels()
{
	/*
	for(int i=0;i<modellist.Size();i++)
	{
		// No, this doesn't free all the data!
		delete model[i]->frames;
		delete modellist[i];
	}
	*/
	if (modelskins) delete [] modelskins;
	modelskins=NULL;
	if (spritemodels) delete [] spritemodels;
	spritemodels=NULL;
	modellist.Clear();
}

//===========================================================================
// AllocAndLoad
//===========================================================================
static void *AllocAndLoad(FileReader & file, int offset, int len)
{
	void   *ptr = new char[len];

	file.Seek(offset, SEEK_SET);
	file.Read(ptr, len);
	return ptr;
}


//===========================================================================
// R_FindModelFor
//===========================================================================
int R_FindModelFor(const char *filename)
{
	int     i;

	for(i = 0; i < modellist.Size(); i++)
		if (!stricmp(modellist[i]->fileName, filename)) return i;
	return -1;
}


//===========================================================================
// R_LoadModelMD2
//===========================================================================
void R_LoadModelMD2(FileReader & f, model_t * mdl)
{
	md2_header_t oldhd;
	dmd_header_t *hd = &mdl->header;
	dmd_info_t *inf = &mdl->info;
	model_frame_t *frame;
	byte   *frames;
	int     i, k, c;
	static const int axis[3] = { 0, 2, 1 };

	f.Read(&oldhd, sizeof(oldhd));

	// Convert it to DMD.
	hd->magic = MD2_MAGIC;
	hd->version = 8;
	hd->flags = 0;
	mdl->vertexUsage = NULL;
	inf->skinWidth = oldhd.skinWidth;
	inf->skinHeight = oldhd.skinHeight;
	inf->frameSize = oldhd.frameSize;
	inf->numLODs = 1;
	inf->numSkins = oldhd.numSkins;
	inf->numTexCoords = oldhd.numTexCoords;
	inf->numVertices = oldhd.numVertices;
	inf->numFrames = oldhd.numFrames;
	inf->offsetSkins = oldhd.offsetSkins;
	inf->offsetTexCoords = oldhd.offsetTexCoords;
	inf->offsetFrames = oldhd.offsetFrames;
	inf->offsetLODs = oldhd.offsetEnd;	// Doesn't exist.
	mdl->lodInfo[0].numTriangles = oldhd.numTriangles;
	mdl->lodInfo[0].numGlCommands = oldhd.numGlCommands;
	mdl->lodInfo[0].offsetTriangles = oldhd.offsetTriangles;
	mdl->lodInfo[0].offsetGlCommands = oldhd.offsetGlCommands;
	inf->offsetEnd = oldhd.offsetEnd;

	// Allocate and load the various arrays.
	/*mdl->texCoords = AllocAndLoad(file, inf->offsetTexCoords, 
	   sizeof(md2_textureCoordinate_t) * inf->numTexCoords); */

	/*mdl->lods[0].triangles = AllocAndLoad(file, 
	   mdl->lodInfo[0].offsetTriangles,
	   sizeof(md2_triangle_t) * mdl->lodInfo[0].numTriangles); */

	// The frames need to be unpacked.

	frames = (unsigned char*)AllocAndLoad(f, inf->offsetFrames, inf->frameSize * inf->numFrames);
	mdl->frames = new model_frame_t[inf->numFrames];

	for(i = 0, frame = mdl->frames; i < inf->numFrames; i++, frame++)
	{
		md2_packedFrame_t *pfr =
			(md2_packedFrame_t *) (frames + inf->frameSize * i);
		md2_triangleVertex_t *pVtx;

		memcpy(frame->name, pfr->name, sizeof(pfr->name));
		frame->vertices = new model_vertex_t[inf->numVertices];
		frame->normals = new model_vertex_t[inf->numVertices];

		// Translate each vertex.
		for(k = 0, pVtx = pfr->vertices; k < inf->numVertices; k++, pVtx++)
		{
			memcpy(frame->normals[k].xyz,
				   avertexnormals[pVtx->lightNormalIndex], sizeof(float) * 3);

			for(c = 0; c < 3; c++)
			{
				frame->vertices[k].xyz[axis[c]] =
					(pVtx->vertex[c] * pfr->scale[c] + pfr->translate[c])/MAP_COEFF;
			}
			// Aspect undoing.
			frame->vertices[k].xyz[VY] *= rModelAspectMod;
		}
	}
	delete frames;

	mdl->lods[0].glCommands = (int*)
		AllocAndLoad(f, mdl->lodInfo[0].offsetGlCommands, sizeof(int) * mdl->lodInfo[0].numGlCommands);

#if 0
	// Load skins.
	mdl->skins = calloc(sizeof(dmd_skin_t), inf->numSkins);
	F_Seek(file, inf->offsetSkins, SEEK_SET);
	for(i = 0; i < inf->numSkins; i++)
		F_Read(mdl->skins[i].name, 64, file);
#endif
}


//===========================================================================
// R_LoadModel
//  Finds the existing model or loads in a new one.
//===========================================================================
model_t * R_LoadModel(const char * filename)
{
	int index;
	model_t *mdl;

	int lump=Wads.GetNumForName(filename);
	if(lump<0) return NULL;				// No model specified.

	// Has this been already loaded?
	index = R_FindModelFor(filename);
	if (index>=0) return modellist[index];

	int len = Wads.LumpLength (lump);
	FWadLump f=Wads.OpenLumpNum(lump);

	// Now we can load in the data.
	mdl=new model_t;
	f.Read(&mdl->header, sizeof(mdl->header));
	if(mdl->header.magic == MD2_MAGIC)	// Load as MD2?
	{
		f.Seek(0,0);
		R_LoadModelMD2(f, mdl);
	}
	/*
	else if(mdl->header.magic == DMD_MAGIC)	// Load as DMD?
	{
		R_LoadModelDMD(file, mdl);
	}
	*/
	else
	{
		// Bad magic! Cancel the loading.
		Printf(PRINT_BOLD,"%s: Bad model\n",filename);
		return NULL;
	}

	// We're done.
	mdl->loaded = true;
	mdl->allowTexComp = true;
	strcpy(mdl->fileName, filename);

	// Determine the actual (full) paths.
	/*
	for(i = 0; i < mdl->info.numSkins; i++)
	{
		R_RegisterModelSkin(mdl, i);
	}
	*/

	modellist.Push(mdl);
	return mdl;
}



//===========================================================================
/*
 * Render a set of GL commands using the given data.
 */
//===========================================================================
void Mod_RenderCommands(void *glCommands, unsigned int numVertices,model_vertex_t * vertices)
{
	byte   *pos;
	glcommand_vertex_t *v;
	int     count;

	for(pos = (byte*)glCommands; *pos;)
	{
		count = *(int *) pos;
		pos += 4;

		// The type of primitive depends on the sign.
		glBegin(count > 0 ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN);
		if(count < 0) count = -count;

		// Increment the total model triangle counter.
		//model_tri_count += count - 2;

		while(count--)
		{
			v = (glcommand_vertex_t *) pos;
			pos += sizeof(glcommand_vertex_t);

			glTexCoord2fv(&v->s);
			glVertex3fv((float*)&vertices[v->index]);
		}

		// The primitive is complete.
		glEnd();
	}
}

//===========================================================================
/*
 * Render a submodel from the GLSprite.
 */
//===========================================================================
void Mod_RenderModel(GLSprite * spr, model_t * mdl, int framenumber)
{
	int activeLod;
	int     numVerts;

	if (framenumber>=mdl->info.numFrames) return;

	model_frame_t *frame = &mdl->frames[framenumber];
	//int mainFlags = mf->flags;


	// Setup transformation.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Model space => World space
	glTranslatef(spr->x, spr->y, spr->z );
		
	// Model rotation.
	glRotatef(180.0f+spr->actor->angle*90.0f/ANGLE_90, 0, 1, 0);

	//gl.Rotatef(spr->data.mo.viewaligned ? spr->data.mo.v2[VY] : pitchAngle, 0, 0, 1);

	// Scaling and model space offset.
	glScalef(	spr->actor->xscale/63.0f,
				spr->actor->yscale/63.0f,
				spr->actor->xscale/63.0f);
	//glTranslatef(smf->offset[VX], smf->offset[VY], smf->offset[VZ]);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();

	// scale texture coordinates to the next largest power of 2 size
	if (!GLTexture::supportsNonPower2)
	{
		float scale_x=   (float)mdl->info.skinWidth / (1<<(quickertoint(ceil(log10((float)mdl->info.skinWidth )/log10(2.0f))))); 
		float scale_y=   (float)mdl->info.skinHeight/ (1<<(quickertoint(ceil(log10((float)mdl->info.skinHeight)/log10(2.0f))))); 
		glScalef(scale_x, scale_y, 1.0f);
	}


	// Now we can draw.
	numVerts = mdl->info.numVertices;

	// Determine the suitable LOD.
	/*
	if(mdl->info.numLODs > 1 && rend_model_lod != 0)
	{
		float   lodFactor =
			rend_model_lod * screenWidth / 640.0f / (fieldOfView / 90.0f);
		if(lodFactor)
			lodFactor = 1 / lodFactor;

		// Determine the LOD we will be using.
		activeLod = (int) (lodFactor * spr->distance);
		if(activeLod < 0)
			activeLod = 0;
		if(activeLod >= mdl->info.numLODs)
			activeLod = mdl->info.numLODs - 1;
		vertexUsage = mdl->vertexUsage;
	}
	else
	*/
	{
		activeLod = 0;
	}

	glDepthFunc(GL_LEQUAL);

	Mod_RenderCommands(mdl->lods[activeLod].glCommands, numVerts, frame->vertices/*, modelColors, NULL*/);

	// We're done!
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDepthFunc(GL_LESS);
}



void gl_InitModels()
//void Mod_LoadModels()
{
	int lump = Wads.CheckNumForName("MODELS");

	if (lump>=0) try
	{
		SC_OpenLumpNum(lump, "MODELS");
		spritemodels=new model_t*[sprites.Size()];
		modelskins=new int[sprites.Size()];
		memset(spritemodels, 0, sizeof(*spritemodels)*sprites.Size());
		memset(modelskins, 0, sizeof(*modelskins)*sprites.Size());
		while (SC_GetString())
		{
			for (int i=0;i<sprites.Size();i++)
			{
				if (!stricmp(sprites[i].name, sc_String))
				{
					SC_GetString();
					spritemodels[i] = R_LoadModel(sc_String);
					modelskins[i] = TexMan.GetTexture(sc_String, FTexture::TEX_Override);
				}
			}
		}
	}
	catch(...)
	{
		// No errors if this fails!
	}
}

bool Mod_GetModelForSprite(int sprite, int frame, model_t ** pModel, FGLTexture ** pTexture)
{
	// This is a cheap useless hack!
	if (spritemodels != NULL && sprite>0 && sprite<sprites.Size() && spritemodels[sprite])
	{
		*pModel=spritemodels[sprite];
		*pTexture=FGLTexture::ValidateTexture(modelskins[sprite], false);
		return true;
	}
	else
	{
		*pModel=NULL;
	}
	return false;
}

