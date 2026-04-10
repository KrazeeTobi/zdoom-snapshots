#include "gl_pch.h"
/*
** gl_wall.cpp
** Wall rendering
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
**    by the Free Software Foundation; either version 2 of the License, or (at
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

#include "p_local.h"
#include "p_lnspec.h"
#include "gl/gl_struct.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_portal.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_basic.h"
#include "gl/gl_functions.h"

// Debugging help
#ifdef _DEBUG
CVAR(Int, break_renderlinedef, -1, 0)
#endif

UniqueList<GLSkyInfo> UniqueSkies;
UniqueList<GLHorizonInfo> UniqueHorizons;
UniqueList<GLSectorStackInfo> UniqueStacks;


CVAR(Bool,r_mirrors,true,0)	// This is for debugging only!

CVAR(Bool,gl_mirror_envmap, true, CVAR_GLOBALCONFIG|CVAR_ARCHIVE)

inline float Dist2(float x1,float y1,float x2,float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


//==========================================================================
//
// Sets up the texture coordinates for one light to be rendered
//
//==========================================================================
bool GLWall::PrepareLight(texcoord * tcs, ADynamicLight * light)
{
	float vtx[]={glseg.x1,ybottom[0],glseg.z1, glseg.x1,ytop[0],glseg.z1, glseg.x2,ytop[1],glseg.z2, glseg.x2,ybottom[1],glseg.z2};
	Plane p;
	Vector nearPt, up, right;
	float scale;

	p.Init(vtx,4);

	if (!p.ValidNormal()) 
	{
		return false;
	}

	if (!gl_SetupLight(p, light, nearPt, up, right, scale, 
		!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE, 
		Colormap.LightColor.a, true)) 
	{
		return false;
	}

	Vector t1;
	int outcnt[4]={0,0,0,0};

	for(int i=0;i<4;i++)
	{
		t1.Set(&vtx[i*3]);
		Vector nearToVert = t1 - nearPt;
		tcs[i].u = (nearToVert.Dot(right) * scale) + 0.5f;
		tcs[i].v = (nearToVert.Dot(up) * scale) + 0.5f;

		// quick check whether the light touches this polygon
		if (tcs[i].u<0) outcnt[0]++;
		if (tcs[i].u>1) outcnt[1]++;
		if (tcs[i].v<0) outcnt[2]++;
		if (tcs[i].v>1) outcnt[3]++;

	}
	// The light doesn't touch this polygon
	if (outcnt[0]==4 || outcnt[1]==4 || outcnt[2]==4 || outcnt[3]==4) return false;

	draw_dlight++;
	return true;
}

//==========================================================================
//
// General purpose wall rendering function
// with the exception of walls lit by glowing flats 
// everything goes through here
//
// Tests have shown that precalculating this data
// doesn't give any noticable performance improvements
//
//==========================================================================

void GLWall::DoRenderWall(bool textured, float * color2, ADynamicLight * light)
{
	int a=sizeof(*this);
	texcoord tcs[4];

	if (!light)
	{
		tcs[0]=lolft;
		tcs[1]=uplft;
		tcs[2]=uprgt;
		tcs[3]=lorgt;
	}
	else
	{
		if (!PrepareLight(tcs, light)) return;
	}

	// the rest of the code is identical for textured rendering and lights

	gl.Begin(GL_TRIANGLE_FAN);

	// lower left corner
	if (textured) gl.TexCoord2f(tcs[0].u,tcs[0].v);
	gl.Vertex3f(glseg.x1,ybottom[0],glseg.z1);

	// upper left corner
	if (textured) gl.TexCoord2f(tcs[1].u,tcs[1].v);
	gl.Vertex3f(glseg.x1,ytop[0],glseg.z1);

	// color for right side
	if (color2) gl.Color4fv(color2);

	// upper right corner
	if (textured) gl.TexCoord2f(tcs[2].u,tcs[2].v);
	gl.Vertex3f(glseg.x2,ytop[1],glseg.z2);
	
	// lower right corner
	if (textured) gl.TexCoord2f(tcs[3].u,tcs[3].v); 
	gl.Vertex3f(glseg.x2,ybottom[1],glseg.z2);

	gl.End();

	vertexcount+=4;

}

//==========================================================================
//
// this does only work for non-sloped walls!
// This algorithm doesn't work in case of slopes
//
//==========================================================================
void GLWall::DoRenderGlowingPoly(bool textured, bool dolight, ADynamicLight * light)
{

	float renderbottom=ybottom[0];
	float rendertop;
	float glowheight=wallglowheight/MAP_COEFF;
	float glowbottomtop=yfloor[0]+glowheight;
	float glowtopbottom=yceil[0]-glowheight;
	bool glowbot=gl_isGlowingTexture(bottomflat) && ybottom[0]<glowbottomtop;
	bool glowtop=gl_isGlowingTexture(topflat) && ytop[0]>glowtopbottom;
	float color_o[4];
	float color_b[4];
	float color_top[4];
	float glowc_b[3];
	float glowc_t[3];
	int i;


	texcoord tcs[4];

	if (!light)
	{
		tcs[0]=lolft;
		tcs[1]=uplft;
		tcs[2]=uprgt;
		tcs[3]=lorgt;

		if (dolight)
		{
			gl_GetLightColor(lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,&color_o[0],&color_o[1],&color_o[2]);
			color_o[3]=alpha;
			memcpy(color_b,color_o,sizeof(color_b));
			if (glowbot)
			{
				gl_GetGlowColor(bottomflat, glowc_b);
				for (i=0;i<3;i++) color_b[i]+=glowc_b[i]*wallglowfactor*(glowbottomtop-renderbottom)/glowheight;
			}
			if (glowtop)
			{
				gl_GetGlowColor(topflat, glowc_t);
				if (glowtopbottom<renderbottom)
					for(i=0;i<3;i++) color_b[i]+=glowc_t[i]*wallglowfactor*(renderbottom-glowtopbottom)/glowheight;
			}
			for(i=0;i<3;i++) if (color_b[i]>1.0f) color_b[i]=1.0f;
		}
	}
	else
	{
		dolight=false;	// just to be sure
		if (!PrepareLight(tcs, light)) return;
	}


	float polyh=ytop[0]-ybottom[0];
	float fact=polyh? (tcs[1].v-tcs[0].v)/polyh:0;

	// The wall is being split into 1-3 gradients for upper glow, lower glow and center part (either nothing or overlap)
	while (renderbottom<ytop[0])
	{
		rendertop=ytop[0];
		if (glowbot && glowbottomtop>renderbottom && glowbottomtop<rendertop) rendertop=glowbottomtop;
		if (glowtop && glowtopbottom>renderbottom && glowtopbottom<rendertop) rendertop=glowtopbottom;

		if (dolight)
		{
			memcpy(color_top,color_o,sizeof(color_top));
			if (glowbot && rendertop<glowbottomtop)
				for (i=0;i<3;i++) color_top[i]+=glowc_b[i]*wallglowfactor*(glowbottomtop-rendertop)/glowheight;

			if (glowtop && glowtopbottom<rendertop)
				for(i=0;i<3;i++) color_top[i]+=glowc_t[i]*wallglowfactor*(rendertop-glowtopbottom)/glowheight;
			
			for(i=0;i<3;i++) if (color_top[i]>1.0f) color_top[i]=1.0f;

		}

		gl.Begin(GL_TRIANGLE_FAN);


		// lower left corner
		if (dolight) gl.Color4fv(color_b);
		if (textured) gl.TexCoord2f(tcs[1].u,fact*(renderbottom-ytop[0])+tcs[1].v);
		gl.Vertex3f(glseg.x1,renderbottom,glseg.z1);

		// upper left corner
		if (dolight) gl.Color4fv(color_top);
		if (textured) gl.TexCoord2f(tcs[1].u,fact*(rendertop-ytop[0])+tcs[1].v);
		gl.Vertex3f(glseg.x1,rendertop,glseg.z1);

		// upper right corner
		if (textured) gl.TexCoord2f(tcs[2].u,fact*(rendertop-ytop[1])+tcs[2].v);
		gl.Vertex3f(glseg.x2,rendertop,glseg.z2);

		// lower right corner
		if (dolight) gl.Color4fv(color_b);
		if (textured) gl.TexCoord2f(tcs[2].u,fact*(renderbottom-ytop[1])+tcs[2].v);
		gl.Vertex3f(glseg.x2,renderbottom,glseg.z2);
		gl.End();


		renderbottom=rendertop;
		memcpy(color_b,color_top,sizeof(color_b));
	}
}

//==========================================================================
//
// 
//
//==========================================================================

void GLWall::RenderFogSheet()
{
	if (gl_depthfog)
	{
		float fogdensity=gl_GetFogDensity(lightlevel, Colormap.FadeColor);

	#if 0
		// It works but the banding effects it creates are rather distracting on Geforce cards
		PalEntry fog=Colormap.FadeColor;
		fog.a=fogdensity*1.6f;
		gl_SetFog(lightlevel, fog, STYLE_Normal);
		gl.Color3f(0,0,0);
		gl.Disable(GL_TEXTURE_2D);
		gl.BlendFunc(GL_ONE,GL_ONE);
		gl.DepthFunc(GL_LEQUAL);
		gl.AlphaFunc(GL_GREATER,0);
		DoRenderWall(false, NULL);
		gl.DepthFunc(GL_LESS);
		gl.AlphaFunc(GL_GEQUAL,0.5f);
		gl.Enable(GL_TEXTURE_2D);
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#else
		float xcamera=-(float)viewx/MAP_SCALE;
		float zcamera=(float)viewy/MAP_SCALE;

		float dist1=Dist2(xcamera,zcamera, glseg.x1,glseg.z1)*FOG_COEFF;
		float dist2=Dist2(xcamera,zcamera, glseg.x2,glseg.z2)*FOG_COEFF;


		// these values were determined by trial and error and are scale dependent!
		float fogd1=(0.95f-exp(-fogdensity*dist1/488.28125f)) * 1.05f;
		float fogd2=(0.95f-exp(-fogdensity*dist2/488.28125f)) * 1.05f;

		float fc[4]={Colormap.FadeColor.r/255.0f,Colormap.FadeColor.g/255.0f,Colormap.FadeColor.b/255.0f,fogd2};

		gl.Disable(GL_TEXTURE_2D);
		gl_EnableFog(false);
		gl.AlphaFunc(GL_GREATER,0);
		gl.DepthFunc(GL_LEQUAL);
		gl.Color4f(fc[0],fc[1],fc[2], fogd1);

		DoRenderWall(false,fc);

		gl.DepthFunc(GL_LESS);
		gl_EnableFog(true);
		gl.AlphaFunc(GL_GEQUAL,0.5f);
		gl.Enable(GL_TEXTURE_2D);
	#endif
	}
}


//==========================================================================
//
// 
//
//==========================================================================
bool GLWall::RenderMirror()
{
	if (!gl_mirror_envmap && r_mirrors) return false;

	int lump=TexMan.CheckForTexture("MIRROR", FTexture::TEX_MiscPatch,FTextureManager::TEXMAN_TryAny);

	if (lump<0) return false;
	FGLTexture * pat=FGLTexture::ValidateTexture(lump);
	pat->BindPatch(Colormap.LightColor.a, 0);

	gl.Enable(GL_TEXTURE_GEN_T);
	gl.Enable(GL_TEXTURE_GEN_S);
	gl.TexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	gl.TexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);

	if (!r_mirrors) gl_SetColor(lightlevel>>1, Colormap.LightColor ,1.0f);
	else 
	{
		gl_SetColor(lightlevel, Colormap.LightColor ,0.1f);
		gl.BlendFunc(GL_SRC_ALPHA,GL_ONE);
		gl.AlphaFunc(GL_GREATER,0);
		gl.DepthFunc(GL_LEQUAL);
	}
	gl_SetFog(lightlevel, Colormap.LightColor,r_mirrors? STYLE_Add : STYLE_Normal);

	DoRenderWall(false,NULL);

	gl.Disable(GL_TEXTURE_GEN_T);
	gl.Disable(GL_TEXTURE_GEN_S);

	if (r_mirrors)
	{
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gl.AlphaFunc(GL_GEQUAL,0.5f);
		gl.DepthFunc(GL_LESS);
	}

	return true;
}


//==========================================================================
//
// 
//
//==========================================================================
void GLWall::RenderTwoSidedWall(int pass)
{
	// The only passes coming through here are the texture passes (GLPASS_UNLIT and GLPASS_TEXTURE)
	if (pass != GLPASS_TEXTURE)
	{
		if (flag!=RENDERWALL_M2SNF) gl_SetFog(lightlevel, Colormap.FadeColor, RenderStyle);
		else gl_EnableFog(false);

		switch(RenderStyle)
		{
		case STYLE_Add:
			gl.BlendFunc(GL_SRC_ALPHA,GL_ONE);

		case STYLE_Translucent:
			gl.AlphaFunc(GL_GEQUAL,0.5f*fabs(alpha));
			break;
		}
	}

	if (gltexture) 
	{
		gltexture->Bind(Colormap.LightColor.a);
		if (clampy) gl.TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		if (clampx) gl.TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	}
	else 
	{
		gl_SetColorMode(CM_DEFAULT);
		gl.Disable(GL_TEXTURE_2D);
	}

	// prevent some ugly artifacts at the borders of fences etc.
	if (flag==RENDERWALL_COLOR)
	{
		// this is always translucent and thus always GLPASS_UNLIT
		gl_SetColor(lightlevel, Colormap.LightColor, (float)fabs(alpha));
		DoRenderWall(false,NULL);
	}
	else
	{
		if ((gl_isGlowingTexture(topflat) || gl_isGlowingTexture(bottomflat)) && 
			!gl_fixedcolormap && ytop[0]==ytop[1] && ybottom[0]==ybottom[1] && gltexture)
		{
			DoRenderGlowingPoly(true, pass!=GLPASS_TEXTURE);
		}
		else
		{
			if (pass!=GLPASS_TEXTURE) gl_SetColor(lightlevel, Colormap.LightColor, (float)fabs(alpha));
			DoRenderWall(true,NULL);
		}
	}


	// restore default settings
	if (pass!=GLPASS_TEXTURE) switch(RenderStyle)
	{
	case STYLE_Add:
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (gltexture)	
	{
		if (clampy) gl.TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		if (clampx) gl.TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	}
	else 
	{
		gl.Enable(GL_TEXTURE_2D);
	}
	if (pass==GLPASS_UNLIT && flag==RENDERWALL_M2SNF) gl_EnableFog(true);
}



//==========================================================================
//
// one sided walls are by definition always opaque so no transparencies will be
// considered here!
//
//==========================================================================
void GLWall::RenderOneSidedWall(int pass)
{
	//int lightpass=0;
	bool renderglowing=false;
	FLightNode * node;

	if ((gl_isGlowingTexture(topflat) || gl_isGlowingTexture(bottomflat)) && !gl_fixedcolormap &&
		(!gl_isWhite(Colormap.LightColor) || lightlevel<255))
	{
		renderglowing=true;
	}
	switch(pass)
	{
	case GLPASS_UNLIT:
		if (!renderglowing) gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,1.0f);
		gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Normal);
		if (gltexture) gltexture->Bind(Colormap.LightColor.a);
		else 
		{
			gl.Disable(GL_TEXTURE_2D);
			gl_SetColorMode(CM_DEFAULT);
		}
		if (!renderglowing) DoRenderWall(true,NULL);
		else DoRenderGlowingPoly();
		if (!gltexture) gl.Enable(GL_TEXTURE_2D);
		break;


	case GLPASS_BASE:
		if (!renderglowing) gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,1.0f);
		gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Normal);
		if (!renderglowing) DoRenderWall(true,NULL);
		else DoRenderGlowingPoly(false, true); 
		break;

	case GLPASS_TEXTURE:
		if (gltexture) 
		{
			gltexture->Bind(Colormap.LightColor.a);
			if (!renderglowing) DoRenderWall(true,NULL);
			else DoRenderGlowingPoly(true, false); 
		}
		break;

	case GLPASS_LIGHT:
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

		if (!seg->bPolySeg)
		{
			// Iterate through all dynamic lights which touch this wall and render them
			node = seg->sidedef->lighthead;
		}
		else if (sub)
		{
			// To avoid constant rechecking for polyobjects use the subsector's lightlist instead
			node = gl_subsectors[sub-subsectors].lighthead;
		}
		else node = NULL;
		while (node)
		{
			if (!(node->lightsource->flags2&MF2_DORMANT))
			{
				iter_dlight++;
				if (!renderglowing) DoRenderWall(true, NULL, node->lightsource);
				else DoRenderGlowingPoly(true, false, node->lightsource);
			}
			node = node->nextLight;
		}
	}
}

//==========================================================================
//
// 
//
//==========================================================================
void GLWall::Draw(int pass)
{
	switch(flag)
	{
	case RENDERWALL_MIRRORSURFACE:
		if (pass!=GLPASS_DECALS)
		{
			if (!RenderMirror()) 
			{
				if (!r_mirrors)
				{
					gl.DepthFunc(GL_LEQUAL);
					RenderOneSidedWall(GLPASS_UNLIT);
					gl.DepthFunc(GL_LESS);
				}
			}
			if (seg->sidedef->BoundActors && r_mirrors)
			{
				ADecal * decal=static_cast<ADecal*>(seg->sidedef->BoundActors);

				// This has to set the decal drawing mode itself because it is not done in 
				// the decal pass
				gl.Enable(GL_POLYGON_OFFSET_FILL);
				gl.PolygonOffset(-1.0f, -128.0f);
				gl.DepthMask(false);
				DoDrawDecals(decal, seg);
				gl.DepthMask(true);
				gl.PolygonOffset(0.0f, 0.0f);
				gl.Disable(GL_POLYGON_OFFSET_FILL);
				gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
		break;									

	case RENDERWALL_FOGSHEET:
		if (pass!=GLPASS_DECALS) RenderFogSheet();
		break;

	case RENDERWALL_M2S:
		if (pass==GLPASS_LIGHT)
		{
			RenderOneSidedWall(pass);
			break;
		}
	case RENDERWALL_M2SNF:
	case RENDERWALL_COLOR:
		if (pass!=GLPASS_DECALS) RenderTwoSidedWall(pass);

		break;

	default:
		if (pass!=GLPASS_DECALS) RenderOneSidedWall(pass);
		else if (seg->sidedef && seg->sidedef->BoundActors)
		{
			ADecal * decal=static_cast<ADecal*>(seg->sidedef->BoundActors);
			gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Normal);
			DoDrawDecals(decal, seg);
		}
		break;
	}
}


//==========================================================================
//
// 
//
//==========================================================================
void GLWall::PutWall(bool translucent)
{
	int list;
	GLPortal * portal;

	static char passflag[]={
		0,		//RENDERWALL_NONE,             
		1,		//RENDERWALL_TOP,              
		1,		//RENDERWALL_M1S,              
		0,		//RENDERWALL_M2S,              
		1,		//RENDERWALL_BOTTOM,           
		2,		//RENDERWALL_SKYDOME,              
		0,		//RENDERWALL_DECAL,            
		0,		//RENDERWALL_FOGSHEET,         
		2,		//RENDERWALL_HORIZON,          
		2,		//RENDERWALL_SKYBOX,           
		2,		//RENDERWALL_SECTORSTACK,
		2,		//RENDERWALL_MIRROR,           
		0,		//RENDERWALL_MIRRORSURFACE,    
		0,		//RENDERWALL_M2SNF,            
		0,		//RENDERWALL_COLOR,            
		1,		//RENDERWALL_FFBLOCK          
	};

	if (gl_fixedcolormap) 
	{
		// light planes don't get drawn with fullbright rendering
		if (!gltexture && passflag[flag]!=2) return;

		Colormap.GetFixedColormap();
	}

	/* this doesn't work that easily
	if (gltexture && gltexture->GlowingTexture())
	{
		lightlevel=255;
		LightColor=0xffffff;
	}
	*/

	switch (passflag[flag])
	{
	default:
		if (translucent)
		{
			viewdistance=P_AproxDistance( ((seg->linedef->v1->x+seg->linedef->v2->x)>>1) - viewx,
				((seg->linedef->v1->y+seg->linedef->v2->y)>>1) - viewy);
			list=GLDL_TRANSLUCENT;
		}
		else if (!gl_lights || !seg->sidedef || (seg->sidedef->lighthead==NULL && !seg->bPolySeg))
		{
			// no light touches this wall
			list=GLDL_UNLIT;
		}
		else if ( !gl_fixedcolormap && passflag[flag]==1)
		{
			list = (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE) ? GLDL_LITFOG : gltexture->tex->bMasked? GLDL_MASKED : GLDL_LIT;
		}
		else if (flag==RENDERWALL_M2S && !gl_fixedcolormap)
		{
			if (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE) list = GLDL_LITFOG;
			else list = GLDL_MASKED;
		}
		else list = GLDL_UNLIT;

		gl_drawlist[list].AddWall(this);
		break;

	case 2:
		switch (flag)
		{
		// portals don't go into the draw list.
		// Instead they are added to the portal manager
		case RENDERWALL_HORIZON:
			horizon=UniqueHorizons.Get(horizon);
			portal=GLPortal::FindPortal(horizon);
			if (!portal) portal=new GLHorizonPortal(horizon);
			portal->AddLine(this);
			break;

		case RENDERWALL_SKYBOX:
			portal=GLPortal::FindPortal(skybox);
			if (!portal) portal=new GLSkyboxPortal(skybox);
			portal->AddLine(this);
			break;

		case RENDERWALL_SECTORSTACK:
			stack=UniqueStacks.Get(stack);	// map all stacks with the same displacement together
			portal=GLPortal::FindPortal(stack);
			if (!portal) portal=new GLSectorStackPortal(stack);
			portal->AddLine(this);
			break;

		case RENDERWALL_MIRROR:
			portal=GLPortal::FindPortal(seg->linedef);
			if (!portal) portal=new GLMirrorPortal(seg->linedef);
			portal->AddLine(this);
			if (r_mirrors) 
			{
				// draw a reflective layer over the mirror
				flag=RENDERWALL_MIRRORSURFACE;
				viewdistance=P_AproxDistance( ((seg->linedef->v1->x+seg->linedef->v2->x)>>1) - viewx,
					((seg->linedef->v1->y+seg->linedef->v2->y)>>1) - viewy);
				gl_drawlist[GLDL_TRANSLUCENTBORDER].AddWall(this);
			}
			break;

		case RENDERWALL_SKY:
			sky=UniqueSkies.Get(sky);
			portal=GLPortal::FindPortal(sky);
			if (!portal) portal=new GLSkyPortal(sky);
			portal->AddLine(this);
			break;
			
			
		}
		break;


	}
}

//==========================================================================
//
//  Splits a wall vertically if a 3D-floor
//	creates different lighting across the wall
//
//==========================================================================
void GLWall::SplitWall(sector_t * frontsector, bool translucent)
{
	GLWall copyWall;
	fixed_t lightbottom;
	float maplightbottom;
	int i;
	TArray<lightlist_t> & lightlist=frontsector->e->lightlist;

#ifdef _DEBUG
	if (seg->linedef-lines==2782)
		__asm nop
#endif

	if (lightlist.Size()>1)
	{
		for(i=0;i<lightlist.Size()-1;i++)
		{
			// ok, this won't work for diagonal splits...
			// both edges have to be clipped independently
			if (i<lightlist.Size()-1) lightbottom=lightlist[i+1].plane.ZatPoint(0,0);
			else lightbottom= -32000*FRACUNIT;
				//realsector->floorplane.ZatPoint(0,0);

			//maplighttop=TO_MAP(frontsector->lightlist[i].height);
			maplightbottom=TO_MAP(lightbottom);

			// The light is completely below the wall!
			if (maplightbottom<ybottom[0] && maplightbottom<ybottom[1]) 
			{
				lightlevel=*lightlist[i].p_lightlevel;
				Colormap.LightColor=(*lightlist[i].p_extra_colormap)->Color;
				PutWall(translucent);
				return;
			}

			if (maplightbottom<ytop[0] && maplightbottom<ytop[1])
			{
				copyWall=*this;
				copyWall.lightlevel=*lightlist[i].p_lightlevel;
				copyWall.Colormap.LightColor=(*lightlist[i].p_extra_colormap)->Color;

				ytop[0]=ytop[1]=copyWall.ybottom[0]=copyWall.ybottom[1]=maplightbottom;
				uprgt.v=uplft.v=copyWall.lolft.v=copyWall.lorgt.v=copyWall.uplft.v+ 
					(maplightbottom-copyWall.ytop[0])*(copyWall.lolft.v-copyWall.uplft.v)/(ybottom[0]-copyWall.ytop[0]);
				copyWall.PutWall(translucent);
			}
			if (ytop[0]==ybottom[0]) return;
		}
	}

	// These values must not be destroyed!
	int ll=lightlevel;
	PalEntry lc=Colormap.LightColor;

	lightlevel=*lightlist[lightlist.Size()-1].p_lightlevel;
	Colormap.LightColor=(*lightlist[lightlist.Size()-1].p_extra_colormap)->Color;
	PutWall(translucent);

	lightlevel=ll;
	Colormap.LightColor=lc;
}


//==========================================================================
//
// 
//
//==========================================================================
bool GLWall::DoHorizon(seg_t * seg,sector_t * fs, vertex_t * v1,vertex_t * v2)
{
	GLHorizonInfo hi;
	lightlist_t * light;

	// ZDoom doesn't support slopes in a horizon sector so I won't either!
	ytop[1]=ytop[0]=(float)fs->ceilingtexz/MAP_SCALE;
	ybottom[1]=ybottom[0]=(float)fs->floortexz/MAP_SCALE;

	if (viewz<fs->ceilingtexz)
	{
		if (viewz>fs->floortexz)
			ybottom[1]=ybottom[0]=(float)viewz/MAP_SCALE;

		if (fs->ceilingpic==skyflatnum)
		{
			SkyTexture(fs->sky, fs->CeilingSkyBox, true);
		}
		else
		{
			flag=RENDERWALL_HORIZON;
			hi.plane.GetFromSector(fs, true);
			hi.lightlevel=GetCeilingLight(fs);
			hi.colormap=fs->ColorMap;

			if (fs->e->ffloors.Size())
			{
				light = P_GetPlaneLight(fs, &fs->ceilingplane, true);

				if(!(fs->CeilingFlags&SECF_ABSLIGHTING)) hi.lightlevel = *light->p_lightlevel;
				hi.colormap.LightColor = (*light->p_extra_colormap)->Color;
			}

			if (gl_fixedcolormap) hi.colormap.GetFixedColormap();
			horizon=&hi;
			topflat=1;
			PutWall(0);
		}
		ytop[1]=ytop[0]=ybottom[0];
	}

	if (viewz>fs->floortexz)
	{
		ybottom[1]=ybottom[0]=(float)fs->floortexz/MAP_SCALE;
		if (fs->floorpic==skyflatnum)
		{
			SkyTexture(fs->sky, fs->FloorSkyBox, false);
		}
		else
		{
			flag=RENDERWALL_HORIZON;
			hi.plane.GetFromSector(fs, false);
			hi.lightlevel=GetFloorLight(fs);
			hi.colormap=fs->ColorMap;

			if (fs->e->ffloors.Size())
			{
				light = P_GetPlaneLight(fs, &fs->floorplane, false);

				if(!(fs->FloorFlags&SECF_ABSLIGHTING)) hi.lightlevel = *light->p_lightlevel;
				hi.colormap.LightColor = (*light->p_extra_colormap)->Color;
			}

			if (gl_fixedcolormap) hi.colormap.GetFixedColormap();
			horizon=&hi;
			topflat=0;
			PutWall(0);
		}
	}
	return true;
}

//==========================================================================
//
// 
//
//==========================================================================
bool GLWall::SetWallCoordinates(seg_t * seg, int texturetop,
								int topleft,int topright, int bottomleft,int bottomright, int t_ofs)

{
	//
	//
	// set up texture coordinate stuff
	//
	// 
	const WorldTextureInfo * wti;
	float l_ul;
	float length= seg->sidedef? seg->sidedef->TexelLength: 
								(P_AproxDistance (seg->v2->x-seg->v1->x, seg->v2->y-seg->v1->y)>>16);
	//fixed_t t_ofs = seg->sidedef? seg->sidedef->textureoffset : 0;
	
	
	if (gltexture) 
	{
		wti=gltexture->GetWorldTextureInfo();
		l_ul=wti->FixToTexU(gltexture->TextureOffset(t_ofs));
	}
	else 
	{
		wti=NULL;
		l_ul=0;
	}


	//
	//
	// set up coordinates for the left side of the polygon
	//
	// check left side for intersections
	if (topleft>=bottomleft)
	{
		// normal case
		ytop[0]=TO_MAP(topleft);
		ybottom[0]=TO_MAP(bottomleft);

		if (wti)
		{
			uplft.u=lolft.u=l_ul;
			uplft.v=wti->FixToTexV(-topleft+texturetop);
			lolft.v=wti->FixToTexV(-bottomleft+texturetop);
		}
	}
	else
	{
		// ceiling below floor - clip to the visible part of the wall
		fixed_t dch=topright-topleft;
		fixed_t dfh=bottomright-bottomleft;
		fixed_t coeff=FixedDiv(bottomleft-topleft, dch-dfh);

		fixed_t inter_y=topleft+FixedMul(coeff,dch);
											 
		float inter_x= coeff/(float)FRACUNIT;

		glseg.x1=glseg.x1+inter_x*(glseg.x2-glseg.x1);
		glseg.z1=glseg.z1+inter_x*(glseg.z2-glseg.z1);
		glseg.noorigverts=true;
		fracleft = inter_x;

		ybottom[0]=ytop[0]=TO_MAP(inter_y);	

		if (wti)
		{
			uplft.u=lolft.u=l_ul+wti->FloatToTexU(inter_x*length);
			lolft.v=uplft.v=wti->FixToTexV(-inter_y+texturetop);
		}
	}

	//
	//
	// set up coordinates for the right side of the polygon
	//
	// check left side for intersections
	if (topright>=bottomright)
	{
		// normal case
		ytop[1]=TO_MAP(topright)		;
		ybottom[1]=TO_MAP(bottomright)		;

		if (wti)
		{
			uprgt.u=lorgt.u=l_ul+wti->FloatToTexU(length);
			uprgt.v=wti->FixToTexV(-topright+texturetop);
			lorgt.v=wti->FixToTexV(-bottomright+texturetop);
		}
	}
	else
	{
		// ceiling below floor - clip to the visible part of the wall
		fixed_t dch=topright-topleft;
		fixed_t dfh=bottomright-bottomleft;
		fixed_t coeff=FixedDiv(bottomleft-topleft, dch-dfh);

		fixed_t inter_y=topleft+FixedMul(coeff,dch);
											 
		float inter_x= coeff/(float)FRACUNIT;

		glseg.x2=glseg.x1+inter_x*(glseg.x2-glseg.x1);
		glseg.z2=glseg.z1+inter_x*(glseg.z2-glseg.z1);
		glseg.noorigverts=true;
		fracright = inter_x;

		ybottom[1]=ytop[1]=TO_MAP(inter_y);
		if (wti)
		{
			uprgt.u=lorgt.u=l_ul+wti->FloatToTexU(inter_x*length);
			lorgt.v=uprgt.v=wti->FixToTexV(-inter_y+texturetop);
		}
	}
	if (gltexture && gltexture->tex->bHasCanvas && clampy)
	{
		// Camera textures are upside down so we have to shift the y-coordinate
		// from [-1..0] to [0..1] when using texture clamping!

		uplft.v+=1.f;
		uprgt.v+=1.f;
		lolft.v+=1.f;
		lorgt.v+=1.f;
	}
	return true;
}

//==========================================================================
//
// 
//
//==========================================================================
void GLWall::DoTexture(int type,seg_t * seg,int peg,
					   int ceilingrefheight,int floorrefheight,
					   int topleft,int topright,
					   int bottomleft,int bottomright,
					   int v_offset)
{
	if (topleft<=bottomleft && topright<=bottomright) return;

	//**************************
	//
	//  Handle one sided walls, upper and lower texture
	//
	//**************************

	// The Vertex values can be destroyed in this function and must be restored aferward!
	GLSeg glsave=glseg;
	int lh=ceilingrefheight-floorrefheight;


	flag=type;
	if (flag==RENDERWALL_M1S && seg->linedef->special==Line_Mirror && !(gl.flags&RFL_NOSTENCIL)) 
		flag=(r_mirrors) ? RENDERWALL_MIRROR : RENDERWALL_MIRRORSURFACE;

	ceilingrefheight+= 	gltexture->RowOffset(seg->sidedef->rowoffset)+
						(peg ? (gltexture->TextureHeight()<<FRACBITS)-lh-v_offset:0);
	if (!SetWallCoordinates(seg, ceilingrefheight, topleft, topright, bottomleft, bottomright, seg->sidedef->textureoffset)) return;

	//
	//
	// Draw the stuff
	//
	//
	if (seg->frontsector->e->lightlist.Size()==0) PutWall(false);
	else SplitWall(seg->frontsector, false);
	glseg=glsave;
}


//==========================================================================
//
// 
//
//==========================================================================
void GLWall::DoMidTexture(seg_t * seg, bool drawfogsheet, 
						  sector_t * realfront, sector_t * realback,
						  fixed_t fch1, fixed_t fch2, fixed_t ffh1, fixed_t ffh2,
						  fixed_t bch1, fixed_t bch2, fixed_t bfh1, fixed_t bfh2)
								
{
	fixed_t topleft,bottomleft,topright,bottomright;
	GLSeg glsave=glseg;
	fixed_t texturetop, texturebottom;

	//
	//
	// Get the base coordinates for the texture
	//
	//
	if (gltexture)
	{
		// Align the texture to the ORIGINAL sector's height!!
		// At this point slopes don't matter because they don't affect the texture's z-position

		fixed_t rowoffset=gltexture->RowOffset(seg->sidedef->rowoffset);
		if ( (seg->linedef->flags & ML_DONTPEGBOTTOM) >0)
		{
			texturebottom=max(realfront->floortexz,realback->floortexz)+rowoffset;
			texturetop=texturebottom+(gltexture->TextureHeight()<<FRACBITS);
		}
		else
		{
			texturetop=min(realfront->ceilingtexz,realback->ceilingtexz)+rowoffset;
			texturebottom=texturetop-(gltexture->TextureHeight()<<FRACBITS);
		}
	}
	else texturetop=texturebottom=0;

	//
	//
	// Depending on missing textures and possible plane intersections
	// decide which planes to use for the polygon
	//
	//
	if (drawfogsheet || realfront!=realback || 
		(realfront->heightsec && !(realfront->heightsec->MoreFlags & SECF_IGNOREHEIGHTSEC)))
	{
		//
		//
		// Set up the top
		//
		//
		FTexture * tex = TexMan(seg->sidedef->toptexture);
		gl_sectordata * glsec = &gl_sectors[realfront - sectors];
		if (!tex || tex->UseType==FTexture::TEX_Null ||
			seg->sidedef->LoopIndex==32767)
		{
			// texture is missing - use the higher plane
			topleft=max(bch1,fch1);
			topright=max(bch2,fch2);
		}
		else if (bch1>fch1 || bch2>fch2) // (!((bch1<=fch1 && bch2<=fch2) || (bch1>=fch1 && bch2>=fch2)))
		{
			// the ceiling planes intersect. Use the backsector's ceiling for drawing so that
			// drawing the front sector's plane clips the polygon automatically.
			topleft=bch1;
			topright=bch2;
		}
		else
		{
			// normal case - use the lower plane
			topleft=min(bch1,fch1);
			topright=min(bch2,fch2);
		}
		
		//
		//
		// Set up the bottom
		//
		//
		tex = TexMan(seg->sidedef->bottomtexture);
		if (!tex || tex->UseType==FTexture::TEX_Null ||
			seg->sidedef->LoopIndex==32767)
		{
			// texture is missing - use the lower plane
			bottomleft=min(bfh1,ffh1);
			bottomright=min(bfh2,ffh2);
		}
		else if (bfh1<ffh1 || bfh2<ffh2) // (!((bfh1<=ffh1 && bfh2<=ffh2) || (bfh1>=ffh1 && bfh2>=ffh2)))
		{
			// the floor planes intersect. Use the backsector's floor for drawing so that
			// drawing the front sector's plane clips the polygon automatically.
			bottomleft=bfh1;
			bottomright=bfh2;
		}
		else
		{
			// normal case - use the higher plane
			bottomleft=max(bfh1,ffh1);
			bottomright=max(bfh2,ffh2);
		}
		
		//
		//
		// if we don't need a fog sheet let's clip away some unnecessary parts of the polygon
		//
		//
		if (!drawfogsheet)
		{
			if (texturetop<topleft && texturetop<topright) topleft=topright=texturetop;
			if (texturebottom>bottomleft && texturebottom>bottomright) bottomleft=bottomright=texturebottom;
		}
	}
	else
	{
		//
		//
		// if both sides of the line are in the same sector and the sector
		// doesn't have a Transfer_Heights special don't clip to the planes
		// Clipping to the planes is not necessary and can even produce
		// unwanted side effects.
		//
		//
		topleft=topright=texturetop;
		bottomleft=bottomright=texturebottom;
	}

	// nothing visible - skip the rest
	if (topleft<=bottomleft && topright<=bottomright) return;


	//
	//
	// set up texture coordinate stuff
	//
	// 
	fixed_t t_ofs = seg->sidedef->textureoffset;

	if (gltexture)
	{
		// Determine clamping based on the linedef, not the seg - even if segs are rendered!
		fixed_t textureoffset=gltexture->TextureOffset(t_ofs);

		/*	
		if (t_ofs<0)
		{
		}
		*/

		int righttex=textureoffset+seg->sidedef->TexelLength;
		
		clampx= (textureoffset==0 && righttex<=gltexture->TextureWidth()) || 
			(righttex==gltexture->TextureWidth() && textureoffset>0);
		clampy=true;
	}
	SetWallCoordinates(seg, texturetop, topleft, topright, bottomleft, bottomright, t_ofs);

	//
	//
	// draw fog sheet if required
	//
	// 
	if (drawfogsheet)
	{
		flag=RENDERWALL_FOGSHEET;
		PutWall(true);
		if (!gltexture) return;
		flag=RENDERWALL_M2SNF;
	}
	else flag=RENDERWALL_M2S;

	//
	//
	// set up alpha blending
	//
	// 
	if (seg->linedef->alpha)// && seg->linedef->special!=Line_Fogsheet)
	{
		bool translucent;

		switch (seg->sidedef->Flags& WALLF_ADDTRANS)//TRANSBITS)
		{
		case 0:
			if (seg->linedef->alpha<255)
			{
				RenderStyle=STYLE_Translucent;
				alpha=(float)seg->linedef->alpha/255.0f;
				translucent=true;
			}
			else if (seg->linedef->alpha==255)
			{
				RenderStyle=STYLE_Normal;
				alpha=1.0f;
				translucent=false;
			}
			break;

		case WALLF_ADDTRANS:
			RenderStyle=STYLE_Add;
			alpha=(float)seg->linedef->alpha/255.0f;
			translucent=true;
			break;
		}

		//
		//
		// for textures with large empty areas only the visible parts are drawn.
		// If these textures come too close to the camera they severely affect performance
		// if stacked closely together
		// Recognizing vertical gaps is rather simple and well worth the effort.
		//
		//
		int v=gltexture->GetAreaCount();
		if (v>0 && !drawfogsheet)
		{
			// split the poly!
			GLWall split;
			int i,t=0;
			GL_RECT * splitrect=gltexture->GetAreas();
			float v_factor=(ybottom[0]-ytop[0])/(lolft.v-uplft.v);
			// only split the vertical area of the polygon that does not contain slopes!
			float splittopv=max(uplft.v, uprgt.v);
			float splitbotv=min(lolft.v, lorgt.v);

			// this is split vertically into sections.
			for(i=0;i<v;i++)
			{
				// the current segment is below the bottom line of the splittable area
				// (iow. the whole wall has been done)
				if (splitrect[i].top>=splitbotv) break;

				float splitbot=splitrect[i].top+splitrect[i].height;

				// the current segment is above the top line of the splittable area
				if (splitbot<=splittopv) continue;

				split=*this;

				// the top line of the current segment is inside the splittable area
				// use the splitrect's top as top of this segment
				// if not use the top of the remaining polygon
				if (splitrect[i].top>splittopv)
				{
					split.ytop[0]=split.ytop[1]= ytop[0]+v_factor*(splitrect[i].top-uplft.v);
					split.uplft.v=split.uprgt.v=splitrect[i].top;
				}
				// the bottom line of the current segment is inside the splittable area
				// use the splitrect's bottom as bottom of this segment
				// if not use the bottom of the remaining polygon
				if (splitbot<splitbotv)
				{
					split.ybottom[0]=split.ybottom[1]=ytop[0]+v_factor*(splitbot-uplft.v);
					split.lolft.v=split.lorgt.v=splitbot;
				}
				//
				//
				// Draw the stuff
				//
				//
				if (realfront->e->lightlist.Size()==0) split.PutWall(translucent);
				else split.SplitWall(realfront, translucent);

				t=1;
			}
			render_texsplit+=t;
		}
		else
		{
			//
			//
			// Draw the stuff without splitting
			//
			//
			if (realfront->e->lightlist.Size()==0) PutWall(translucent);
			else SplitWall(realfront, translucent);
		}
		alpha=1.0f;
		RenderStyle=STYLE_Normal;
	}
	// restore some values that have been altered in this function!
	glseg=glsave;
	clampx=clampy=false;
}


//==========================================================================
//
// 
//
//==========================================================================
void GLWall::BuildFFBlock(seg_t * seg, F3DFloor * rover,
						  fixed_t ff_topleft, fixed_t ff_topright, 
						  fixed_t ff_bottomleft, fixed_t ff_bottomright)
{
	side_t * mastersd=&sides[rover->master->sidenum[0]];
	int to;
	lightlist_t * light;
	bool translucent;
	byte savelight=lightlevel;
	PalEntry savecolor=Colormap.LightColor;
	float ul, ur;

	if (rover->flags&FF_FOG)
	{
		if (!gl_fixedcolormap)
		{
			// this may not yet be done!
			light=P_GetPlaneLight(rover->target, rover->top.plane,true);
			Colormap.LightColor=(*light->p_extra_colormap)->Fade;
			// the fog plane defines the light level, not the front seclightr!
			lightlevel=*light->p_lightlevel;
			gltexture=NULL;
		}
		else return;
	}
	else
	{
		if (rover->flags&FF_UPPERTEXTURE)
			gltexture=FGLTexture::ValidateTexture(seg->sidedef->toptexture);
		else if (rover->flags&FF_LOWERTEXTURE)
			gltexture=FGLTexture::ValidateTexture(seg->sidedef->bottomtexture);
		else
			gltexture=FGLTexture::ValidateTexture(mastersd->midtexture);

		if (!gltexture) return;
		const WorldTextureInfo * wti=gltexture->GetWorldTextureInfo();
		if (!wti) return;

		
		to=(rover->flags&(FF_UPPERTEXTURE|FF_LOWERTEXTURE))? 
			0:gltexture->TextureOffset(mastersd->textureoffset);
		ul=wti->FixToTexU(to+gltexture->TextureOffset(seg->sidedef->textureoffset));
		ur=ul+wti->FloatToTexU(seg->sidedef->TexelLength);

		uplft.u=lolft.u=ul;
		uprgt.u=lorgt.u=ur;
		
		fixed_t rowoffset=gltexture->RowOffset(seg->sidedef->rowoffset);
		to=(rover->flags&(FF_UPPERTEXTURE|FF_LOWERTEXTURE))? 
			0:gltexture->RowOffset(mastersd->rowoffset);
		
		uplft.v=wti->FixToTexV(to+rowoffset+*rover->top.texheight-ff_topleft);
		uprgt.v=wti->FixToTexV(to+rowoffset+*rover->top.texheight-ff_topright);
		lolft.v=wti->FixToTexV(to+rowoffset+*rover->top.texheight-ff_bottomleft);
		lorgt.v=wti->FixToTexV(to+rowoffset+*rover->top.texheight-ff_bottomright);
		flag=RENDERWALL_FFBLOCK;
	}

	ytop[0]=TO_MAP(ff_topleft);
	ytop[1]=TO_MAP(ff_topright);
	ybottom[0]=TO_MAP(ff_bottomleft);//-0.001f;
	ybottom[1]=TO_MAP(ff_bottomright);

	if (rover->flags&FF_TRANSLUCENT)
	{
		alpha=rover->alpha/255.0f;
		RenderStyle=STYLE_Translucent;
		translucent=true;
		flag=gltexture? RENDERWALL_M2S:RENDERWALL_COLOR;
	}
	else 
	{
		alpha=1.0f;
		RenderStyle=STYLE_Normal;
		translucent=false;
	}
	
	if (seg->frontsector->e->lightlist.Size()==0) PutWall(translucent);
	else SplitWall(seg->frontsector, translucent);
	alpha=1.0f;
	lightlevel=savelight;
	Colormap.LightColor=savecolor;
}


//==========================================================================
//
// 
//
//==========================================================================
void GLWall::InverseFloors(seg_t * seg, sector_t * frontsector,
						   fixed_t topleft, fixed_t topright, 
						   fixed_t bottomleft, fixed_t bottomright)
{
	TArray<F3DFloor *> & frontffloors=frontsector->e->ffloors;

	for(int i=0;i<frontffloors.Size();i++)
	{
		F3DFloor * rover=frontffloors[i];
		if (!(rover->flags&FF_EXISTS)) continue;
		if (!(rover->flags&FF_RENDERSIDES)) continue;
		if (!(rover->flags&(FF_INVERTSIDES|FF_ALLSIDES))) continue;

		fixed_t ff_topleft=rover->top.plane->ZatPoint(vertexes[0]);
		fixed_t ff_topright=rover->top.plane->ZatPoint(vertexes[1]);
		fixed_t ff_bottomleft=rover->bottom.plane->ZatPoint(vertexes[0]);
		fixed_t ff_bottomright=rover->bottom.plane->ZatPoint(vertexes[1]);


		// above ceiling
		if (ff_bottomleft>topleft && ff_bottomright>topright) continue;

		if (ff_topleft>topleft && ff_topright>topright)
		{
			// the new section overlaps with the previous one - clip it!
			ff_topleft=topleft;
			ff_topright=topright;
		}
		if (ff_bottomleft<bottomleft && ff_bottomright<bottomright)
		{
			ff_bottomleft=bottomleft;
			ff_bottomright=bottomright;
		}
		if (ff_topleft<ff_bottomleft || ff_topright<ff_bottomright) continue;

		BuildFFBlock(seg, rover, ff_topleft, ff_topright, ff_bottomleft, ff_bottomright);
		topleft=ff_bottomleft;
		topright=ff_bottomright;

		if (topleft<=bottomleft && topright<=bottomright) return;
	}

}

//==========================================================================
//
// 
//
//==========================================================================
void GLWall::ClipFFloors(seg_t * seg, F3DFloor * ffloor, sector_t * frontsector,
						 fixed_t topleft, fixed_t topright, 
						 fixed_t bottomleft, fixed_t bottomright)
{
	TArray<F3DFloor *> & frontffloors=frontsector->e->ffloors;

	int flags=ffloor->flags&FF_SWIMMABLE|FF_TRANSLUCENT;

	for(int i=0;i<frontffloors.Size();i++)
	{
		F3DFloor * rover=frontffloors[i];
		if (!(rover->flags&FF_EXISTS)) continue;
		if (!(rover->flags&FF_RENDERSIDES)) continue;
		if ((rover->flags&flags)!=flags) continue;

		fixed_t ff_topleft=rover->top.plane->ZatPoint(vertexes[0]);
		fixed_t ff_topright=rover->top.plane->ZatPoint(vertexes[1]);
		fixed_t ff_bottomleft=rover->bottom.plane->ZatPoint(vertexes[0]);
		fixed_t ff_bottomright=rover->bottom.plane->ZatPoint(vertexes[1]);


		// above top line
		if (ff_bottomleft>topleft && ff_bottomright>topright) continue;

		// we are completely below the bottom so unless there are some
		// (unsupported) intersections there won't be any more floors that
		// could clip this one.
		if (ff_topleft<bottomleft && ff_topright<bottomright) return;

		// overlapping the top line
		if (ff_topleft>=topleft && ff_topright>=topright)
		{
			// overlapping with the entire range
			if (ff_bottomleft<=bottomleft && ff_bottomright<=bottomright) return;
			else if (ff_bottomleft>bottomleft && ff_bottomright>bottomright)
			{
				topleft=ff_bottomleft;
				topright=ff_bottomright;
			}
			else
			{
				// an intersecting case.
				// proper handling requires splitting but
				// I don't need this right now.
			}
		}
		else if (ff_topleft<=topleft && ff_topright<=topright)
		{
			BuildFFBlock(seg, ffloor, topleft, topright, ff_topleft, ff_topright);
			if (ff_bottomleft<=bottomleft && ff_bottomright<=bottomright) return;
			topleft=ff_bottomleft;
			topright=ff_bottomright;
		}
		else
		{
			// an intersecting case.
			// proper handling requires splitting but
			// I don't need this right now.
		}
	}

	// if the program reaches here there is one block left to draw
	BuildFFBlock(seg, ffloor, topleft, topright, bottomleft, bottomright);
}

//==========================================================================
//
// 
//
//==========================================================================
void GLWall::DoFFloorBlocks(seg_t * seg,sector_t * frontsector,sector_t * backsector,
							fixed_t fch1, fixed_t fch2, fixed_t ffh1, fixed_t ffh2,
							fixed_t bch1, fixed_t bch2, fixed_t bfh1, fixed_t bfh2)

{
	TArray<F3DFloor *> & backffloors=backsector->e->ffloors;
	fixed_t topleft, topright, bottomleft, bottomright;
	bool renderedsomething=false;

	// if the ceilings intersect use the backsector's height because this sector's ceiling will
	// obstruct the redundant parts.

	if (fch1<bch1 && fch2<bch2) 
	{
		topleft=fch1;
		topright=fch2;
	}
	else
	{
		topleft=bch1;
		topright=bch2;
	}

	if (ffh1>bfh1 && ffh2>bfh2) 
	{
		bottomleft=ffh1;
		bottomright=ffh2;
	}
	else
	{
		bottomleft=bfh1;
		bottomright=bfh2;
	}

	for(int i=0;i<backffloors.Size();i++)
	{
		F3DFloor * rover=backffloors[i];
		if (!(rover->flags&FF_EXISTS)) continue;
		if (!(rover->flags&FF_RENDERSIDES) || (rover->flags&FF_INVERTSIDES)) continue;

		fixed_t ff_topleft=rover->top.plane->ZatPoint(vertexes[0]);
		fixed_t ff_topright=rover->top.plane->ZatPoint(vertexes[1]);
		fixed_t ff_bottomleft=rover->bottom.plane->ZatPoint(vertexes[0]);
		fixed_t ff_bottomright=rover->bottom.plane->ZatPoint(vertexes[1]);


		// completely above ceiling
		if (ff_bottomleft>topleft && ff_bottomright>topright && !renderedsomething) continue;

		if (ff_topleft>topleft && ff_topright>topright)
		{
			// the new section overlaps with the previous one - clip it!
			ff_topleft=topleft;
			ff_topright=topright;
		}

		// do all inverse floors above the current one it there is a gap between the
		// last 3D floor and this one.
		if (topleft>ff_topleft && topright>ff_topright)
			InverseFloors(seg, frontsector, topleft, topright, ff_topleft, ff_topright);

		// if translucent or liquid clip away adjoining parts of the same type of FFloors on the other side
		if (rover->flags&(FF_SWIMMABLE|FF_TRANSLUCENT))
			ClipFFloors(seg, rover, frontsector, ff_topleft, ff_topright, ff_bottomleft, ff_bottomright);
		else
			BuildFFBlock(seg, rover, ff_topleft, ff_topright, ff_bottomleft, ff_bottomright);

		topleft=ff_bottomleft;
		topright=ff_bottomright;
		renderedsomething=true;
		if (topleft<=bottomleft && topright<=bottomright) return;
	}

	if (backffloors.Size() == 0 && frontsector->e->ffloors.Size() > 0)
	{
		InverseFloors(seg, frontsector, topleft, topright, bottomleft, bottomright);
	}
}



//==========================================================================
//
// 
//
//==========================================================================
void GLWall::Process(seg_t *seg, sector_t * frontsector, sector_t * backsector, subsector_t * polysub)
{
	vertex_t * v1, * v2;
	fixed_t fch1;
	fixed_t ffh1;
	fixed_t fch2;
	fixed_t ffh2;
	sector_t * realfront;
	sector_t * realback;

#ifdef _DEBUG
	if (seg->linedef-lines==break_renderlinedef && IsDebuggerPresent())
		__asm int 3;	
	//if (seg->linedef-lines==2815)
	//if (/*seg->linedef-lines==12599 ||*/ seg->linedef-lines==12577 || seg->linedef-lines==12593)
		//__asm nop
#endif

	this->seg=seg;
	this->sub=polysub;

	if (polysub && seg->backsector)
	{
		// Textures on 2-sided polyobjects are aligned to the actual seg's sectors!
		realfront = seg->frontsector;
		realback = seg->backsector;
	}
	else
	{
		// Need these for aligning the textures!
		realfront = &sectors[frontsector->sectornum];
		realback = backsector? &sectors[backsector->sectornum] : NULL;
	}

	if (seg->sidedef == &sides[seg->linedef->sidenum[0]])
	{
		v1=seg->linedef->v1;
		v2=seg->linedef->v2;
	}
	else
	{
		v1=seg->linedef->v2;
		v2=seg->linedef->v1;
	}
	vertexes[0]=v1;
	vertexes[1]=v2;

	glseg.x1=-v1->x/(float)MAP_SCALE;
	glseg.z1= v1->y/(float)MAP_SCALE;
	glseg.x2=-v2->x/(float)MAP_SCALE;
	glseg.z2= v2->y/(float)MAP_SCALE;
	fracleft=0;
	fracright=1;
	glseg.noorigverts=false;
	clampx=clampy=false;

	lightlevel=frontsector->lightlevel;

	alpha=1.0f;
	RenderStyle=STYLE_Normal;
	gltexture=NULL;
	Colormap=frontsector->ColorMap;

	topflat=frontsector->ceilingpic;	// for glowing textures
	bottomflat=frontsector->floorpic;

	// Save a little time (up to 0.3 ms per frame ;) )
	if (frontsector->floorplane.a | frontsector->floorplane.b)
	{
		ffh1=frontsector->floorplane.ZatPoint(v1); 
		ffh2=frontsector->floorplane.ZatPoint(v2); 
		yfloor[0]=ffh1/MAP_SCALE;
		yfloor[1]=ffh2/MAP_SCALE;
	}
	else
	{
		ffh1 = ffh2 = frontsector->floortexz; 
		yfloor[0] = yfloor[1] = ffh2/MAP_SCALE;
	}

	if (frontsector->ceilingplane.a | frontsector->ceilingplane.b)
	{
		fch1=frontsector->ceilingplane.ZatPoint(v1);
		fch2=frontsector->ceilingplane.ZatPoint(v2);
		yceil[0]= fch1/MAP_SCALE;
		yceil[1]= fch2/MAP_SCALE;
	}
	else
	{
		fch1 = fch2 = frontsector->ceilingtexz;
		yceil[0] = yceil[1] = fch2/MAP_SCALE;
	}


	if (seg->linedef->special==Line_Horizon && !(gl.flags&RFL_NOSTENCIL))
	{
		SkyNormal(frontsector,v1,v2);
		DoHorizon(seg,frontsector, v1,v2);
		return;
	}

	//return;
	if (!backsector || !(seg->linedef->flags&ML_TWOSIDED)) /* onesided */
	{
		// sector's sky
		SkyNormal(frontsector,v1,v2);
		
		// normal texture
		gltexture=FGLTexture::ValidateTexture(seg->sidedef->midtexture);
		if (!gltexture) return;

		DoTexture(RENDERWALL_M1S,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
						  realfront->ceilingtexz,realfront->floortexz,	// must come from the original!
						  fch1,fch2,ffh1,ffh2,0);
	}
	else /* twosided */
	{

		fixed_t bch1;
		fixed_t bch2;
		fixed_t bfh1;
		fixed_t bfh2;

		if (backsector->floorplane.a | backsector->floorplane.b)
		{
			bfh1=backsector->floorplane.ZatPoint(v1); 
			bfh2=backsector->floorplane.ZatPoint(v2); 
		}
		else
		{
			bfh1 = bfh2 = backsector->floortexz; 
		}

		if (backsector->ceilingplane.a | backsector->ceilingplane.b)
		{
			bch1=backsector->ceilingplane.ZatPoint(v1);
			bch2=backsector->ceilingplane.ZatPoint(v2);
		}
		else
		{
			bch1 = bch2 = backsector->ceilingtexz;
		}

			// upper sky
		SkyTop(seg,frontsector,backsector,v1,v2);
		
		// upper texture
		if (!((frontsector->ceilingpic==skyflatnum) && (backsector->ceilingpic==skyflatnum)))
		{
			fixed_t bch1a=bch1, bch2a=bch2;
			if (frontsector->floorpic!=skyflatnum || backsector->floorpic!=skyflatnum)
			{
				// the back sector's floor obstructs part of this wall				
				if (ffh1>bch1 && ffh2>bch2) 
				{
					bch2a=ffh2;
					bch1a=ffh1;
				}
			}

			if (bch1a<fch1 || bch2a<fch2)
			{
				gltexture=FGLTexture::ValidateTexture(seg->sidedef->toptexture);
				if (gltexture) 
				{
					DoTexture(RENDERWALL_TOP,seg,(seg->linedef->flags & (ML_DONTPEGTOP))==0,
						realfront->ceilingtexz,realback->ceilingtexz,
						fch1,fch2,bch1a,bch2a,0);
				}
				else if ((frontsector->ceilingplane.a | frontsector->ceilingplane.b | 
						 backsector->ceilingplane.a | backsector->ceilingplane.b) && 
						frontsector->ceilingpic!=skyflatnum &&
						backsector->ceilingpic!=skyflatnum)
				{
					gltexture=FGLTexture::ValidateTexture(frontsector->ceilingpic);
					if (gltexture)
					{
						DoTexture(RENDERWALL_TOP,seg,(seg->linedef->flags & (ML_DONTPEGTOP))==0,
							realfront->ceilingtexz,realback->ceilingtexz,
							fch1,fch2,bch1a,bch2a,0);
					}
				}
				else
				{
					AddUpperMissingTexture(seg, bch1a);
				}
			}
		}

		/* mid texture */

		// in all other cases this might create more problems than it solves.
		bool drawfogsheet=((frontsector->ColorMap->Fade&0xffffff)!=0 && 
							(backsector->ColorMap->Fade&0xffffff)==0 &&
							!gl_fixedcolormap &&
							(frontsector->ceilingpic!=skyflatnum || backsector->ceilingpic!=skyflatnum));

		gltexture=FGLTexture::ValidateTexture(seg->sidedef->midtexture);
		if (gltexture || drawfogsheet)
		{
			DoMidTexture(seg, drawfogsheet, realfront, realback, fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
		}

		if (backsector->e->ffloors.Size() || frontsector->e->ffloors.Size()) 
		{
			DoFFloorBlocks(seg,frontsector,backsector, fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
		}
		
		/* bottom sky */
		SkyBottom(seg,frontsector,backsector,v1,v2);
		
		if (1)//!((frontsector->floorpic==skyflatnum) && (backsector->floorpic==skyflatnum)))
		{
			/* bottom texture */
			if (frontsector->ceilingpic!=skyflatnum || backsector->ceilingpic!=skyflatnum)
			{
				// the back sector's ceiling obstructs part of this wall				
				if (fch1<bfh1 && fch2<bfh2)
				{
					bfh1=fch1;
					bfh2=fch2;
				}
			}

			if (bfh1>ffh1 || bfh2>ffh2)
			{
				gltexture=FGLTexture::ValidateTexture(seg->sidedef->bottomtexture);
				if (gltexture) 
				{
					DoTexture(RENDERWALL_BOTTOM,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
						realback->floortexz,realfront->floortexz,
						bfh1,bfh2,ffh1,ffh2,
						frontsector->ceilingpic==skyflatnum && backsector->ceilingpic==skyflatnum ?
							realfront->floortexz-realback->ceilingtexz : 
							realfront->floortexz-realfront->ceilingtexz);
				}
				else if ((frontsector->floorplane.a | frontsector->floorplane.b | 
						backsector->floorplane.a | backsector->floorplane.b) && 
						frontsector->floorpic!=skyflatnum &&
						backsector->floorpic!=skyflatnum)
				{
					// render it anyway with the sector's floorpic. With a background sky
					// there are ugly holes otherwise and slopes are simply not precise enough
					// to mach in any case.
					gltexture=FGLTexture::ValidateTexture(frontsector->floorpic);
					if (gltexture)
					{
						DoTexture(RENDERWALL_BOTTOM,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
							realback->floortexz,realfront->floortexz,
							bfh1,bfh2,ffh1,ffh2, realfront->floortexz-realfront->ceilingtexz);
					}
				}
				else
				{
					AddLowerMissingTexture(seg, bfh1);
				}
			}
		}
	}
}

//==========================================================================
//
// 
//
//==========================================================================
void GLWall::ProcessLowerMiniseg(seg_t *seg, sector_t * frontsector, sector_t * backsector)
{
	if (frontsector->floorpic==skyflatnum) return;

	fixed_t ffh = frontsector->floortexz; 
	fixed_t bfh = backsector->floortexz; 
	if (bfh>ffh)
	{
		this->seg=seg;
		this->sub=NULL;

		vertex_t * v1=seg->v1;
		vertex_t * v2=seg->v2;
		vertexes[0]=v1;
		vertexes[1]=v2;

		glseg.x1=-v1->x/(float)MAP_SCALE;
		glseg.z1= v1->y/(float)MAP_SCALE;
		glseg.x2=-v2->x/(float)MAP_SCALE;
		glseg.z2= v2->y/(float)MAP_SCALE;
		glseg.noorigverts=true;
		clampx=clampy=false;

		lightlevel=frontsector->lightlevel;

		alpha=1.0f;
		RenderStyle=STYLE_Normal;
		Colormap=frontsector->ColorMap;

		topflat=frontsector->ceilingpic;	// for glowing textures
		bottomflat=frontsector->floorpic;

		yfloor[0] = yfloor[1] = ffh/MAP_SCALE;

		gltexture=FGLTexture::ValidateTexture(frontsector->floorpic);

		if (gltexture) 
		{
			flag=RENDERWALL_BOTTOM;
			SetWallCoordinates(seg, bfh, bfh, bfh, ffh, ffh, 0);
			PutWall(false);
		}
	}
}

