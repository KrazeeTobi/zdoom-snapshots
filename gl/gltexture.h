
#ifndef __GLTEXTURE_H
#define __GLTEXTURE_H

#define SHADED_TEXTURE -1
#define DIRECT_PALETTE -2

#include "gl/gl_values.h"
#include "tarray.h"

class FCanvasTexture;
class AActor;

class GLTexture
{
	friend void gl_RenderTextureView(FCanvasTexture *Texture, AActor * Viewpoint, int FOV);

	static struct TexFilter_s
	{
		int minfilter;
		int magfilter;
		bool mipmapping;
	} 
	TexFilter[];

	static struct TexFormat_s
	{
		int texformat;
	}
	TexFormat[];

	struct TranslatedTexture
	{
		unsigned int glTexID;
		int translation;
		int cm;
	};

public:

	static unsigned int lastbound;
	//static int tex_format;
	//static int tex_filter_min;
	//static int tex_filter_mag;
	//static bool use_mipmapping;
	static bool supportsNonPower2;
	static int max_texturesize;

	static int GetTexDimension(int value);

	//static void SetTextureFormat(const char * );
	//static void SetTextureFilter(const char * );

private:

	//short width,height;
	short tex_width,tex_height;
	short realtexwidth, realtexheight;
	bool mipmap;
	byte cm_arraysize;

	unsigned int * glTexID;
	TArray<TranslatedTexture> glTexID_Translated;

	void LoadImage(unsigned char * buffer,unsigned int & glTexID,int wrapparam, bool alphatexture=false);
	unsigned * GetTexID(int cm, int translation, const unsigned char * translationtbl);

public:
	GLTexture(int w, int h, bool mip);
	~GLTexture();

	unsigned int Bind(int cm, int translation=0, const unsigned char * translationtbl=NULL);
	unsigned int CreateTexture(unsigned char * buffer, bool wrap, int cm, int translation=0, const unsigned char * translationtbl=NULL);

	void Clean(bool all);


	// Get right/bottom UV coordinates for patch drawing
	float GetUR() { return (float)realtexwidth/(float)tex_width; }
	float GetVB() { return (float)realtexheight/(float)tex_height; }
	float GetU(float upix) { return upix/(float)tex_width; }
	float GetV(float vpix) { return vpix/(float)tex_height; }

	// gets a texture coordinate from a pixel coordinate
	float FloatToTexU(float v) { return v/(float)realtexwidth; }
	float FixToTexU(int v) { return (float)v/(float)FRACUNIT/(float)realtexwidth; }
	float FixToTexV(int v) { return (float)v/(float)FRACUNIT/(float)realtexheight; }
};


#endif