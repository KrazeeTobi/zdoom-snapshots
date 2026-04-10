#include "gl_pch.h"
/*
** gl_flat.cpp
** Flat rendering
**
**---------------------------------------------------------------------------
** Copyright 2000-2005 Christoph Oelckers
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
#include "gl/gl_functions.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_intern.h"
#include "gl/gl_basic.h"


EXTERN_CVAR (Bool, gl_lights_checkside);
SQWORD oldms;

/*****************
*               *
* Flats         *
*               *
*****************/

void gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FGLTexture * gltexture)
{
	float uoffs=(float)secplane->xoffs/(float)FRACUNIT;
	float voffs=(float)(secplane->yoffs)/(float)FRACUNIT;
	float xscale=(float)1.f/FRACUNIT*(float)secplane->xscale/gltexture->TextureWidth()*64.0f;
	float yscale=(float)1.f/FRACUNIT*(float)secplane->yscale/gltexture->TextureHeight()*64.0f;
	float angle=-(float)(secplane->angle)*(45.0f/ANG45);

	gl.MatrixMode(GL_TEXTURE);
	gl.PushMatrix();
	gl.Translatef(uoffs/gltexture->TextureWidth(),voffs/gltexture->TextureHeight(),0.0f);
	gl.Scalef(xscale,yscale,1.0f);
	gl.Rotatef(angle,0.0f,0.0f,1.0f);
}


//==========================================================================
//
//
//
//==========================================================================

void GLFlat::DrawSubsectorLights(gl_subsectordata * glsub)
{
	Plane p;
	Vector nearPt, up, right;
	float scale;
	int k;

	FLightNode * node = glsub->lighthead;

	if (glsub-gl_subsectors==314)
	{
		__asm nop
	}

	while (node)
	{
		ADynamicLight * light = node->lightsource;
		
		if (light->flags2&MF2_DORMANT)
		{
			node=node->nextLight;
			continue;
		}

		// we must do the side check here because gl_SetupLight needs the correct plane orientation
		// which we don't have for Legacy-style 3D-floors
		fixed_t planeh = plane.plane.ZatPoint(light->x, light->y);
		if (gl_lights_checkside && ((planeh<light->z && ceiling) || (planeh>light->z && !ceiling)))
		{
			node=node->nextLight;
			continue;
		}

		p.Set(plane.plane);
		if (!gl_SetupLight(p, light, nearPt, up, right, scale, 
							!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE, 
							Colormap.LightColor.a, false)) 
		{
			node=node->nextLight;
			continue;
		}
		draw_dlightf++;

		// Set the plane
		if (plane.plane.a || plane.plane.b) for(k = 0; k < glsub->numvertices; k++)
		{
			// Unfortunately the rendering inaccuracies prohibit any kind of plane translation
			// This must be done on a per-vertex basis.
			gl_vertices[glsub->firstvertex + k].y =
				TO_MAP(plane.plane.ZatPoint(gl_vertices[glsub->firstvertex + k].vt));
		}
		else for(k = 0; k < glsub->numvertices; k++)
		{
			gl_vertices[glsub->firstvertex + k].y = z;
		}

		// Render the light
		gl.Begin(GL_TRIANGLE_FAN);
		for(k = 0; k < glsub->numvertices; k++)
		{
			Vector t1;
			GLVertex * vt = &gl_vertices[glsub->firstvertex + k];

			t1.Set(vt->x, vt->y, vt->z);
			Vector nearToVert = t1 - nearPt;
			gl.TexCoord2f( (nearToVert.Dot(right) * scale) + 0.5f, (nearToVert.Dot(up) * scale) + 0.5f);
			gl.Vertex3f(vt->x, vt->y, vt->z);
		}
		gl.End();
		node = node->nextLight;
	}
}

//==========================================================================
//
//
//
//==========================================================================

void GLFlat::DrawSubsector(gl_subsectordata * glsub)
{
	int k;

	// Set the plane
	if (plane.plane.a || plane.plane.b) for(k = 0; k < glsub->numvertices; k++)
	{
		// Unfortunately the rendering inaccuracies prohibit any kind of plane translation
		// This must be done on a per-vertex basis.
		gl_vertices[glsub->firstvertex + k].y =
			TO_MAP(plane.plane.ZatPoint(gl_vertices[glsub->firstvertex + k].vt));
	}
	else for(k = 0; k < glsub->numvertices; k++)
	{
		// Quicker way for non-sloped sectors
		gl_vertices[glsub->firstvertex + k].y = z;
	}
	gl.DrawArrays(GL_TRIANGLE_FAN, glsub->firstvertex, glsub->numvertices);
	flatvertices += glsub->numvertices;
	flatprimitives++;
}

//==========================================================================
//
//
//
//==========================================================================
void GLFlat::Draw(int pass)
{
	int i;
	gl_sectordata * glsec = &gl_sectors[sector->sectornum];


	if (pass==GLPASS_LIGHT)
	{
		if (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE) 
		{
			// STYLE_Add forces black fog for lights
			gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Add);	
		}
		else 
		{
			// black fog is diminishing light and shouldn't affect the depth fading of lights that strongly.
			gl_SetFog((255+lightlevel)>>1, Colormap.FadeColor, STYLE_Normal);
		}

		if (sub)
		{
			DrawSubsectorLights(sub);
		}
		else
		{
			// Draw the subsectors belonging to this sector
			for (i=0; i<glsec->subsectorcount; i++)
			{
				gl_subsectordata * glsub = glsec->gl_subsectors[i];

				if (gl_ss_renderflags[glsub-gl_subsectors]&renderflags)
				{
					DrawSubsectorLights(glsub);
				}
			}

			// Draw the subsectors assigned to it due to missing textures
			if (!(renderflags&SSRF_RENDER3DPLANES))
			{
				gl_subsectorrendernode * node = glsec->otherplanes[!(renderflags&SSRF_RENDERFLOOR)];
				while (node)
				{
					DrawSubsectorLights(node->glsub);
					node = node->next;
				}
			}
		}

		return;
	}

	if (pass==GLPASS_DECALS) return;

	// set light level
	if (pass==GLPASS_UNLIT || pass==GLPASS_BASE)
	{
		gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Normal);
		// gltexture==NULL means this is a plane of an FF_FOG volume.

		if (gltexture) gl_SetColor(lightlevel+(extralight*gl_weaponlight), Colormap.LightColor,alpha);
		else gl_SetColor(lightlevel, Colormap.LightColor, alpha);
	}

	// set the texture
	bool needtexture= (pass==GLPASS_UNLIT || pass==GLPASS_TEXTURE || pass==GLPASS_DEPTH);
	if (needtexture)
	{
		if (gltexture)
		{
			gltexture->Bind(Colormap.LightColor.a);
			gl_SetPlaneTextureRotation(&plane, gltexture);
		}
		else
		{
			gl.Disable(GL_TEXTURE_2D);
			gl_SetColorMode(CM_DEFAULT);
		}
	}

	if (alpha<1.0f-FLT_EPSILON) 
	{
		gl.AlphaFunc(GL_GEQUAL,0.5f*(alpha));
		gl.DepthFunc(GL_LEQUAL);
	}

	if (sub)
	{
		// This represents a single subsector
		DrawSubsector(sub);
	}
	else
	{
		// Draw the subsectors belonging to this sector
		for (i=0; i<glsec->subsectorcount; i++)
		{
			gl_subsectordata * glsub = glsec->gl_subsectors[i];

			// This is just a quick hack to make translucent 3D floors and portals work
			// Since the translucency sorting code has to be rewritten anyway I won't
			// add some complicated handling now because it'd get removed anyway.
			if (gl_ss_renderflags[glsub-gl_subsectors]&renderflags || alpha < 1.0f-FLT_EPSILON)
			{
				DrawSubsector(glsub);
			}
		}

		// Draw the subsectors assigned to it due to missing textures
		if (!(renderflags&SSRF_RENDER3DPLANES))
		{
			gl_subsectorrendernode * node = glsec->otherplanes[!(renderflags&SSRF_RENDERFLOOR)];
			while (node)
			{
				DrawSubsector(node->glsub);
				node = node->next;
			}
		}
	}

	if (needtexture && !gltexture) gl.Enable(GL_TEXTURE_2D);
	else gl.PopMatrix();
	if (alpha<1.0f-FLT_EPSILON) 
	{
		gl.DepthFunc(GL_LESS);
	}
}

//==========================================================================
//
//
//
//==========================================================================
inline void GLFlat::PutFlat(bool translucent)
{
	if (gl_fixedcolormap) 
	{
		Colormap.GetFixedColormap();
	}
	if (translucent)
	{
		if (renderflags&SSRF_RENDER3DPLANES)
		{
			gl_drawlist[GLDL_TRANSLUCENT].AddFlat(this);
		}
		else
		{
			// These come from a stacked sector. Sorting these polygons
			// is not necessary and might even interfere with proper
			// drawing order.
			gl_drawlist[GLDL_TRANSLUCENTBORDER].AddFlat(this);
		}
		return;
	}
	else if (gl_lights && !gl_fixedcolormap)
	{
		gl_sectordata * sec = &gl_sectors[sector->sectornum];
		for(int i=0;i<sec->subsectorcount;i++) if (sec->gl_subsectors[i]->lighthead)
		{
			if (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE)
				gl_drawlist[GLDL_LITFOG].AddFlat(this);
			else if (!gltexture->tex->bMasked)
				gl_drawlist[GLDL_LIT].AddFlat(this);
			else
				gl_drawlist[GLDL_MASKED].AddFlat(this);

			return;
		}
	}
	{
		gl_drawlist[GLDL_UNLIT].AddFlat(this);
	}
}

//==========================================================================
//
// This draws one flat 
// The passed sector does not indicate the area which is rendered. 
// It is only used as source for the plane data.
// The whichplane boolean indicates if the flat is a floor(false) or a ceiling(true)
//
//==========================================================================

void GLFlat::Process(sector_t * sector, bool whichplane, bool notexture)
{
	plane.GetFromSector(sector, whichplane);

	if (!notexture)
	{
		if (plane.texture==skyflatnum) return;

		gltexture=FGLTexture::ValidateTexture(plane.texture);
		if (!gltexture) 
		{
			return;
		}
	}
	else gltexture=NULL;

	if (gltexture && gl_isGlowingTexture(plane.texture)) 
	{
		// glowing textures are always drawn full bright without colored light
		Colormap.LightColor.r = Colormap.LightColor.g = Colormap.LightColor.b = 0xff;
		lightlevel=255;
	}
	else lightlevel=abs(lightlevel);

	// get height from vplane
	z=(float)plane.texheight/MAP_SCALE;

	if (!whichplane && gl_sectors[sector->sectornum].transdoor) 
		z -= F_TO_MAP(1.f);
	
	// dont check for alpha==1.0f due to precision problems. It never is equal!
	PutFlat(alpha<1.0f-FLT_EPSILON);
	rendered_flats++;
}

//==========================================================================
//
// Process a sector's flats for rendering
//
//==========================================================================
#define CenterSpot(sec) (vertex_t*)&(sec)->soundorg[0]

void GLFlat::ProcessSector(sector_t * frontsector, subsector_t * sub)
{
	gl_sectordata * glsec = &gl_sectors[frontsector->sectornum];
	gl_subsectordata * glsub = &gl_subsectors[sub-subsectors]; 
	lightlist_t * light;

#ifdef _DEBUG
	if (frontsector==NULL)
	{
		__asm int 3
	}
	if (frontsector->sectornum==869)
	{
		__asm nop
	}
#endif

	sector=&sectors[frontsector->sectornum];	// this must be the real sector, not the fake one!
	this->sub=NULL;

	gl_ss_renderflags[sub-subsectors]|=SSRF_PROCESSED;
	if (glsub->hacked&1) AddHackedSubsector(sub);

	//
	//
	//
	// do floors
	//
	//
	//
	if (frontsector->floorplane.ZatPoint(viewx, viewy) <= viewz)
	{
		gl_ss_renderflags[sub-subsectors]|=SSRF_RENDERFLOOR;

		// process the original floor first.
		if (frontsector->FloorSkyBox && frontsector->FloorSkyBox->bAlways) AddFloorStack(sub);

		if (!(glsec->renderflags&SSRF_RENDERFLOOR))
		{
			glsec->renderflags |= SSRF_RENDERFLOOR;

			lightlevel = GetFloorLight(frontsector);
			Colormap=frontsector->ColorMap;
			alpha=frontsector->FloorSkyBox && frontsector->FloorSkyBox->bAlways ? 
				frontsector->FloorSkyBox->PlaneAlpha/65536.0f : 1.0f-frontsector->floor_reflect;

			ceiling=false;
			renderflags=SSRF_RENDERFLOOR;

			if (sector->e->ffloors.Size())
			{
				light = P_GetPlaneLight(sector, &frontsector->floorplane, false);
				if (!(sector->FloorFlags&SECF_ABSLIGHTING) || light!=&sector->e->lightlist[0])	
					lightlevel = *light->p_lightlevel;

				Colormap.LightColor = (*light->p_extra_colormap)->Color;
			}
			if (alpha!=0.0f) Process(frontsector, false, false);
		}
	}
	
	//
	//
	//
	// do ceilings
	//
	//
	//
	if (frontsector->ceilingplane.ZatPoint(viewx, viewy) >= viewz)
	{
		gl_ss_renderflags[sub-subsectors]|=SSRF_RENDERCEILING;

		// process the original ceiling first.
		if (frontsector->CeilingSkyBox && frontsector->CeilingSkyBox->bAlways) AddCeilingStack(sub);

		if (!(glsec->renderflags&SSRF_RENDERCEILING))
		{
			glsec->renderflags |= SSRF_RENDERCEILING;

			lightlevel = GetCeilingLight(frontsector);
			Colormap=frontsector->ColorMap;
			alpha=frontsector->CeilingSkyBox && frontsector->CeilingSkyBox->bAlways ? 
				frontsector->CeilingSkyBox->PlaneAlpha/65536.0f : 1.0f-frontsector->ceiling_reflect;

			ceiling=true;
			renderflags=SSRF_RENDERCEILING;

			if (sector->e->ffloors.Size())
			{
				light = P_GetPlaneLight(sector, &sector->ceilingplane, true);

				if(!(sector->CeilingFlags&SECF_ABSLIGHTING)) lightlevel = *light->p_lightlevel;
				Colormap.LightColor = (*light->p_extra_colormap)->Color;
			}
			if (alpha!=0.0f) Process(frontsector, true, false);
		}
	}

	//
	//
	//
	// do 3D floors
	//
	//
	//

	if (sector->e->ffloors.Size())
	{
		player_t * player=players[consoleplayer].camera->player;

		// do the plane setup only once and just mark all loops that have to be processed
		gl_ss_renderflags[sub-subsectors]|=SSRF_RENDER3DPLANES;
		renderflags=SSRF_RENDER3DPLANES;
		if (!(glsec->renderflags&SSRF_RENDER3DPLANES))
		{
			glsec->renderflags |= SSRF_RENDER3DPLANES;
			// 3d-floors must not overlap!
			fixed_t lastceilingheight=sector->CenterCeiling();	// render only in the range of the
			fixed_t lastfloorheight=sector->CenterFloor();		// current sector part (if applicable)
			F3DFloor * rover;	
			int k;
			
			// floors are ordered now top to bottom so scanning the list for the best match
			// is no longer necessary.

			ceiling=true;
			for(k=0;k<sector->e->ffloors.Size();k++)
			{
				rover=sector->e->ffloors[k];
				
				if ((rover->flags&(FF_EXISTS|FF_RENDERPLANES))==(FF_EXISTS|FF_RENDERPLANES))
				{
					if (rover->flags&FF_FOG && gl_fixedcolormap) continue;
					if (rover->flags&(FF_INVERTPLANES|FF_BOTHPLANES))
					{
						fixed_t ff_top=rover->top.plane->ZatPoint(CenterSpot(sector));
						if (ff_top<lastceilingheight)
						{
							if (viewz<=rover->top.plane->ZatPoint(viewx, viewy))
							{
								// FF_FOG requires an inverted logic where to get the light from
								light=P_GetPlaneLight(sector, rover->top.plane,!!(rover->flags&FF_FOG));
								lightlevel=*light->p_lightlevel;
								
								Colormap.LightColor = (rover->flags&FF_FOG)?
												(*light->p_extra_colormap)->Fade:
												(*light->p_extra_colormap)->Color;
								Colormap.FadeColor=frontsector->ColorMap->Fade;

								alpha=rover->alpha/255.0f;
								Process(rover->top.model, rover->top.isceiling, !!(rover->flags&FF_FOG));
							}
							lastceilingheight=ff_top;
						}
					}
					if (!(rover->flags&FF_INVERTPLANES))
					{
						fixed_t ff_bottom=rover->bottom.plane->ZatPoint(CenterSpot(sector));
						if (ff_bottom<lastceilingheight)
						{
							if (viewz<=rover->bottom.plane->ZatPoint(viewx, viewy))
							{
								light=P_GetPlaneLight(sector, rover->bottom.plane,!(rover->flags&FF_FOG));
								lightlevel=*light->p_lightlevel;

								Colormap.LightColor = (rover->flags&FF_FOG)?
												(*light->p_extra_colormap)->Fade:
												(*light->p_extra_colormap)->Color;
								Colormap.FadeColor=frontsector->ColorMap->Fade;

								alpha=rover->alpha/255.0f;
								Process(rover->bottom.model, rover->bottom.isceiling, !!(rover->flags&FF_FOG));
							}
							lastceilingheight=ff_bottom;
							if (rover->alpha<255) lastceilingheight++;
						}
					}
				}
			}
				  
			ceiling=false;
			for(k=sector->e->ffloors.Size()-1;k>=0;k--)
			{
				rover=sector->e->ffloors[k];
				
				if ((rover->flags&(FF_EXISTS|FF_RENDERPLANES))==(FF_EXISTS|FF_RENDERPLANES))
				{
					if (rover->flags&FF_FOG && gl_fixedcolormap) continue;
					if (rover->flags&(FF_INVERTPLANES|FF_BOTHPLANES))
					{
						fixed_t ff_bottom=rover->bottom.plane->ZatPoint(CenterSpot(sector));
						if (ff_bottom>lastfloorheight || (rover->flags&FF_FIX))
						{
							if (viewz>=rover->bottom.plane->ZatPoint(viewx, viewy))
							{
								if (rover->flags&FF_FIX)
								{
									lightlevel = rover->model->lightlevel;
									Colormap = rover->model->ColorMap;
								}
								else
								{
									light=P_GetPlaneLight(sector, rover->bottom.plane,!(rover->flags&FF_FOG));
									lightlevel=*light->p_lightlevel;

									Colormap.LightColor = (rover->flags&FF_FOG)?
													(*light->p_extra_colormap)->Fade:
													(*light->p_extra_colormap)->Color;
									Colormap.FadeColor=frontsector->ColorMap->Fade;
								}

								alpha=rover->alpha/255.0f;
								Process(rover->bottom.model, rover->bottom.isceiling, !!(rover->flags&FF_FOG));
							}
							lastfloorheight=ff_bottom;
						}
					}
					if (!(rover->flags&FF_INVERTPLANES))
					{
						fixed_t ff_top=rover->top.plane->ZatPoint(CenterSpot(sector));
						if (ff_top>lastfloorheight)
						{
							if (viewz>=rover->top.plane->ZatPoint(viewx, viewy))
							{
								light=P_GetPlaneLight(sector, rover->top.plane,!!(rover->flags&FF_FOG));
								lightlevel=*light->p_lightlevel;

								Colormap.LightColor = (rover->flags&FF_FOG)?
												(*light->p_extra_colormap)->Fade:
												(*light->p_extra_colormap)->Color;
								Colormap.FadeColor=frontsector->ColorMap->Fade;

								alpha=rover->alpha/255.0f;
								Process(rover->top.model, rover->top.isceiling, !!(rover->flags&FF_FOG));
							}
							lastfloorheight=ff_top;
							if (rover->alpha<255) lastfloorheight--;
						}
					}
				}
			}
		}
	}
}



