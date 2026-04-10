/*
** t_load.cpp
** FraggleScript loader
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


#include "w_wad.h"
#include "tarray.h"
#include "g_level.h"
#include "sc_man.h"
#include "s_sound.h"
#include "r_data.h"
#include "r_sky.h"
#include "t_script.h"

enum
{
  RT_SCRIPT,
  RT_INFO,
  RT_OTHER,
} readtype;


TArray<DActorPointer*> SpawnedThings;

//-----------------------------------------------------------------------------
//
// Process the lump to strip all unneeded information from it
//
//-----------------------------------------------------------------------------
static void ParseInfoCmd(char *line)
{
	int allocsize;
	char *temp;
	
	// clear any control chars
	for(temp=line; *temp; temp++) if (*temp<32) *temp=32;

	if(readtype != RT_SCRIPT)       // not for scripts
	{
		temp = line+strlen(line)-1;

		// strip spaces at the beginning and end of the line
		while(*temp == ' ')	*temp-- = 0;
		while(*line == ' ') line++;
		
		if(!*line) return;
		
		if((line[0] == '/' && line[1] == '/') ||     // comment
			line[0] == '#' || line[0] == ';') return;
	}
	
	if(*line == '[')                // a new section seperator
	{
		line++;
		
		if(!strnicmp(line, "scripts", 7))
		{
			readtype = RT_SCRIPT;
			HasScripts = true;    // has scripts
		}
		else if (!strnicmp(line, "level info", 10))
		{
			readtype = RT_INFO;
		}
		return;
	}
	
	if (readtype==RT_SCRIPT)
	{
		allocsize = strlen(line) + strlen(levelscript.data) + 10;
		levelscript.data = (char *)realloc(levelscript.data, allocsize);
		
		// add the new line to the current data using sprintf (ugh)
		sprintf(levelscript.data, "%s%s\n", levelscript.data, line);
	}
	else if (readtype==RT_INFO)
	{
		// Read the usable parts of the level info header 
		// and ignore the rest.
		SC_OpenMem("LEVELINFO", line, strlen(line));
		SC_SetCMode(true);
		SC_MustGetString();
		if (SC_Compare("levelname"))
		{
			char * beg = strchr(line, '=')+1;
			while (*beg<' ') beg++;
			char * comm = strstr(beg, "//");
			if (comm) *comm=0;
			strncpy(level.level_name, beg, 63);
			level.level_name[63]=0;
		}
		else if (SC_Compare("partime"))
		{
			SC_MustGetString();
			SC_MustGetNumber();
			level.partime=sc_Number;
		}
		else if (SC_Compare("music"))
		{
			SC_MustGetString();
			SC_MustGetString();

			if (Wads.CheckNumForName(sc_String)<0 || !S_ChangeMusic(sc_String,true))
			{
				// Retry with D_ prepended to the music name.
				// Originally this was the only valid method but I don't want to limit myself this way!
				char buffer[12];
				sprintf(buffer, "D_%.8s", sc_String);
				buffer[8]=0;
				if (Wads.CheckNumForName(buffer)<0 || !S_ChangeMusic(buffer,true))
				{
					S_ChangeMusic(level.music, level.musicorder);
				}
			}
		}
		else if (SC_Compare("skyname"))
		{
			SC_MustGetString();
			SC_MustGetString();
		
			strncpy(level.skypic1, sc_String, 8);
			strncpy(level.skypic2, sc_String, 8);
			level.skypic1[8]=level.skypic2[8]=0;
			sky2texture = sky1texture = TexMan.GetTexture (sc_String, FTexture::TEX_Wall, FTextureManager::TEXMAN_Overridable);
			R_InitSkyMap ();
		}
		else if (SC_Compare("interpic"))
		{
			SC_MustGetString();
			SC_MustGetString();
			strncpy(level.info->exitpic, sc_String, 8);
			level.info->exitpic[8]=0;
		}
		else if (SC_Compare("gravity"))
		{
			SC_MustGetString();
			SC_MustGetNumber();
			level.gravity=sc_Number*8.f;
		}
		else if (SC_Compare("nextlevel"))
		{
			SC_MustGetString();
			SC_MustGetString();
			strncpy(level.nextmap, sc_String, 8);
			level.nextmap[8]=0;
		}
		else if (SC_Compare("nextsecret"))
		{
			SC_MustGetString();
			SC_MustGetString();
			strncpy(level.secretmap, sc_String, 8);
			level.secretmap[8]=0;
		}
		else if (SC_Compare("consolecmd"))
		{
			char * beg = strchr(line, '=')+1;
			while (*beg<' ') beg++;
			char * comm = strstr(beg, "//");
			if (comm) *comm=0;
			FS_EmulateCmd(beg);
		}
		// Ignore anything unknows
		SC_Close();
	}
}


//-----------------------------------------------------------------------------
//
// This thinker eliminates the need to call the Fragglescript functions from the main code
//
//-----------------------------------------------------------------------------
class DFraggleThinker : public DThinker
{
	DECLARE_CLASS(DFraggleThinker, DThinker)
public:

	DFraggleThinker() {}


	void Serialize(FArchive & arc)
	{
		Super::Serialize(arc);
		T_SerializeScripts(arc);
	}
	void Tick()
	{
		T_DelayedScripts();
	}
};

IMPLEMENT_CLASS(DFraggleThinker)

//-----------------------------------------------------------------------------
//
// Loads the scripts for the current map
// Initializes all FS data
//
//-----------------------------------------------------------------------------

void T_LoadLevelInfo(int lumpnum)
{
	char *lump;
	char *rover;
	char *startofline;
	int lumpsize;

	// Global initializazion if not done yet.
	static bool done=false;

	if (!done)
	{
		T_Init();
		done=true;
	}

	// Clear the old data
	for(unsigned int i=0;i<SpawnedThings.Size();i++)
	{
		delete SpawnedThings[i];
	}
	SpawnedThings.Clear();
	T_ClearScripts();
	
	// Load the script lump
	lumpsize=Wads.LumpLength(lumpnum);
	if (lumpsize==0)
	{
		// Try a global FS lump
		lumpnum=Wads.CheckNumForName("FSGLOBAL");
		if (lumpnum<0) return;
		lumpsize=Wads.LumpLength(lumpnum);
	}
	lump=new char[lumpsize+3];
	Wads.ReadLump(lumpnum,lump);
	// Append a new line. The parser likes to crash when the last character is a valid token.
	lump[lumpsize]='\n';
	lump[lumpsize+1]='\r';
	lump[lumpsize+2]=0;
	lumpsize+=2;
	
	rover = startofline = lump;
	HasScripts=false;

	readtype = RT_OTHER;
	while(rover < lump+lumpsize)
    {
		if(*rover == '\n') // end of line
		{
			*rover = 0;               // make it an end of string (0)
			ParseInfoCmd(startofline);
			startofline = rover+1; // next line
			*rover = '\n';            // back to end of line
		}
		rover++;
    }
	if (HasScripts) new DFraggleThinker;
	delete lump;
}


//-----------------------------------------------------------------------------
//
// Registers an entry in the SpawnedThings table
// If no actor is spawned it will remain NULL, otherwise T_RegisterSpawnThing
// will be called to set it
//
// This uses a DActorPointer so that it is subject to automatic pointer cleanup
//
//-----------------------------------------------------------------------------

void T_PrepareSpawnThing()
{
	if (HasScripts)
	{
		DActorPointer * acp = new DActorPointer;
		SpawnedThings.Push(acp);
		if (SpawnedThings.Size()==316)
			__asm nop
	}
}

//-----------------------------------------------------------------------------
//
// Sets the last entry in the table to the passed actor
//
//-----------------------------------------------------------------------------

void T_RegisterSpawnThing(AActor * ac)
{
	if (HasScripts)
	{
		SpawnedThings[SpawnedThings.Size()-1]->actor=ac;
	}
}