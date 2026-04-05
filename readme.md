# ZDoom dev build recreation
intended date: December 9th, 2005

## Requirements
* [NASM 0.98.39](https://sourceforge.net/projects/nasm/files/Win32%20binaries/0.98.39/nasm-0.98.39-win32.zip/download)
* [FMOD 3.75 Programmers API](https://web.archive.org/web/20061016195047/http://www.fmod.org/files/fmodapi375win.zip)
* [DirectX SDK (December 2005)](https://archive.org/details/dxsdk_dec2005)
* Microsoft Visual C++ .NET 2003
 
## Environment setup
1. Extract NASM and FMOD into a location (remember where they are!). 2. Install both Visual C++ .NET 2003 and the DirectX SDK.
3. Open up Visual C++, and go to Tools > Options. Scroll down and click "Projects", and then "VC++ Directories". Make sure the Platform is Win32 and the "Show directories for" is set to "Executable files". Add the NASM root directory.
4. After doing that, go to "Show directories for" and select "Include files". Add both the DirectX SDK (DXSDKLOCATION/Include) and FMOD (FMODLOCATION/api/inc). 
5. After doing that, go to "Show directories for" and select "Library files". Add both the DirectX SDK (DXSDKLOCATION/Lib/x86) and FMOD (FMODLOCATION/api/lib). 
 
## Building ZDoom
Before compiling, go to Build > Configuration Manager, and change "Active Solution Configuration" from "Debug" to "Release". Afterwards, go to Build > Build Solution. You might get a fair few warnings but should compile the following:
<table style="border-collapse: collapse; width: 100%; height: 72px; color: #ffffff" border="0">
<tbody>
<tr style="color: #FFD800;" "height: 18px;">
<td style="width: 20%; height: 18px;" align="center">C:\zdoom.exe</td>
<td style="width: 20%; height: 18px;" align="left">ZDoom executable</td>
</tr>
<tr style="color: #FFD800;" "height: 18px;">
<td style="width: 20%; height: 18px;" align="center">C:\zdoom.wad</td>
<td style="width: 20%; height: 18px;" align="left">Supporting ZDoom data</td>
</tr>
</table>

## To-do
### 20051107
・Fixed: Although bit 27 of the actor's flag is now used for something different than in Doom, it's okay to specify it by number in a Dehacked patch, because it's used to set the color translation. When the translation is set, it will be 0'ed then.
<br>・Fixed: FActorInfo::ApplyDefaults() should initialize datastr to NULL.
<br>・Fixed: TArray::Delete's parameter should be unsigned.
<br>・Fixed: The fix for rotating polyobjects broke polyobject doors.
<br>・Fixed: Using the IDBEHOLDS cheat did not give you full health.
<br>・Backported the fix for scrolling Heretic/Hexen/Strife specials from 2.1.0.
<br>・Increased KneelingGuy's height to 17 so that you cannot step on top of the projector without jumping.
<br>・Added a confirmation screen for the menu choices that reset your options.
<br>・Added a new automap menu underneath the display options menu. Moved all the automap options into it and added a few new ones.
### 20051108
・Fixed: APowerStrength::GetBlend()'s equation did not match Doom's.
<br>・Fixed: If you have weapon sprites in a wad and try to use a frame from that sprite that does not exist, the game accesses might crash. Fixing this involves two things: Make the range checks in R_DrawPSprite() unconditional, so they exist in both debug and release builds. Also add a check for the Null texture to catch "holes" in the available frames.
<br>・Fixed: Setmodeneeded is always true before starting a game, because it only gets cleared when the StatusBar is around. The most obvious manifestation of this problem is that you cannot move left and right in the video modes menu before starting a game.
<br>・Fixed: The "give all" cheat did not give a backpack, and the non-console equivalents always gave you a backpack, even if you weren't playing Doom.
<br>・Fixed: The "give armor" cheat gave you armor with a SavePercent of .5 in Heretic. This would be correct for the others games, but not for Heretic, where .5 is what the lesser armor provides.
### 20051113
・Fixed: The user-configurable BOOM startup strings were always printed, even if they were empty.
### 20051114
・Fixed: ClientObituary() crashed if the killing player didn't have a weapon.
<br>・Fixed: The Touch functions assumed the toucher was a valid player and crashed if it wasn't.
<br>・Fixed: APlayerPawn::Die() assumed the actor was always associated with a player and crashed if it wasn't and you had weapon dropping turned on.
<br>・Fixed: There were two pfile variables defined in R_InitTextures().
### 20051117
・Fixed: Various Strife actors had MF_COUNTKILL that didn't need it: Beggar, KneelingGuy, Macil1, RatBuddy, and AcolyteToBe.
<br>・Fixed: PowerMask and PowerShadow both need the IF_HUBPOWER flag.
<br>・Fixed: The Strife quest log should be initialized with the text "Find help".
### 20051118
・Fixed: You could create a chain reaction where merchants would continually enter their pain state because there was a delay between the time when they alerted and the time they alerted others.
<br>・Fixed: If the Inquisitor died while flying, it would continue to play the flying sound.<br>・Fixed: The LEGO cheat didn't check if the Sigil was successfully added and would set the player to the NULL weapon if it isn't (which happens with the shareware).
<br>・Fixed: Macil was alerted by water splashing.
<br>・Reworded some of the compatibility options menu items so they fit on the screen when playing Strife.
<br>・Fixed: The Oracle's Death checked the wrong quest item to determine if you had already killed Macil.
### 20051122
・Fixed: Dssswtchx and dspistol are obviously wrong choices for Strife menu sounds. I don't think it uses anything for those sounds, but I picked analogs for them anyway.
<br>・Fixed: Merchants ignored attacks from "silent" weapons.
<br>・Fixed: Strife humans did not play their "yeargh!" sound for their zap deaths.
<br>・Added a new Travelled() method for inventory items. This is called each time an item travels to a new map and gives it a chance to perform any needed reinitialization. Specficially, PowerTargeter needs it to set the player's psprites again.
<br>・Fixed: Dropping coins should do so in increments of 50 if you have enough, not increments of 1.
<br>・Added IF_UNDROPPABLE to HealthTraining and GunTraining. Because HealthTraining also gives GunTraining when picked up, you could cheat by dropping the HealthTraining item and picking it up to get as many GunTraining upgrades as you wanted. GunTraining doesn't really need to be undroppable; the flag is just there for it for the sake of symmetry.
<br>・Fixed: It looks like just about everything in Strife should have MF2_FLOORCLIP set. Strife probably applied floorclip to everything without MF_NOGRAVITY set.
<br>・Fixed: The perfectly vertical missile movement fix was backwards with regards to damage values.
<br>・Fixed: AddCommandString() scanned past terminating '\0's in unclosed strings while searching for semicolons that break commands. Thus, if the previous command added had a semicolon in just the right place, it could try to add it again.
<br>・Fixed: The fly cheat did not check to make sure you had a body.
### 20051201
・Backported miscellaneous sc_man.cpp fixes from 2.1.0.
<br>・Fixed: P_BobWeapon() checked flags instead of WeaponFlags for WIF_DONTBOB.
<br>・R_ProjectSprite() now checks for NULL things passed to it.
<br>・Fixed: The game crashed if a slope thing of type 9500 or 9501 was placed directly on its target line. Now you get a warning in the console instead.
<br>・Returned Berserk's behavior to giving you 100 health instead of 100% health.
<br>・Changed: When no screenshot_dir is specified, the game now tries to save screenshots to the program directory instead of the current directory. If you really want screenshots to be saved to the current directory, you can do so by setting screenshot_dir to "."
<br>・Fixed: StringFormat::VWorker cleared the zero flag for numbers. (I know I tested this, so I'm not sure when it went wrong.)
<br>・Fixed: The game did not check if actors had pain states before putting them in their pain states.
<br>・Fixed: The shazam cheat could not deactivate the Tome of Power, nor did it display any text informing you of its action.
<br>・Added additional decals from Graf Zahl's DECALDEF.
<br>・Updated the obituaries in english-us.txt for Heretic to reflect the published names of various Heretic monsters.
<br>・Fixed: There is no reason why the Heretic and Hexen status bars should clamp the displayed health to a maximum of 100.
<br>・Fixed: P_PlayerInSpecialSector() duplicated the damage done by P_PlayerOnSpecialFlat().
<br>・Fixed: The actor serializer was off by one when checking the number of conversation nodes in a script.
<br>・Fixed: In Strife dialogues, yes texts are printed even when it jumps immediately to another dialogue node. I think this is bad design, and the yes text should be part of the normal dialogue, but the person in the east room of MAP27 abuses this, so I need to support it too. This also means the notify text is no longer hidden when the menu is up. (Maybe they mistakenly made it jump, and you were really supposed to try talking to him again when he tells you the master doesn't like visitors.)
<br>・Fixed: Anybody in Strife who does not have their name provided in their dialogue script should use the default name "Person".
<br>・Fixed: P_RecursiveSound() did not consider closed split doors as closed.
<br>・Fixed: MummyFX1 and Whirlwind should have the MF2_SEEKERMISSILE flag set.
<br>・Fixed: A_VileChase() did not clear the MF3_CRASHED flag.
<br>・Fixed: P_SpawnPlayer() should always attach the StatusBar if the consoleplayer is spawning.
<br>・Fixed: HateTarget did not take damage due to its lack of a death state.
<br>・Fixed: CheckInventory(), TakeInventory(), and GiveInventory need NULL type checks.
<br>・Fixed: SC_GetString() did not understand escaped quote characters in strings.
<br>・Fixed: There were synchronization issues while drawing the console. Calls to DrawTexture() can trigger input events to be processed, which can cause the contents of the console to change WHILE THE CONSOLE IS BEING DRAWN. So now any changes that would alter the contents of the console text buffer are deferred until after the buffer is drawn, and a copy of the command line is drawn instead of the real command line.
### 20051204
・Removed the ancient and unused Fixed(Mul|Div)_(ASM|C) functions.
<br>・Discovered the GCC outputs nice code for MulScale and friends, so I removedthe assembly for those routines in gccinlines.h. Alas, the same cannot be said of DivScale's variants.
<br>・Removed the assembly code for BigShort and BigLong and replaced it with compiler intrinsics provided by VC++.