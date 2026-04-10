
#ifndef __GL_GLOW
#define __GL_GLOW

#pragma warning(disable:4018)

EXTERN_CVAR(Int, wallglowheight)
EXTERN_CVAR(Float, wallglowfactor)

extern bool * GlowingTextures;
extern int MaxGlowingTexture;

void gl_InitGlow(const char * lumpnm);
void gl_GetGlowColor(unsigned int texno, float * data);


inline bool gl_isGlowingTexture(unsigned int texno)
{
	if (texno<(unsigned)MaxGlowingTexture && GlowingTextures) return GlowingTextures[texno];
	return false;
}


#endif
