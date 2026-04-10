
#ifndef __GL_LIGHTS_H__
#define __GL_LIGHTS_H__

#include "gl/gl_geometric.h"
#include "gl/gl_Intern.h"
#include "gl/gl_data.h"


EXTERN_CVAR(Bool, gl_lights)
EXTERN_CVAR(Bool, gl_attachedlights)
EXTERN_CVAR(Bool, gl_bulletlight)

class ADynamicLight;
class FArchive;

typedef enum
{
   CYCLE_Linear,
   CYCLE_Sin,
   CYCLE_Cos
} CycleType;

inline FArchive &operator<< (FArchive &arc, CycleType &type)
{
	BYTE val = (BYTE)type;
	arc << val;
	type = (CycleType)val;
	return arc;
}


class FCycler
{
public:
   FCycler();
   void Update(float diff);
   void SetParams(float start, float end, float cycle);
   void ShouldCycle(bool sc);
   void SetCycleType(CycleType ct);
   float GetVal();
   
   void Serialize(FArchive & arc);
protected:
   float m_start, m_end, m_current;
   float m_time, m_cycle;
   bool m_increment, m_shouldCycle;
   CycleType m_cycleType;
};

inline FArchive &operator<< (FArchive &arc, FCycler &type)
{
	type.Serialize(arc);
	return arc;
}


enum
{
   LIGHT_RED = 0,
   LIGHT_GREEN = 1,
   LIGHT_BLUE = 2,
   LIGHT_INTENSITY = 3,
   LIGHT_SECONDARY_INTENSITY = 4,
   LIGHT_SCALE = 3,
};

// This is as good as something new - and it can be set directly in the ActorInfo!
#define MF4_SUBTRACTIVE MF4_STRIFEDAMAGE

enum ELightType
{
	PointLight,
	PulseLight,
	FlickerLight,
	RandomFlickerLight,
	SectorLight,
	SpotLight,
};


struct FLightNode
{
	FLightNode ** prevTarget;
	FLightNode * nextTarget;
	FLightNode ** prevLight;
	FLightNode * nextLight;
	ADynamicLight * lightsource;
	union
	{
		side_t * targLine;
		gl_subsectordata * targSubsector;
		void * targ;
	};
};


//
// Base class
//
// [CO] I merged everything together in this one class so that I don't have
// to create and re-create an excessive amount of objects
//
class FLightDefaults;

class ADynamicLight : public AActor
{
	DECLARE_STATELESS_ACTOR (ADynamicLight, AActor)
public:
	virtual void Tick();
	void Serialize(FArchive &arc);
	byte GetRed() const { return args[LIGHT_RED]; }
	byte GetGreen() const { return args[LIGHT_GREEN]; }
	byte GetBlue() const { return args[LIGHT_BLUE]; }
	float GetIntensity() const { return m_currentIntensity; }
	float GetRadius() const { return (IsActive() ? GetIntensity() * 2.f : 0.f); }
	void LinkLight();

	virtual void BeginPlay();
	void PostBeginPlay();
	void Destroy();
	void Activate(AActor *activator);
	void Deactivate(AActor *activator);
	void SetOffset(fixed_t x, fixed_t y, fixed_t z)
	{
		m_offX = x;
		m_offY = y;
		m_offZ = z;
	}
	void UpdateLocation();
	bool IsOwned() const { return owned; }
	bool IsActive() const { return !(flags2&MF2_DORMANT); }
	bool IsSubtractive() { return !!(flags4&MF4_SUBTRACTIVE); }
	FState *targetState;
	FLightNode * touching_sides;
	FLightNode * touching_subsectors;

private:
	float DistToSeg(seg_t *seg);
	void CollectWithinRadius(subsector_t *subSec, float radius);

protected:
	fixed_t m_offX, m_offY, m_offZ;
	float m_currentIntensity;
	int m_tickCount;
	unsigned int m_lastUpdate;
	FCycler m_cycler;
	subsector_t * subsector;
	
public:
	byte lightflags;
	byte lighttype;
	bool owned;
	bool maybevisible;
	bool halo;

	// intermediate texture coordinate data
	// this is stored in the light object to avoid recalculating it
	// several times during rendering of a flat
	Vector nearPt, up, right;
	float scale;

};

class APointLight : public ADynamicLight
{
	DECLARE_STATELESS_ACTOR (APointLight, ADynamicLight)
public:
   virtual void BeginPlay();
};

class APointLightPulse : public APointLight
{
   DECLARE_STATELESS_ACTOR (APointLightPulse, APointLight)
public:
   virtual void BeginPlay();
};

class APointLightFlicker : public APointLight
{
   DECLARE_STATELESS_ACTOR (APointLightFlicker, APointLight)
public:
   virtual void BeginPlay();
};

class APointLightFlickerRandom : public APointLight
{
   DECLARE_STATELESS_ACTOR (APointLightFlickerRandom, APointLight)
public:
   virtual void BeginPlay();
};

class ASectorPointLight : public APointLight
{
   DECLARE_STATELESS_ACTOR (ASectorPointLight, APointLight)
public:
   virtual void BeginPlay();
};


// subtractive variants
class APointLightSubtractive : public APointLight
{
	DECLARE_STATELESS_ACTOR (APointLightSubtractive, APointLight)
public:
};

class APointLightPulseSubtractive : public APointLightPulse
{
   DECLARE_STATELESS_ACTOR (APointLightPulseSubtractive, APointLightPulse)
public:
};

class APointLightFlickerSubtractive : public APointLightFlicker
{
   DECLARE_STATELESS_ACTOR (APointLightFlickerSubtractive, APointLightFlicker)
public:
};

class APointLightFlickerRandomSubtractive : public APointLightFlickerRandom
{
   DECLARE_STATELESS_ACTOR (APointLightFlickerRandomSubtractive, APointLightFlickerRandom)
public:
};

class ASectorPointLightSubtractive : public ASectorPointLight
{
   DECLARE_STATELESS_ACTOR (ASectorPointLightSubtractive, ASectorPointLight)
public:
};

class AVavoomLight : public APointLight
{
   DECLARE_STATELESS_ACTOR (AVavoomLight, APointLight)
public:
   virtual void BeginPlay();
};

class AVavoomLightWhite : public AVavoomLight
{
   DECLARE_STATELESS_ACTOR (AVavoomLightWhite, AVavoomLight)
public:
   virtual void BeginPlay();
};

class AVavoomLightColor : public AVavoomLight
{
   DECLARE_STATELESS_ACTOR (AVavoomLightColor, AVavoomLight)
public:
   virtual void BeginPlay();
};


enum
{
	STAT_DLIGHT=64
};

extern TArray<ADynamicLight *> PointLights;



//
// Light helper methods
//


extern unsigned int frameStartMS;

bool gl_SetupLight(Plane & p, ADynamicLight * light, Vector & nearPt, Vector & up, Vector & right, float & scale, bool additive, int desaturation, bool checkside=true);
bool gl_SetupLightTexture();
void gl_GetLightForThing(AActor * thing, float upper, float lower, float & r, float & g, float & b);


#endif