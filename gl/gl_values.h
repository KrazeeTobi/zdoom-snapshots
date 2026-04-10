#ifndef __GL_VALUES
#define __GL_VALUES

#include "doomtype.h"

enum GLDrawItemType
{
	GLDIT_WALL,
	GLDIT_FLAT,
	GLDIT_SPRITE,
	GLDIT_POLY,
};

enum DrawListType
{
	GLDL_TRANSLUCENT,
	GLDL_UNLIT,
	GLDL_LIT,
	GLDL_LITFOG,
	GLDL_MASKED,
	GLDL_SKY,
	GLDL_TRANSLUCENTBORDER,


	GLDL_TYPES
};

enum Drawpasses
{
	GLPASS_UNLIT,
	GLPASS_DEPTH,
	GLPASS_BASE,
	GLPASS_LIGHT,
	GLPASS_TEXTURE,
	GLPASS_FOG,
	GLPASS_DECALS
};

enum WallTypes
{
	RENDERWALL_NONE,
	RENDERWALL_TOP,
	RENDERWALL_M1S,
	RENDERWALL_M2S,
	RENDERWALL_BOTTOM,
	RENDERWALL_SKY,
	RENDERWALL_DECAL,
	RENDERWALL_FOGSHEET,
	RENDERWALL_HORIZON,
	RENDERWALL_SKYBOX,
	RENDERWALL_SECTORSTACK,
	RENDERWALL_MIRROR,
	RENDERWALL_MIRRORSURFACE,
	RENDERWALL_M2SNF,
	RENDERWALL_COLOR,
	RENDERWALL_FFBLOCK,
	// Insert new types at the end!
};


enum SectorRenderFlags
{
	// This is used to avoid creating too many drawinfos
	SSRF_RENDERFLOOR=1,
	SSRF_RENDERCEILING=2,
	SSRF_RENDER3DPLANES=4,
	SSRF_RENDERALL=7,
	SSRF_PROCESSED=8,
};

enum EColorManipulation
{
	CM_INVALID=-1,
	CM_DEFAULT=0,					// untranslated
	CM_DESAT0=CM_DEFAULT,
	CM_DESAT1,					// minimum desaturation
	CM_DESAT31=CM_DESAT1+30,	// maximum desaturation = grayscale
	CM_INVERT,					// Doom's invulnerability colormap
	CM_GOLDMAP,					// Heretic's invulnerability colormap
	CM_SHADE,					// alpha channel texture
	CM_LIMIT,					// Max. manipulation value for regular textures. Everything above is for special use.
	CM_FIRSTCOLORMAP=CM_LIMIT,	// Boom colormaps

	CM_LITE=246,				// special values to handle these items without excessive hacking
	CM_TORCH=247,				// These are not real color manipulations
};

#define MAP_COEFF 128.0f
#define FOG_COEFF (MAP_COEFF/128.f)
#define MAP_SCALE	(MAP_COEFF*(float)FRACUNIT)
#define TO_MAP(v) ((float)(v)/(float)MAP_SCALE)
#define FROM_MAP(f) (fixed_t)(f*MAP_SCALE)
#define F_TO_MAP(v) ((v)/(float)MAP_COEFF)
#define MIN_EQ (0.0005f/MAP_COEFF)


#endif