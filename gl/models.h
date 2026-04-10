
#ifndef __MODELS_H
#define __MODELS_H

#define MD2_MAGIC			0x32504449
#define NUMVERTEXNORMALS	162
#define MAX_MODELS			768

// "DMDM" = Doomsday/Detailed MoDel Magic
#define DMD_MAGIC			0x4D444D44
#define MAX_LODS			4

enum { VX, VY, VZ };

struct gl_vertex_t 
{
	float           xyz[4];		   // The fourth is padding.
};

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

struct md2_modelVertex_t
{
	float           vertex[3];
	byte            lightNormalIndex;
};

// Translated frame (vertices in model space).
struct md2_frame_t
{
	char            name[16];
	md2_modelVertex_t *vertices;
};

struct md2_skin_t
{
	char            name[256];
	int             id;
};

struct md2_triangle_t
{
	short           vertexIndices[3];
	short           textureIndices[3];
};

struct md2_textureCoordinate_t
{
	short           s, t;
};

struct md2_glCommandVertex_t
{
	float           s, t;
	int             vertexIndex;
};



struct  dmd_header_t
{
	int             magic;
	int             version;
	int             flags;
};

// Chunk types.
enum 
{
	DMC_END,					   // Must be the last chunk.
	DMC_INFO					   // Required; will be expected to exist.
};

struct dmd_chunk_t
{
	int             type;
	int             length;		   // Next chunk follows...
};

struct dmd_info_t
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

struct dmd_levelOfDetail_t
{
	int             numTriangles;
	int             numGlCommands;
	int             offsetTriangles;
	int             offsetGlCommands;
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


struct dmd_skin_t
{
	char            name[256];
	int             id;
} ;

struct dmd_triangle_t
{
	short           vertexIndices[3];
	short           textureIndices[3];
};

struct dmd_textureCoordinate_t
{
	short           s, t;
};

struct  dmd_glCommandVertex_t
{
	float           s, t;
	int             vertexIndex;
};

struct  dmd_lod_t
{
	//dmd_triangle_t    *triangles;
	int            *glCommands;
};

struct model_vertex_t
{
	float           xyz[3];
};


struct model_frame_t
{
	char            name[16];
	model_vertex_t *vertices;
	model_vertex_t *normals;
};


struct model_t
{
	bool         loaded;
	char            fileName[256]; // Name of the md2 file.
	dmd_header_t    header;
	dmd_info_t      info;
	dmd_skin_t     *skins;
	//dmd_textureCoordinate_t *texCoords;
	model_frame_t  *frames;
	dmd_levelOfDetail_t lodInfo[MAX_LODS];
	dmd_lod_t       lods[MAX_LODS];
	char           *vertexUsage;   // Bitfield for each vertex.
	bool         allowTexComp;  // Allow texture compression with this.
};

struct glcommand_vertex_t 
{
	float           s, t;
	int             index;
};

class GLSprite;
class FGLTexture;

void R_ClearModels();
model_t * R_LoadModel(const char * filename);
void Mod_RenderModel(GLSprite * spr, model_t * mdl, int framenumber);
bool Mod_GetModelForSprite(int sprite, int frame, model_t ** pModel, FGLTexture ** pTexture);


#endif