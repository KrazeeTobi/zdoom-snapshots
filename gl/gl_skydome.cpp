#include "gl_pch.h"
/*
** gl_sky.cpp
**
** Draws the sky.  Loosely based on the JDoom sky and the ZDoomGL 0.66.2 sky.
**
**---------------------------------------------------------------------------
** Copyright 2003 Tim Stump
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
#include "gl/gl_portal.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "gl/gl_intern.h"


//
// Shamelessly lifted from Doomsday (written by Jaakko Keränen)
// also shamelessly lifted from ZDoomGL! ;)
//


CVAR (Int, gl_sky_detail, 16, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
EXTERN_CVAR (Bool, r_stretchsky)

static PalEntry * SkyColors;
size_t MaxSkyTexture;	// TexMan.NumTextures can't be used because it may change after allocating the array here!
extern int skyfog;

void gl_ResetSkies()
{
	if (SkyColors) delete [] SkyColors;
	SkyColors=NULL;
	MaxSkyTexture=0;
}


//===========================================================================
// 
//	Gets the average color of a texture for use as a sky cap color
//
//===========================================================================
static PalEntry SkyCapColor(unsigned int texno, bool bottom)
{
	PalEntry col;

	if (!SkyColors)
	{
		MaxSkyTexture=TexMan.NumTextures();
		SkyColors=new PalEntry[MaxSkyTexture*2];	// once for top cap, once for bottom cap
		memset(SkyColors, 0, sizeof(PalEntry)*MaxSkyTexture);
	}

	if (texno<MaxSkyTexture)
	{
		if (SkyColors[texno].a==0)
		{
			FGLTexture * tex = FGLTexture::ValidateTexture(texno);
			if (tex)
			{
				unsigned char * buffer = tex->CreateTexBuffer(CM_DEFAULT, 0);

				if (buffer)
				{
					int w=tex->GetWidth();
					int h=tex->GetHeight();
					SkyColors[texno]=averageColor((unsigned long *) buffer, w * MIN(30, h), false);
					if (h>30)
					{
						SkyColors[texno+MaxSkyTexture]=	averageColor(((unsigned long *) buffer)+(h-30)*w, w * 30, false);
					}
					else SkyColors[texno+MaxSkyTexture]=SkyColors[texno];
					delete buffer;
					SkyColors[texno].a=1;	// mark as processed
				}
			}
		}
		return SkyColors[texno+MaxSkyTexture*bottom];
	}
	else
	{
		return 0;
	}
}




// The texture offset to be applied to the texture coordinates in SkyVertex().
static angle_t maxSideAngle = ANGLE_180 / 3;
static float texoff;
static int rows, columns;	
static fixed_t scale = 10000 << FRACBITS;
static bool yflip;
static int texw;
static float yMult, yAdd;
static bool foglayer;
static bool secondlayer;
static float R,G,B;

#define SKYHEMI_UPPER		0x1
#define SKYHEMI_LOWER		0x2
#define SKYHEMI_JUST_CAP	0x4	// Just draw the top or bottom cap.


static void SkyVertex(int r, int c)
{
	angle_t topAngle= (angle_t)(c / (float)columns * ANGLE_MAX);
	angle_t sideAngle = maxSideAngle * (rows - r) / rows;
	fixed_t height = finesine[sideAngle>>ANGLETOFINESHIFT];
	fixed_t realRadius = FixedMul(scale, finecosine[sideAngle>>ANGLETOFINESHIFT]);
	fixed_t x = FixedMul(realRadius, finecosine[topAngle>>ANGLETOFINESHIFT]);
	fixed_t y = (!yflip) ? FixedMul(scale, height) : FixedMul(scale, height) * -1;
	fixed_t z = FixedMul(realRadius, finesine[topAngle>>ANGLETOFINESHIFT]);
	float fx, fy, fz;
	float color = r * 1.f / rows;
	float u, v;
	float timesRepeat;
	
	timesRepeat = (short)(4 * (256.f / texw));
	if (timesRepeat == 0.f) timesRepeat = 1.f;
	
	if (!foglayer)
	{
		gl_SetColor(255, 255, 255, 255,r==0? 0.0f : 1.0f);
		
		// And the texture coordinates.
		if(!yflip)	// Flipped Y is for the lower hemisphere.
		{
			u = (-timesRepeat * c / (float)columns) ;//* yMult;
			v = (r / (float)rows) * 1.f * yMult + yAdd;
		}
		else
		{
			u = (-timesRepeat * c / (float)columns) ;//* yMult;
			v = ((rows-r)/(float)rows) * 1.f * yMult + yAdd;
		}
		
		
		glTexCoord2f(u, v);
	}
	// And finally the vertex.
	fx = x / (float)FRACUNIT;
	fy = y / (float)FRACUNIT;
	fz = z / (float)FRACUNIT;
	glVertex3f(fx, fy - 1.f, fz);
}


// Hemi is Upper or Lower. Zero is not acceptable.
// The current texture is used. SKYHEMI_NO_TOPCAP can be used.
static void RenderSkyHemisphere(int hemi)
{
	int r, c;
	
	if (hemi & SKYHEMI_LOWER)
	{
		yflip = true;
	}
	else
	{
		yflip = false;
	}
	
	// The top row (row 0) is the one that's faded out.
	// There must be at least 4 columns. The preferable number
	// is 4n, where n is 1, 2, 3... There should be at least
	// two rows because the first one is always faded.
	rows = 4;
	
	if (hemi & SKYHEMI_JUST_CAP)
	{
		return;
	}

	// Draw the cap as one solid color polygon
	if (!foglayer)
	{
		columns = 4 * (gl_sky_detail > 0 ? gl_sky_detail : 1);
		foglayer=true;
		glDisable(GL_TEXTURE_2D);


		if (!secondlayer)
		{
			glColor3f(R, G ,B);
			glBegin(GL_TRIANGLE_FAN);
			for(c = 0; c < columns; c++)
			{
				SkyVertex(1, c);
			}
			glEnd();
		}

		glEnable(GL_TEXTURE_2D);
		foglayer=false;
	}
	else
	{
		columns=4;	// no need to do more!
		glBegin(GL_TRIANGLE_FAN);
		for(c = 0; c < columns; c++)
		{
			SkyVertex(0, c);
		}
		glEnd();
	}
	
	// The total number of triangles per hemisphere can be calculated
	// as follows: rows * columns * 2 + 2 (for the top cap).
	for(r = 0; r < rows; r++)
	{
		if (yflip)
		{
			glBegin(GL_TRIANGLE_STRIP);
            SkyVertex(r + 1, 0);
			SkyVertex(r, 0);
			for(c = 1; c <= columns; c++)
			{
				SkyVertex(r + 1, c);
				SkyVertex(r, c);
			}
			glEnd();
		}
		else
		{
			glBegin(GL_TRIANGLE_STRIP);
            SkyVertex(r, 0);
			SkyVertex(r + 1, 0);
			for(c = 1; c <= columns; c++)
			{
				SkyVertex(r, c);
				SkyVertex(r + 1, c);
			}
			glEnd();
		}
	}
}


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
static void RenderDome(int texno, FGLTexture * tex, float x_offset, float y_offset, int CM_Index)
{
	int texh;

	bool skystretch = (r_stretchsky && !(level.flags & LEVEL_FORCENOSKYSTRETCH));


	if (tex)
	{
		tex->Bind(CM_Index);
		texw = tex->TextureWidth();
		texh = tex->TextureHeight();

		if (texh>190 && skystretch) texh=190;

		glRotatef(180.0f-x_offset, 0.f, 1.f, 0.f);

		yAdd = y_offset/texh;

		// The non-stretched skies still need some work

		if (texh<=180) // && skystretch)
		{
			yMult=1.0f;
		}
		else
		{
			yMult= 180.0f/texh;
			/*
			if (texh<=180)
			{
				yAdd-=(180-texh)/(float)texh;
			}
			*/
		}
	}

	if (tex && !secondlayer) 
	{
		PalEntry pe = SkyCapColor(texno, false);
		if (CM_Index!=CM_DEFAULT) ModifyPalette(&pe, CM_Index, 1);
		R=pe.r/255.0f;
		G=pe.g/255.0f;
		B=pe.b/255.0f;
	}

	RenderSkyHemisphere(SKYHEMI_UPPER);

	if(tex)
	{
		yAdd = y_offset/texh;

		if (texh<=180)
		{
			yMult=1.0f;
		}
		else
		{
			yAdd+=180.0f/texh;
		}
	}

	if (tex && !secondlayer) 
	{
		PalEntry pe = SkyCapColor(texno, true);
		if (CM_Index!=CM_DEFAULT) ModifyPalette(&pe, CM_Index, 1);
		R=pe.r/255.0f;
		G=pe.g/255.0f;
		B=pe.b/255.0f;
	}

	RenderSkyHemisphere(SKYHEMI_LOWER);

	if (tex)
	{
		glRotatef(-180.0f+x_offset, 0.f, 1.f, 0.f);
	}
}


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void GLSkyPortal::DrawContents()
{
	bool drawBoth = false;
	int CM_Index;
	PalEntry FadeColor(0,0,0,0);

	if (gl_fixedcolormap) 
	{
		CM_Index=gl_fixedcolormap<CM_LIMIT? gl_fixedcolormap:CM_DEFAULT;
	}
	else 
	{
		CM_Index=CM_DEFAULT;
		FadeColor=origin->fadecolor;
	}

	if (origin->texture[0]==origin->texture[1] && origin->doublesky) origin->doublesky=false;	

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
	gl_SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1), true);
	glTranslatef(0.f, -1000.f, 0.f);

	if (origin->texture[0])
	{
		if (fixedcolormap)
		{
			float rr,gg,bb;

			gl_GetLightColor(255, 255,255,255, &rr, &gg, &bb);
			R*=rr;
			G*=gg;
			B*=bb;
		}

		RenderDome(origin->skytexno1, origin->texture[0], origin->x_offset[0], origin->y_offset, CM_Index);
	}
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL,0.05f);
	
	if (origin->doublesky && origin->texture[1])
	{
		secondlayer=true;
		RenderDome(0, origin->texture[1], origin->x_offset[1], origin->y_offset, CM_Index);
		secondlayer=false;
	}

	if (skyfog>0 && (FadeColor.r ||FadeColor.g || FadeColor.b))
	{
		/*else
		{
			int fogd;
			
			// If the level has global fog use this for the fog sheet!
			// Ok, this is extremely hackish but if you don't want just don't use it!
			/*
			if (outsidefogcolor!=0xff) fogd = outsidefogdensity;
			else if (fogdensity!=0) fogd=level_fogdensity;
			else fogd=130;//colormap.fadecolor.a<<1;
			if (fogd<300) 
			{
				fd=(float)sqrt(sqrt(fogd/255.0f));
				fd=min(fd,0.95f);
			}
			else fd=1.0f;
		}
		*/

		glDisable(GL_TEXTURE_2D);
		foglayer=true;
		glColor4f(FadeColor.r/255.0f,FadeColor.g/255.0f,FadeColor.b/255.0f,skyfog/255.0f);
		RenderDome(0, NULL, 0, 0, CM_DEFAULT);
		glEnable(GL_TEXTURE_2D);
		foglayer=false;
	}

	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

