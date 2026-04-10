/*
** gl_renderstruct.h
**   Generalized portal maintenance classes to make rendering special effects easier
**   and help add future extensions
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

#ifndef __GL_PORTAL_H
#define __GL_PORTAL_H

#include "tarray.h"
#include "gl/gl_intern.h"
#include "gl/gl_renderstruct.h"

struct skycube_t
{
	int textures[6];
};

class GLUtesselator;


class GLPortal
{
	static TArray<GLPortal *> portals;
	static int recursion;
protected:
	static int MirrorFlag;
	static int PlaneMirrorFlag;
	static int renderdepth;

public:
	static int PlaneMirrorMode;
	static line_t * mirrorline;
	static 	bool inupperstack;
	static bool	inlowerstack;
	static bool	inskybox;

private:
	void DrawPortalStencil();

	fixed_t savedviewx;
	fixed_t savedviewy;
	fixed_t savedviewz;
	angle_t savedviewangle;
	AActor * savedviewactor;
	area_t savedviewarea;

protected:
	TArray<GLWall> lines;
	int level;
	GLDrawList drawinfo[GLDL_TYPES];

	GLPortal() { portals.Push(this); }
	virtual ~GLPortal() { }

	void Start(bool usestencil);
	void End(bool usestencil);
	virtual void DrawContents()=0;
	virtual void * GetSource() const =0;	// GetSource MUST be implemented!
	void ClearClipper();
	virtual bool IsSky() { return false; }

public:
	void RenderPortal(bool usestencil=true)
	{
		Start(usestencil);
		DrawContents();
		End(usestencil);
	}

	void AddLine(GLWall * l)
	{
		lines.Push(*l);
	}

	static int GetRecursion()
	{
		return recursion;
	}


	static void StartFrame();
	static bool RenderFirstSkyPortal();
	static void EndFrame();
	static GLPortal * FindPortal(const void * src);
};


struct GLMirrorPortal : public GLPortal
{
	// mirror portals always consist of single linedefs!
	line_t * linedef;

protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return linedef; }

public:
	
	GLMirrorPortal(line_t * line)
	{
		linedef=line;
	}

};


struct GLSkyboxPortal : public GLPortal
{
	AActor * origin;

protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return origin; }
//	virtual bool IsSky() { return true; } later!

public:

	
	GLSkyboxPortal(AActor * pt)
	{
		origin=pt;
	}

};


struct GLSkyPortal : public GLPortal
{
	GLSkyInfo * origin;

protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return origin; }
	virtual bool IsSky() { return true; }

public:

	
	GLSkyPortal(GLSkyInfo *  pt)
	{
		origin=pt;
	}

};



struct GLSectorStackPortal : public GLPortal
{
protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return origin; }
	GLSectorStackInfo * origin;

public:
	
	GLSectorStackPortal(GLSectorStackInfo * pt) 
	{
		origin=pt;
	}

};

struct GLPlaneMirrorPortal : public GLPortal
{
protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return origin; }
	secplane_t * origin;

public:

	GLPlaneMirrorPortal(secplane_t * pt) 
	{
		origin=pt;
	}

};


struct GLHorizonPortal : public GLPortal
{
	GLHorizonInfo * origin;

protected:
	virtual void DrawContents();
	virtual void * GetSource() const { return origin; }

public:
	
	GLHorizonPortal(GLHorizonInfo * pt)
	{
		origin=pt;
	}

};


#endif