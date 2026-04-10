#include "gl_pch.h"
/*
** gl_scene.cpp
** manages the rendering of the player's view
**
**---------------------------------------------------------------------------
** Copyright 2004-2005 Christoph Oelckers
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

#include "gi.h"
#include "st_stuff.h"
#include "win32iface.h"
#include "gl/win32gliface.h"
#include "gl/gl_struct.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_portal.h"
#include "gl/gl_clipper.h"
#include "gl/gl_lights.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_basic.h"
#include "gl/gl_functions.h"

#define DEG2RAD( a ) ( a * M_PI ) / 180.0F

//==========================================================================
//
//
//
//==========================================================================
CVAR(Bool, gl_texture, true, 0)
CVAR(Int,gl_nearclip,5,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)

void R_SetupFrame (AActor * camera);
extern int viewpitch;
 
int rendered_lines,rendered_flats,rendered_sprites,render_vertexsplit,render_texsplit,rendered_decals;
int iter_dlightf, iter_dlight, draw_dlight, draw_dlightf;
Clock RenderWall,SetupWall,ClipWall;
Clock RenderFlat,SetupFlat;
Clock RenderSprite,SetupSprite;
Clock All, Finish, PortalAll;
int vertexcount, flatvertices, flatprimitives;
int palette_brightness;


Clock ProcessAll;
Clock RenderAll;

extern UniqueList<GLSkyInfo> UniqueSkies;
extern UniqueList<GLHorizonInfo> UniqueHorizons;
extern UniqueList<GLSectorStackInfo> UniqueStacks;
EXTERN_CVAR (Bool, cl_capfps)
CVAR(Bool, gl_blendcolormaps, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)


// please don't ask me why this is necessary at all.
// A 90 degree FOV is smaller than it is in the software renderer
#define FOV_CORRECTION_FACTOR 1.13776f//0.9115f

float viewvecX,viewvecY;

float roll     = 0.0f;
float yaw      = 0.0f;
float pitch    = 0.0f;

DWORD			gl_fixedcolormap;
DWORD			gl_boomcolormap;
float			currentFoV;
AActor *		viewactor;
area_t			in_area;

//-----------------------------------------------------------------------------
//
// R_FrustumAngle
//
//-----------------------------------------------------------------------------
angle_t gl_FrustumAngle()
{
	float tilt= (float)fabs(((double)(int)(viewpitch))/ANGLE_1);
	if (tilt>90.0f) tilt=90.0f;

	// If the pitch is larger than this you can look all around at a FOV of 90°
	if (abs(viewpitch)>46*ANGLE_1) return 0xffffffff;


	// ok, this is a gross hack that barely works...
	// but at least it doesn't overestimate too much...
	double floatangle=2.0+(45.0+((tilt/1.9)))*currentFoV*48.0/BaseRatioSizes[WidescreenRatio][3]/90.0;
	angle_t a1 = ANGLE_1*toint(floatangle);
	if (a1>=ANGLE_180) return 0xffffffff;
	return a1;

	// This is ZDoomGL's code which works better for a larger pitch
	//float vp = clamp<float>(tilt + (currentFoV / 2), 0.f, 90.f);
	//angle_t a2 = (angle_t)(2.f * vp * ANGLE_1);

	// return the smaller one of the 2 values!
	//return min(a1,a2);
	
}



//==========================================================================
//
//
//
//==========================================================================
static void infinitePerspective(GLdouble fovy, GLdouble aspect, GLdouble znear)
{
	GLdouble left, right, bottom, top;
	GLdouble m[16];

	top = znear * tan(fovy / 360.f * PI);
	//top = znear * tan(fovy / 180.f * PI) / 2.f;
	bottom = -top;
	left = bottom * aspect;
	right = top * aspect;

	m[ 0] = (2 * znear) / (right - left);
	m[ 4] = 0;
	m[ 8] = (right + left) / (right - left);
	m[12] = 0;

	m[ 1] = 0;
	m[ 5] = (2 * znear) / (top - bottom);
	m[ 9] = (top + bottom) / (top - bottom);
	m[13] = 0;

	m[ 2] = 0;
	m[ 6] = 0;
	//m[10] = - (zfar + znear) / (zfar - znear);
	//m[14] = - (2 * zfar * znear) / (zfar - znear);
	m[10] = -1;
	m[14] = -2 * znear;

	m[ 3] = 0;
	m[ 7] = 0;
	m[11] = -1;
	m[15] = 0;

	glMultMatrixd(m);

}


//==========================================================================
//
// SV_AddBlend
// [RH] This is from Q2.
//
//==========================================================================
static void gl_AddBlend (float r, float g, float b, float a, float v_blend[4])
{
	float a2, a3;

	if (a <= 0)
		return;
	a2 = v_blend[3] + (1-v_blend[3])*a;	// new total alpha
	a3 = v_blend[3]/a2;		// fraction of color from old

	v_blend[0] = v_blend[0]*a3 + r*(1-a3);
	v_blend[1] = v_blend[1]*a3 + g*(1-a3);
	v_blend[2] = v_blend[2]*a3 + b*(1-a3);
	v_blend[3] = a2;
}




void gl_ResetViewport()
{
	int trueheight = static_cast<Win32GLFrameBuffer*>(screen)->GetTrueHeight();	// ugh...
	glViewport(0, (trueheight-screen->GetHeight())/2, screen->GetWidth(), screen->GetHeight()); 
}

//-----------------------------------------------------------------------------
//
// gl_StartDrawScene
// sets 3D viewport and initializes hardware for 3D rendering
//
//-----------------------------------------------------------------------------
static void gl_StartDrawScene(GL_IRECT * bounds, float fov, float ratio)
{
	if (!bounds)
	{
		int height, width;

		// Special handling so the view with a visible status bar displays properly

		if (screenblocks >= 10)
		{
			height = SCREENHEIGHT;
			width  = SCREENWIDTH;
		}
		/*
		else if (screenblocks == 10)
		{
			height = SCREENHEIGHT;
			width  = SCREENWIDTH;
		}
		*/
		else
		{
			height = (screenblocks*SCREENHEIGHT/10) & ~7;
			width = (screenblocks*SCREENWIDTH/10);
		}

		int trueheight = static_cast<Win32GLFrameBuffer*>(screen)->GetTrueHeight();	// ugh...
		int bars = (trueheight-screen->GetHeight())/2; 

		int vw = realviewwidth;
		int vh = realviewheight;
		glViewport(viewwindowx, trueheight-bars-(height+viewwindowy-((height-vh)/2)), vw, height);
		glScissor(viewwindowx, trueheight-bars-(vh+viewwindowy), vw, vh);
	}
	else
	{
		glViewport(bounds->left, bounds->top, bounds->width, bounds->height);
		glScissor(bounds->left, bounds->top, bounds->width, bounds->height);
	}
	glEnable(GL_SCISSOR_TEST);
	
	#ifdef _DEBUG
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	#else
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,0,~0);	// default stencil
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	infinitePerspective(fov/1.6f * FOV_CORRECTION_FACTOR, ratio, (float)gl_nearclip/MAP_COEFF);

	// reset statistics counters
	render_texsplit=render_vertexsplit=rendered_lines=rendered_flats=rendered_sprites=rendered_decals = 0;
	iter_dlightf = iter_dlight = draw_dlight = draw_dlightf = 0;
	RenderWall.Reset();
	SetupWall.Reset();
	ClipWall.Reset();
	RenderFlat.Reset();
	SetupFlat.Reset();
	RenderSprite.Reset();
	SetupSprite.Reset();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	UniqueSkies.Clear();
	UniqueHorizons.Clear();
	UniqueStacks.Clear();
	currentFoV=fov;
}




//-----------------------------------------------------------------------------
//
// gl_SetupView
// Setup the view rotation matrix for the given viewpoint
//
//-----------------------------------------------------------------------------
void gl_SetupView(fixed_t viewx, fixed_t viewy, fixed_t viewz, angle_t viewangle, bool mirror, bool nosectorclear)
{
	float fviewangle=(float)(viewangle>>ANGLETOFINESHIFT)*360.0f/FINEANGLES;
	float xCamera,yCamera;
	float trZ = -5.0f;
	float trY ;
	
	yaw=270.0f-fviewangle;
	
	viewvecY=-sin(DEG2RAD(fviewangle));
	viewvecX= cos(DEG2RAD(fviewangle));

	// Player coordinates
	xCamera=-(float)viewx/MAP_SCALE;
	yCamera=(float)viewy/MAP_SCALE;
	trY=(float)viewz/MAP_SCALE;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(roll,  0.0f, 0.0f, 1.0f);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);

	if (!mirror)
	{
		glRotatef(yaw,   0.0f, 1.0f, 0.0f);
		glTranslatef(-xCamera, -trY, -yCamera);
		glScalef(1, 1, 1);
	}
	else
	{
		glRotatef(yaw,   0.0f, -1.0f, 0.0f);
		glTranslatef(xCamera, -trY, -yCamera);
		glScalef(-1, 1, 1);
	}

	// Clear the flat render info 
	if (!nosectorclear)
	{
		for(int i=0;i<numsectors;i++)
		{
			gl_sectors[i].renderflags=0;
			gl_subsectorrendernode * node = gl_sectors[i].otherplanes[0];
			while (node)
			{
				gl_subsectorrendernode * n = node;
				node = node->next;
				SSR_List.Release(n);
			}
			node = gl_sectors[i].otherplanes[1];
			while (node)
			{
				gl_subsectorrendernode * n = node;
				node = node->next;
				SSR_List.Release(n);
			}
			gl_sectors[i].otherplanes[0]=gl_sectors[i].otherplanes[1]=NULL;

		}
		memset(gl_ss_renderflags, 0, numsubsectors);
	}
}

//-----------------------------------------------------------------------------
//
// Sets the area the camera is in
//
//-----------------------------------------------------------------------------
void gl_SetViewArea()
{
	// The render_sector is better suited to represent the current position in GL
	sector_t * rendersector = gl_subsectors[R_PointInSubsector2(viewx, viewy)-subsectors].render_sector;

	// keep the view within the render sector's floor and ceiling
	fixed_t theZ = rendersector->ceilingplane.ZatPoint (viewx, viewy) - 4*FRACUNIT;
	if (viewz > theZ)
	{
		viewz = theZ;
	}

	theZ = rendersector->floorplane.ZatPoint (viewx, viewy) + 4*FRACUNIT;
	if (viewz < theZ)
	{
		viewz = theZ;
	}

	// Get the heightsec state from the render sector, not the current one!
	if (rendersector->heightsec && !(rendersector->heightsec->MoreFlags & SECF_IGNOREHEIGHTSEC))
	{
		in_area = viewz<=rendersector->heightsec->floorplane.ZatPoint(viewx,viewy) ? area_below :
				   (viewz>rendersector->heightsec->ceilingplane.ZatPoint(viewx,viewy) &&
				   !(rendersector->heightsec->MoreFlags&SECF_FAKEFLOORONLY)) ? area_above:area_normal;
	}
	else
	{
		in_area=area_default;	// depends on exposed lower sectors
	}
}


//-----------------------------------------------------------------------------
//
// gl_drawscene - this function renders the scene from the current
// viewpoint, including mirrors and skyboxes and other portals
// It is assumed that the GLPortal::EndFrame returns with the 
// stencil, z-buffer and the projection matrix intact!
//
//-----------------------------------------------------------------------------

void gl_DrawScene()
{
	static int recursion=0;


	// reset the portal manager
	GLPortal::StartFrame();

	ProcessAll.Start();

	// clip the scene and fill the drawlists
	gl_RenderBSPNode (nodes + numnodes - 1);

	// And now the crappy hacks that have to be done to avoid rendering anomalies:

	gl_RenderMissingLines();	// Omitted lines by the node builder
	HandleMissingTextures();	// Missing upper/lower textures
	HandleHackedSubsectors();	// open sector hacks for deep water
	ProcessSectorStacks();		// merge visplanes of sector stacks

	ProcessAll.Stop();
	RenderAll.Start();

	/*if (!recursion) */GLPortal::RenderFirstSkyPortal();

	// first pass: unlit surfaces
	glEnable(GL_FOG);
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_ONE,GL_ZERO);
	gl_drawlist[GLDL_UNLIT].Sort();
	gl_drawlist[GLDL_UNLIT].Draw(GLPASS_UNLIT);
	gl_drawlist[GLDL_LITFOG].Sort();
	gl_drawlist[GLDL_LITFOG].Draw(GLPASS_UNLIT);


	// second pass: untextured ambient light of lit surfaces

	// first for masked textures (2-sided walls)
	// create a blank surface that only fills the nontransparent parts of the texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE); 
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);

	gl_drawlist[GLDL_MASKED].Sort();
	gl_drawlist[GLDL_MASKED].Draw(GLPASS_UNLIT);	// this really is GLPASS_BASE but it needs the texture.

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// now for unmasked textures
	glDisable(GL_TEXTURE_2D);
	gl_drawlist[GLDL_LIT].Sort();
	gl_drawlist[GLDL_LIT].Draw(GLPASS_BASE);

	// third pass: draw lights (on fogged surfaces they are added to the textures!)
	if (gl_lights)
	{
		if (gl_SetupLightTexture())
		{
			glEnable(GL_TEXTURE_2D);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthFunc(GL_EQUAL);

			gl_drawlist[GLDL_LIT].Draw(GLPASS_LIGHT);
			gl_drawlist[GLDL_LITFOG].Draw(GLPASS_LIGHT);
			gl_drawlist[GLDL_MASKED].Draw(GLPASS_LIGHT);
		}
		else gl_lights=false;
	}

	// fourth pass: modulated texture
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glDisable(GL_FOG);
	glDepthFunc(GL_LEQUAL);
	if (gl_texture) 
	{
		gl_drawlist[GLDL_LIT].Draw(GLPASS_TEXTURE);
		gl_drawlist[GLDL_MASKED].Draw(GLPASS_TEXTURE);
	}

	// fifth pass: decals
	glEnable(GL_FOG);
	glDepthFunc(GL_LESS);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0f, -128.0f);
	glDepthMask(false);

	gl_drawlist[GLDL_UNLIT].Draw(GLPASS_DECALS);
	gl_drawlist[GLDL_LITFOG].Draw(GLPASS_DECALS);
	gl_drawlist[GLDL_LIT].Draw(GLPASS_DECALS);

	/// Push bleeding floor/ceiling textures back a little in the z-buffer
	// so they don't interfere with overlapping mid textures.
	glPolygonOffset(1.0f, 128.0f);

	// flood all the gaps with the back sector's flat texture
	DrawUnhandledMissingTextures();

	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);

	RenderAll.Stop();
	// Handle all portals after rendering the opaque objects but before
	// doing all translucent stuff
	glDepthMask(true);
	recursion++;
	GLPortal::EndFrame();
	recursion--;
	glDepthMask(false);

	RenderAll.Start();

	// sixth pass: translucent stuff
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_drawlist[GLDL_TRANSLUCENTBORDER].Draw(GLPASS_UNLIT);
	gl_drawlist[GLDL_TRANSLUCENT].DrawSorted();

	glDepthMask(true);

	glAlphaFunc(GL_GEQUAL,0.5f);
	RenderAll.Stop();
}


//==========================================================================
//
// Draws a blend over the entire view
//
// This mostly duplicates the code in shared_sbar.cpp
// but I can't use that one because it is done too late so I don't get
// the blend in time.
//
//==========================================================================
static void gl_DrawBlend(sector_t * viewsector)
{
	float blend[4]={0,0,0,0};
	int cnt;
	PalEntry blendv=0;
	float extra_red;
	float extra_green;
	float extra_blue;
	player_t * player=players[displayplayer].camera->player;

	// [RH] Amount of red flash for up to 114 damage points. Calculated by hand
	//		using a logarithmic scale and my trusty HP48G.
	static const byte DamageToAlpha[114] =
	{
		  0,   8,  16,  23,  30,  36,  42,  47,  53,  58,  62,  67,  71,  75,  79,
		 83,  87,  90,  94,  97, 100, 103, 107, 109, 112, 115, 118, 120, 123, 125,
		128, 130, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 155, 157,
		159, 160, 162, 164, 165, 167, 169, 170, 172, 173, 175, 176, 178, 179, 181,
		182, 183, 185, 186, 187, 189, 190, 191, 192, 194, 195, 196, 197, 198, 200,
		201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216,
		217, 218, 219, 220, 221, 221, 222, 223, 224, 225, 226, 227, 228, 229, 229,
		230, 231, 232, 233, 234, 235, 235, 236, 237
	};

	// don't draw sector based blends when an invulnerability colormap is active
	if (!gl_fixedcolormap)
	{
		if (!viewsector->e->ffloors.Size())
		{
			if (viewsector->heightsec && !(viewsector->MoreFlags&SECF_IGNOREHEIGHTSEC))
			{
				switch(in_area)
				{
				default:
				case area_normal: blendv=viewsector->heightsec->midmap; break;
				case area_above: blendv=viewsector->heightsec->topmap; break;
				case area_below: blendv=viewsector->heightsec->bottommap; break;
				}
			}
		}
		else
		{
			TArray<lightlist_t> & lightlist = viewsector->e->lightlist;

			for(int i=0;i<lightlist.Size();i++)
			{
				fixed_t lightbottom;
				if (i<lightlist.Size()-1) 
					lightbottom=lightlist[i+1].plane.ZatPoint(viewx,viewy);
				else 
					lightbottom=viewsector->floorplane.ZatPoint(viewx,viewy);

				if (lightbottom<viewz)
				{
					// 3d floor 'fog' is rendered as a blending value
					blendv=(*lightlist[i].p_extra_colormap)->Fade;
					// If this is the same as the sector's it doesn't apply!
					if (blendv == viewsector->ColorMap->Fade) blendv=0;
					// a little hack to make this work for Legacy maps.
					if (blendv.a==0 && blendv!=0) blendv.a=128;
					break;
				}
			}
		}
	}

	if (gl_blendcolormaps && blendv.a==0)
	{
		blendv = R_BlendForColormap(blendv);
		if (blendv.a==255)
		{
			// The calculated average is too dark so brighten it according to the palettes's overall brightness
			int maxcol = MAX<int>(MAX<int>(palette_brightness, blendv.r), MAX<int>(blendv.g, blendv.b));
			blendv.r = blendv.r * 255 / maxcol;
			blendv.g = blendv.g * 255 / maxcol;
			blendv.b = blendv.b * 255 / maxcol;
		}
	}

	if (blendv.a==255)
	{

		extra_red = blendv.r / 255.0f;
		extra_green = blendv.g / 255.0f;
		extra_blue = blendv.b / 255.0f;

		// If this is a multiplicative blend do it separately and add the additive ones on top of it!
		blendv=0;

		// black multiplicative blends are ignored
		if (extra_red || extra_green || extra_blue)
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_TEXTURE_2D);
			glBlendFunc(GL_DST_COLOR,GL_ZERO);
			glColor4f(extra_red, extra_green, extra_blue, 1.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f( 0.0f, 0.0f);
			glVertex2f( 0.0f, (float)SCREENHEIGHT);
			glVertex2f( (float)SCREENWIDTH, 0.0f);
			glVertex2f( (float)SCREENWIDTH, (float)SCREENHEIGHT);
			glEnd();
		}
	}
	else if (blendv.a)
	{
		gl_AddBlend (blendv.r / 255.f, blendv.g / 255.f, blendv.b / 255.f, blendv.a/255.0f,blend);
	}

	if (player)
	{
		AInventory * in;

		for(in=player->mo->Inventory;in;in=in->Inventory)
		{
			PalEntry color = in->GetBlend ();
			if (color.a != 0)
			{
				gl_AddBlend (color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f, blend);
			}
		}
		if (player->bonuscount)
		{
			cnt = player->bonuscount << 3;
			gl_AddBlend (0.8431f, 0.7333f, 0.2706f, cnt > 128 ? 0.5f : cnt / 256.f, blend);
		}
		
		// FIXME!
		cnt = DamageToAlpha[MIN (113, player->damagecount)];
		
		if (cnt)
		{
			if (cnt > 175) cnt = 175; // too strong and it gets too opaque
			
			gl_AddBlend(1.0f, 0, 0, cnt/255.f,  blend);

		}
		
		if (player->poisoncount)
		{
			cnt = MIN (player->poisoncount, 64);
			gl_AddBlend (0.04f, 0.2571f, 0.f, cnt/93.2571428571f, blend);
		}
		else if (player->hazardcount)
		{
			cnt= MIN(player->hazardcount/8, 64);
			gl_AddBlend (0.04f, 0.2571f, 0.f, cnt/93.2571428571f, blend);
		}
		if (player->mo->DamageType==MOD_ICE)
		{
			gl_AddBlend (0.25f, 0.25f, 0.853f, 0.4f, blend);
		}

		// translucency may not go below 50%!
		if (blend[3]>0.5f) blend[3]=0.5f;

		// except for fadeto effects
		gl_AddBlend (player->BlendR, player->BlendG, player->BlendB, player->BlendA, blend);


	}

	if (blend[3]>0.0f)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glColor4fv(blend);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f( 0.0f, 0.0f);
		glVertex2f( 0.0f, (float)SCREENHEIGHT);
		glVertex2f( (float)SCREENWIDTH, 0.0f);
		glVertex2f( (float)SCREENWIDTH, (float)SCREENHEIGHT);
		glEnd();
	}
}


//-----------------------------------------------------------------------------
//
// gl_SetupView
// Draws player sprites and color blend
//
//-----------------------------------------------------------------------------
void gl_EndDrawScene()
{
	extern void gl_DrawPlayerSprites (sector_t *);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_SMOOTH);

	glDisable(GL_FOG); 
	gl_Set2DMode();

	gl_ResetViewport();
	gl_DrawPlayerSprites (viewsector);
	gl_DrawBlend(viewsector);

	// Restore standard rendering state
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_SCISSOR_TEST);
}


//-----------------------------------------------------------------------------
//
// R_RenderView - renders one view - either the screen or a camera texture
//
//-----------------------------------------------------------------------------
static GLDrawList GlobalDrawList[GLDL_TYPES];	// outside the function for debugging purposes!

void gl_RenderView (AActor * camera, GL_IRECT * bounds, float fov, float ratio, bool mainview)
{       
	R_SetupFrame (camera);
	gl_SetViewArea();
	pitch = clamp<float>((float)((double)(int)(viewpitch))/ANGLE_1, -90, 90);

	// Handle Boom colormaps
	gl_boomcolormap=CM_DEFAULT;
	if (mainview && !gl_blendcolormaps)
	{
		if (!viewsector->e->ffloors.Size() && gl_fixedcolormap==CM_DEFAULT && 
			viewsector->heightsec && !(viewsector->MoreFlags&SECF_IGNOREHEIGHTSEC))
		{
			PalEntry blendv;
			
			if (in_area == area_above)  blendv=viewsector->heightsec->topmap;
			if (in_area == area_below)  blendv=viewsector->heightsec->bottommap;
			else						blendv=viewsector->heightsec->midmap;

			// Is it a colormap lump?
			if (blendv.a==0 && blendv!=0) gl_boomcolormap=blendv+CM_FIRSTCOLORMAP;
		}
	}

	if (camera->player && camera->player-players==displayplayer &&
		camera->player->cheats&CF_CHASECAM && camera==camera->player->mo)
	{
		viewactor=NULL;
	}
	else
	{
		viewactor=camera;
	}

	TThinkerIterator<ADynamicLight> it(STAT_DLIGHT);
	ADynamicLight * mo;
	while (mo=it.Next()) mo->maybevisible=false;

	gl_StartDrawScene(bounds, fov, ratio);	// switch to perspective mode and set up clipper

	gl_StartDrawInfo(GlobalDrawList);
	gl_SetupView(viewx, viewy, viewz, viewangle, false);

	clipper.Clear();
	angle_t a1 = gl_FrustumAngle();
	clipper.SafeAddClipRange(viewangle+a1, viewangle-a1);

	gl_DrawScene();
	gl_EndDrawInfo();

	restoreinterpolations ();
}


//-----------------------------------------------------------------------------
//
// R_RenderTextureView - renders camera textures
//
//-----------------------------------------------------------------------------
void gl_RenderTextureView(FCanvasTexture *Texture, AActor * Viewpoint, int FOV)
{
	GL_IRECT bounds;
	FGLTexture * gltex = FGLTexture::ValidateTexture(Texture);

	int width=Texture->GetWidth();
	int height=Texture->GetHeight();

	bounds.left=bounds.top=0;
	bounds.width=GLTexture::GetTexDimension(width);
	bounds.height=GLTexture::GetTexDimension(height);
	glFlush();
	gl_RenderView(Viewpoint, &bounds, FOV, (float)width/height*1.6/1.333, false);
	glFlush();
	gltex->Bind(CM_DEFAULT);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, bounds.width, bounds.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLTexture::TexFilter[gl_texture_filter].magfilter);
}


//-----------------------------------------------------------------------------
//
// gl_RenderViewToCanvas
//
//-----------------------------------------------------------------------------

void gl_RenderViewToCanvas(DCanvas * pic, int x, int y, int width, int height)
{
	int indexSrc, indexDst;
	GL_IRECT bounds;
	PalEntry p;

	bounds.left=0;
	bounds.top=0;
	bounds.width=width;
	bounds.height=height;
	glFlush();
	gl_RenderView(players[displayplayer].camera, &bounds, FieldOfView * 360.0f / FINEANGLES, 1.6f, true);
	glFlush();

	byte * scr = (byte *)Malloc(width * height * 4);
	glReadPixels(0,0,width, height,GL_RGBA,GL_UNSIGNED_BYTE,scr);

	pic->Lock();
	byte * dst = pic->GetBuffer();

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			indexSrc = x + (y * width);
			indexDst = x + ((height - (y + 1)) * width);
			p.r = scr[(indexSrc * 4) + 0];
			p.g = scr[(indexSrc * 4) + 1];
			p.b = scr[(indexSrc * 4) + 2];
			dst[indexDst] = ColorMatcher.Pick(p.r, p.g, p.b);
		}
	}
	//pic->Unlock();
}

//-----------------------------------------------------------------------------
//
// R_RenderPlayerView - the main rendering function
//
//-----------------------------------------------------------------------------
EXTERN_CVAR (Int, r_detail)

void gl_RenderPlayerView (player_t* player)
{       
	static AActor * LastCamera;

	if (player->camera != LastCamera)
	{
		// If the camera changed don't interpolate
		// Otherwise there will be some not so nice effects.
		R_ResetViewInterpolation();
		LastCamera=player->camera;
	}

	//Printf("Starting scene\n");
	All.Reset();
	All.Start();
	PortalAll.Reset();
	RenderAll.Reset();
	ProcessAll.Reset();
	flatvertices=flatprimitives=vertexcount=0;

	//gl_LinkLights();

	// Get this before everything else
	if (cl_capfps || r_NoInterpolate) r_TicFrac = FRACUNIT;
	else r_TicFrac = I_GetTimeFrac (&r_FrameTime);

	R_FindParticleSubsectors ();

	// prepare all camera textures that have been used in the last frame
	gl_fixedcolormap=0;
	FCanvasTextureInfo::UpdateAll();

	gl_fixedcolormap=CM_DEFAULT;

	// check for special colormaps
	if (player->camera->player) 
	{
		gl_fixedcolormap=player->camera->player->fixedcolormap;

		if (player->camera->player->extralight<0)
		{
			gl_fixedcolormap=CM_INVERT;
			extralight=0;
		}
		else
		{
			for(AInventory * in = player->mo->Inventory; in; in = in->Inventory)
			{
				PalEntry color = in->GetBlend ();

				// Allow access to these colormaps through special blend values.
				if (color==0x01000000) 
				{
					gl_fixedcolormap=CM_INVERT;
					break;
				}
				else if (color==0x02000000) 
				{
					gl_fixedcolormap=CM_GOLDMAP;
					break;
				}
				else if (player->camera->player->fixedcolormap!=0 && player->camera->player->fixedcolormap<=NUMCOLORMAPS) 
				{
					// Need special handling for light amplifiers and invulnerability
					if (in->IsA(RUNTIME_CLASS(APowerTorch)))
					{
						gl_fixedcolormap = player->camera->player->fixedcolormap + CM_TORCH;
					}
					else if (in->IsA(RUNTIME_CLASS(APowerLightAmp)))
					{
						gl_fixedcolormap = CM_LITE;
					}
					else if (in->IsA(RUNTIME_CLASS(APowerInvulnerable)))
					{
						if (gameinfo.gametype == GAME_Doom || gameinfo.gametype == GAME_Strife)
						{
							gl_fixedcolormap = CM_INVERT;
							break;
						}
						else if (gameinfo.gametype == GAME_Heretic)
						{
							gl_fixedcolormap = CM_GOLDMAP;
							break;
						}
					}
				}
			}
		}
	}

	// now render the main view
	float ratio = 64.f / BaseRatioSizes[WidescreenRatio][3] / 1.333f * 1.6f;
	gl_RenderView(player->camera, NULL, FieldOfView * 360.0f / FINEANGLES, ratio, true);
	gl_EndDrawScene();

	All.Stop();
	//Printf("Finishing scene\n");
}

//-----------------------------------------------------------------------------
//
// Rendering statistics
//
//-----------------------------------------------------------------------------
ADD_STAT(rendertimes,out)
{
	static char buff[1000];
	static int lasttime=0;
	int t=I_MSTime();
	if (t-lasttime>1000) 
	{
		sprintf(buff,"W: Render=%2.2f, Setup=%2.2f, Clip=%2.2f - F: Render=%2.2f, Setup=%2.2f - S: Render=%2.2f, Setup=%2.2f - All=%2.2f, Render=%2.2f, Setup=%2.2f, Portal=%2.2f, Finish=%2.2f\n",
		SecondsPerCycle * (double)*RenderWall * 1000,
		SecondsPerCycle * (double)* SetupWall * 1000,
		SecondsPerCycle * (double)*  ClipWall * 1000,
		SecondsPerCycle * (double)*RenderFlat * 1000,
		SecondsPerCycle * (double)* SetupFlat * 1000,
		SecondsPerCycle * (double)*RenderSprite* 1000,
		SecondsPerCycle * (double)* SetupSprite* 1000,
		SecondsPerCycle * (double)*       All * 1000,
		SecondsPerCycle * (double)* RenderAll * 1000,
		SecondsPerCycle * (double)*ProcessAll * 1000,
		SecondsPerCycle * (double)*PortalAll * 1000,
		SecondsPerCycle * (double)*    Finish * 1000);

		lasttime=t;
	}
		strcpy(out,buff);

}

ADD_STAT(renderstats,out)
{
	sprintf(out,"Walls: %d (%d splits, %d t-splits, %d vertices), Flats: %d (%d primitives, %d vertices), Sprites: %d, Decals=%d\n", 
		rendered_lines, render_vertexsplit, render_texsplit, vertexcount, rendered_flats, flatprimitives, flatvertices, rendered_sprites,rendered_decals );
}

ADD_STAT(lightstats,out)
{
	sprintf(out,"DLight - Walls: %d processed, %d rendered - Flats: %d processed, %d rendered\n", 
		iter_dlight, draw_dlight, iter_dlightf, draw_dlightf );
}

