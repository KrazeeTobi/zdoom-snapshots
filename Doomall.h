#ifdef _WIN32
//#define __RPCNDR_H__		// this header causes problems!
//#define __wtypes_h__
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINDOWS 0x410
#define _WIN32_WINNT 0x0501			// Support the mouse wheel and session notification.
#define _WIN32_IE 0x0500
#define DIRECTINPUT_VERSION 0x800
#define DIRECTDRAW_VERSION 0x0300
#include <windows.h>
#include <mmsystem.h>
#include <winsock.h>
#include <dshow.h>
//#include <d3dx9.h>
#include <dsound.h>
#include <dinput.h>
#include <lmcons.h>
#include <shlobj.h>
#endif
#ifndef CALLBACK
#define CALLBACK
#endif
#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <malloc.h>
#include <time.h>

#ifdef _MSC_VER
#define    F_OK    0    /* Check for file existence */
#define    W_OK    2    /* Check for write permission */
#define    R_OK    4    /* Check for read permission */
#include <io.h>
#else
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fmod.h>
#include <FLAC++/decoder.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <gl/wglext.h>

#ifdef LoadMenu
#undef LoadMenu
#endif
#ifdef DrawText
#undef DrawText
#endif
#ifdef GetCharWidth
#undef GetCharWidth
#endif

#include "zlib/zlib.h"

#undef S_NORMAL
#undef OPAQUE


#include "m_alloc.h"

inline void Free(void * block)
{
	free(block);
}

#include "templates.h"
#include "m_fixed.h"
#include "colormatcher.h"

#include "files.h"

// these include files don't depend on anything else!
#include "version.h"
#include "tarray.h"
#include "doomdef.h"
#include "d_event.h"
#include "win32/i_input.h"
#include "gstrings.h"
#include "m_bbox.h"

// these do!
#include "configfile.h"
#include "gameconfigfile.h"
#include "gi.h"
#include "m_swap.h"
#include "d_protocol.h"
#include "win32/i_net.h"
#include "win32/i_system.h"

#ifdef _MSC_VER
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA

#pragma warning(disable : 4018)     // signed/unsigned mismatch
#pragma warning(disable : 4305)     // truncate from double to float
#endif

class FPlayerSkin;
struct model_t;
struct subsector_s;

#include "res_colormap.h"
#include "farchive.h"
#include "doomdata.h"
#include "info.h"
#include "doomstat.h"

#include "p_conversation.h"
#include "p_pspr.h"
#include "d_netinf.h"
#include "r_main.h"

#include "p_local.h"
#include "actor.h"
#include "d_player.h"


#include "g_shared/a_sharedglobal.h"

// later!
#include "g_shared/a_pickups.h"
#include "g_shared/a_artifacts.h"

#pragma warning(disable:4530)


//#define NO_PERF

#ifndef DWORD_MAX
#define DWORD_MAX     0xffffffffUL  /* maximum unsigned long value */
#endif
