/* Emacs style mode select   -*- C++ -*- 
 *-----------------------------------------------------------------------------
 *
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *
 *---------------------------------------------------------------------
 */

#ifndef _GL_INTERN_H
#define _GL_INTERN_H

#include "r_defs.h"
#include "c_cvars.h"

#pragma warning(disable:4244)

EXTERN_CVAR(Bool,gl_enhanced_lightamp)
EXTERN_CVAR(Int, screenblocks);
EXTERN_CVAR(Bool, gl_texture)
EXTERN_CVAR(Int, gl_texture_filter)
EXTERN_CVAR(Float, gl_texture_filter_anisotropic)
EXTERN_CVAR(Int, gl_texture_format)

EXTERN_CVAR (Bool, gl_lights);
EXTERN_CVAR (Bool, gl_attachedlights);
EXTERN_CVAR (Bool, gl_lights_checkside);
EXTERN_CVAR (Float, gl_lights_intensity);
EXTERN_CVAR (Float, gl_lights_size);
EXTERN_CVAR (Bool, gl_light_sprites);
EXTERN_CVAR (Bool, gl_light_particles);

EXTERN_CVAR(Bool, gl_depthfog)
EXTERN_CVAR(Bool,gl_mirror_envmap)

extern float pitch;
extern float viewvecX,viewvecY;

extern int rendered_lines,rendered_flats,rendered_sprites,rendered_decals,render_vertexsplit,render_texsplit;
extern int iter_dlightf, iter_dlight, draw_dlight, draw_dlightf;
extern DWORD gl_fixedcolormap;
extern int palette_brightness;
extern AActor * viewactor;
extern bool gl_shaderactive;

typedef enum
{
        area_normal,
        area_below,
        area_above,
		area_default
} area_t;

extern area_t			in_area;


#endif // _GL_INTERN_H
