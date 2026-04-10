#ifndef __GL_FUNCT
#define __GL_FUNCT

#include "doomtype.h"
#include "templates.h"

class FArchive;

class AActor;
class FTexture;
class FFont;
struct GLSectorPlane;
struct sector_t;
class player_s;
struct GL_IRECT;
class FGLTexture;
class FCanvasTexture;

// gl_data.cpp

void gl_CheckNodes(int lumpnum);
bool gl_LoadGLNodes(int lumpnum);
void gl_InitData();
void gl_CleanLevelData();
void gl_PreprocessLevel(void);

inline void gl_CleanResources()
{
void gl_ResetSkies();
void gl_ResetGlow();

	gl_ResetSkies();
	gl_ResetGlow();
}


// Light + color

void gl_CalcLightTable(int usegamma);

extern float lighttable[256];
inline int gl_CalcLightLevel(int lightlevel) 
{
	return clamp<int>(lightlevel,0, 255);
}

inline bool gl_isBlack(PalEntry color)
{
	return color.r + color.g + color.b == 0;
}

inline bool gl_isWhite(PalEntry color)
{
	return color.r + color.g + color.b == 3*0xff;
}

void gl_GetLightColor(int light, int red, int green, int blue, float * pred, float * pgreen, float * pblue);
inline void gl_GetLightColor(int light, PalEntry pe, float * pred, float * pgreen, float * pblue)
{
	gl_GetLightColor(light,pe.r,pe.g,pe.b,pred,pgreen,pblue);
}

void gl_SetColor(int light, int red, int green, int blue, float alpha, PalEntry ThingColor = 0xffffff);
inline void gl_SetColor(int light, PalEntry pe, float alpha, PalEntry ThingColor = 0xffffff)
{
	gl_SetColor(light,pe.r,pe.g,pe.b,alpha, ThingColor);
}

void gl_SetSpriteLight( AActor * thing, int light, int red, int green, int blue, int desat, float alpha, PalEntry ThingColor = 0xffffff);
inline void gl_SetSpriteLight(AActor * thing, int light, PalEntry pe, float alpha, PalEntry ThingColor = 0xffffff)
{
	gl_SetSpriteLight(thing, light,pe.r,pe.g,pe.b,pe.a,alpha, ThingColor);
}

struct particle_t;
void gl_SetSpriteLight( particle_t * thing, int lightlevel, int red, int green, int blue, int desaturation, float alpha, PalEntry ThingColor);
inline void gl_SetSpriteLight(particle_t * thing, int light, PalEntry pe, float alpha, PalEntry ThingColor = 0xffffff)
{
	gl_SetSpriteLight(thing, light,pe.r,pe.g,pe.b,pe.a,alpha, ThingColor);
}

void gl_InitFog();
void gl_SetFogParams(int _fogdensity, PalEntry _outsidefogcolor, int _outsidefogdensity, int _skyfog);
int gl_GetFogDensity(int lightlevel, PalEntry fogcolor);
void gl_SetFog(int lightlevel, PalEntry pe, int renderstyle);

// textures + sprites

void gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FGLTexture * gltexture);

void gl_InitShaders();
void gl_EnableShader(bool on);

// These are wrappers around the different means to pass this data
// to either the shader or the conventional GL state.
void gl_FogColor(float *  rgb);
void gl_FogDensity(float dens);
void gl_EnableFog(bool on);
void gl_SetCamera(float x, float y, float z);
bool gl_SetColorMode(int cm, bool force=false);
void gl_EnableTexture(bool on);


int gl_GetSpriteFrame(unsigned sprite, int frame, int rot, angle_t angle=0);


#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

// Render

void gl_RenderBSPNode (void *node);
sector_t * gl_FakeFlat(sector_t * sec, sector_t * dest, bool back);

#define INVALID_SPRITE 0xffffff

// Data

void SaveGFX(const char * fn, unsigned char * buffer, int w, int h);
PalEntry averageColor(const unsigned long *data, int size, bool maxout);
void gl_ScreenShot(const char *filename);
bool gl_SetShader(int cmap);
void gl_Init(int width, int height);

void gl_DrawLine(int x0, int y0, int x1, int y1, int BaseColor);
void gl_Dim (PalEntry color, float amount, int x1, int y1, int w, int h);
void gl_FlatFill (int left, int top, int right, int bottom, FTexture *src);
void gl_Clear (int left, int top, int right, int bottom, int color);

struct FTexInfo
{
   FTexture *tex;
   FFont * font;
   float x;
   float y;
   float width;
   float height;
   int clipLeft, clipRight, clipTop, clipBottom;
   int windowLeft, windowRight;
   const BYTE *translation;
   bool loadAlpha, flipX;
   float alpha;
   int fillColor;
};

void gl_DrawTexture(FTexInfo *texInfo);
void gl_DrawBuffer(byte * buffer, int width, int height, int x, int y, int dx, int dy, PalEntry * palette);
void gl_DrawCanvas(DCanvas * canvas, int x, int y, int dx, int dy, PalEntry * palette);
void gl_DrawSavePic(DCanvas * canvas, const char * Filename, int x, int y, int dx, int dy);


// Scene

void gl_Set2DMode();
void gl_SetupView(fixed_t viewx, fixed_t viewy, fixed_t viewz, angle_t viewangle, bool mirror, bool nosectorclear=false);
void gl_SetViewArea();
void gl_DrawScene();
void gl_EndDrawScene();
void gl_RenderView (AActor * camera, GL_IRECT * bounds, float fov, float ratio, bool mainview);
void gl_RenderPlayerView(player_s *player);   // Called by G_Drawer.
void gl_RenderViewToCanvas(DCanvas * pic, int x, int y, int width, int height);
void gl_RenderTextureView(FCanvasTexture *Texture, AActor * Viewpoint, int FOV);
angle_t gl_FrustumAngle();

void gl_LinkLights();

//missing textures

void AddUpperMissingTexture(seg_t * seg, fixed_t backheight);
void AddLowerMissingTexture(seg_t * seg, fixed_t backheight);
void HandleMissingTextures();
void DrawUnhandledMissingTextures();
void AddHackedSubsector(subsector_t * sub);
void HandleHackedSubsectors();
void AddFloorStack(subsector_t * sub);
void AddCeilingStack(subsector_t * sub);
void ProcessSectorStacks();

// ZDBSP shittiness compensation
void gl_CollectMissingLines();
void gl_RenderMissingLines();


void gl_SetActorLights(AActor *);
void gl_DeleteAllAttachedLights();
void gl_RecreateAllAttachedLights();


void I_RestartRenderer();
void I_CheckRestartRenderer();

EXTERN_CVAR(Int, screenblocks)

#endif
