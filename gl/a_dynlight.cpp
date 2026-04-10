#include "gl_pch.h"

/*
** a_dynlight.cpp
** Implements actors representing dynamic lights (hardware independent)
**
**---------------------------------------------------------------------------
** Copyright 2003 Timothy Stump
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

#include "templates.h"
#include "m_random.h"
#include "r_main.h"
#include "p_local.h"
#include "c_dispatch.h"
#include "gl/gl_lights.h"
#include "gl/gl_data.h"
#include "gl/gl_basic.h"

#define ANGLE_TO_FLOAT(ang) ((float)(ang * 180.0f / ANGLE_180))


EXTERN_CVAR (Float, gl_lights_size);

//==========================================================================
//
// Actor classes
//
// For flexibility all functionality has been packed into a single class
// which is controlled by flags
//
//==========================================================================
IMPLEMENT_STATELESS_ACTOR (ADynamicLight, Any, -1, 0)
   PROP_HeightFixed (0)
   PROP_Flags (MF_NOBLOCKMAP|MF_NOGRAVITY)
   PROP_RenderFlags (RF_INVISIBLE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (APointLight, Any, 9800, 0)
END_DEFAULTS

void APointLight::BeginPlay ()
{
	Super::BeginPlay();
	lighttype = PointLight;
}

IMPLEMENT_STATELESS_ACTOR (APointLightPulse, Any, 9801, 0)
END_DEFAULTS

void APointLightPulse::BeginPlay ()
{
	Super::BeginPlay();
	lighttype = PulseLight;
}

IMPLEMENT_STATELESS_ACTOR (APointLightFlicker, Any, 9802, 0)
END_DEFAULTS

void APointLightFlicker::BeginPlay ()
{
	Super::BeginPlay();
	lighttype = FlickerLight;
}

IMPLEMENT_STATELESS_ACTOR (ASectorPointLight, Any, 9803, 0)
END_DEFAULTS

void ASectorPointLight::BeginPlay ()
{
	Super::BeginPlay();
	lighttype = SectorLight;
}

IMPLEMENT_STATELESS_ACTOR (APointLightFlickerRandom, Any, 9804, 0)
END_DEFAULTS

void APointLightFlickerRandom::BeginPlay ()
{
	Super::BeginPlay();
	lighttype=RandomFlickerLight;
}


//IMPLEMENT_STATELESS_ACTOR (ASpotLight, Any, 9850, 0)
//END_DEFAULTS

//IMPLEMENT_STATELESS_ACTOR (ASpotTarget, Any, 9851, 0)
//END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (APointLightSubtractive, Any, 9820, 0)
   PROP_Flags4 (MF4_SUBTRACTIVE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (APointLightPulseSubtractive, Any, 9821, 0)
   PROP_Flags4 (MF4_SUBTRACTIVE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (APointLightFlickerSubtractive, Any, 9822, 0)
   PROP_Flags4 (MF4_SUBTRACTIVE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (ASectorPointLightSubtractive, Any, 9823, 0)
   PROP_Flags4 (MF4_SUBTRACTIVE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (APointLightFlickerRandomSubtractive, Any, 9824, 0)
   PROP_Flags4 (MF4_SUBTRACTIVE)
END_DEFAULTS

IMPLEMENT_STATELESS_ACTOR (AVavoomLight, Any, 9825, 0)
END_DEFAULTS

void AVavoomLight::BeginPlay ()
{
	Super::BeginPlay();
	if (Sector) z -= Sector->floorplane.ZatPoint(x, y);
}

IMPLEMENT_STATELESS_ACTOR (AVavoomLightWhite, Any, 1502, 0)
END_DEFAULTS

void AVavoomLightWhite::BeginPlay ()
{
	byte l_args[5];
	memcpy(l_args, args, 5);
	args[LIGHT_INTENSITY] = l_args[0]*4;
	args[LIGHT_RED] = 128;
	args[LIGHT_GREEN] = 128;
	args[LIGHT_BLUE] = 128;

	Super::BeginPlay();
}

IMPLEMENT_STATELESS_ACTOR (AVavoomLightColor, Any, 1503, 0)
END_DEFAULTS

void AVavoomLightColor::BeginPlay ()
{
	byte l_args[5];
	memcpy(l_args, args, 5);
	args[LIGHT_INTENSITY] = l_args[0]*4;
	args[LIGHT_RED] = l_args[1] >> 1;
	args[LIGHT_GREEN] = l_args[2] >> 1;
	args[LIGHT_BLUE] = l_args[3] >> 1;

	Super::BeginPlay();
}

static FRandom randLight;

//==========================================================================
//
// The cycler for the pulsing light
//
//==========================================================================


//==========================================================================
//
//
//
//==========================================================================

FCycler::FCycler()
{
	m_cycle = 0.f;
	m_cycleType = CYCLE_Linear;
	m_shouldCycle = false;
	m_start = m_current = 0.f;
	m_end = 0.f;
	m_increment = true;
}


//==========================================================================
//
//
//
//==========================================================================

void FCycler::SetParams(float start, float end, float cycle)
{
	m_cycle = cycle;
	m_time = 0.f;
	m_start = m_current = start;
	m_end = end;
	m_increment = true;
}


//==========================================================================
//
//
//
//==========================================================================

void FCycler::Update(float diff)
{
	float mult, angle;
	float step = m_end - m_start;
	
	if (!m_shouldCycle)
	{
		return;
	}
	
	m_time += diff;
	if (m_time >= m_cycle)
	{
		m_time = m_cycle;
	}
	
	mult = m_time / m_cycle;
	
	switch (m_cycleType)
	{
	case CYCLE_Linear:
		if (m_increment)
		{
			m_current = m_start + (step * mult);
		}
		else
		{
			m_current = m_end - (step * mult);
		}
		break;
	case CYCLE_Sin:
		angle = M_PI * 2.f * mult;
		mult = sinf(angle);
		mult = (mult + 1.f) / 2.f;
		m_current = m_start + (step * mult);
		break;
	case CYCLE_Cos:
		angle = M_PI * 2.f * mult;
		mult = cosf(angle);
		mult = (mult + 1.f) / 2.f;
		m_current = m_start + (step * mult);
		break;
	}
	
	if (m_time == m_cycle)
	{
		m_time = 0.f;
		m_increment = !m_increment;
	}
}


//==========================================================================
//
//
//
//==========================================================================

float FCycler::GetVal()
{
	return m_current;
}


//==========================================================================
//
//
//
//==========================================================================

void FCycler::ShouldCycle(bool sc)
{
	m_shouldCycle = sc;
}


//==========================================================================
//
//
//
//==========================================================================

void FCycler::SetCycleType(CycleType ct)
{
	m_cycleType = ct;
}


//==========================================================================
//
// Base class
//
//==========================================================================

//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::Serialize(FArchive &arc)
{
	Super::Serialize (arc);
	arc << lightflags << lighttype;

	if (arc.IsLoading() && lighttype == PulseLight)
	{
		float pulseTime = ANGLE_TO_FLOAT(this->angle) / TICRATE;
		m_lastUpdate = gametic;
		m_cycler.SetParams(args[LIGHT_SECONDARY_INTENSITY], args[LIGHT_INTENSITY], pulseTime);
		m_cycler.ShouldCycle(true);
		m_cycler.SetCycleType(CYCLE_Sin);
		m_currentIntensity = m_cycler.GetVal();
	}
	if (arc.IsLoading()) LinkLight();

}


//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::BeginPlay()
{
	Super::BeginPlay();
	ChangeStatNum(STAT_DLIGHT);

	if (args[LIGHT_RED]==0 && args[LIGHT_GREEN]==0 && args[LIGHT_BLUE]==0)
	{
		// Hackhack! ;)
		PalEntry pe = GetClass()->Meta.GetMetaInt(AMETA_BloodColor);
		args[LIGHT_RED] = pe.r;
		args[LIGHT_GREEN] = pe.g;
		args[LIGHT_BLUE] = pe.b;
		args[LIGHT_INTENSITY] = (byte)(radius>>FRACBITS);
	}
	radius=0;
}

//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::PostBeginPlay()
{
	Super::PostBeginPlay();
	
	if (!(SpawnFlags & MTF_DORMANT))
	{
		Activate (NULL);
	}

	m_currentIntensity = args[LIGHT_INTENSITY];
	m_tickCount = 0;

	if (lighttype == PulseLight)
	{
		float pulseTime = ANGLE_TO_FLOAT(this->angle) / TICRATE;
		
		m_lastUpdate = gametic;
		m_cycler.SetParams(args[LIGHT_SECONDARY_INTENSITY], args[LIGHT_INTENSITY], pulseTime);
		m_cycler.ShouldCycle(true);
		m_cycler.SetCycleType(CYCLE_Sin);
		m_currentIntensity = (byte)m_cycler.GetVal();
	}

	subsector = R_PointInSubsector2(x,y);
}


//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::Activate(AActor *activator)
{
	//Super::Activate(activator);
	flags2&=~MF2_DORMANT;	
}


//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::Deactivate(AActor *activator)
{
	//Super::Deactivate(activator);
	flags2|=MF2_DORMANT;	
}


//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::Tick()
{

	if (IsOwned())
	{
		if (!target || !target->state)
		{
			this->Destroy();
			return;
		}
	}

	// Don't bother if the light won't be shown
	if (!IsActive()) return;

	// I am doing this with a type field so that I can dynamically alter the type of light
	// without having to create or maintain multiple objects.
	switch(lighttype)
	{
	case PulseLight:
	{
		float diff = (gametic - m_lastUpdate) / (float)TICRATE;
		
		m_lastUpdate = gametic;
		m_cycler.Update(diff);
		m_currentIntensity = m_cycler.GetVal();
		break;
	}

	case RandomFlickerLight:
	{
		byte rnd = randLight();
		float pct = this->angle * 1.f / ANGLE_MAX;
		
		if (rnd >= pct * 255)
		{
			m_currentIntensity = args[LIGHT_SECONDARY_INTENSITY];
		}
		else
		{
			m_currentIntensity = args[LIGHT_INTENSITY];
		}
		break;
	}

	case FlickerLight:
	{
		byte flickerRange = args[LIGHT_SECONDARY_INTENSITY] - args[LIGHT_INTENSITY];
		float amt = randLight() / 255.f;
		
		m_tickCount++;
		
		if (m_tickCount > (angle / ANGLE_1))
		{
			m_currentIntensity = args[LIGHT_INTENSITY] + (amt * flickerRange);
			m_tickCount = 0;
		}
		break;
	}

	case SectorLight:
	{
		float intensity;
		float scale = args[LIGHT_SCALE] / 8.f;
		
		if (scale == 0.f) scale = 1.f;
		
		intensity = Sector->lightlevel * scale;
		intensity = clamp<float>(intensity, 0.f, 255.f);
		
		m_currentIntensity = intensity;
		break;
	}

	case PointLight:
		m_currentIntensity = args[LIGHT_INTENSITY];
		break;
	}

	UpdateLocation();
}




//==========================================================================
//
//
//
//==========================================================================
void ADynamicLight::UpdateLocation()
{
	fixed_t oldx=x;
	fixed_t oldy=y;
	fixed_t oldradius=radius;
	float intensity;

	if (IsActive())
	{
		if (target)
		{
			PrevX = x = target->x + m_offX;
			PrevY = y = target->y + m_offZ;
			PrevZ = z = target->z + m_offY;
			subsector = R_PointInSubsector2(x, y);
			Sector = subsector->sector;
		}


		// The radius being used here is always the maximum possible with the
		// current settings. This avoids constant relinking of flickering lights

		if (lighttype == FlickerLight) 
		{
			intensity = args[LIGHT_SECONDARY_INTENSITY];
		}
		else
		{
			intensity = m_currentIntensity;
		}
		radius = (fixed_t)(intensity * 2.0f * gl_lights_size * FRACUNIT);

		if (x!=oldx || y!=oldy || radius!=oldradius) 
		{
			//Update the light lists
			LinkLight();
		}
	}
}



//=============================================================================
//
// These have been copied from the secnode code and modified for the light links
//
// P_AddSecnode() searches the current list to see if this sector is
// already there. If not, it adds a sector node at the head of the list of
// sectors this object appears in. This is called when creating a list of
// nodes that will get linked in later. Returns a pointer to the new node.
//
//=============================================================================
static FreeList<FLightNode> freelist;

FLightNode * AddLightNode(FLightNode ** thread, void * linkto, ADynamicLight * light, FLightNode *& nextnode)
{
	FLightNode * node;

	node = nextnode;
	while (node)
    {
		if (node->targ==linkto)   // Already have a node for this sector?
		{
			node->lightsource = light; // Yes. Setting m_thing says 'keep it'.
			return(nextnode);
		}
		node = node->nextTarget;
    }

	// Couldn't find an existing node for this sector. Add one at the head
	// of the list.
	
	node = freelist.GetNew();
	
	node->targ = linkto;
	node->lightsource = light; 

	node->prevTarget = &nextnode; 
	node->nextTarget = nextnode;

	if (nextnode) nextnode->prevTarget = &node->nextTarget;
	
	// Add new node at head of sector thread starting at s->touching_thinglist
	
	node->prevLight = thread;  	
	node->nextLight = *thread; 
	if (node->nextLight) node->nextLight->prevLight=&node->nextLight;
	*thread = node;
	return(node);
}


//=============================================================================
//
// P_DelSecnode() deletes a sector node from the list of
// sectors this object appears in. Returns a pointer to the next node
// on the linked list, or NULL.
//
//=============================================================================

static FLightNode * DeleteLightNode(FLightNode * node)
{
	FLightNode * tn;  // next node on thing thread
	
	if (node)
    {
		
		*node->prevTarget = node->nextTarget;
		if (node->nextTarget) node->nextTarget->prevTarget=node->prevTarget;

		*node->prevLight = node->nextLight;
		if (node->nextLight) node->nextLight->prevLight=node->prevLight;
		
		// Return this node to the freelist
		tn=node->nextTarget;
		freelist.Release(node);
		return(tn);
    }
	return(NULL);
}                             // phares 3/13/98



#define FIX2FLT(v) ((v)/65536.0f)

//==========================================================================
//
// Gets the light's distance to a line
//
//==========================================================================

float ADynamicLight::DistToSeg(seg_t *seg)
{
   float u, px, py;

   float seg_dx = FIX2FLT(seg->v2->x - seg->v1->x);
   float seg_dy = FIX2FLT(seg->v2->y - seg->v1->y);
   float seg_length_sq = seg_dx * seg_dx + seg_dy * seg_dy;

   u = (  FIX2FLT(x - seg->v1->x) * seg_dx + FIX2FLT(y - seg->v1->y) * seg_dy) / seg_length_sq;
   if (u < 0.f) u = 0.f; // clamp the test point to the line segment
   if (u > 1.f) u = 1.f;

   px = FIX2FLT(seg->v1->x) + (u * seg_dx);
   py = FIX2FLT(seg->v1->y) + (u * seg_dy);

   px -= FIX2FLT(x);
   py -= FIX2FLT(y);

   return (px*px) + (py*py);
}


//==========================================================================
//
// Collect all touched sidedefs and subsectors
// to sidedefs and sector parts.
//
//==========================================================================

void ADynamicLight::CollectWithinRadius(subsector_t *subSec, float radius)
{
	if (!subSec) return;

	subSec->validcount = ::validcount;

	gl_subsectordata * glsub=&gl_subsectors[subSec-subsectors];
	touching_subsectors = AddLightNode(&glsub->lighthead, glsub, this, touching_subsectors);

	for (unsigned int i = 0; i < subSec->numlines; i++)
	{
		seg_t * seg = segs + subSec->firstline + i;

		if (seg->sidedef && seg->linedef && seg->linedef->validcount!=::validcount)
		{
			// light is in front of the seg
			if (DMulScale32 (y-seg->v1->y, seg->v2->x-seg->v1->x, seg->v1->x-x, seg->v2->y-seg->v1->y) <=0)
			{
				seg->linedef->validcount=validcount;
				touching_sides = AddLightNode(&seg->sidedef->lighthead, seg->sidedef, this, touching_sides);
			}
		}

		if (seg->PartnerSeg && seg->PartnerSeg->Subsector->validcount!=::validcount)
		{
			// check distance from x/y to seg and if within radius add PartnerSeg->Subsector (lather/rinse/repeat)
			if (DistToSeg(seg) <= radius)
			{
				CollectWithinRadius(seg->PartnerSeg->Subsector, radius);
			}
		}
	}
}

//==========================================================================
//
// Link the light into the world
//
//==========================================================================

void ADynamicLight::LinkLight()
{
	// mark the old light nodes
	FLightNode * node;
	
	node = touching_sides;
	while (node)
    {
		node->lightsource = NULL;
		node = node->nextTarget;
    }
	node = touching_subsectors;
	while (node)
    {
		node->lightsource = NULL;
		node = node->nextTarget;
    }

	if (radius>0)
	{
		// passing in radius*radius allows us to do a distance check without any calls to sqrtf
		::validcount++;
		subsector_t * subSec = R_PointInSubsector2(x, y);
		if (subSec)
		{
			float fradius = radius/65536.0f;
			CollectWithinRadius(subSec, fradius*fradius);
		}
	}
		
	// Now delete any nodes that won't be used. These are the ones where
	// m_thing is still NULL.
	
	node = touching_sides;
	while (node)
	{
		if (node->lightsource == NULL)
		{
			node = DeleteLightNode(node);
		}
		else
			node = node->nextTarget;
	}

	node = touching_subsectors;
	while (node)
	{
		if (node->lightsource == NULL)
		{
			node = DeleteLightNode(node);
		}
		else
			node = node->nextTarget;
	}
}


//==========================================================================
//
// Deletes the link lists
//
//==========================================================================

void ADynamicLight::Destroy()

{
	while (touching_sides) touching_sides = DeleteLightNode(touching_sides);
	while (touching_subsectors) touching_subsectors = DeleteLightNode(touching_subsectors);
	Super::Destroy();
}



CCMD(listlights)
{
	int walls, sectors;
	int allwalls=0, allsectors=0;
	int i=0;
	ADynamicLight * dl;
	TThinkerIterator<ADynamicLight> it;

	while (dl=it.Next())
	{
		walls=0;
		sectors=0;
		Printf("%s at (%d, %d, %d), color = 0x%02x%02x%02x, radius = %d ",
			dl->target? dl->target->GetName() : dl->GetName(),
			dl->x>>16, dl->y>>16, dl->z>>16, dl->args[LIGHT_RED], 
			dl->args[LIGHT_GREEN], dl->args[LIGHT_BLUE], dl->radius>>16);
		i++;

		FLightNode * node;

		node=dl->touching_sides;

		while (node)
		{
			walls++;
			allwalls++;
			node = node->nextTarget;
		}

		node=dl->touching_subsectors;

		while (node)
		{
			allsectors++;
			sectors++;
			node = node->nextTarget;
		}

		Printf("- %d walls, %d subsectors\n", walls, sectors);

	}
	Printf("%i dynamic lights, %d walls, %d subsectors\n\n\n", i, allwalls, allsectors);
}