#include "gl_pch.h"
/*
** gl_sky.cpp
** Sky preparation code.
**
**---------------------------------------------------------------------------
** Copyright 2002-2005 Christoph Oelckers
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

#include "a_sharedglobal.h"
#include "r_sky.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_functions.h"
#include "gl/gl_data.h"
#include "gl/gl_portal.h"
#include "gl/gl_texture.h"

CVAR(Bool,gl_noskyboxes, false, 0)
extern int skyfog;

enum
{
	NoSkyDraw = 89
};

//==========================================================================
//
//  Calculate sky texture
//
//==========================================================================
void GLWall::SkyTexture(int sky1,ASkyViewpoint * skyboxx, bool ceiling)
{
	// JUSTHIT is used as an indicator that a skybox is in use.
	// This is to avoid recursion
	if (!gl_noskyboxes && !gl_nostencil && skyboxx && viewactor!=skyboxx && !(skyboxx->flags&MF_JUSTHIT))
	{
		if (!skyboxx->Mate) 
		{
			flag=RENDERWALL_SKYBOX;
			skybox=skyboxx;
		}
		else 
		{
			static GLSectorStackInfo stackinfo;
			if (ceiling && GLPortal::inlowerstack) return;
			if (!ceiling && GLPortal::inupperstack) return;
			flag=RENDERWALL_SECTORSTACK;
			stackinfo.deltax = skyboxx->Mate->x - skyboxx->x;
			stackinfo.deltay = skyboxx->Mate->y - skyboxx->y;
			stackinfo.deltaz = 0;
			stackinfo.isupper= ceiling;
			stack=&stackinfo;
		}
	}
	else
	{
		if (skyboxx && skyboxx->Mate) return;

		// VC's optimizer totally screws up if this is made local...
		static GLSkyInfo skyinfo;

		memset(&skyinfo, 0, sizeof(skyinfo));
		if ((sky1 & PL_SKYFLAT) && (sky1 & (PL_SKYFLAT-1)) && !gl_nostencil)
		{
			const line_t *l = &lines[(sky1&(PL_SKYFLAT-1))-1];
			const side_t *s = &sides[l->sidenum[0]];
			
			skyinfo.texture[0]=FGLTexture::ValidateTexture(s->toptexture);
			if (!skyinfo.texture[0]) return;
			skyinfo.skytexno1=s->toptexture;
			skyinfo.x_offset[0] = s->textureoffset/(float)ANGLE_1;
			skyinfo.y_offset = s->rowoffset/(float)FRACUNIT;
			skyinfo.mirrored = !l->args[2];
		}
		else
		{
			if (level.flags&LEVEL_DOUBLESKY)
			{
				skyinfo.texture[1]=FGLTexture::ValidateTexture(sky1texture);
				if (!skyinfo.texture[1]) return;
				skyinfo.x_offset[1] = sky1pos/(float)FRACUNIT*90.0f/256.0f;
				skyinfo.doublesky = true;
			}
			
			if ((level.flags&LEVEL_SWAPSKIES || (sky1==PL_SKYFLAT && !gl_nostencil) || (level.flags&LEVEL_DOUBLESKY)) &&
				sky2texture!=sky1texture)	// If both skies are equal use the scroll offset of the first!
			{
				skyinfo.texture[0]=FGLTexture::ValidateTexture(sky2texture);
				skyinfo.skytexno1=sky2texture;
				skyinfo.x_offset[0] = sky2pos/(float)FRACUNIT*90.0f/256.0f;
			}
			else
			{
				skyinfo.texture[0]=FGLTexture::ValidateTexture(sky1texture);
				skyinfo.skytexno1=sky1texture;
				skyinfo.x_offset[0] = sky1pos/(float)FRACUNIT*90.0f/256.0f;
			}
			if (!skyinfo.texture[0]) return;

		}
		if (skyfog>0) 
		{
			skyinfo.fadecolor=Colormap.FadeColor;
			skyinfo.fadecolor.a=0;
		}
		else skyinfo.fadecolor=0;

		flag=RENDERWALL_SKY;
		sky = &skyinfo;
	}
	PutWall(0);
}


//==========================================================================
//
//  Skies on one sided walls
//
//==========================================================================
void GLWall::SkyNormal(sector_t * fs,vertex_t * v1,vertex_t * v2)
{
	if (fs->ceilingpic==skyflatnum || (fs->CeilingSkyBox && fs->CeilingSkyBox->bAlways))
	{
		ytop[0]=ytop[1]=10000.0f;
		ybottom[0]=yceil[0];
		ybottom[1]=yceil[1];
		SkyTexture(fs->sky,fs->CeilingSkyBox, true);
	}
	if (fs->floorpic==skyflatnum || (fs->FloorSkyBox && fs->FloorSkyBox->bAlways))
	{
		ytop[0]=yfloor[0];
		ytop[1]=yfloor[1];
		ybottom[0]=ybottom[1]=-10000.0f;
		SkyTexture(fs->sky,fs->FloorSkyBox, false);
	}
}


//==========================================================================
//
//  Upper Skies on two sided walls
//
//==========================================================================
void GLWall::SkyTop(seg_t * seg,sector_t * fs,sector_t * bs,vertex_t * v1,vertex_t * v2)
{
	if (fs->ceilingpic==skyflatnum)
	{
		if ((bs->special&0xff) == NoSkyDraw) return;
		if (bs->ceilingpic==skyflatnum) 
		{
			// if the back sector is closed the sky must be drawn!
			if (bs->ceilingplane.ZatPoint(v1) > bs->floorplane.ZatPoint(v1) ||
				bs->ceilingplane.ZatPoint(v2) > bs->floorplane.ZatPoint(v2) || gl_sectors[bs->sectornum].transdoor)
					return;
		}

		ytop[0]=ytop[1]=10000.0f;

		FTexture * tex = TexMan(seg->sidedef->toptexture);
		if (tex && tex->UseType!=FTexture::TEX_Null && bs->ceilingpic != skyflatnum)
		{
			ybottom[0]=yceil[0];
			ybottom[1]=yceil[1];
		}
		else
		{
			ybottom[0]=TO_MAP(bs->ceilingplane.ZatPoint(v1));
			ybottom[1]=TO_MAP(bs->ceilingplane.ZatPoint(v2));
		}

		SkyTexture(fs->sky,fs->CeilingSkyBox, true);
	}
	else if (fs->CeilingSkyBox && fs->CeilingSkyBox->Mate && fs->CeilingSkyBox!=bs->CeilingSkyBox)
	{
		// stacked sectors
		fixed_t bsc1=bs->ceilingplane.ZatPoint(v1);
		fixed_t bsc2=bs->ceilingplane.ZatPoint(v2);
		fixed_t fsc1=fs->ceilingplane.ZatPoint(v1);
		fixed_t fsc2=fs->ceilingplane.ZatPoint(v2);

		ytop[0]=ytop[1]=10000.0f;
		ybottom[0]=TO_MAP(MAX(bsc1,fsc1));
		ybottom[1]=TO_MAP(MAX(bsc2,fsc2));
		SkyTexture(fs->sky,fs->CeilingSkyBox, true);
	}

}


//==========================================================================
//
//  Lower Skies on two sided walls
//
//==========================================================================
void GLWall::SkyBottom(seg_t * seg,sector_t * fs,sector_t * bs,vertex_t * v1,vertex_t * v2)
{
	if (fs->floorpic==skyflatnum)
	{
		if ((bs->special&0xff) == NoSkyDraw) return;
		FTexture * tex = TexMan(seg->sidedef->bottomtexture);
		
		// For lower skies the normal logic only applies to walls with no lower texture!
		if (tex->UseType==FTexture::TEX_Null)
		{
			if (bs->floorpic==skyflatnum)
			{
				// if the back sector is closed the sky must be drawn!
				if (bs->ceilingplane.ZatPoint(v1) > bs->floorplane.ZatPoint(v1) ||
					bs->ceilingplane.ZatPoint(v2) > bs->floorplane.ZatPoint(v2))
						return;

			}
			else
			{
				// Special hack for Vrack2b
				if (bs->floorplane.ZatPoint(viewx, viewy) > viewz) return;
			}
		}
		ybottom[0]=ybottom[1]=-10000.0f;

		if (tex && tex->UseType!=FTexture::TEX_Null)
		{
			ytop[0]=yfloor[0];
			ytop[1]=yfloor[1];
		}
		else
		{
			ytop[0]=TO_MAP(bs->floorplane.ZatPoint(v1));
			ytop[1]=TO_MAP(bs->floorplane.ZatPoint(v2));
		}

		SkyTexture(fs->sky,fs->FloorSkyBox, false);
	}
	else if (fs->FloorSkyBox && fs->FloorSkyBox->Mate && fs->FloorSkyBox!=bs->FloorSkyBox)
	{
		// stacked sectors
		fixed_t bsc1=bs->floorplane.ZatPoint(v1);
		fixed_t bsc2=bs->floorplane.ZatPoint(v2);
		fixed_t fsc1=fs->floorplane.ZatPoint(v1);
		fixed_t fsc2=fs->floorplane.ZatPoint(v2);

		ybottom[0]=ybottom[1]=-10000.0f;
		ytop[0]=TO_MAP(MIN(bsc1,fsc1));
		ytop[1]=TO_MAP(MIN(bsc2,fsc2));

		SkyTexture(fs->sky,fs->FloorSkyBox, false);
	}
}


