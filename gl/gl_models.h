#ifndef __GL_MODELS_H_
#define __GL_MODELS_H_

#include "r_data.h"

#define MAX_LODS			4


struct FTriangle
{
	short           vertexIndices[3];
	short           textureIndices[3];
};

struct FModelVertex
{
	float           xyz[3];
};

struct FTexCoord
{
	short           s, t;
};

struct FGLCommandVertex
{
	float           s, t;
	int             index;
};


class FModel
{
public:
	FModel() { filename = NULL; }
	virtual ~FModel() { if (filename!=NULL) delete [] filename; }

	virtual bool Load(const char * fn, const char * buffer, int length) = 0;
	virtual int FindFrame(const char * name) = 0;
	virtual void RenderFrame(FTexture * skin, int frame, int cm) = 0;

	char * filename;
};

class FDMDModel : public FModel
{
protected:
	struct DMDHeader
	{
		int             magic;
		int             version;
		int             flags;
	};

	struct DMDInfo
	{
		int             skinWidth;
		int             skinHeight;
		int             frameSize;
		int             numSkins;
		int             numVertices;
		int             numTexCoords;
		int             numFrames;
		int             numLODs;
		int             offsetSkins;
		int             offsetTexCoords;
		int             offsetFrames;
		int             offsetLODs;
		int             offsetEnd;
	};

	struct DMDSkin
	{
		char            * name;
		FTexture		* texture;
		int             id;

		DMDSkin()
		{
			name = NULL;
			texture = NULL;
			id = 0;
		}

		~DMDSkin()
		{
			if (name) delete [] name;
			if (texture) delete [] texture;
		}
	} ;

	struct ModelFrame
	{
		char            name[16];
		FModelVertex *vertices;
		FModelVertex *normals;

		ModelFrame()
		{
			vertices = normals = NULL;
		}

		~ModelFrame()
		{
			if (vertices) delete [] vertices;
			if (normals) delete [] normals;
		}
	};

	struct DMDLoDInfo
	{
		int             numTriangles;
		int             numGlCommands;
		int             offsetTriangles;
		int             offsetGlCommands;
	};

	struct DMDLoD
	{
		FTriangle		* triangles;
		int				* glCommands;
	};


	bool			loaded;
	DMDHeader	    header;
	DMDInfo			info;
	DMDSkin *		skins;
	FTexCoord *		texCoords;
	
	ModelFrame  *	frames;
	DMDLoDInfo		lodInfo[MAX_LODS];
	DMDLoD			lods[MAX_LODS];
	char           *vertexUsage;   // Bitfield for each vertex.
	bool			allowTexComp;  // Allow texture compression with this.

public:
	FDMDModel() { loaded = false; }
	virtual ~FDMDModel();

	virtual bool Load(const char * fn, const char * buffer, int length);
	virtual int FindFrame(const char * name);
	virtual void RenderFrame(FTexture * skin, int frame, int cm);

};

// This uses the same internal representation as DMD
class FMD2Model : public FDMDModel
{
public:
	FMD2Model() {}
	virtual ~FMD2Model();

	virtual bool Load(const char * fn, const char * buffer, int length);

};


#define MAX_MODELS_PER_FRAME 4

struct FSpriteModelFrame
{
	FModel * models[MAX_MODELS_PER_FRAME];
	FTexture * skins[MAX_MODELS_PER_FRAME];
	int modelframes[MAX_MODELS_PER_FRAME];
	float xscale, yscale, zscale;
	const TypeInfo * type;
	short sprite;
	short frame;
	FState * state;	// for later!
	int hashnext;
};

class GLSprite;

void gl_InitModels();
FSpriteModelFrame * gl_FindModelFrame(const TypeInfo * ti, int sprite, int frame);
void gl_RenderModel(GLSprite * spr, int cm);


#endif