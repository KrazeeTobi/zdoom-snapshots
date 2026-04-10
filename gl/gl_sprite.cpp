#include "gl_pch.h"

/*
** gl_sprite.cpp
** Sprite/Particle rendering
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
#include "p_local.h"
#include "gl/gl_struct.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "gl/gl_portal.h"
#include "gl/models.h"

CVAR(Bool, gl_usecolorblending, true, CVAR_ARCHIVE)
EXTERN_CVAR (Float, transsouls)

const BYTE SF_FRAMEMASK  = 0x1f;

inline float Dist2(float x1,float y1,float x2,float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


//==========================================================================
//
// 
//
//==========================================================================
void GLSprite::Draw(int pass)
{
	if (pass==GLPASS_DECALS) return;

	if (gltexture) gltexture->BindPatch(Colormap.LightColor.a,translation);
	else 
	{
		glDisable(GL_TEXTURE_2D);
		gl_SetShader(CM_DEFAULT);
	}

	if (RenderStyle != STYLE_Normal && actor && !(actor->momx|actor->momy))
	{
		// Draw translucent non-moving sprites with a slightly altered z-offset to avoid z-fighting 
		// when in the same position as a regular sprite
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -64.0f);
	}

	if(RenderStyle==STYLE_Fuzzy)
	{
		float fuzzalpha=0.44f;
		float minalpha=0.1f;
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

		// fog + fuzz don't work well without some fiddling with the alpha value!
		if (!gl_isBlack(Colormap.FadeColor))
		{
			float xcamera=-(float)viewx/MAP_SCALE;
			float zcamera=(float)viewy/MAP_SCALE;

			float dist=Dist2(xcamera,zcamera, x,z);

			if (!Colormap.FadeColor.a) Colormap.FadeColor.a=clamp<int>(255-lightlevel,60,255);

			// this value was determined by trial and error and is scale dependent!
			float factor=0.05f+exp(-Colormap.FadeColor.a*dist/488.28125f);
			fuzzalpha*=factor;
			minalpha*=factor;
		}
		

		glAlphaFunc(GL_GEQUAL,minalpha);
		glColor4f(0.2f,0.2f,0.2f,fuzzalpha);
	}
	else
	{
		//trans=testtrans;
		switch(RenderStyle)
		{
		case STYLE_Add:
			if (trans<1.0-FLT_EPSILON || !gl_usecolorblending || !(actor->renderflags & RF_FULLBRIGHT))
			{
				glBlendFunc(GL_SRC_ALPHA,GL_ONE);
				glAlphaFunc(GL_GEQUAL,0.5f*trans);
				break;
			}
			glBlendFunc(GL_SRC_COLOR, GL_ONE);
			break;

		case STYLE_Normal:
			trans=1.0f;
			break;

		case STYLE_Translucent:
			glAlphaFunc(GL_GEQUAL,0.5f*trans);
			break;
		}



		bool res=false;
		if (gl_lights && !gl_fixedcolormap)
		{
			if (actor)
			{
				if (gl_light_sprites && !(actor->renderflags & RF_FULLBRIGHT))
				{
					gl_SetSpriteLight(actor, lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,trans, ThingColor);
					res=true;
				}
			}
			else if (gl_light_particles)
			{
				// FIXME:
			}
		}
		if (!res) gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,trans, ThingColor);
	}
	if (gl_isBlack(Colormap.FadeColor)) foglevel=lightlevel;
	gl_SetFog(foglevel,  Colormap.FadeColor, RenderStyle);

	if (!model)
	{
		glBegin(GL_TRIANGLE_STRIP);
		if (gltexture)
		{
			glTexCoord2f(ul, vt); glVertex3f(x1, y1, z1);
			glTexCoord2f(ur, vt); glVertex3f(x2, y1, z2);
			glTexCoord2f(ul, vb); glVertex3f(x1, y2, z1);
			glTexCoord2f(ur, vb); glVertex3f(x2, y2, z2);
		}
		else	// Particle
		{
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y2, z1);
			glVertex3f(x2, y2, z2);
		}
		glEnd();
	}
	else
	{
		Mod_RenderModel(this, model, actor->frame&SF_FRAMEMASK);
	}

	// For translucent objects restore the default blending mode here!
	switch (RenderStyle)
	{
		case STYLE_Add:
		case STYLE_Fuzzy:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}
	if (RenderStyle != STYLE_Normal && actor && !(actor->momx|actor->momy))
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0, 0);
	}

	if (!gltexture)	glEnable(GL_TEXTURE_2D);
}


//==========================================================================
//
// 
//
//==========================================================================
inline void GLSprite::PutSprite(bool translucent)
{
	if (translucent)
	{
		gl_drawlist[GLDL_TRANSLUCENT].AddSprite(this);
	}
	else
	{
		gl_drawlist[GLDL_UNLIT].AddSprite(this);
	}
}

//==========================================================================
//
// 
//
//==========================================================================
void GLSprite::SplitSprite(sector_t * frontsector, bool translucent)
{
	GLSprite copySprite;
	fixed_t lightbottom;
	float maplightbottom;
	int i;
	bool put=false;
	TArray<lightlist_t> & lightlist=frontsector->e->lightlist;

	//y1+=y;
	//y2+=y;
	//y=0;
	for(i=0;i<lightlist.Size();i++)
	{
		// ok, this won't work for diagonal splits...
		// both edges have to be clipped independently
		if (i<lightlist.Size()-1) lightbottom=lightlist[i+1].plane.ZatPoint(0,0);
		else lightbottom=frontsector->floorplane.ZatPoint(0,0);

		//maplighttop=TO_MAP(lightlist[i].height);
		maplightbottom=TO_MAP(lightbottom);
		if (maplightbottom<y2) maplightbottom=y2;

		if (maplightbottom<y1)
		{
			copySprite=*this;
			copySprite.lightlevel=*lightlist[i].p_lightlevel;
			copySprite.Colormap.LightColor=(*lightlist[i].p_extra_colormap)->Color;

			if (!gl_isWhite(ThingColor))
			{
				copySprite.Colormap.LightColor.r=(copySprite.Colormap.LightColor.r*ThingColor.r)>>8;
				copySprite.Colormap.LightColor.g=(copySprite.Colormap.LightColor.g*ThingColor.g)>>8;
				copySprite.Colormap.LightColor.b=(copySprite.Colormap.LightColor.b*ThingColor.b)>>8;
			}

			y1=copySprite.y2=maplightbottom;
			vt=copySprite.vb=copySprite.vt+ 
				(maplightbottom-copySprite.y1)*(copySprite.vb-copySprite.vt)/(y2-copySprite.y1);
			copySprite.PutSprite(translucent);
			put=true;
		}
	}
	//if (y1<y2) PutSprite(translucent);
}



//==========================================================================
//
// 
//
//==========================================================================
void GLSprite::SetThingColor(PalEntry pe)
{
	byte red=pe.r;
	byte green=pe.g;
	byte blue=pe.b;
	int gray;
	int fac;

	if (Colormap.LightColor.a==CM_INVERT || Colormap.LightColor.a==CM_LITE)
	{
		gray=255-((red*77 + green*143 + blue*37)>>8);
		red=green=blue=clamp<int>(gray,0,255);
	}
	else if (Colormap.LightColor.a==CM_GOLDMAP)
	{
		gray=(red*77 + green*143 + blue*37)>>8;
		red=clamp<int>(gray+(gray>>1),0,255);
		green=clamp<int>(gray,0,255);
		blue=0;
	}
	else if (Colormap.LightColor.a>=CM_DESAT1 && Colormap.LightColor.a<=CM_DESAT31)
	{
		fac=Colormap.LightColor.a-CM_DESAT0;
		gray=(red*77 + green*143 + blue*37)>>8;
		red  = (red  *(32-fac)+ gray*fac)/32;
		green= (green*(32-fac)+ gray*fac)/32;
		blue = (blue *(32-fac)+ gray*fac)/32;
	}
	else if (Colormap.LightColor.a>=CM_FIRSTCOLORMAP)
	{
		// Get the most appropriate translated color from the colormap
		int palindex = ColorMatcher.Pick(red, green, blue);
		int newindex = realcolormaps [NUMCOLORMAPS*256*(Colormap.LightColor.a - CM_FIRSTCOLORMAP) + palindex];

		red = GPalette.BaseColors[newindex].r;
		green = GPalette.BaseColors[newindex].g;
		blue = GPalette.BaseColors[newindex].b;
	}

	ThingColor=PalEntry(red, green, blue);
}

//==========================================================================
//
// 
//
//==========================================================================
void GLSprite::Process(AActor* thing,sector_t * sector)
{
	sector_t rs;
	sector_t * rendersector;
	// don't draw the thing that's used as camera (for viewshifts during quakes!)
	if (thing==viewactor) return;

 	// invisible things
	if (thing->renderflags&RF_INVISIBLE || thing->RenderStyle==STYLE_None) return; 
	if (thing->sprite==0) return;

	// [RH] Interpolate the sprite's position to make it look smooth
	fixed_t thingx = thing->PrevX + FixedMul (r_TicFrac, thing->x - thing->PrevX);
	fixed_t thingy = thing->PrevY + FixedMul (r_TicFrac, thing->y - thing->PrevY);
	fixed_t thingz = thing->PrevZ + FixedMul (r_TicFrac, thing->z - thing->PrevZ);

	// too close to the camera. This doesn't look good.
	if (P_AproxDistance(thingx-viewx, thingy-viewy)<2*FRACUNIT) return;

	// don't draw first frame of a player missile
	if (thing->flags&MF_MISSILE && thing->target==viewactor && viewactor != NULL)
	{
		if (P_AproxDistance(thingx-viewx, thingy-viewy) < thing->Speed ) return;
	}

	if (GLPortal::mirrorline)
	{
		// this thing is behind the mirror!
		if (P_PointOnLineSide(thingx, thingy, GLPortal::mirrorline)) return;
	}


	player_t *player=&players[displayplayer];
	GL_RECT r;
	bool enhancedvision=false;
	bool fullbright=!!(thing->renderflags & RF_FULLBRIGHT);

	if (sector->sectornum!=thing->Sector->sectornum)
	{
		rendersector=gl_FakeFlat(thing->Sector, &rs, false);
	}
	else
	{
		rendersector=sector;
	}
	
	lightlevel=fullbright? 255 : rendersector->lightlevel;
	foglevel = rendersector->lightlevel;

	if (gl_isGlowingTexture(rendersector->floorpic))
	{
		if (thingz-thing->floorz<wallglowheight)
		{
			int maxlight=(255+lightlevel)>>1;
			fixed_t lightfrac=FixedDiv(thingz-thing->floorz, wallglowheight);
			if (lightfrac<0) lightfrac=0;
			lightlevel= (lightfrac*lightlevel + maxlight*(FRACUNIT-lightfrac))>>FRACBITS;
		}
	}

	// colormap stuff is a little more complicated here...
	if (gl_fixedcolormap) 
	{
		enhancedvision=true;
		Colormap.GetFixedColormap();

		if (gl_fixedcolormap==CM_LITE)
		{
			if (gl_enhanced_lightamp &&
				(thing->flags&(MF_SPECIAL) || thing->flags3&MF3_ISMONSTER || thing->flags&MF_MISSILE || thing->flags&MF_CORPSE))
			{
				Colormap.LightColor.a=CM_LITE;
			}
		}
	}
	else 
	{
		Colormap=rendersector->ColorMap;
		if (fullbright)
		{
			// Only make the light white but keep everything else (fog, desaturation and Boom colormap.)
			Colormap.LightColor.r=
			Colormap.LightColor.g=
			Colormap.LightColor.b=0xff;
		}
		else if (level.flags&LEVEL_NOCOLOREDSPRITELIGHTING)
		{
			int v = (Colormap.LightColor.r /* * 77 */ + Colormap.LightColor.g /**143 */ + Colormap.LightColor.b /**35*/)/3;//255;
			Colormap.LightColor.r=
			Colormap.LightColor.g=
			Colormap.LightColor.b=(255+v+v)/3;
		}
	}

	translation=thing->Translation;

	// Since it is easy to get the blood color's RGB value
	// there is no need to limit this to the current palette.
	if ((translation>>8)==TRANSLATION_Blood)
	{
		extern PalEntry BloodTranslations[256];
		SetThingColor(BloodTranslations[translation&255]);
		translation = TRANSLATION(TRANSLATION_Standard, 8);
	}
	else ThingColor=0xffffff;

	RenderStyle=thing->RenderStyle;
	trans=thing->alpha/(float)FRACUNIT;


	if (RenderStyle == STYLE_OptFuzzy)
	{
		RenderStyle = STYLE_Fuzzy;	// This option is useless in OpenGL The fuzz effect looks much better here!
	}
	else if (RenderStyle == STYLE_SoulTrans)
	{
		RenderStyle = STYLE_Translucent;
		trans = transsouls;
	}

	if (enhancedvision)
	{
		if (RenderStyle==STYLE_Fuzzy)
		{
			// enhanced vision makes them more visible!
			trans=0.5f;
			RenderStyle=STYLE_Translucent;
		}
		else if (thing->flags&MF_STEALTH)	
		{
			// enhanced vision overcomes stealth!
			if (trans<0.5f) trans=0.5f;
		}
	}

	if (trans==0.0f) return;

	x =-TO_MAP(thingx);
	y = TO_MAP(thingz-thing->floorclip);
	z = TO_MAP(thingy);
	
	if (!Mod_GetModelForSprite(thing->sprite, thing->frame, &model, &gltexture))
	{
		angle_t ang = R_PointToAngle(thingx, thingy);

		int patch = gl_GetSpriteFrame(thing->sprite, thing->frame, -1, ang - thing->angle);
		if (patch==INVALID_SPRITE) return;
		gltexture=FGLTexture::ValidateTexture(abs(patch), false);
		if (!gltexture) return;

		const PatchTextureInfo * pti = gltexture->GetPatchTextureInfo();
	
		vt=0.0f;
		vb=pti->GetVB();
		gltexture->GetRect(&r);
		if (patch<0)
		{
			r.left=-r.width-r.left;	// mirror the sprite's x-offset
			ul=0.0f;
			ur=pti->GetUR();
		}
		else
		{
			ul=pti->GetUR();
			ur=0.0f;
		}
		r.Scale(thing->xscale/63.0f,thing->yscale/63.0f);

		float rightfac=-r.left/(float)MAP_COEFF;
		float leftfac=rightfac-r.width/(float)MAP_COEFF;

		y1=y-r.top/(float)MAP_COEFF;
		y2=y1-r.height/(float)MAP_COEFF;

		// Tests show that this doesn't look good for many decorations and corpses
		if ((thing->player || thing->flags3&MF3_ISMONSTER || thing->flags&MF_SPECIAL) && 
			(thing->flags&MF_ICECORPSE || !(thing->flags&MF_CORPSE)))
		{
			//if (!thing->floorclip)
			{
				float btm=1000000.0f;

				if (thing->Sector->e->ffloors.Size())
				{
					for(int i=0;i<thing->Sector->e->ffloors.Size();i++)
					{
						F3DFloor * ff=thing->Sector->e->ffloors[i];
						fixed_t floorh=ff->top.plane->ZatPoint(thingx, thingy);
						if (floorh==thing->floorz) 
						{
							btm=floorh/MAP_SCALE;
							break;
						}
					}
				}
				else if (thing->Sector->heightsec && !(thing->Sector->heightsec->MoreFlags & SECF_IGNOREHEIGHTSEC))
				{
					if (thing->flags2&MF2_ONMOBJ && thing->floorz==
						thing->Sector->heightsec->floorplane.ZatPoint(thingx, thingy))
					{
						btm=thing->floorz/MAP_SCALE;
					}
				}
				if (btm==1000000.0f) 
					btm= (thing->Sector->floorplane.ZatPoint(thingx, thingy)-thing->floorclip)/MAP_SCALE;

				float diff = y2 - btm;
				if (diff >= 0 /*|| !gl_sprite_clip_to_floor*/) diff = 0;
				if (diff <=-10) diff=0;	// such a large displacement can't be correct!
				y2-=diff;
				y1-=diff;
			}
		}

		x1=x-viewvecY*leftfac;
		x2=x-viewvecY*rightfac;
		z1=z+viewvecX*leftfac;
		z2=z+viewvecX*rightfac;

		scale=P_AproxDistance(thing->x-viewx, thing->y-viewy);
	}

	actor=thing;
	if (RenderStyle==STYLE_Translucent && trans>=1.0f-FLT_EPSILON) RenderStyle=STYLE_Normal;

	if (thing->Sector->e->lightlist.Size()==0 || gl_fixedcolormap || fullbright) 
	{
		PutSprite(RenderStyle!=STYLE_Normal);
	}
	else if (model)
	{
		// FIXME: Get the appropriate light color here!
		PutSprite(RenderStyle!=STYLE_Normal);
	}
	else
	{
		SplitSprite(thing->Sector,RenderStyle!=STYLE_Normal);
	}
	rendered_sprites++;
}


//==========================================================================
//
// 
//
//==========================================================================

void GLSprite::ProcessParticle (particle_t *particle, sector_t *sector)//, int shade, int fakeside)
{
	if (GLPortal::mirrorline)
	{
		// this particle is  behind the mirror!
		if (P_PointOnLineSide(particle->x, particle->y, GLPortal::mirrorline)) return;
	}

	player_t *player=&players[displayplayer];
	
	if (particle->trans==0) return;
	lightlevel = sector->lightlevel;

	if (gl_fixedcolormap) 
	{
		Colormap.GetFixedColormap();
	}
	else
	{
		Colormap=sector->ColorMap;
	}

	trans=particle->trans/255.0f;

	SetThingColor(GPalette.BaseColors[particle->color]);

	model=NULL;
	gltexture=NULL;

	x=-TO_MAP(particle->x);
	y= TO_MAP(particle->z);
	z= TO_MAP(particle->y);
	
	float scalefac=particle->size/(4.0f*MAP_COEFF);

	x2=x-viewvecY*scalefac;
	x1=x+viewvecY*scalefac;
	z1=z-viewvecX*scalefac;
	z2=z+viewvecX*scalefac;
	y1=y-scalefac;
	y2=y+scalefac;
	scale=P_AproxDistance(particle->x-viewx, particle->y-viewy);
	actor=NULL;
	
	if (trans>=1.0f-FLT_EPSILON) RenderStyle=STYLE_Normal;
	else RenderStyle=STYLE_Translucent;

	PutSprite(RenderStyle==STYLE_Translucent);
	rendered_sprites++;
}



//===========================================================================
//
//  Gets the texture index for a sprite frame
//
//===========================================================================
extern TArray<spritedef_t> sprites;
extern TArray<spriteframe_t> SpriteFrames;

int gl_GetSpriteFrame(unsigned sprite, int frame, int rot, angle_t ang)
{
	frame&=SF_FRAMEMASK;
	spritedef_t *sprdef = &sprites[sprite];
	if (frame >= sprdef->numframes)
	{
		// If there are no frames at all for this sprite, don't draw it.
		return INVALID_SPRITE;
	}
	else
	{
		//picnum = SpriteFrames[sprdef->spriteframes + thing->frame].Texture[0];
		// choose a different rotation based on player view
		spriteframe_t *sprframe = &SpriteFrames[sprdef->spriteframes + frame];
		if (rot==-1)
		{
			if (sprframe->Texture[0] == sprframe->Texture[1])
			{
				rot = (ang + (angle_t)(ANGLE_45/2)*9) >> 28;
			}
			else
			{
				rot = (ang + (angle_t)(ANGLE_45/2)*9-(angle_t)(ANGLE_180/16)) >> 28;
			}
		}
		int rv = sprframe->Texture[rot];
		if (sprframe->Flip&(1<<rot)) rv=-rv;
		return rv;
	}
}

