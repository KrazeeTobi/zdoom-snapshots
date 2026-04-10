
#ifndef __GL_DATA_H
#define __GL_DATA_H


#include "r_defs.h"
#include "gl/gl_basic.h"
#include "gl/gl_Intern.h"


side_t* getNextSide(sector_t * sec, line_t* line);



//==========================================================================
//
// Subsector data
//
//==========================================================================
struct gl_subsectordata
{
	FLightNode *				lighthead;
	sector_t *					render_sector;
	subsector_t *				sub;
	int							firstvertex;	// index into the gl_vertices array
	int							numvertices;
	int							validcount;
	fixed_t						bbox[4];
	int							hacked;			// 1: is part of a render hack
												// 2: is neighboring only subsectors which are part of a render hack
};

//==========================================================================
//
// these are used to link faked planes due to missing textures to a sector
//
//==========================================================================
struct gl_subsectorrendernode
{
	gl_subsectorrendernode *	next;
	gl_subsectordata *			glsub;
};

//==========================================================================
//
// Sector data
//
//==========================================================================
struct gl_sectordata
{
	fixed_t						transdoorheight;
	bool						transdoor;
	byte						renderflags;
	int							subsectorcount;
	gl_subsectordata		**	gl_subsectors;
	gl_subsectorrendernode	*	otherplanes[2];

};

extern gl_subsectordata *		gl_subsectors;
extern gl_sectordata *			gl_sectors;
extern byte	*					gl_ss_renderflags;

extern FreeList<gl_subsectorrendernode> SSR_List;


#endif