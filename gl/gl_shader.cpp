#include "gl_pch.h"
/*
** gl_shader.cpp
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
#include "gl/gl_values.h"
#include "c_cvars.h"
#include "v_video.h"



bool gl_shaderactive;
static bool have_fp=false;
static unsigned glID_FP[2];
static GLhandleARB shader, vprog, fprog;
static GLint u_fogcolor, u_fogdensity, u_camera, u_textureenabled, u_fogenabled;


// The old fragment program code is only used to apply the invulnerability colormaps
// to camera textures

static const char * invulmap = 
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

static const char * goldmap =
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


// GLSL shaders for radial fog
// It looks great but the performance hit on a Geforce 6800 is a little high. :(

static const char * vshader=

	"varying vec4 position;"

	"void main()"
	"{"
		"gl_BackColor = gl_FrontColor = gl_Color;"
		"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
		"gl_Position = position = ftransform();"
	"}"
;

static const char * fshader=
	"varying vec4 position;"
	"uniform vec3 fogcolor;"
	"uniform float fogdensity;"
	"uniform vec3 camera;"
	"uniform sampler2D tex;"

	"void main()"
	"{"
		"vec4 texel = texture2D(tex,gl_TexCoord[0].st) * gl_Color;"

		"float factor = exp ( -fogdensity * distance(position.xyz, camera));"
		"vec3 fogged = mix (fogcolor, texel.rgb, factor);" // (fogcolor * (1.0 - factor) + texel.rgb * factor);"

		"gl_FragColor = vec4(fogged, texel.a);"
	"}"
;


static char buffer[10000];


void gl_InitShaders() 
{

	if (gl.flags & RFL_FRAGMENT_PROGRAM)
	{
		gl.Enable(GL_FRAGMENT_PROGRAM_ARB);
		gl.GenProgramsARB(2, glID_FP);

		gl.BindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID_FP[0]);
		gl.ProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, 
			strlen(invulmap), (const GLbyte *)invulmap);

		gl.BindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID_FP[1]);
		gl.ProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, 
			strlen(goldmap), (const GLbyte *)goldmap);

		if (gl.IsProgramARB(glID_FP[0]) && gl.IsProgramARB(glID_FP[1]))
		{
			have_fp=true;
		}
		else
		{
			gl.DeleteProgramsARB(2, glID_FP);
		}
		gl.Disable(GL_FRAGMENT_PROGRAM_ARB);
	}

	if (gl.flags & RFL_GLSL)
	{
		vprog = gl.CreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		fprog = gl.CreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

		gl.ShaderSourceARB(vprog, 1, &vshader,NULL);
		gl.ShaderSourceARB(fprog, 1, &fshader,NULL);

		gl.CompileShaderARB(vprog);
		gl.CompileShaderARB(fprog);

		shader = gl.CreateProgramObjectARB();

		gl.AttachObjectARB(shader,vprog);
		gl.AttachObjectARB(shader,fprog);

		gl.LinkProgramARB(shader);

		gl.GetInfoLogARB(shader, 10000, NULL, buffer);
		if (*buffer) 
		{
			Printf("Init Shaders: %s\n", buffer);
			gl.flags&=~RFL_GLSL;
			return;
		}

		u_fogcolor = gl.GetUniformLocationARB(shader,"fogcolor");
		u_fogdensity = gl.GetUniformLocationARB(shader, "fogdensity");
		u_camera = gl.GetUniformLocationARB(shader, "camera");
	}
}


static bool i_useshaders;

CUSTOM_CVAR(Bool, gl_useshaders, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
{
	if (gl.flags&RFL_GLSL)
	{
		//i_useshaders=self;
	}
}


void gl_FogColor(float *  rgb)
{
	if (i_useshaders) 
	{
		gl.Uniform3fARB(u_fogcolor, rgb[0], rgb[1], rgb[2]);
	}
	else 
	{
		gl.Fogfv(GL_FOG_COLOR, rgb);
	}
}

void gl_FogDensity(float dens)
{
	if (i_useshaders)
	{
		gl.Uniform1fARB(u_fogdensity, dens);
	}
	else if (dens!=0)
	{
		gl.Fogf(GL_FOG_DENSITY, dens);
	}
}

void gl_EnableFog(bool on)
{
	if (i_useshaders) 
	{
		if (on!=gl_shaderactive)
		{
			gl_shaderactive=on;
			gl.UseProgramObjectARB(on? shader:0);
			if (on) gl.Disable(GL_FOG);
		}
	}
	else if (on)
	{
		gl.Enable(GL_FOG);
	}
	else
	{
		gl.Disable(GL_FOG);
	}
}

void gl_SetCamera(float x, float y, float z)
{
	if (i_useshaders)
	{
		gl.Uniform4fARB(u_camera, x, y, z, 0);
	}
}

bool gl_SetColorMode(int cm, bool force)
{
	if (gl_shaderactive)
	{
		return false;
	}
	else if (have_fp)
	{
		// Only used for camera textures!
		static int lastcmap=-1;

		if (lastcmap==cm) return true;

		switch(force? cm : CM_INVALID)
		{
		default:
			cm=CM_DEFAULT;
		case CM_DEFAULT:
			gl.Disable(GL_FRAGMENT_PROGRAM_ARB);
			break;
		case CM_INVERT:
			gl.Enable(GL_FRAGMENT_PROGRAM_ARB);
			gl.BindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID_FP[0]);
			break;
		case CM_GOLDMAP:
			gl.Enable(GL_FRAGMENT_PROGRAM_ARB);
			gl.BindProgramARB(GL_FRAGMENT_PROGRAM_ARB, glID_FP[1]);
			break;
		}
		lastcmap=cm;
		return force;
	}
	return false;
}

