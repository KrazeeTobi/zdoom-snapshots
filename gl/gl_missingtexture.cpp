#include "gl_pch.h"
/*
** gl_missingtexture.cpp
** Handles missing upper and lower textures and self referencing sector hacks
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
#include "gl/gl_struct.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_portal.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_data.h"
#include "gl/gl_texture.h"
#include "gl/gl_basic.h"
#include "gl/gl_functions.h"

// This is for debugging maps.
CVAR(Bool, gl_notexturefill, false, 0);
extern int firstmissingseg;

struct MissingTextureInfo
{
	seg_t * seg;
	gl_subsectordata * glsub;
	fixed_t planez;
	fixed_t planezfront;
};

struct MissingSegInfo
{
	seg_t * seg;
	int MTI_Index;	// tells us which MissingTextureInfo represents this seg.
};

struct SubsectorHackInfo
{
	gl_subsectordata * glsub;
	byte flags;
};

static TArray<MissingTextureInfo> MissingUpperTextures;
static TArray<MissingTextureInfo> MissingLowerTextures;

static TArray<MissingSegInfo> MissingUpperSegs;
static TArray<MissingSegInfo> MissingLowerSegs;

static TArray<SubsectorHackInfo> SubsectorHacks;
// collect all the segs

static TArray<subsector_t *> CeilingStacks;
static TArray<subsector_t *> FloorStacks;


static TArray<gl_subsectordata *> HandledSubsectors;

FreeList<gl_subsectorrendernode> SSR_List;

static int totalupper, totallower, totalsectors;
static QWORD totalms, showtotalms;
static QWORD totalssms;
static sector_t fakesec;


//==========================================================================
//
// Collects all sectors that might need a fake ceiling
//
//==========================================================================
void AddUpperMissingTexture(seg_t * seg, fixed_t backheight)
{
	if (!seg->backsector) return;

	clock(totalms);
	MissingTextureInfo mti;
	MissingSegInfo msi;

	if (!seg->Subsector) 
	{
		msi.MTI_Index = -1;
		msi.seg=seg;
		MissingLowerSegs.Push(msi);
		return;
	}

	gl_subsectordata * glsub = &gl_subsectors[seg->Subsector-subsectors];

	// Should never happen because these are by definition completely self-referencing
	if (glsub->render_sector != seg->Subsector->sector) 
	{
		unclock(totalms);
		return;
	}

	for(int i=0;i<MissingUpperTextures.Size();i++)
	{
		if (MissingUpperTextures[i].glsub == glsub)
		{
			// Use the lowest adjoining height to draw a fake ceiling if necessary
			if (backheight < MissingUpperTextures[i].planez) 
			{
				MissingUpperTextures[i].planez = backheight;
				MissingUpperTextures[i].seg = seg;
			}

			msi.MTI_Index = i;
			msi.seg=seg;
			MissingUpperSegs.Push(msi);

			unclock(totalms);
			return;
		}
	}
	mti.seg=seg;
	mti.glsub=glsub;
	mti.planez=backheight;
	msi.MTI_Index = MissingUpperTextures.Push(mti);
	msi.seg=seg;
	MissingUpperSegs.Push(msi);
	unclock(totalms);
}

//==========================================================================
//
// Collects all sectors that might need a fake floor
//
//==========================================================================
void AddLowerMissingTexture(seg_t * seg, fixed_t backheight)
{
	if (!seg->backsector) return;
	if (gl_sectors[seg->backsector->sectornum].transdoor)
	{
		if (gl_sectors[seg->backsector->sectornum].transdoorheight == seg->backsector->floortexz) return;
	}

	clock(totalms);
	MissingTextureInfo mti;
	MissingSegInfo msi;

	if (!seg->Subsector) 
	{
		msi.MTI_Index = -1;
		msi.seg=seg;
		MissingLowerSegs.Push(msi);
		return;
	}

	gl_subsectordata * glsub = &gl_subsectors[seg->Subsector-subsectors];

	// Should never happen because these are by definition completely self-referencing
	if (glsub->render_sector != seg->Subsector->sector) 
	{
		unclock(totalms);
		return;
	}

	// Ignore FF_FIX's because they are designed to abuse missing textures
	if (seg->backsector->e->ffloors.Size() && seg->backsector->e->ffloors[0]->flags&FF_FIX)
	{
		unclock(totalms);
		return;
	}

	for(int i=0;i<MissingLowerTextures.Size();i++)
	{
		if (MissingLowerTextures[i].glsub == glsub)
		{
			// Use the highest adjoining height to draw a fake floor if necessary
			if (backheight > MissingLowerTextures[i].planez) 
			{
				MissingLowerTextures[i].planez = backheight;
				MissingLowerTextures[i].seg = seg;
			}

			msi.MTI_Index = i;
			msi.seg=seg;
			MissingLowerSegs.Push(msi);

			unclock(totalms);
			return;
		}
	}
	mti.seg=seg;
	mti.glsub = glsub;
	mti.planez=backheight;
	msi.MTI_Index = MissingLowerTextures.Push(mti);
	msi.seg=seg;
	MissingLowerSegs.Push(msi);
	unclock(totalms);
}


//==========================================================================
//
// 
//
//==========================================================================
static bool DoOneSectorUpper(subsector_t * subsec, fixed_t planez)
{
	// Is there a one-sided wall in this sector?
	// Do this first to avoid unnecessary recursion
	for(int i=0; i< subsec->numlines; i++)
	{
		if (segs[subsec->firstline + i].backsector == NULL) return false;
	}

	for(int i=0; i< subsec->numlines; i++)
	{
		seg_t * seg = &segs[subsec->firstline + i];
		subsector_t * backsub = seg->PartnerSeg->Subsector;

		// already checked?
		if (backsub->validcount == validcount) continue;	
		backsub->validcount=validcount;

		if (seg->frontsector != seg->backsector && seg->linedef)
		{
			// Note: if this is a real line between sectors
			// we can be sure that render_sector is the real sector!

			sector_t * sec = gl_FakeFlat(seg->backsector, &fakesec, true);

			// Don't bother with slopes
			if (sec->ceilingplane.a!=0 || sec->ceilingplane.b!=0)  return false;

			// Is the neighboring ceiling lower than the desired height?
			if (sec->ceilingtexz<planez) 
			{
				// todo: check for missing textures.
				return false;
			}

			// This is an exact height match which means we don't have to do any further checks for this sector
			if (sec->ceilingtexz==planez) 
			{
				// If there's a texture abort
				FTexture * tex = TexMan[seg->sidedef->toptexture];
				if (!tex || tex->UseType==FTexture::TEX_Null) continue;
				else return false;
			}
		}
		if (!DoOneSectorUpper(backsub, planez)) return false;
	}
	// all checked ok. This subsector is part of the current fake plane

	HandledSubsectors.Push(&gl_subsectors[subsec-subsectors]);
	return 1;
}

//==========================================================================
//
// 
//
//==========================================================================
static bool DoOneSectorLower(subsector_t * subsec, fixed_t planez)
{
	// Is there a one-sided wall in this subsector?
	// Do this first to avoid unnecessary recursion
	for(int i=0; i< subsec->numlines; i++)
	{
		if (segs[subsec->firstline + i].backsector == NULL) return false;
	}

	for(int i=0; i< subsec->numlines; i++)
	{
		seg_t * seg = &segs[subsec->firstline + i];
		subsector_t * backsub = seg->PartnerSeg->Subsector;

		// already checked?
		if (backsub->validcount == validcount) continue;	
		backsub->validcount=validcount;

		if (seg->frontsector != seg->backsector && seg->linedef)
		{
			// Note: if this is a real line between sectors
			// we can be sure that render_sector is the real sector!

			sector_t * sec = gl_FakeFlat(seg->backsector, &fakesec, true);

			// Don't bother with slopes
			if (sec->floorplane.a!=0 || sec->floorplane.b!=0)  return false;

			// Is the neighboring floor higher than the desired height?
			if (sec->floortexz>planez) 
			{
				// todo: check for missing textures.
				return false;
			}

			// This is an exact height match which means we don't have to do any further checks for this sector
			if (sec->floortexz==planez) 
			{
				// If there's a texture abort
				FTexture * tex = TexMan[seg->sidedef->bottomtexture];
				if (!tex || tex->UseType==FTexture::TEX_Null) continue;
				else return false;
			}
		}
		if (!DoOneSectorLower(backsub, planez)) return false;
	}
	// all checked ok. This sector is part of the current fake plane

	HandledSubsectors.Push(&gl_subsectors[subsec-subsectors]);
	return 1;
}


//==========================================================================
//
//
//
//==========================================================================
static bool DoFakeBridge(subsector_t * subsec, fixed_t planez)
{
	// Is there a one-sided wall in this sector?
	// Do this first to avoid unnecessary recursion
	for(int i=0; i< subsec->numlines; i++)
	{
		if (segs[subsec->firstline + i].backsector == NULL) return false;
	}

	for(int i=0; i< subsec->numlines; i++)
	{
		seg_t * seg = &segs[subsec->firstline + i];
		subsector_t * backsub = seg->PartnerSeg->Subsector;

		// already checked?
		if (backsub->validcount == validcount) continue;	
		backsub->validcount=validcount;

		if (seg->frontsector != seg->backsector && seg->linedef)
		{
			// Note: if this is a real line between sectors
			// we can be sure that render_sector is the real sector!

			sector_t * sec = gl_FakeFlat(seg->backsector, &fakesec, true);

			// Don't bother with slopes
			if (sec->floorplane.a!=0 || sec->floorplane.b!=0)  return false;

			// Is the neighboring floor higher than the desired height?
			if (sec->floortexz<planez) 
			{
				// todo: check for missing textures.
				return false;
			}

			// This is an exact height match which means we don't have to do any further checks for this sector
			// No texture checks though! 
			if (sec->floortexz==planez) continue;
		}
		if (!DoFakeBridge(backsub, planez)) return false;
	}
	// all checked ok. This sector is part of the current fake plane

	HandledSubsectors.Push(&gl_subsectors[subsec-subsectors]);
	return 1;
}

//==========================================================================
//
//
//
//==========================================================================
static bool DoFakeCeilingBridge(subsector_t * subsec, fixed_t planez)
{
	// Is there a one-sided wall in this sector?
	// Do this first to avoid unnecessary recursion
	for(int i=0; i< subsec->numlines; i++)
	{
		if (segs[subsec->firstline + i].backsector == NULL) return false;
	}

	for(int i=0; i< subsec->numlines; i++)
	{
		seg_t * seg = &segs[subsec->firstline + i];
		subsector_t * backsub = seg->PartnerSeg->Subsector;

		// already checked?
		if (backsub->validcount == validcount) continue;	
		backsub->validcount=validcount;

		if (seg->frontsector != seg->backsector && seg->linedef)
		{
			// Note: if this is a real line between sectors
			// we can be sure that render_sector is the real sector!

			sector_t * sec = gl_FakeFlat(seg->backsector, &fakesec, true);

			// Don't bother with slopes
			if (sec->ceilingplane.a!=0 || sec->ceilingplane.b!=0)  return false;

			// Is the neighboring ceiling higher than the desired height?
			if (sec->ceilingtexz>planez) 
			{
				// todo: check for missing textures.
				return false;
			}

			// This is an exact height match which means we don't have to do any further checks for this sector
			// No texture checks though! 
			if (sec->ceilingtexz==planez) continue;
		}
		if (!DoFakeCeilingBridge(backsub, planez)) return false;
	}
	// all checked ok. This sector is part of the current fake plane

	HandledSubsectors.Push(&gl_subsectors[subsec-subsectors]);
	return 1;
}


//==========================================================================
//
// Draws the fake planes
//
//==========================================================================
void HandleMissingTextures()
{
	sector_t fake;
	clock(totalms);
	totalupper=MissingUpperTextures.Size();
	totallower=MissingLowerTextures.Size();

	for(int i=0;i<MissingUpperTextures.Size();i++)
	{
		if (!MissingUpperTextures[i].seg) continue;
		HandledSubsectors.Clear();
		validcount++;

		if (MissingUpperTextures[i].planez > viewz) 
		{
			// close the hole only if all neighboring sectors are an exact height match
			// Otherwise just fill in the missing textures.
			MissingUpperTextures[i].glsub->sub->validcount=validcount;
			if (DoOneSectorUpper(MissingUpperTextures[i].glsub->sub, MissingUpperTextures[i].planez))
			{
				gl_sectordata * glsec = &gl_sectors[MissingUpperTextures[i].seg->backsector->sectornum];
				// The mere fact that this seg has been added to the list means that the back sector
				// will be rendered so we can safely assume that it is already in the render list

				for(int j=0;j<HandledSubsectors.Size();j++)
				{				
					gl_subsectorrendernode * node = SSR_List.GetNew();
					node->glsub = HandledSubsectors[j];
					node->next = glsec->otherplanes[1];
					glsec->otherplanes[1]=node;
				}

				if (HandledSubsectors.Size()!=1)
				{
					// mark all subsectors in the missing list that got processed by this
					for(int j=0;j<HandledSubsectors.Size();j++)
					{
						for(int k=0;k<MissingUpperTextures.Size();k++)
						{
							if (MissingUpperTextures[k].glsub==HandledSubsectors[j])
							{
								MissingUpperTextures[k].seg=NULL;
							}
						}
					}
				}
				else MissingUpperTextures[i].seg=NULL;
				continue;
			}
		}

		{
			// It isn't a hole. Now check whether it might be a fake bridge
			sector_t * fakesector = gl_FakeFlat(MissingUpperTextures[i].seg->frontsector, &fake, false);
			fixed_t planez = fakesector->ceilingtexz;

			MissingUpperTextures[i].seg->PartnerSeg->Subsector->validcount=validcount;
			if (DoFakeCeilingBridge(MissingUpperTextures[i].seg->PartnerSeg->Subsector, planez))
			{
				gl_sectordata * glsec = &gl_sectors[fakesector->sectornum];
				// The mere fact that this seg has been added to the list means that the back sector
				// will be rendered so we can safely assume that it is already in the render list

				for(int j=0;j<HandledSubsectors.Size();j++)
				{				
					gl_subsectorrendernode * node = SSR_List.GetNew();
					node->glsub = HandledSubsectors[j];
					node->next = glsec->otherplanes[1];
					glsec->otherplanes[1]=node;
				}
			}
			continue;
		}
	}

	for(int i=0;i<MissingLowerTextures.Size();i++)
	{
		if (!MissingLowerTextures[i].seg) continue;
		HandledSubsectors.Clear();
		validcount++;

		if (MissingLowerTextures[i].planez < viewz) 
		{
			// close the hole only if all neighboring sectors are an exact height match
			// Otherwise just fill in the missing textures.
			MissingLowerTextures[i].glsub->sub->validcount=validcount;
			if (DoOneSectorLower(MissingLowerTextures[i].glsub->sub, MissingLowerTextures[i].planez))
			{
				gl_sectordata * glsec = &gl_sectors[MissingLowerTextures[i].seg->backsector->sectornum];
				// The mere fact that this seg has been added to the list means that the back sector
				// will be rendered so we can safely assume that it is already in the render list

				for(int j=0;j<HandledSubsectors.Size();j++)
				{				
					gl_subsectorrendernode * node = SSR_List.GetNew();
					node->glsub = HandledSubsectors[j];
					node->next = glsec->otherplanes[0];
					glsec->otherplanes[0]=node;
				}

				if (HandledSubsectors.Size()!=1)
				{
					// mark all subsectors in the missing list that got processed by this
					for(int j=0;j<HandledSubsectors.Size();j++)
					{
						for(int k=0;k<MissingLowerTextures.Size();k++)
						{
							if (MissingLowerTextures[k].glsub==HandledSubsectors[j])
							{
								MissingLowerTextures[k].seg=NULL;
							}
						}
					}
				}
				else MissingLowerTextures[i].seg=NULL;
				continue;
			}
		}

		{
			// It isn't a hole. Now check whether it might be a fake bridge
			sector_t * fakesector = gl_FakeFlat(MissingLowerTextures[i].seg->frontsector, &fake, false);
			fixed_t planez = fakesector->floortexz;

			MissingLowerTextures[i].seg->PartnerSeg->Subsector->validcount=validcount;
			if (DoFakeBridge(MissingLowerTextures[i].seg->PartnerSeg->Subsector, planez))
			{
				gl_sectordata * glsec = &gl_sectors[fakesector->sectornum];
				// The mere fact that this seg has been added to the list means that the back sector
				// will be rendered so we can safely assume that it is already in the render list

				for(int j=0;j<HandledSubsectors.Size();j++)
				{				
					gl_subsectorrendernode * node = SSR_List.GetNew();
					node->glsub = HandledSubsectors[j];
					node->next = glsec->otherplanes[0];
					glsec->otherplanes[0]=node;
				}
			}
			continue;
		}
	}

	unclock(totalms);
	showtotalms=totalms;
	totalms=0;
}

//==========================================================================
//
// Flood gaps with the back side's ceiling/floor texture
// This requires a stencil because the projected plane interferes with
// the depth buffer
//
//==========================================================================

struct wallseg
{
	float x1, y1, z1, x2, y2, z2;
};

static void SetupFloodStencil(wallseg * ws)
{
	int recursion = GLPortal::GetRecursion();

	// Create stencil 
	gl.StencilFunc(GL_EQUAL,recursion,~0);		// create stencil
	gl.StencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
	gl.ColorMask(0,0,0,0);						// don't write to the graphics buffer
	gl.Disable(GL_TEXTURE_2D);
	gl.Color3f(1,1,1);

	gl.Begin(GL_TRIANGLE_FAN);
	gl.Vertex3f(ws->x1, ws->z1, ws->y1);
	gl.Vertex3f(ws->x1, ws->z2, ws->y1);
	gl.Vertex3f(ws->x2, ws->z2, ws->y2);
	gl.Vertex3f(ws->x2, ws->z1, ws->y2);
	gl.End();

	gl.StencilFunc(GL_EQUAL,recursion+1,~0);		// draw sky into stencil
	gl.StencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// this stage doesn't modify the stencil

	gl.ColorMask(1,1,1,1);						// don't write to the graphics buffer
	gl.Enable(GL_TEXTURE_2D);
	gl.DepthFunc(GL_ALWAYS);
}

static void ClearFloodStencil(wallseg * ws)
{
	int recursion = GLPortal::GetRecursion();

	gl.DepthFunc(GL_LESS);
	gl.StencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	gl.Disable(GL_TEXTURE_2D);					// Disable textures (important!)
	gl.ColorMask(0,0,0,0);						// don't write to the graphics buffer
	gl.Color3f(1,1,1);

	gl.Begin(GL_TRIANGLE_FAN);
	gl.Vertex3f(ws->x1, ws->z1, ws->y1);
	gl.Vertex3f(ws->x1, ws->z2, ws->y1);
	gl.Vertex3f(ws->x2, ws->z2, ws->y2);
	gl.Vertex3f(ws->x2, ws->z1, ws->y2);
	gl.End();

	// restore old stencil op.
	gl.StencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	gl.StencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
	gl.Enable(GL_TEXTURE_2D);
	gl.ColorMask(1,1,1,1);
}

//==========================================================================
//
// Draw the plane segment into the gap
//
//==========================================================================
static void DrawFloodedPlane(wallseg * ws, float planez, sector_t * sec, bool ceiling)
{
	GLSectorPlane plane;
	int lightlevel;
	FColormap Colormap;
	FGLTexture * gltexture;

	plane.GetFromSector(sec, ceiling);
	if (plane.texture==skyflatnum) return;

	gltexture=FGLTexture::ValidateTexture(plane.texture);
	if (!gltexture) return;

	if (gl_fixedcolormap) 
	{
		Colormap.GetFixedColormap();
		lightlevel=255;
	}
	else
	{
		Colormap=sec->ColorMap;
		if (gl_isGlowingTexture(plane.texture)) 
		{
			// glowing textures are always drawn full bright without colored light
			Colormap.LightColor.r = Colormap.LightColor.g = Colormap.LightColor.b = 0xff;
			lightlevel=255;
		}
		else lightlevel=abs(ceiling? GetCeilingLight(sec) : GetFloorLight(sec));
	}

	gl_SetFog(lightlevel, Colormap.FadeColor, STYLE_Normal);
	gl_SetColor(lightlevel+(extralight<<LIGHTSEGSHIFT), Colormap.LightColor,1.0f);
	gltexture->Bind(Colormap.LightColor.a);
	gl_SetPlaneTextureRotation(&plane, gltexture);

	float fviewx =-TO_MAP(viewx);
	float fviewy = TO_MAP(viewy);
	float fviewz = TO_MAP(viewz);

	gl.Begin(GL_TRIANGLE_FAN);
	float prj_fac1 = (planez-fviewz)/(ws->z1-fviewz);
	float prj_fac2 = (planez-fviewz)/(ws->z2-fviewz);

	float px1 = fviewx + prj_fac1 * (ws->x1-fviewx);
	float py1 = fviewy + prj_fac1 * (ws->y1-fviewy);

	float px2 = fviewx + prj_fac2 * (ws->x1-fviewx);
	float py2 = fviewy + prj_fac2 * (ws->y1-fviewy);

	float px3 = fviewx + prj_fac2 * (ws->x2-fviewx);
	float py3 = fviewy + prj_fac2 * (ws->y2-fviewy);

	float px4 = fviewx + prj_fac1 * (ws->x2-fviewx);
	float py4 = fviewy + prj_fac1 * (ws->y2-fviewy);

	gl.TexCoord2f(px1 * -2, py1 * -2);
	gl.Vertex3f(px1, planez, py1);

	gl.TexCoord2f(px2 * -2, py2 * -2);
	gl.Vertex3f(px2, planez, py2);

	gl.TexCoord2f(px3 * -2, py3 * -2);
	gl.Vertex3f(px3, planez, py3);

	gl.TexCoord2f(px4 * -2, py4 * -2);
	gl.Vertex3f(px4, planez, py4);

	gl.End();

	gl.MatrixMode(GL_TEXTURE);
	gl.PopMatrix();
	gl.MatrixMode(GL_MODELVIEW);
}

//==========================================================================
//
//
//
//==========================================================================

static void FloodUpperGap(seg_t * seg)
{
	wallseg ws;
	sector_t ffake, bfake;
	sector_t * fakefsector = gl_FakeFlat(seg->frontsector, &ffake, false);
	sector_t * fakebsector = gl_FakeFlat(seg->backsector, &bfake, true);

	vertex_t * v1, * v2;
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

	ws.x1=-TO_MAP(v1->x);
	ws.y1= TO_MAP(v1->y);
	ws.x2=-TO_MAP(v2->x);
	ws.y2= TO_MAP(v2->y);

	ws.z1= TO_MAP(fakefsector->ceilingtexz);
	ws.z2= TO_MAP(fakebsector->ceilingtexz);

	// Step1: Draw a stencil into the gap
	SetupFloodStencil(&ws);

	// Step2: Project the ceiling plane into the gap
	DrawFloodedPlane(&ws, ws.z2, fakebsector, true);

	// Step3: Delete the stencil
	ClearFloodStencil(&ws);
}

//==========================================================================
//
//
//
//==========================================================================

static void FloodLowerGap(seg_t * seg)
{
	wallseg ws;
	sector_t ffake, bfake;
	sector_t * fakefsector = gl_FakeFlat(seg->frontsector, &ffake, false);
	sector_t * fakebsector = gl_FakeFlat(seg->backsector, &bfake, true);

	vertex_t * v1, * v2;
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

	ws.x1=-TO_MAP(v1->x);
	ws.y1= TO_MAP(v1->y);
	ws.x2=-TO_MAP(v2->x);
	ws.y2= TO_MAP(v2->y);

	ws.z2= TO_MAP(fakefsector->floortexz);
	ws.z1= TO_MAP(fakebsector->floortexz);

	// Step1: Draw a stencil into the gap
	SetupFloodStencil(&ws);

	// Step2: Project the ceiling plane into the gap
	DrawFloodedPlane(&ws, ws.z1, fakebsector, false);

	// Step3: Delete the stencil
	ClearFloodStencil(&ws);
}

//==========================================================================
//
//
//
//==========================================================================

void DrawUnhandledMissingTextures()
{
	// Set the drawing mode
	gl.DepthMask(false);							// don't write to Z-buffer!
	gl_EnableFog(true);
	gl.AlphaFunc(GL_GEQUAL,0.5f);
	gl.BlendFunc(GL_ONE,GL_ZERO);

	validcount++;
	for(int i=0;i<MissingUpperSegs.Size(); i++)
	{
		int index = MissingUpperSegs[i].MTI_Index;
		if (index>=0 && MissingUpperTextures[index].seg==NULL) continue;

		seg_t * seg = MissingUpperSegs[i].seg;

		// already done!
		if (seg->linedef->validcount==validcount) continue;		// already done
		seg->linedef->validcount=validcount;
		if (seg->frontsector->ceilingtexz < viewz) continue;	// out of sight
		if (gl_sectors[seg->backsector->sectornum].transdoor) continue;
		if (seg->backsector->CeilingSkyBox && seg->backsector->CeilingSkyBox->bAlways) continue;

		if (!gl_notexturefill) FloodUpperGap(seg);
	}

	validcount++;
	for(int i=0;i<MissingLowerSegs.Size(); i++)
	{
		int index = MissingLowerSegs[i].MTI_Index;
		if (index>=0 && MissingLowerTextures[index].seg==NULL) continue;

		seg_t * seg = MissingLowerSegs[i].seg;

		// already done!
		if (seg->linedef->validcount==validcount) continue;		// already done
		seg->linedef->validcount=validcount;
		if (!(gl_sectors[seg->backsector->sectornum].renderflags&SSRF_RENDERFLOOR)) continue;
		if (seg->frontsector->floortexz > viewz) continue;	// out of sight
		if (gl_sectors[seg->backsector->sectornum].transdoor) continue;
		if (seg->backsector->FloorSkyBox && seg->backsector->FloorSkyBox->bAlways) continue;

		if (!gl_notexturefill) FloodLowerGap(seg);
	}
	MissingUpperTextures.Clear();
	MissingLowerTextures.Clear();
	MissingUpperSegs.Clear();
	MissingLowerSegs.Clear();

	gl.DepthMask(true);
}

ADD_STAT(missingtextures,out)
{
	sprintf(out,"%d upper, %d lower, %.3f ms\n", 
		totalupper, totallower, SecondsPerCycle*showtotalms*1000);
}


//==========================================================================
//
// Multi-sector deep water hacks
//
//==========================================================================

void AddHackedSubsector(subsector_t * sub)
{
	if (firstmissingseg<numsegs)
	{
		SubsectorHackInfo sh={&gl_subsectors[sub-subsectors], 0};
		SubsectorHacks.Push (sh);
	}
}

//==========================================================================
//
// Finds a subsector whose plane can be used for rendering
//
//==========================================================================

bool CheckAnchorFloor(gl_subsectordata * glsub)
{
	// This subsector has a one sided wall and can be used.
	if (glsub->hacked==3) return true;

	subsector_t * sub = glsub->sub;

	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (!seg->PartnerSeg) return true;

		subsector_t * backsub = seg->PartnerSeg->Subsector;
		gl_subsectordata * backglsub = &gl_subsectors[backsub-subsectors];

		// Find a linedef with a different visplane on the other side.
		if (seg->linedef && 
			(glsub->render_sector != backglsub->render_sector && sub->sector != backsub->sector))
		{
			// I'm ignoring slopes, scaling and rotation here. The likelihood of ZDoom maps
			// using such crap hacks is simply too small
			if (glsub->render_sector->floorpic==backglsub->render_sector->floorpic &&
				glsub->render_sector->floortexz==backglsub->render_sector->floortexz &&
				GetFloorLight(glsub->render_sector)==GetFloorLight(backglsub->render_sector))
			{
				continue;
			}
			// This means we found an adjoining subsector that clearly would go into another
			// visplane. That means that this subsector can be used as an anchor.
			return true;
		}
	}
	return false;
}

//==========================================================================
//
// Collect connected subsectors that have to be rendered with the same plane
//
//==========================================================================

void CollectSubsectorsFloor(gl_subsectordata * glsub, sector_t * anchor)
{
	static GLWall wall;
	// mark it checked
	glsub->validcount=validcount;


	// We must collect any subsector that either is connected to this one with a miniseg
	// or has the same visplane.
	// We must not collect any subsector that  has the anchor's visplane!
	if (glsub->sub->numlines>2) 
	{
		// Is not being rendererd so don't bother.
		if (!(gl_ss_renderflags[glsub-gl_subsectors]&SSRF_PROCESSED)) return;

		if (glsub->render_sector->floorpic!=anchor->floorpic ||
			glsub->render_sector->floortexz!=anchor->floortexz ||
			GetFloorLight(glsub->render_sector)!=GetFloorLight(anchor)) 
		{
			HandledSubsectors.Push (glsub);
		}
	}

	// We can assume that all segs in this subsector are connected to a subsector that has
	// to be checked as well
	subsector_t * sub = glsub->sub;
	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (seg->PartnerSeg)
		{
			gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

			// could be an anchor itself.
			if (!CheckAnchorFloor (backglsub)) // must not be an anchor itself!
			{
				if (backglsub->validcount!=validcount) CollectSubsectorsFloor (backglsub, anchor);
			}
			if (!seg->linedef || (seg->frontsector==seg->backsector && glsub->render_sector!=backglsub->render_sector))
				wall.ProcessLowerMiniseg ( seg, glsub->render_sector, backglsub->render_sector);
		}
	}
}

//==========================================================================
//
// Finds a subsector whose plane can be used for rendering
//
//==========================================================================

bool CheckAnchorCeiling(gl_subsectordata * glsub)
{
	// This subsector has a one sided wall and can be used.
	if (glsub->hacked==3) return true;

	subsector_t * sub = glsub->sub;

	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (!seg->PartnerSeg) return true;

		subsector_t * backsub = seg->PartnerSeg->Subsector;
		gl_subsectordata * backglsub = &gl_subsectors[backsub-subsectors];

		// Find a linedef with a different visplane on the other side.
		if (seg->linedef && 
			(glsub->render_sector != backglsub->render_sector && sub->sector != backsub->sector))
		{
			// I'm ignoring slopes, scaling and rotation here. The likelihood of ZDoom maps
			// using such crap hacks is simply too small
			if (glsub->render_sector->ceilingpic==backglsub->render_sector->ceilingpic &&
				glsub->render_sector->ceilingtexz==backglsub->render_sector->ceilingtexz &&
				GetCeilingLight(glsub->render_sector)==GetCeilingLight(backglsub->render_sector))
			{
				continue;
			}
			// This means we found an adjoining subsector that clearly would go into another
			// visplane. That means that this subsector can be used as an anchor.
			return true;
		}
	}
	return false;
}

//==========================================================================
//
// Collect connected subsectors that have to be rendered with the same plane
//
//==========================================================================

void CollectSubsectorsCeiling(gl_subsectordata * glsub, sector_t * anchor)
{
	static GLWall wall;
	// mark it checked
	glsub->validcount=validcount;


	// We must collect any subsector that either is connected to this one with a miniseg
	// or has the same visplane.
	// We must not collect any subsector that  has the anchor's visplane!
	if (glsub->sub->numlines>2) 
	{
		// Is not being rendererd so don't bother.
		if (!(gl_ss_renderflags[glsub-gl_subsectors]&SSRF_PROCESSED)) return;

		if (glsub->render_sector->ceilingpic!=anchor->ceilingpic ||
			glsub->render_sector->ceilingtexz!=anchor->ceilingtexz ||
			GetCeilingLight(glsub->render_sector)!=GetCeilingLight(anchor)) 
		{
			HandledSubsectors.Push (glsub);
		}
	}

	// We can assume that all segs in this subsector are connected to a subsector that has
	// to be checked as well
	subsector_t * sub = glsub->sub;
	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (seg->PartnerSeg)
		{
			gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

			// could be an anchor itself.
			if (!CheckAnchorCeiling (backglsub)) // must not be an anchor itself!
			{
				if (backglsub->validcount!=validcount) CollectSubsectorsCeiling (backglsub, anchor);
			}
		}
	}
}

//==========================================================================
//
// Process the subsectors that have been marked as hacked
//
//==========================================================================

void HandleHackedSubsectors()
{
	GLWall wall;
	totalssms=0;
	clock(totalssms);

	// Each subsector may only be processed once in this loop!
	validcount++;
	for(int i=0;i<SubsectorHacks.Size();i++)
	{
		gl_subsectordata * glsub = SubsectorHacks[i].glsub;
		if (glsub->validcount!=validcount && CheckAnchorFloor(glsub))
		{
			// Now collect everything that is connected with this subsector.
			CollectSubsectorsFloor(glsub, glsub->render_sector);

			for(int j=0;j<HandledSubsectors.Size();j++)
			{				
				gl_sectordata * glsec = &gl_sectors[glsub->render_sector->sectornum];
				gl_subsectorrendernode * node = SSR_List.GetNew();

				node->glsub = HandledSubsectors[j];
				node->next = glsec->otherplanes[0];
				glsec->otherplanes[0]=node;
			}
			HandledSubsectors.Clear();
		}
	}

	// Each subsector may only be processed once in this loop!
	validcount++;
	for(int i=0;i<SubsectorHacks.Size();i++)
	{
		gl_subsectordata * glsub = SubsectorHacks[i].glsub;
		if (glsub->validcount!=validcount && CheckAnchorCeiling(glsub))
		{
			// Now collect everything that is connected with this subsector.
			CollectSubsectorsCeiling(glsub, glsub->render_sector);

			for(int j=0;j<HandledSubsectors.Size();j++)
			{				
				gl_sectordata * glsec = &gl_sectors[glsub->render_sector->sectornum];
				gl_subsectorrendernode * node = SSR_List.GetNew();

				node->glsub = HandledSubsectors[j];
				node->next = glsec->otherplanes[1];
				glsec->otherplanes[1]=node;
			}
			HandledSubsectors.Clear();
		}
	}


	SubsectorHacks.Clear();
	unclock(totalssms);
}

ADD_STAT(sectorhacks,out)
{
	sprintf(out,"%.3f ms\n", 
		SecondsPerCycle*totalssms*1000);
}


//==========================================================================
//
// This merges visplanes that lie inside a sector stack together
// to avoid rendering these unneeded flats
//
//==========================================================================

void AddFloorStack(subsector_t * sub)
{
	FloorStacks.Push(sub);
}

void AddCeilingStack(subsector_t * sub)
{
	CeilingStacks.Push(sub);
}

//==========================================================================
//
//
//
//==========================================================================

void CollectSectorStacksCeiling(gl_subsectordata * glsub, sector_t * anchor)
{
	static GLWall wall;
	// mark it checked
	glsub->validcount=validcount;

	// Has a sector stack or skybox itself!
	if (glsub->render_sector->CeilingSkyBox && glsub->render_sector->CeilingSkyBox->bAlways) return;

	// Don't bother processing unrendered subsectors
	if (glsub->sub->numlines>2 && !(gl_ss_renderflags[glsub-gl_subsectors]&SSRF_PROCESSED)) return;

	// Must be the exact same visplane
	if (glsub->render_sector->ceilingpic != anchor->ceilingpic ||
		glsub->render_sector->ceilingplane != anchor->ceilingplane ||
		GetCeilingLight(glsub->render_sector) != GetCeilingLight(anchor) ||
		glsub->render_sector->ColorMap != anchor->ColorMap ||
		glsub->render_sector->ceiling_xoffs != anchor->ceiling_xoffs || 
		glsub->render_sector->ceiling_yoffs + glsub->render_sector->base_ceiling_yoffs !=
			anchor->ceiling_yoffs + anchor->base_ceiling_yoffs ||
		glsub->render_sector->ceiling_xscale != anchor->ceiling_xscale || 
		glsub->render_sector->ceiling_yscale != anchor->ceiling_yscale ||
		glsub->render_sector->ceiling_angle + glsub->render_sector->base_ceiling_angle !=
			anchor->ceiling_angle + anchor->base_ceiling_angle)
	{
		// different visplane so it can't belong to this stack
		return;
	}

	HandledSubsectors.Push (glsub);

	subsector_t * sub = glsub->sub;
	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (seg->PartnerSeg)
		{
			gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

			if (backglsub->validcount!=validcount) CollectSectorStacksCeiling (backglsub, anchor);
		}
	}
}

//==========================================================================
//
//
//
//==========================================================================

void CollectSectorStacksFloor(gl_subsectordata * glsub, sector_t * anchor)
{
	static GLWall wall;
	// mark it checked
	glsub->validcount=validcount;

	// Has a sector stack or skybox itself!
	if (glsub->render_sector->FloorSkyBox && glsub->render_sector->FloorSkyBox->bAlways) return;

	// Don't bother processing unrendered subsectors
	if (glsub->sub->numlines>2 && !(gl_ss_renderflags[glsub-gl_subsectors]&SSRF_PROCESSED)) return;

	// Must be the exact same visplane
	if (glsub->render_sector->floorpic != anchor->floorpic ||
		glsub->render_sector->floorplane != anchor->floorplane ||
		GetFloorLight(glsub->render_sector) != GetFloorLight(anchor) ||
		glsub->render_sector->ColorMap != anchor->ColorMap ||
		glsub->render_sector->floor_xoffs != anchor->floor_xoffs || 
		glsub->render_sector->floor_yoffs + glsub->render_sector->base_floor_yoffs !=
		anchor->floor_yoffs + anchor->base_floor_yoffs ||
		glsub->render_sector->floor_xscale != anchor->floor_xscale || 
		glsub->render_sector->floor_yscale != anchor->floor_yscale ||
		glsub->render_sector->floor_angle + glsub->render_sector->base_floor_angle !=
		anchor->floor_angle + anchor->base_floor_angle)
	{
		// different visplane so it can't belong to this stack
		return;
	}

	HandledSubsectors.Push (glsub);

	subsector_t * sub = glsub->sub;
	for(int j=0;j<sub->numlines;j++)
	{
		seg_t * seg = &segs[sub->firstline+j];
		if (seg->PartnerSeg)
		{
			gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

			if (backglsub->validcount!=validcount) CollectSectorStacksFloor (backglsub, anchor);
		}
	}
}

//==========================================================================
//
//
//
//==========================================================================

void ProcessSectorStacks()
{
	int i;

	validcount++;
	for (i=0;i<CeilingStacks.Size (); i++)
	{
		subsector_t * sub = CeilingStacks[i];
		gl_subsectordata * glsub = &gl_subsectors[sub-subsectors];

		for(int j=0;j<sub->numlines;j++)
		{
			seg_t * seg = &segs[sub->firstline+j];
			if (seg->PartnerSeg)
			{
				gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

				if (backglsub->validcount!=validcount) CollectSectorStacksCeiling (backglsub, glsub->render_sector);
			}
		}

		for(int j=0;j<HandledSubsectors.Size();j++)
		{				
			gl_sectordata * glsec = &gl_sectors[glsub->render_sector->sectornum];

			gl_ss_renderflags[HandledSubsectors[j]-gl_subsectors] &= ~SSRF_RENDERCEILING;

			if (glsub->render_sector->CeilingSkyBox->PlaneAlpha!=0)
			{
				gl_subsectorrendernode * node = SSR_List.GetNew();
				node->glsub = HandledSubsectors[j];
				node->next = glsec->otherplanes[1];
				glsec->otherplanes[1]=node;
			}
		}
		HandledSubsectors.Clear();
	}

	validcount++;
	for (i=0;i<FloorStacks.Size (); i++)
	{
		subsector_t * sub = FloorStacks[i];
		gl_subsectordata * glsub = &gl_subsectors[sub-subsectors];

		for(int j=0;j<sub->numlines;j++)
		{
			seg_t * seg = &segs[sub->firstline+j];
			if (seg->PartnerSeg)
			{
				gl_subsectordata * backglsub = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

				if (backglsub->validcount!=validcount) CollectSectorStacksFloor (backglsub, glsub->render_sector);
			}
		}

		for(int j=0;j<HandledSubsectors.Size();j++)
		{				
			gl_sectordata * glsec = &gl_sectors[glsub->render_sector->sectornum];

			gl_ss_renderflags[HandledSubsectors[j]-gl_subsectors] &= ~SSRF_RENDERFLOOR;

			if (glsub->render_sector->FloorSkyBox->PlaneAlpha!=0)
			{
				gl_subsectorrendernode * node = SSR_List.GetNew();
				node->glsub = HandledSubsectors[j];
				node->next = glsec->otherplanes[0];
				glsec->otherplanes[0]=node;
			}
		}
		HandledSubsectors.Clear();
	}

	FloorStacks.Clear();
	CeilingStacks.Clear();
}


