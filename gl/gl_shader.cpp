#include "gl_pch.h"
/*
** gl_shader.cpp
** Pixel shaders to apply the invulnerability colormaps.
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
#include "gl/gl_values.h"


bool have_shaders=false;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
PFNGLBINDPROGRAMARBPROC glBindProgramARB;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
PFNGLISPROGRAMARBPROC glIsProgramARB;


struct GLFragmentProgram
{
	unsigned glID;

	GLFragmentProgram(const char * string);
	bool isValid() const { return !!glIsProgramARB(glID); }
	void Bind();
	~GLFragmentProgram();
};


GLFragmentProgram::GLFragmentProgram(const char * program_string)
{
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glGenProgramsARB(1, &glID);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID);
	glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, 
						strlen(program_string), (const GLbyte *)program_string);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void GLFragmentProgram::Bind()
{
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID);
}

GLFragmentProgram::~GLFragmentProgram()
{
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDeleteProgramsARB(1, &glID);
}


GLFragmentProgram * fp_invulmap, * fp_goldmap;


const char * invulmap = 
"!!ARBfp1.0"
"OPTION ARB_precision_hint_fastest;"
"PARAM c[1] = { { 0.14, 0.30000001, 0.56, 1 } };"
"TEMP R0;"
"TEX R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R0.y, R0, c[0].z;"
"MAD R0.x, R0, c[0].y, R0.y;"
"MAD R0.x, R0.z, c[0], R0;"
"ADD result.color.xyz, -R0.x, c[0].w;"
"MUL result.color.w, fragment.color.primary, R0;"
"END";

const char * goldmap =
"!!ARBfp1.0"
"OPTION ARB_precision_hint_fastest;"
"PARAM c[2] = { { 0.14, 0.30000001, 0.56, 1.5 },"
"		{ 0 } };"
"TEMP R0;"
"TEX R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R0.y, R0, c[0].z;"
"MAD R0.x, R0, c[0].y, R0.y;"
"MAD R0.x, R0.z, c[0], R0;"
"MUL_SAT result.color.x, R0, c[0].w;"
"MOV result.color.y, R0.x;"
"MUL result.color.w, fragment.color.primary, R0;"
"MOV result.color.z, c[1].x;"
"END";

/*
const char * goldmap =
"!!ARBfp1.0"
"PARAM c[2] = { { 0.14, 0.30000001, 0.56, 1.5 },"
"		{ 0 } };"
"TEMP R0;"
"TEX R0, fragment.texcoord[0], texture[0], 2D;"
"MUL R0.y, R0, c[0].z;"
"MAD R0.x, R0, c[0].y, R0.y;"
"MAD R0.x, R0.z, c[0], R0;"
"MUL_SAT result.color.x, R0, c[0].w;"
"MOV result.color.y, R0.x;"
"MUL result.color.w, fragment.color.primary, R0;"
"MOV result.color.z, c[1].x;"
"END";
*/


void gl_InitShaders()
{
	glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
	glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
	glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
	glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC) wglGetProcAddress("glDeleteProgramsARB");
	glIsProgramARB = (PFNGLISPROGRAMARBPROC)wglGetProcAddress("glIsProgramARB");

	if (glGenProgramsARB && glBindProgramARB && glProgramStringARB && glDeleteProgramsARB) 
	{
		fp_invulmap=new GLFragmentProgram(invulmap);
		fp_goldmap=new GLFragmentProgram(goldmap);

		if (fp_invulmap->isValid() && fp_goldmap->isValid())
		{
			have_shaders=true;
		}
		else
		{
			delete fp_invulmap;
			delete fp_goldmap;
		}
	}
}


bool gl_SetShader(int cmap)
{
	static int lastcmap=-1;

	if (!have_shaders) return false;
	if (lastcmap==cmap) return true;

	switch(cmap)
	{
	default:
		lastcmap=CM_DEFAULT;
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
		return false;

	case CM_DEFAULT:
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
		break;
	case CM_INVERT:
		fp_invulmap->Bind();
		break;
	case CM_GOLDMAP:
		fp_goldmap->Bind();
		break;
	}
	lastcmap=cmap;
	return true;
}