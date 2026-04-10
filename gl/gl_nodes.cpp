/*
** gl_nodes.cpp
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
#include <math.h>
#ifdef _MSC_VER
#include <malloc.h>		// for alloca()
#endif

#include "templates.h"
#include "m_alloc.h"
#include "m_argv.h"
#include "m_swap.h"
#include "g_game.h"
#include "i_system.h"
#include "w_wad.h"
#include "doomdef.h"
#include "p_local.h"
#include "nodebuild.h"
#include "doomstat.h"
#include "vectors.h"
#include "stats.h"
#include "doomerrors.h"

node_t * gamenodes;
int numgamenodes;
subsector_t * gamesubsectors;
int numgamesubsectors;
void P_GetPolySpots (int lump, TArray<FNodeBuilder::FPolyStart> &spots, TArray<FNodeBuilder::FPolyStart> &anchors);

extern bool	UsingGLNodes;

// fixed 32 bit gl_vert format v2.0+ (glBsp 1.91)
typedef struct
{
  fixed_t x,y;
} mapglvertex_t;

typedef struct 
{
	long numsegs;
	long firstseg;    // Index of first one; segs are stored sequentially.
} gl3_mapsubsector_t;

typedef struct
{
	unsigned short	v1;		 // start vertex		(16 bit)
	unsigned short	v2;		 // end vertex			(16 bit)
	unsigned short	linedef; // linedef, or -1 for minisegs
	short			side;	 // side on linedef: 0 for right, 1 for left
	unsigned short	partner; // corresponding partner seg, or -1 on one-sided walls
} glseg_t;

typedef struct
{
	long			v1;
	long			v2;
	short			linedef;
	short			side;
	long			partner;
} glseg3_t;


typedef struct
{
	short 	x,y,dx,dy;	// partition line
	short 	bbox[2][4];	// bounding box for each child
	// If NF_SUBSECTOR is or'ed in, it's a subsector,
	// else it's a node of another subtree.
	unsigned int children[2];
} gl5_mapnode_t;

#define GL5_NF_SUBSECTOR (1 << 31)
//==========================================================================
//
// Checks whether the nodes are suitable for GL rendering
//
//==========================================================================

bool gl_CheckForGLNodes()
{
	int i;

	for(i=0;i<numsubsectors;i++)
	{
		subsector_t * sub = &subsectors[i];
		seg_t * firstseg = &segs[sub->firstline];
		seg_t * lastseg = &segs[sub->firstline+sub->numlines-1];

		if (firstseg->v1 != lastseg->v2)
		{
			// This subsector is incomplete which means that these
			// are normal nodes
			return false;
		}
		else
		{
			for(DWORD j=0;j<sub->numlines;j++)
			{
				if (segs[j].linedef==NULL)	// miniseg
				{
					// We already have GL nodes. Great!
					return true;
				}
			}
		}
	}
	// all subsectors were closed but there are no minisegs
	// Although unlikely this can happen. Such nodes are not a problem.
	return true;
}


//==========================================================================
//
// gl_LoadVertexes
//
// loads GL vertices
//
//==========================================================================
#define gNd2            0x32644E67
#define gNd4            0x34644E67
#define gNd5            0x35644E67
#define GL_VERT_OFFSET  4
static int firstglvertex;
static bool format5;

static bool gl_LoadVertexes(FILE * f, wadlump_t * lump)
{
	byte *gldata;
	int                 i;

	firstglvertex = numvertexes;
	
	int gllen=lump->Size;

	gldata = new byte[gllen];
	fseek(f, lump->FilePos, SEEK_SET);
	fread(gldata, gllen, 1, f);

	if (*(int *)gldata == gNd5) 
	{
		format5=true;
	}
	else if (*(int *)gldata != gNd2) 
	{
		// GLNodes V1 and V4 are unsupported.
		// V1 because the precision is insufficient and
		// V4 due to the missing partner segs
		Printf("GL nodes v%d found. This format is not supported by GZDoom\n",
			(*(int *)gldata == gNd4)? 4:1);

		delete [] gldata;
		return false;
	}
	else format5=false;

	mapglvertex_t*	mgl;

	vertex_t * oldvertexes = vertexes;
	numvertexes += (gllen - GL_VERT_OFFSET)/sizeof(mapglvertex_t);
	vertexes	 = new vertex_t[numvertexes];
	mgl			 = (mapglvertex_t *) (gldata + GL_VERT_OFFSET);	

	memcpy(vertexes, oldvertexes, firstglvertex * sizeof(vertex_t));
	for(i=0;i<numlines;i++)
	{
		lines[i].v1 = vertexes + (lines[i].v1 - oldvertexes);
		lines[i].v2 = vertexes + (lines[i].v2 - oldvertexes);
	}

	for (i = firstglvertex; i < numvertexes; i++)
	{
		vertexes[i].x = mgl->x;
		vertexes[i].y = mgl->y;
		mgl++;
	}
	delete gldata;
	return true;
}

//==========================================================================
//
// GL Nodes utilities
//
//==========================================================================

static inline int checkGLVertex(int num)
{
	if (num & 0x8000)
		num = (num&0x7FFF)+firstglvertex;
	return num;
}

static inline int checkGLVertex3(int num)
{
	if (num & 0xc0000000)
		num = (num&0x3FFFFFFF)+firstglvertex;
	return num;
}

//==========================================================================
//
// gl_LoadGLSegs
//
//==========================================================================

bool gl_LoadGLSegs(FILE * f, wadlump_t * lump)
{
	char		*data;
	int			i;
	line_t		*ldef=NULL;
	
	numsegs = lump->Size;
	data= new char[numsegs];
	fseek(f, lump->FilePos, SEEK_SET);
	fread(data, lump->Size, 1, f);

	if (!format5 && memcmp(data, "gNd3", 4))
	{
		numsegs/=sizeof(glseg_t);
		segs = new seg_t[numsegs];
		memset(segs,0,sizeof(seg_t)*numsegs);
		
		glseg_t * ml = (glseg_t*)data;
		for(i = 0; i < numsegs; i++)
		{							// check for gl-vertices
			segs[i].v1 = &vertexes[checkGLVertex(SHORT(ml->v1))];
			segs[i].v2 = &vertexes[checkGLVertex(SHORT(ml->v2))];
			
			segs[i].PartnerSeg=&segs[SHORT(ml->partner)];
			if(ml->linedef != 0xffff)
			{
				ldef = &lines[SHORT(ml->linedef)];
				segs[i].linedef = ldef;

				
				ml->side=SHORT(ml->side);
				segs[i].sidedef = &sides[ldef->sidenum[ml->side]];
				segs[i].frontsector = sides[ldef->sidenum[ml->side]].sector;
				if (ldef->flags & ML_TWOSIDED && ldef->sidenum[ml->side^1]!=NO_INDEX)
					segs[i].backsector = sides[ldef->sidenum[ml->side^1]].sector;
				else
				{
					ldef->flags &= ~ML_TWOSIDED;
					segs[i].backsector = 0;
				}

			}
			else
			{
				segs[i].linedef = NULL;
				segs[i].sidedef = NULL;

				segs[i].frontsector = NULL;
				segs[i].backsector  = NULL;
			}
			ml++;		
		}
	}
	else
	{
		if (!format5) numsegs-=4;
		numsegs/=sizeof(glseg3_t);
		segs = new seg_t[numsegs];
		memset(segs,0,sizeof(seg_t)*numsegs);
		
		glseg3_t * ml = (glseg3_t*)(data+ (format5? 0:4));
		for(i = 0; i < numsegs; i++)
		{							// check for gl-vertices
			segs[i].v1 = &vertexes[checkGLVertex3(LONG(ml->v1))];
			segs[i].v2 = &vertexes[checkGLVertex3(LONG(ml->v2))];
			
			segs[i].PartnerSeg=&segs[LONG(ml->partner)];
			if(ml->linedef != -1) // skip minisegs 
			{
				ldef = &lines[LONG(ml->linedef)];
				segs[i].linedef = ldef;

				
				ml->side=SHORT(ml->side);
				segs[i].sidedef = &sides[ldef->sidenum[ml->side]];
				segs[i].frontsector = sides[ldef->sidenum[ml->side]].sector;
				if (ldef->flags & ML_TWOSIDED && ldef->sidenum[ml->side^1]!=NO_INDEX)
					segs[i].backsector = sides[ldef->sidenum[ml->side^1]].sector;
				else
				{
					ldef->flags &= ~ML_TWOSIDED;
					segs[i].backsector = 0;
				}

			}
			else
			{
				segs[i].linedef = NULL;
				segs[i].sidedef = NULL;
				segs[i].frontsector = NULL;
				segs[i].backsector  = NULL;
			}
			ml++;		
		}
	}
	delete data;
	return true;
}


//==========================================================================
//
// gl_LoadGLSubsectors
//
//==========================================================================

bool gl_LoadGLSubsectors(FILE * f, wadlump_t * lump)
{
	char * datab;
	int  i;
	
	numsubsectors = lump->Size;
	datab = new char[numsubsectors];
	fseek(f, lump->FilePos, SEEK_SET);
	fread(datab, lump->Size, 1, f);
	
	if (numsubsectors == 0)
	{
		return false;
	}
	
	if (!format5 && memcmp(datab, "gNd3", 4))
	{
		mapsubsector_t * data = (mapsubsector_t*) datab;
		numsubsectors /= sizeof(mapsubsector_t);
		subsectors = new subsector_t[numsubsectors];
		memset(subsectors,0,numsubsectors * sizeof(subsector_t));
	
		for (i=0; i<numsubsectors; i++)
		{
			subsectors[i].numlines  = SHORT(data[i].numsegs );
			subsectors[i].firstline = SHORT(data[i].firstseg);

			if (subsectors[i].numlines == 0)
			{
				return false;
			}
		}
	}
	else
	{
		gl3_mapsubsector_t * data = (gl3_mapsubsector_t*) (datab+(format5? 0:4));
		numsubsectors /= sizeof(gl3_mapsubsector_t);
		subsectors = new subsector_t[numsubsectors];
		memset(subsectors,0,numsubsectors * sizeof(subsector_t));
	
		for (i=0; i<numsubsectors; i++)
		{
			subsectors[i].numlines  = LONG(data[i].numsegs );
			subsectors[i].firstline = LONG(data[i].firstseg);

			if (subsectors[i].numlines == 0)
			{
				return false;
			}
		}
	}

	for (i=0; i<numsubsectors; i++)
	{
		for(unsigned j=0;j<subsectors[i].numlines;j++)
		{
			seg_t * seg = &segs[subsectors[i].firstline+j];
			if (seg->linedef==NULL) seg->frontsector = seg->backsector = segs[subsectors[i].firstline].frontsector;
		}
	}

	delete datab;	
	return true;
}

//==========================================================================
//
// P_LoadNodes
//
//==========================================================================

static bool gl_LoadNodes (FILE * f, wadlump_t * lump)
{
	int 		i;
	int 		j;
	int 		k;
	node_t* 	no;
	WORD*		used;

	if (!format5)
	{
		mapnode_t*	mn, * basemn;
		numnodes = lump->Size / sizeof(mapnode_t);

		if (numnodes == 0) return false;

		nodes = new node_t[numnodes];		
		fseek(f, lump->FilePos, SEEK_SET);

		basemn = mn = new mapnode_t[numnodes];
		fread(mn, lump->Size, 1, f);

		used = (WORD *)alloca (sizeof(WORD)*numnodes);
		memset (used, 0, sizeof(WORD)*numnodes);

		no = nodes;

		for (i = 0; i < numnodes; i++, no++, mn++)
		{
			no->x = SHORT(mn->x)<<FRACBITS;
			no->y = SHORT(mn->y)<<FRACBITS;
			no->dx = SHORT(mn->dx)<<FRACBITS;
			no->dy = SHORT(mn->dy)<<FRACBITS;
			for (j = 0; j < 2; j++)
			{
				WORD child = SHORT(mn->children[j]);
				if (child & NF_SUBSECTOR)
				{
					child &= ~NF_SUBSECTOR;
					if (child >= numsubsectors)
					{
						delete [] basemn;
						return false;
					}
					no->children[j] = (BYTE *)&subsectors[child] + 1;
				}
				else if (child >= numnodes)
				{
					delete [] basemn;
					return false;
				}
				else if (used[child])
				{
					delete [] basemn;
					return false;
				}
				else
				{
					no->children[j] = &nodes[child];
					used[child] = j + 1;
				}
				for (k = 0; k < 4; k++)
				{
					no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
				}
			}
		}
		delete [] basemn;
	}
	else
	{
		gl5_mapnode_t*	mn, * basemn;
		numnodes = lump->Size / sizeof(gl5_mapnode_t);

		if (numnodes == 0) return false;

		nodes = new node_t[numnodes];		
		fseek(f, lump->FilePos, SEEK_SET);

		basemn = mn = new gl5_mapnode_t[numnodes];
		fread(mn, lump->Size, 1, f);

		used = (WORD *)alloca (sizeof(WORD)*numnodes);
		memset (used, 0, sizeof(WORD)*numnodes);

		no = nodes;

		for (i = 0; i < numnodes; i++, no++, mn++)
		{
			no->x = SHORT(mn->x)<<FRACBITS;
			no->y = SHORT(mn->y)<<FRACBITS;
			no->dx = SHORT(mn->dx)<<FRACBITS;
			no->dy = SHORT(mn->dy)<<FRACBITS;
			for (j = 0; j < 2; j++)
			{
				SDWORD child = LONG(mn->children[j]);
				if (child & GL5_NF_SUBSECTOR)
				{
					child &= ~GL5_NF_SUBSECTOR;
					if (child >= numsubsectors)
					{
						delete [] basemn;
						return false;
					}
					no->children[j] = (BYTE *)&subsectors[child] + 1;
				}
				else if (child >= numnodes)
				{
					delete [] basemn;
					return false;
				}
				else if (used[child])
				{
					delete [] basemn;
					return false;
				}
				else
				{
					no->children[j] = &nodes[child];
					used[child] = j + 1;
				}
				for (k = 0; k < 4; k++)
				{
					no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
				}
			}
		}
		delete [] basemn;
	}
	return true;
}

//==========================================================================
//
// loads the GL node data
//
//==========================================================================

bool gl_DoLoadGLNodes(FILE * f, wadlump_t * lumps)
{
	if (!gl_LoadVertexes(f, &lumps[0]))
	{
		return false;
	}
	if (!gl_LoadGLSegs(f, &lumps[1]))
	{
		delete [] segs;
		segs = NULL;
		return false;
	}
	if (!gl_LoadGLSubsectors(f, &lumps[2]))
	{
		delete [] subsectors;
		subsectors = NULL;
		delete [] segs;
		segs = NULL;
		return false;
	}
	if (!gl_LoadNodes(f, &lumps[3]))
	{
		delete [] nodes;
		nodes = NULL;
		delete [] subsectors;
		subsectors = NULL;
		delete [] segs;
		segs = NULL;
		return false;
	}

	// Quick check for the validity of the nodes
	// For invalid nodes there is a high chance that this test will fail

	for (int i = 0; i < numsubsectors; i++)
	{
		seg_t * seg = &segs[subsectors[i].firstline];
		if (!seg->sidedef) 
		{
			Printf("GWA file contains invalid nodes. The BSP has to be rebuilt.\n");
			delete [] nodes;
			nodes = NULL;
			delete [] subsectors;
			subsectors = NULL;
			delete [] segs;
			segs = NULL;
			return false;
		}
	}
	return true;
}

//==========================================================================
//
// Checks for the presence of GL nodes in the loaded WADs or a .GWA file
//
//==========================================================================

bool gl_LoadGLNodes(int lumpnum)
{
	wadinfo_t wadheader;
	wadlump_t gwalumps[4];
	char path[256];
	char glname[16];
	int li;
	int lumpfile = Wads.GetLumpFile(lumpnum);
	const char * name = Wads.GetWadFullName(lumpfile);

	strcpy(glname, "GL_");
	Wads.GetLumpName(glname+3, lumpnum);
	if (strlen(glname)>8) return false;

	if ((li=Wads.CheckNumForName(glname))>=0 && Wads.GetLumpFile(li)==lumpfile)
	{
		// GL nodes are loaded with a WAD
		for(int i=0;i<4;i++)
		{
			gwalumps[i].FilePos=Wads.GetLumpOffset(li+i+1);
			gwalumps[i].Size=Wads.LumpLength(li+i+1);
		}
		// reopen the WAD as a standard FILE
		FILE * f_wad = fopen(name, "rb");
		if (f_wad)
		{
			bool res = gl_DoLoadGLNodes(f_wad, gwalumps);
			fclose(f_wad);
			return res;
		}
	}
	else
	{
		strcpy(path, name);

		char * ext = strrchr(path, '.');
		if (ext)
		{
			strcpy(ext, ".gwa");
			// Todo: Compare file dates

			FILE * f_gwa = fopen(path, "rb");

			if (f_gwa)
			{
				fread(&wadheader, 1, sizeof(wadheader), f_gwa);
				if (wadheader.Magic!=IWAD_ID && wadheader.Magic!=PWAD_ID)
				{
					fclose(f_gwa);
					return false;
				}
				fseek(f_gwa, wadheader.InfoTableOfs, SEEK_SET);
				for(unsigned i=0; i<wadheader.NumLumps-5;i++)
				{
					wadlump_t lump;

					fread(&lump, 1, sizeof (lump), f_gwa);
					if (!strnicmp(glname, lump.Name, 8))
					{
						fread(gwalumps, 4, sizeof(wadlump_t), f_gwa);

						bool res = gl_DoLoadGLNodes(f_gwa, gwalumps);
						fclose(f_gwa);
						return res;
					}
				}
				fclose(f_gwa);
			}
		}
	}
	return false;
}

//==========================================================================
//
// Checks whether nodes are GL friendly or not
//
//==========================================================================

void gl_CheckNodes(int lumpnum)
{
	// Save the old nodes so that R_PointInSubsector can use them
	// Unfortunately there are some screwed up WADs which can not
	// be reliably processed by the internal node builder
	// It is not necessary to keep the segs (and vertices) because they aren't used there.
	if (nodes && subsectors)
	{
		gamenodes = nodes;
		numgamenodes = numnodes;
		gamesubsectors = subsectors;
		numgamesubsectors = numsubsectors;
	}
	else
	{
		gamenodes=NULL;
	}

	if (!gl_CheckForGLNodes())
	{
		for (int i = 0; i < numsubsectors; i++)
		{
			gamesubsectors[i].sector = segs[gamesubsectors[i].firstline].sidedef->sector;
		}

		nodes = NULL;
		numnodes = 0;
		subsectors = NULL;
		numsubsectors = 0;
		if (segs) delete [] segs;
		segs = NULL;
		numsegs = 0;

		// Try to load regular GL nodes
		if (!gl_LoadGLNodes(lumpnum))
		{
			// none found - we have to build new ones!
			unsigned int startTime, endTime;

			startTime = I_MSTime ();
			TArray<FNodeBuilder::FPolyStart> polyspots, anchors;
			P_GetPolySpots (lumpnum+ML_THINGS, polyspots, anchors);
			FNodeBuilder::FLevel leveldata =
			{
				vertexes, numvertexes,
				sides, numsides,
				lines, numlines
			};
			FNodeBuilder builder (leveldata, polyspots, anchors, true);
			UsingGLNodes = true;
			delete[] vertexes;
			builder.Extract (nodes, numnodes,
				segs, numsegs,
				subsectors, numsubsectors,
				vertexes, numvertexes);
			endTime = I_MSTime ();
			DPrintf ("BSP generation took %.3f sec (%d segs)\n", (endTime - startTime) * 0.001, numsegs);
		}
	}
	if (!gamenodes)
	{
		gamenodes = nodes;
		numgamenodes = numnodes;
		gamesubsectors = subsectors;
		numgamesubsectors = numsubsectors;
	}

}
