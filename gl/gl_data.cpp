#include "gl_pch.h"

/*
** gl_data.cpp
** Maintenance data for GL renderer (mostly to handle rendering hacks)
**
**---------------------------------------------------------------------------
** Copyright 2005 Christoph Oelckers
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

#include "i_system.h"
#include "p_local.h"
#include "c_dispatch.h"
#include "gl/gl_lights.h"
#include "gl/gl_glow.h"
#include "gl/gl_data.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_functions.h"


CVAR(Int, wallglowheight, 128, CVAR_ARCHIVE)
CVAR(Float, wallglowfactor, 0.6f, CVAR_ARCHIVE)

//==========================================================================
//
// Portal identifier lists
//
//==========================================================================

TArray<GLVertex> gl_vertices(1024);

gl_sectordata * gl_sectors;
gl_subsectordata * gl_subsectors;
byte * gl_ss_renderflags;
static line_t ** gl_linebuffer;

// A simple means so that I don't have to link to the debug stuff when I don't need it!
void (*gl_DebugHook)();


//==========================================================================
//
// prepare subsectors for GL rendering
//
//==========================================================================

CVAR(Int,forceglnodes, 0, CVAR_GLOBALCONFIG)	// only for testing - don't save!


inline void M_ClearBox (fixed_t *box)
{
	box[BOXTOP] = box[BOXRIGHT] = INT_MIN;
	box[BOXBOTTOM] = box[BOXLEFT] = INT_MAX;
}

inline void M_AddToBox(fixed_t* box,fixed_t x,fixed_t y)
{
	if (x<box[BOXLEFT]) box[BOXLEFT] = x;
	if (x>box[BOXRIGHT]) box[BOXRIGHT] = x;
	if (y<box[BOXBOTTOM]) box[BOXBOTTOM] = y;
	if (y>box[BOXTOP]) box[BOXTOP] = y;
}

static void SpreadHackedFlag(gl_subsectordata * glsub)
{
	// The subsector pointer hasn't been set yet!
	subsector_t * sub = &subsectors[glsub-gl_subsectors];
	for(int i=0;i<sub->numlines;i++)
	{
		seg_t * seg = &segs[sub->firstline+i];

		if (seg->PartnerSeg)
		{
			gl_subsectordata * glsub2 = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];

			if (!(glsub2->hacked&1) && glsub2->render_sector==glsub->render_sector)
			{
				glsub2->hacked|=1;
				SpreadHackedFlag (glsub2);
			}
		}
	}
}

static void PrepareSectorData()
{
	int 				i;
	int 				j;
	gl_sectordata *		gl_sector;
	DBoundingBox		bbox;
	size_t				/*ii,*/ jj;
	TArray<subsector_t *> undetermined;
	subsector_t *		ss;
	gl_subsectordata *	glss;

	// The GL node builder produces screwed output when two-sided walls overlap with one-sides ones!
	for(i=0;i<numsegs;i++)
	{
		int partner= segs[i].PartnerSeg-segs;

		if (partner<0 || partner>=numsegs || &segs[partner]!=segs[i].PartnerSeg)
		{
			segs[i].PartnerSeg=NULL;
		}

		// glbsp creates such incorrect references for Strife.
		if (segs[i].linedef && segs[i].PartnerSeg && !segs[i].PartnerSeg->linedef)
		{
			segs[i].PartnerSeg = segs[i].PartnerSeg->PartnerSeg = NULL;
		}
	}

	// look up sector number for each subsector
	for (i = 0; i < numsubsectors; i++)
	{
		// For rendering pick the sector from the first seg that is a sector boundary
		// this takes care of self-referencing sectors
		ss = &subsectors[i];
		glss = &gl_subsectors[i];
		seg_t *seg = &segs[ss->firstline];
		M_ClearBox(glss->bbox);
		for(jj=0; jj<ss->numlines; jj++)
		{
			M_AddToBox(glss->bbox,seg->v1->x, seg->v1->y);
			seg++;
		}

		if (forceglnodes<2)
		{
			seg_t * seg = &segs[ss->firstline];
			for(j=0; j<ss->numlines; j++)
			{
				if(seg->sidedef && (!seg->PartnerSeg || seg->sidedef->sector!=seg->PartnerSeg->sidedef->sector))
				{
					glss->render_sector = seg->sidedef->sector;
					break;
				}
				seg++;
			}
			if(glss->render_sector == NULL) 
			{
				undetermined.Push(ss);
			}
		}
		else glss->render_sector=ss->sector;
	}

	// assign a vaild render sector to all subsectors which haven't been processed yet.
	while (undetermined.Size())
	{
		bool deleted=false;
		for(i=undetermined.Size()-1;i>=0;i--)
		{
			ss=undetermined[i];
			seg_t * seg = &segs[ss->firstline];
			
			for(j=0; j<ss->numlines; j++)
			{
				if (seg->PartnerSeg && seg->PartnerSeg->Subsector)
				{
					sector_t * backsec = gl_subsectors[seg->PartnerSeg->Subsector-subsectors].render_sector;
					if (backsec)
					{
						gl_subsectors[ss-subsectors].render_sector=backsec;
						undetermined.Delete(i);
						deleted=1;
						break;
					}
				}
				seg++;
			}
		}
		if (!deleted && undetermined.Size()) 
		{
			// This only happens when a subsector is off the map.
			// Don't bother and just assign the real sector for rendering
			for(i=undetermined.Size()-1;i>=0;i--)
			{
				ss=undetermined[i];
				gl_subsectors[ss-subsectors].render_sector=ss->sector;
			}
			break;
		}
	}

	// now group the subsectors by sector
	gl_subsectordata ** gl_subsectorbuffer = new gl_subsectordata * [numsubsectors];

	for(i=0, glss=gl_subsectors; i<numsubsectors; i++, glss++)
	{
		gl_sectors[glss->render_sector->sectornum].subsectorcount++;
	}

	for (i=0, gl_sector = gl_sectors; i<numsectors; i++, gl_sector++) 
	{
		gl_sector->gl_subsectors = gl_subsectorbuffer;
		gl_subsectorbuffer += gl_sector->subsectorcount;
		gl_sector->subsectorcount = 0;
	}
	
	for(i=0, glss = gl_subsectors; i<numsubsectors; i++, glss++)
	{
		gl_sector = &gl_sectors[glss->render_sector->sectornum];
		gl_sector->gl_subsectors[gl_sector->subsectorcount++]=glss;
	}

	// marks all malformed subsectors so rendering tricks using them can be handled more easily
	for (i = 0; i < numsubsectors; i++)
	{
		if (subsectors[i].sector == gl_subsectors[i].render_sector)
		{
			seg_t * seg = &segs[subsectors[i].firstline];
			for(int j=0;j<subsectors[i].numlines;j++)
			{
				if (!(gl_subsectors[i].hacked&1) && seg[j].linedef==0 && 
						seg[j].PartnerSeg!=NULL && gl_subsectors[i].render_sector != 
						gl_subsectors[seg[j].PartnerSeg->Subsector-subsectors].render_sector)
				{
					Printf("Found hack: %d,%d %d,%d\n", seg[j].v1->x>>16, seg[j].v1->y>>16, seg[j].v2->x>>16, seg[j].v2->y>>16);
					gl_subsectors[i].hacked|=1;
					SpreadHackedFlag(&gl_subsectors[i]);
				}
				if (seg[j].PartnerSeg==NULL) gl_subsectors[i].hacked|=2;	// used for quick termination checks
			}
		}
	}
}

//==========================================================================
//
// Some processing for transparent door hacks
//
//==========================================================================
static void PrepareTransparentDoors(sector_t * sector)
{
	gl_sectordata * glsec = &gl_sectors[sector-sectors];
	bool solidwall=false;
	int notextures=0;
	int selfref=0;
	int i;
	sector_t * nextsec=NULL;

#ifdef _DEBUG
	if (sector-sectors==142)
	{
		__asm nop
	}
#endif

	P_Recalculate3DFloors(sector);
	if (glsec->subsectorcount==0) return;

	glsec->transdoorheight=sector->floortexz;
	glsec->transdoor= !(sector->e->ffloors.Size() || sector->heightsec || sector->floorplane.a || sector->floorplane.b);

	if (glsec->transdoor)
	{
		for (i=0; i<sector->linecount; i++)
		{
			if (sector->lines[i]->frontsector==sector->lines[i]->backsector) 
			{
				selfref++;
				continue;
			}

			sector_t * sec=getNextSector(sector->lines[i], sector);
			if (sec==NULL) 
			{
				solidwall=true;
				continue;
			}
			else
			{
				nextsec=sec;

				int side=sides[sector->lines[i]->sidenum[0]].sector==sec;

				if (sector->floortexz!=sec->floortexz+FRACUNIT) 
				{
					glsec->transdoor=false;
					return;
				}
				if (sides[sector->lines[i]->sidenum[1-side]].toptexture==0) notextures++;
			}
		}
		if (selfref+notextures==sector->linecount || sector->ceilingpic==skyflatnum)
		{
			glsec->transdoor=false;
			return;
		}

		if (solidwall)
		{
			// This is a crude attempt to fix an incorrect transparent door effect I found in some
			// WolfenDoom maps but considering the amount of code required to handle it I left it in.
			if (nextsec)
			{
				sector->heightsec=nextsec;
				sector->heightsec->MoreFlags=0;
			}
			glsec->transdoor=false;
		}
	}
}



//=============================================================================
//
//
//
//=============================================================================
side_t* getNextSide(sector_t * sec, line_t* line)
{
	if (sec==line->frontsector)
	{
		if (sec==line->backsector) return NULL;	
		if (line->sidenum[1]!=NO_INDEX) return &sides[line->sidenum[1]];
	}
	else
	{
		if (line->sidenum[0]!=NO_INDEX) return &sides[line->sidenum[0]];
	}
	return NULL;
}

//==========================================================================
//
// Initialize the level data for the GL renderer
//
//==========================================================================
void gl_InitModels();

void gl_PreprocessLevel()
{
	int i,j;

	static bool modelsdone=false;

	if (!modelsdone)
	{
		modelsdone=true;
		gl_InitModels();
	}

	R_ResetViewInterpolation ();


	// Nasty: I can't rely upon the sidedef assignments because ZDBSP likes to screw them up
	// if the sidedefs are compressed and both sides are the same.
	for(i=0;i<numsegs;i++)
	{
		seg_t * seg=&segs[i];
		if (seg->backsector == seg->frontsector && seg->linedef)
		{
			fixed_t d1=P_AproxDistance(seg->v1->x-seg->linedef->v1->x,seg->v1->y-seg->linedef->v1->y);
			fixed_t d2=P_AproxDistance(seg->v2->x-seg->linedef->v1->x,seg->v2->y-seg->linedef->v1->y);

			if (d2<d1)	// backside
			{
				seg->sidedef = &sides[seg->linedef->sidenum[1]];
			}
			else	// front side
			{
				seg->sidedef = &sides[seg->linedef->sidenum[0]];
			}
		}
	}

	gl_subsectors = new gl_subsectordata[numsubsectors];
	memset(gl_subsectors, 0, numsubsectors * sizeof(gl_subsectordata));

	gl_ss_renderflags = new byte[numsubsectors];
	memset(gl_ss_renderflags, 0, numsubsectors * sizeof(byte));

	gl_sectors = new gl_sectordata[numsectors];
	memset(gl_sectors, 0, numsectors * sizeof(gl_sectordata));

	gl_linebuffer = new line_t *[numsides];
	PrepareSectorData();
	for(i=0;i<numsectors;i++) PrepareTransparentDoors(&sectors[i]);
	pitch=0.0f;

	gl_vertices.Resize(100);	

	// Create the flat vertex array
	for (i=0; i<numsubsectors; i++)
	{
		subsector_t * ssector = &subsectors[i];
		gl_subsectordata * glsub = &gl_subsectors[i];

		glsub->sub = ssector;
		if (ssector->numlines<=2) continue;
			
		glsub->numvertices = ssector->numlines;
		glsub->firstvertex = gl_vertices.Size();

		for(j = 0;  j < ssector->numlines; j++)
		{
			seg_t * seg = &segs[ssector->firstline + j];
			vertex_t * vtx = seg->v1;
			GLVertex * vt=&gl_vertices[gl_vertices.Reserve(1)];

			vt->u =( (float)(vtx->x)/FRACUNIT)/64.0f;
			vt->v =(-(float)(vtx->y)/FRACUNIT)/64.0f;
			vt->x = -TO_MAP(vtx->x);
			vt->z =  TO_MAP(vtx->y);
			vt->y = 0.0f;
			vt->vt = vtx;
		}
	}
	gl_CollectMissingLines();

	// This code can be called when the hardware renderer is inactive!
	if (currentrenderer!=0) gl.ArrayPointer(&gl_vertices[0], sizeof(GLVertex));

	if (gl_DebugHook) gl_DebugHook();
}


CCMD(dumpgeometry)
{
	for(int i=0;i<numsectors;i++)
	{
		gl_sectordata * glsec = &gl_sectors[i];

		Printf("Sector %d\n",i);
		for(int j=0;j<glsec->subsectorcount;j++)
		{
			gl_subsectordata * glsub = glsec->gl_subsectors[j];
			subsector_t * sub = glsub->sub;

			Printf("    Subsector %d - real sector = %d - %s\n", sub-subsectors, sub->sector->sectornum, glsub->hacked&1? "hacked":"");
			for(int k=0;k<sub->numlines;k++)
			{
				seg_t * seg = &segs[sub->firstline+k];
				if (seg->linedef)
				{
				Printf("      (%4.4f, %4.4f), (%4.4f, %4.4f) - seg %d, linedef %d, side %d", 
					seg->v1->x/65536.0f, seg->v1->y/65536.0f, seg->v2->x/65536.0f, seg->v2->y/65536.0f,
					seg-segs, seg->linedef-lines, seg->sidedef!=&sides[seg->linedef->sidenum[0]]);
				}
				else
				{
					Printf("      (%4.4f, %4.4f), (%4.4f, %4.4f) - seg %d, miniseg", 
						seg->v1->x/65536.0f, seg->v1->y/65536.0f, seg->v2->x/65536.0f, seg->v2->y/65536.0f,
						seg-segs);
				}
				if (seg->PartnerSeg) 
				{
					gl_subsectordata * glsub2 = &gl_subsectors[seg->PartnerSeg->Subsector-subsectors];
					Printf(", back sector = %d, real back sector = %d", glsub2->render_sector->sectornum, seg->PartnerSeg->frontsector->sectornum);
				}
				Printf("\n");
			}
		}
	}
}

//==========================================================================
//
// Cleans up all the GL data for the last level
//
//==========================================================================
void gl_CleanLevelData()
{
	// Dynamic lights must be destroyed before the sector information here is deleted!
	TThinkerIterator<ADynamicLight> it(STAT_DLIGHT);
	AActor * mo=it.Next();
	while (mo)
	{
		AActor * next = it.Next();
		mo->Destroy();
		mo=next;
	}

	if (gl_sectors) 
	{
		if (gl_sectors[0].gl_subsectors) delete gl_sectors[0].gl_subsectors;
		delete [] gl_sectors;
	}

	if (gl_linebuffer) delete [] gl_linebuffer;

	if (gamenodes && gamenodes!=nodes)
	{
		delete [] gamenodes;
		gamenodes=NULL;
	}
}


