#include "gl_pch.h"
/*
** gl_main.cpp
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

#include "c_dispatch.h"
#include "gl/gl_data.h"
#include "gl/gltexture.h"
#include "gl/gl_functions.h"
#include "gl/gl_renderstruct.h"
#include "gl/gl_texture.h"

//==========================================================================
//
//
//
//==========================================================================

void gl_SetVSync(bool self)
{
	if (gl.SetVSync!=NULL) gl.SetVSync(self);
}

//==========================================================================
//
//
//
//==========================================================================
void gl_Set2DMode()
{
	gl_SetColorMode(CM_DEFAULT);	// no colormap translations in 3D mode!
	gl.MatrixMode(GL_MODELVIEW);
	gl.LoadIdentity();
	gl.MatrixMode(GL_PROJECTION);
	gl.LoadIdentity();
	gl.Ortho(
		(GLdouble) 0,
		(GLdouble) SCREENWIDTH, 
		(GLdouble) SCREENHEIGHT, 
		(GLdouble) 0,
		(GLdouble) -1.0, 
		(GLdouble) 1.0 
		);
	gl.Disable(GL_DEPTH_TEST);
}


AT_GAME_SET(OpenGL)
{
	const TypeInfo * ti = TypeInfo::FindType("WallTorch");
	GetDefaultByType(ti)->radius = 3*FRACUNIT;
}