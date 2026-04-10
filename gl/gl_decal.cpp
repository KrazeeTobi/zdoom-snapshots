#include "gl_pch.h"
/*
** gl_decal.cpp
** OpenGL decal rendering code
**
**---------------------------------------------------------------------------
** Copyright 2003-2005 Christoph Oelckers
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

#include "a_sharedglobal.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "gl/gl_intern.h"

struct DecalVertex
{
	float x,y,z;
	float u,v;
};

//==========================================================================
//
//
//
//==========================================================================
void GLWall::DrawDecal(ADecal *actor, seg_t *seg, sector_t *frontSector, sector_t *backSector)
{
	line_t * line=seg->linedef;
	side_t * side=seg->sidedef;
	int i;
	fixed_t zpos;
	int light;
	float a;
	bool flipx, flipy, loadAlpha;
	PalEntry p;
	DecalVertex dv[4];
	int decalTile;
	

	if (actor->renderflags & RF_INVISIBLE) return;
	if (type==RENDERWALL_FFBLOCK && gltexture->tex->bMasked) return;	// No decals on 3D floors with transparent textures.

	//if (actor->sprite != 0xffff)
	{
		decalTile = actor->picnum;
		flipx = !!(actor->renderflags & RF_XFLIP);
		flipy = !!(actor->renderflags & RF_YFLIP);
	}
	/*
	else
	{
	decalTile = SpriteFrames[sprites[actor->sprite].spriteframes + actor->frame].lump[0];
	flipx = SpriteFrames[sprites[actor->sprite].spriteframes + actor->frame].flip & 1;
	}
	*/


	FGLTexture * tex=FGLTexture::ValidateTexture(decalTile, false);
	if (!tex) return;
	
	switch (actor->renderflags & RF_RELMASK)
	{
	default:
		// No valid decal can have this type. If one is encountered anyway
		// it is in some way invalid.
		return;
		//zpos = actor->z;
		//break;

	case RF_RELUPPER:
		if (type!=RENDERWALL_TOP) return;
		if (line->flags & ML_DONTPEGTOP)
		{
			zpos = actor->z + frontSector->ceilingtexz;
		}
		else
		{
			zpos = actor->z + backSector->ceilingtexz;
		}
		break;
	case RF_RELLOWER:
		if (type!=RENDERWALL_BOTTOM) return;
		if (line->flags & ML_DONTPEGBOTTOM)
		{
			zpos = actor->z + frontSector->ceilingtexz;
		}
		else
		{
			zpos = actor->z + backSector->floortexz;
		}
		break;
	case RF_RELMID:
		if (type==RENDERWALL_TOP || type==RENDERWALL_BOTTOM) return;
		if (line->flags & ML_DONTPEGBOTTOM)
		{
			zpos = actor->z + frontSector->floortexz;
		}
		else
		{
			zpos = actor->z + frontSector->ceilingtexz;
		}
	}
	
	if (actor->renderflags & RF_FULLBRIGHT)
	{
		light = 255;
	}
	else
	{
		light = lightlevel;
	}
	
	int r = RPART(actor->alphacolor);
	int g = GPART(actor->alphacolor);
	int b = BPART(actor->alphacolor);
	
	if (!(level.flags&LEVEL_NOCOLOREDSPRITELIGHTING))
		p = Colormap.LightColor;
	else
	{
		int v = (Colormap.LightColor.r * 77 + Colormap.LightColor.g*143 + Colormap.LightColor.b*35)/255;
		p=PalEntry(v,v,v);
	}
	
	float red, green, blue;
	
	if (actor->RenderStyle == STYLE_Shaded)
	{
		loadAlpha = true;
		p.a=CM_SHADE;

		gl_GetLightColor(light, p.r,p.g,p.b, &red, &green, &blue);
		
		red = r * red / 255.f;
		green = g * green / 255.f;
		blue = b * blue / 255.f;
		
		// adjust colors to current colormap
		if (Colormap.LightColor.a>=CM_FIRSTCOLORMAP)
		{
			// Get the most appropriate translated color from the colormap
			int palindex = ColorMatcher.Pick(quickertoint(red*255), quickertoint(green*255), quickertoint(blue*255));
			int newindex = realcolormaps [NUMCOLORMAPS*256*(Colormap.LightColor.a - CM_FIRSTCOLORMAP) + palindex];

			red = GPalette.BaseColors[newindex].r / 255.f;
			green = GPalette.BaseColors[newindex].g / 255.f;
			blue = GPalette.BaseColors[newindex].b / 255.f;
		}
		else if (!gl_shaderactive)
		{
			if (Colormap.LightColor.a>=1 && Colormap.LightColor.a<=CM_DESAT31)
			{
				int fac=Colormap.LightColor.a-CM_DESAT0;
				float gray=(red*77 + green*143 + blue*37)/257.0f;

				red =   (red  *(31-fac)+ gray*fac)/31;
				green = (green*(31-fac)+ gray*fac)/31;
				blue =  (blue *(31-fac)+ gray*fac)/31;
			}
			else if (Colormap.LightColor.a==CM_INVERT)
			{
				red=green=blue=clamp<float>(255-(red*77 + green*143 + blue*37)/257.0f,0.0f,1.0f);
			}
			else if (Colormap.LightColor.a==CM_GOLDMAP)
			{
				float gray=(red*77 + green*143 + blue*37)/257.0f;
				red=clamp<float>(gray*1.5f, 0.0f, 1.0f);
				green=clamp<float>(gray, 0.0f, 1.0f);
				blue=0;
			}
		}
	}	
	else
	{
		loadAlpha = false;
		red = 1.f;
		green = 1.f;
		blue = 1.f;
	}
	a = actor->alpha / (FRACUNIT * 1.f);
	
	// now clip the decal to the actual polygon
	float decalwidth = (tex->TextureWidth()*actor->xscale)/63.f;
	float decalheight= (tex->TextureHeight()*actor->yscale)/63.f;
	float decallefto = (tex->GetLeftOffset()*actor->xscale)/63.f;
	float decaltopo  = (tex->GetTopOffset()*actor->yscale)/63.f;

	
	float leftedge = glseg.fracleft * side->TexelLength;
	float linelength = glseg.fracright * side->TexelLength - leftedge;

	// texel index of the decal's left edge
	float decalpixpos = (float)side->TexelLength * actor->floorclip / (1<<20) - (flipx? decalwidth-decallefto : decallefto) - leftedge;

	float left,right;
	float lefttex,righttex;

	// decal is off the left edge
	if (decalpixpos < 0)
	{
		left = 0;
		lefttex = -decalpixpos;
	}
	else
	{
		left = decalpixpos;
		lefttex = 0;
	}
	
	// decal is off the right edge
	if (decalpixpos + decalwidth > linelength)
	{
		right = linelength;
		righttex = right - decalpixpos;
	}
	else
	{
		right = decalpixpos + decalwidth;
		righttex = decalwidth;
	}
	if (right<=left) return;	// nothing to draw

	float fleft = F_TO_MAP(left);
	float fright = F_TO_MAP(right);

	float flength = F_TO_MAP(linelength);

	// one texture unit on the wall as vector
	float vx=(glseg.x2-glseg.x1)/flength;
	float vz=(glseg.z2-glseg.z1)/flength;
		
	dv[1].x=dv[0].x=glseg.x1+vx*fleft;
	dv[1].z=dv[0].z=glseg.z1+vz*fleft;

	dv[3].x=dv[2].x=glseg.x1+vx*fright;
	dv[3].z=dv[2].z=glseg.z1+vz*fright;
		
	zpos+= FRACUNIT*(flipy? decalheight-decaltopo : decaltopo);

	dv[1].y=dv[2].y=zpos / MAP_SCALE;
	dv[0].y=dv[3].y=(zpos-decalheight*FRACUNIT) / MAP_SCALE;
	dv[1].v=dv[2].v=0;


	const PatchTextureInfo * pti=tex->BindPatch(p.a, actor->Translation);
	dv[1].u=dv[0].u=pti->GetU(lefttex*64/actor->xscale);
	dv[3].u=dv[2].u=pti->GetU(righttex*64/actor->xscale);
	dv[0].v=dv[3].v=pti->GetVB();


	// now clip to the top plane
	float vyt=(ytop[1]-ytop[0])/flength;
	float topleft=this->ytop[0]+vyt*fleft;
	float topright=this->ytop[0]+vyt*fright;

	// completely below the wall
	if (topleft<dv[0].y && topright<dv[3].y) 
		return;

	if (topleft<dv[1].y || topright<dv[2].y)
	{
		// decal has to be clipped at the top
		// let texture clamping handle all extreme cases
		dv[1].v=(dv[1].y-topleft)/(dv[1].y-dv[0].y)*dv[0].v;
		dv[2].v=(dv[2].y-topright)/(dv[2].y-dv[3].y)*dv[3].v;
		dv[1].y=topleft;
		dv[2].y=topright;
	}

	// now clip to the bottom plane
	float vyb=(ybottom[1]-ybottom[0])/flength;
	float bottomleft=this->ybottom[0]+vyb*fleft;
	float bottomright=this->ybottom[0]+vyb*fright;

	// completely above the wall
	if (bottomleft>dv[1].y && bottomright>dv[2].y) 
		return;

	if (bottomleft>dv[0].y || bottomright>dv[3].y)
	{
		// decal has to be clipped at the bottom
		// let texture clamping handle all extreme cases
		dv[0].v=(dv[1].y-bottomleft)/(dv[1].y-dv[0].y)*(dv[0].v-dv[1].v) + dv[1].v;
		dv[3].v=(dv[2].y-bottomright)/(dv[2].y-dv[3].y)*(dv[3].v-dv[2].v) + dv[2].v;
		dv[0].y=bottomleft;
		dv[3].y=bottomright;
	}


	if (flipx)
	{
		float ur=pti->GetUR();
		for(i=0;i<4;i++) dv[i].u=ur-dv[i].u;
	}
	if (flipy)
	{
		float vb=pti->GetVB();
		for(i=0;i<4;i++) dv[i].v=vb-dv[i].v;
	}
	// fog is set once per wall in the calling function and not per decal!

	gl.Color4f(red, green, blue, a);
	switch(actor->RenderStyle)
	{
	case STYLE_Shaded:
	case STYLE_Translucent:
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gl.AlphaFunc(GL_GREATER,0.0f);
		break;

	case STYLE_Add:
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE);
		gl.AlphaFunc(GL_GREATER,0.0f);
		break;

	case STYLE_Fuzzy:
		gl.BlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		gl.AlphaFunc(GL_GREATER,0.0f);
		break;

	default:
		gl.BlendFunc(GL_ONE,GL_ZERO);	
		gl.AlphaFunc(GL_GEQUAL,0.5f);
		break;

	}
	gl.Begin(GL_TRIANGLE_FAN);
	for(i=0;i<4;i++)
	{
		gl.TexCoord2f(dv[i].u,dv[i].v);
		gl.Vertex3f(dv[i].x,dv[i].y,dv[i].z);
	}
	gl.End();
	rendered_decals++;
}

//==========================================================================
//
//
//
//==========================================================================
void GLWall::DoDrawDecals(ADecal * decal, seg_t * seg)
{
	while (decal)
	{
		// the sectors are only used for their texture origin coordinates
		// so we don't need the fake sectors for deep water etc.
		// As this is a completely split wall fragment no further splits are
		// necessary for the decal.
		sector_t * frontsector;

		// for 3d-floor segments use the model sector as reference
		if ((decal->renderflags&RF_CLIPMASK)==RF_CLIPMID) frontsector=decal->Sector;
		else frontsector=seg->frontsector;

		DrawDecal(decal,seg,frontsector,seg->backsector);
		decal=static_cast<ADecal*>(decal->snext);
	}
}


