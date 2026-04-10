#include "gl_pch.h"
/*
** gl_light.cpp
** Light level / fog management / dynamic lights
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

#include "c_dispatch.h"
#include "p_local.h"
#include "vectors.h"
#include "gl/gl_system.h"
#include "gl/gl_struct.h"
#include "gl/gl_lights.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "gl/gl_portal.h"


CVAR (Bool, gl_lights_debug, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CUSTOM_CVAR (Bool, gl_lights, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
{
	if (self) gl_RecreateAllAttachedLights();
	else gl_DeleteAllAttachedLights();
}
CVAR (Bool, gl_attachedlights, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Bool, gl_bulletlight, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Bool, gl_lights_checkside, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Float, gl_lights_intensity, 1.0f, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Float, gl_lights_size, 1.0f, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Bool, gl_light_sprites, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR (Bool, gl_light_particles, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);

CVAR(Bool,gl_enhanced_lightamp,true,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
CVAR(Bool,gl_depthfog,true,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)


static byte distfogtable[256];	// light to fog conversion table for black fog

static int fogdensity;
static PalEntry outsidefogcolor;
static int outsidefogdensity;
int skyfog;

//==========================================================================
//
// Set fog parameters for the level
//
//==========================================================================
void gl_SetFogParams(int _fogdensity, PalEntry _outsidefogcolor, int _outsidefogdensity, int _skyfog)
{
	fogdensity=_fogdensity;
	if (_fogdensity==0) _fogdensity=70;
	outsidefogcolor=_outsidefogcolor;
	outsidefogdensity=_outsidefogdensity? _outsidefogdensity : _fogdensity;
	skyfog=_skyfog;

	for (int i=0;i<256;i++)
	{
		if (i<164)
		{
			distfogtable[i]= (_fogdensity>>1) + (_fogdensity)*(164-i)/164;
		}
		else if (i<230)
		{											    
			distfogtable[i]= (_fogdensity>>1) - (_fogdensity>>1)*(i-164)/(230-164);
		}
		else distfogtable[i]=0;
	}
	outsidefogdensity>>=1;
	fogdensity>>=1;
}

//==========================================================================
//
// Get current light color
//
//==========================================================================
void gl_GetLightColor(int lightlevel, int red, int green, int blue, float * pred, float * pgreen, float * pblue)
{
	float & r=*pred,& g=*pgreen,& b=*pblue;
	int torch=0;

	if (gl_fixedcolormap) 
	{
		if (gl_fixedcolormap==CM_LITE)
		{
			if (gl_enhanced_lightamp) r=0.375f, g=1.0f, b=0.375f;
			else r=g=b=1.0f;
		}
		else if (gl_fixedcolormap>=CM_TORCH)
		{
			if (gl_enhanced_lightamp) 
			{
				int flicker=gl_fixedcolormap-CM_TORCH;
				r=(0.8f+(7-flicker)/70.0f);
				if (r>1.0f) r=1.0f;
				g=r;
				b=r*0.75f;
			}
			else r=g=b=1.0f;
		}
		else r=g=b=1.0f;
		return;
	}

	//float light=lighttable[clamp<int>(lightlevel,30,255)];
	float light=clamp<int>(lightlevel,30,255)/255.0f;
	r=red*light/255.0f;
	g=green*light/255.0f;
	b=blue*light/255.0f;
}

//==========================================================================
//
// set current light color
//
//==========================================================================
void gl_SetColor(int light, int red, int green, int blue, float alpha, PalEntry ThingColor)
{ 
	float r,g,b;
	gl_GetLightColor(light,red,green,blue,&r,&g,&b);
	gl.Color4f(r * ThingColor.r/255.0f, g * ThingColor.g/255.0f, b * ThingColor.b/255.0f, alpha);
}

//==========================================================================
//
// calculates the current fog density
//
//	Rules for fog:
//
//	1. black fog means no fog and always uses the distfogtable based on the level's fog density setting
//	2. If outside fog is defined it and the current fog color is the same as the outside fog
//	   the engine always uses the outside fog density to make the fog uniform across the level.
//	   If the outside fog's density is undefined it uses the level's fog density and if that is
//	   not defined it uses a default of 70.
//	3. If a global fog density is specified it is being used for all fog on the level
//	4. If none of the above apply fog density is based on the light level as for the software renderer.
//
//==========================================================================
int gl_GetFogDensity(int lightlevel, PalEntry fogcolor)
{
	int density;

	if (gl_fixedcolormap) 
	{
		return 0;
	}
	if (gl_isBlack(fogcolor))
	{
		// case 1
		density=distfogtable[lightlevel];
	}
	else if (outsidefogcolor.a!=0xff && 
			fogcolor.r==outsidefogcolor.r && 
			fogcolor.g==outsidefogcolor.g &&
			fogcolor.b==outsidefogcolor.b) 
	{
		// case 2. outsidefogdensity has already been set as needed
		density=outsidefogdensity;
	}
	else  if (fogdensity!=0)
	{
		// case 3
		density=fogdensity;
	}
	else 
	{
		// case 4
		density=clamp<int>(255-lightlevel,30,255);
	}
	return density;
}


static PalEntry cfogcolor=-1;
static int cfogdensity=-1;

void gl_InitFog()
{
	cfogcolor=-1;
	cfogdensity=-1;
	gl_EnableFog(false);
	gl.Hint(GL_FOG_HINT, GL_FASTEST);
	gl.Fogi(GL_FOG_MODE, GL_EXP);
}
//==========================================================================
//
// Sets the fog for the current polygon
//
//==========================================================================

void gl_SetFog(int lightlevel, PalEntry fogcolor, int blendmode)
{

	int fogdensity;

	if (level.flags&LEVEL_HASFADETABLE)
	{
		fogdensity=70;
		fogcolor=0x808080;
	}
	else fogdensity = gl_GetFogDensity(lightlevel, fogcolor);

	// For additive rendering using the regular fog color here would mean applying it twice
	// so always use black
	if (blendmode==STYLE_Add || blendmode==STYLE_Fuzzy)
	{
		fogcolor=0;
	}

	// Make fog a little denser when inside a skybox
	if (GLPortal::inskybox) fogdensity+=fogdensity/2;

	// no fog in enhanced vision modes!
	if (fogdensity==0 || !gl_depthfog)
	{
		cfogcolor=cfogdensity=-1;
		gl_EnableFog(false);
	}
	else
	{
		// Only change fog settings if they differ from the previous ones.
		// Setting Fog can be an expensive operation on some graphics cards.
		fogcolor.a=0;
		if (fogcolor!=cfogcolor)
		{
			if (fogcolor!=-1)
			{
				GLfloat FogColor[4]={fogcolor.r/255.0f,fogcolor.g/255.0f,fogcolor.b/255.0f,0.0f};
				if (cfogcolor==-1) gl_EnableFog(true);
				gl_FogColor(FogColor);
			}
			else gl_EnableFog(false);
			cfogcolor=fogcolor;
		}
		if (fogdensity!=cfogdensity)
		{
			gl_FogDensity(fogdensity/500.0f * FOG_COEFF);
			cfogdensity=fogdensity;
		}
	}
}




//==========================================================================
//
// Sets up the parameters to render one dynamic light onto one plane
//
//==========================================================================
bool gl_SetupLight(Plane & p, ADynamicLight * light, Vector & nearPt, Vector & up, Vector & right, float & scale, bool additive, int desaturation, bool checkside)
{
	Vector fn, pos;

    float x =-TO_MAP(light->x);
	float y = TO_MAP(light->z);
	float z = TO_MAP(light->y);
	
	float dist = fabsf(p.DistToPoint(x, y, z));
	float radius = (light->GetRadius() * gl_lights_size) / MAP_COEFF;
	
	if (radius <= 0.f) return false;
	if (dist > radius) return false;
	if (checkside && gl_lights_checkside && p.PointOnSide(x, y, z))
	{
		return false;
	}

	scale = 1.0f / ((2.f * radius) - dist);

	// project light position onto plane (find closest point on plane)


	pos.Set(x,y,z);
	fn=p.Normal();
	fn.GetRightUp(right, up);

	nearPt = pos + fn * dist;

	float cs = 1.0f - (dist / radius);
	if (additive) cs*=0.2f;				// otherwise the light gets too strong.
	//cs *= frac;
	float r = light->GetRed() / 255.0f * cs * gl_lights_intensity;
	float g = light->GetGreen() / 255.0f * cs * gl_lights_intensity;
	float b = light->GetBlue() / 255.0f * cs * gl_lights_intensity;

	if (light->IsSubtractive())
	{
		Vector v;
		
		gl.BlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		v.Set(r, g, b);
		r = v.Length() - r;
		g = v.Length() - g;
		b = v.Length() - b;
	}
	else
	{
		gl.BlendEquation(GL_FUNC_ADD);
	}
	if (desaturation>0)
	{
		float gray=(r*77 + g*143 + b*37)/257;

		r= (r*(32-desaturation)+ gray*desaturation)/32;
		g= (g*(32-desaturation)+ gray*desaturation)/32;
		b= (b*(32-desaturation)+ gray*desaturation)/32;
	}
	gl.Color3f(r,g,b);
	return true;
}


//==========================================================================
//
//
//
//==========================================================================
bool gl_SetupLightTexture()
{
	int lump=TexMan.CheckForTexture("GLLIGHT", FTexture::TEX_MiscPatch,FTextureManager::TEXMAN_TryAny);

	if (lump<0) return false;
	FGLTexture * pat=FGLTexture::ValidateTexture(lump, false);
	pat->BindPatch(CM_DEFAULT, 0);
	return true;
}


inline fixed_t P_AproxDistance3(fixed_t dx, fixed_t dy, fixed_t dz)
{
	return P_AproxDistance(P_AproxDistance(dx,dy),dz);
}

//==========================================================================
//
// Sets the light for a sprite - takes dynamic lights into account
//
//==========================================================================
void gl_SetSpriteLight(fixed_t x, fixed_t y, fixed_t z, subsector_t * subsec, int lightlevel, int red, int green, int blue, int desaturation, float alpha, PalEntry ThingColor)
{
	FLightNode * node = gl_subsectors[subsec-subsectors].lighthead;
	float r,g,b;
	Vector lightColor;
	ADynamicLight *light;
	float frac, dist, tr, tg, tb, lr, lg, lb;
	float radius;
	
	tr=tg=tb=0;
	while (node)
	{
		light=node->lightsource;
		if (!(light->flags2&MF2_DORMANT))
		{
			vec3_t lvec = { TO_MAP(x - light->x), TO_MAP(y - light->y), 
							TO_MAP(z - light->z) };

			dist = VectorLength(lvec);
			radius = F_TO_MAP(light->GetRadius() * gl_lights_size);
			
			if (dist < radius)
			{
				frac = 1.0f - (dist / radius);
				
				if (frac > 0)
				{
					lr = light->GetRed() / 255.0f * gl_lights_intensity;
					lg = light->GetGreen() / 255.0f * gl_lights_intensity;
					lb = light->GetBlue() / 255.0f * gl_lights_intensity;
					if (light->IsSubtractive())
					{
						lightColor.Set(lr, lg, lb);
						lr = (lightColor.Length() - lightColor.X()) * -1;
						lg = (lightColor.Length() - lightColor.Y()) * -1;
						lb = (lightColor.Length() - lightColor.Z()) * -1;
					}
					
					tr += lr * frac;
					tg += lg * frac;
					tb += lb * frac;
				}
				
			}
		}
		node = node->nextLight;
	}

	if (desaturation>0 && desaturation<=CM_DESAT31)
	{
		float gray=(tr*77 + tg*143 + tb*37)/257;

		tr= (tr*(32-desaturation)+ gray*desaturation)/32;
		tg= (tg*(32-desaturation)+ gray*desaturation)/32;
		tb= (tb*(32-desaturation)+ gray*desaturation)/32;
	}

	gl_GetLightColor(lightlevel,red,green,blue,&r,&g,&b);
	r = clamp<float>(r+tr, 0, 1.0f);
	g = clamp<float>(g+tg, 0, 1.0f);
	b = clamp<float>(b+tb, 0, 1.0f);

	gl.Color4f(r * ThingColor.r/255.0f, g * ThingColor.g/255.0f, b * ThingColor.b/255.0f, alpha);
}

void gl_SetSpriteLight( AActor * thing, int lightlevel, int red, int green, int blue, int desaturation, float alpha, PalEntry ThingColor)
{ 
	subsector_t * subsec = R_PointInSubsector2(thing->x, thing->y);

	gl_SetSpriteLight(thing->x, thing->y, (thing->z+thing->height)>>1, subsec, lightlevel, red, green, blue, desaturation, alpha, ThingColor);
}

void gl_SetSpriteLight( particle_t * thing, int lightlevel, int red, int green, int blue, int desaturation, float alpha, PalEntry ThingColor)
{ 
	gl_SetSpriteLight(thing->x, thing->y, thing->z, thing->subsector, lightlevel, red, green, blue, desaturation, alpha, ThingColor);
}


//==========================================================================
//
// For testing sky fog sheets
//
//==========================================================================
CCMD(skyfog)
{
	if (argv.argc()>1)
	{
		skyfog=strtol(argv[1],NULL,0);
	}
}

