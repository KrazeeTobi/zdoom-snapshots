# Microsoft Developer Studio Project File - Name="CDoom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CDoom - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "CDoom.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "CDoom.mak" CFG="CDoom - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "CDoom - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "CDoom - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "CDoom - Win32 Optimized Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CDoom - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /I "flac" /I "." /I "sound" /I "win32" /I "zlib" /I "g_shared" /I "g_raven" /I "g_heretic" /I "g_doom" /I "g_strife" /I "g_hexen" /I "private" /D "NDEBUG" /D "USEASM" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "HAVE_STRUPR" /D "HAVE_FILELENGTH" /FAcs /FR /Yu"doomall.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib dsound.lib dxguid.lib ddraw.lib dinput.lib fmodvc.lib winmm.lib wsock32.lib ole32.lib shell32.lib advapi32.lib comctl32.lib comdlg32.lib /nologo /subsystem:windows /map /machine:I386 /out:"c:/spiele/doom/CDoom.exe"

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "flac" /I "." /I "sound" /I "win32" /I "zlib" /I "g_shared" /I "g_raven" /I "g_heretic" /I "g_doom" /I "g_strife" /I "g_hexen" /I "private" /D "DEBUG" /D "_DEBUG" /D "USEASM" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "HAVE_STRUPR" /D "HAVE_FILELENGTH" /FR /Yu"doomall.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib dsound.lib dxguid.lib ddraw.lib dinput.lib fmodvc.lib winmm.lib wsock32.lib ole32.lib shell32.lib advapi32.lib comctl32.lib comdlg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CDoom___Win32_Optimized_Debug"
# PROP BASE Intermediate_Dir "CDoom___Win32_Optimized_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CDoom___Win32_Optimized_Debug"
# PROP Intermediate_Dir "CDoom___Win32_Optimized_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /MD /W3 /O2 /I "flac" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "HAVE_CONFIG_H" /FAs /FR /Yu"doomall.h" /FD /c
# ADD CPP /nologo /Gz /MD /W3 /Zi /O2 /I "flac" /I "." /I "sound" /I "win32" /I "zlib" /I "g_shared" /I "g_raven" /I "g_heretic" /I "g_doom" /I "g_strife" /I "g_hexen" /I "private" /D "NDEBUG" /D "RELDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "HAVE_STRUPR" /D "HAVE_FILELENGTH" /FAs /FR /Yu"doomall.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib dsound.lib dxguid.lib ddraw.lib dinput.lib fmodvc.lib winmm.lib wsock32.lib ole32.lib shell32.lib advapi32.lib /nologo /subsystem:windows /map /machine:I386 /out:"c:/spiele/doom/CDoom.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib dsound.lib dxguid.lib ddraw.lib dinput.lib fmodvc.lib winmm.lib wsock32.lib ole32.lib shell32.lib advapi32.lib comctl32.lib comdlg32.lib /nologo /subsystem:windows /map /debug /machine:I386

!ENDIF 

# Begin Target

# Name "CDoom - Win32 Release"
# Name "CDoom - Win32 Debug"
# Name "CDoom - Win32 Optimized Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Asm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\a.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\a.nas
InputName=a

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\a.nas
InputName=a

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\a.nas
InputName=a

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\blocks.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\blocks.nas
InputName=blocks

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\blocks.nas
InputName=blocks

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\blocks.nas
InputName=blocks

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\misc.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)...
IntDir=.\Release
InputPath=.\misc.nas
InputName=misc

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)...
IntDir=.\Debug
InputPath=.\misc.nas
InputName=misc

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)...
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\misc.nas
InputName=misc

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tmap.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\tmap.nas
InputName=tmap

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\tmap.nas
InputName=tmap

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\tmap.nas
InputName=tmap

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tmap2.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\tmap2.nas
InputName=tmap2

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\tmap2.nas
InputName=tmap2

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\tmap2.nas
InputName=tmap2

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tmap3.nas

!IF  "$(CFG)" == "CDoom - Win32 Release"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\tmap3.nas
InputName=tmap3

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\tmap3.nas
InputName=tmap3

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# Begin Custom Build - Assembling $(InputPath)
IntDir=.\CDoom___Win32_Optimized_Debug
InputPath=.\tmap3.nas
InputName=tmap3

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)\$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Thingdef"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\decorations.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\thingdef.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\thingdef_codeptr.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\thingdef_specials.gperf
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\thingdef_specials.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\thingdef_specials1.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Am_map.cpp
# End Source File
# Begin Source File

SOURCE=.\Autostart.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\autozend.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\b_bot.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\b_func.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\b_game.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\b_move.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\b_think.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\bbannouncer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\c_bind.cpp
# End Source File
# Begin Source File

SOURCE=.\c_cmds.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\c_console.cpp
# End Source File
# Begin Source File

SOURCE=.\c_cvars.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\c_dispatch.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\c_expr.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\cmdlib.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\colormatcher.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\configfile.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ct_chat.cpp
# End Source File
# Begin Source File

SOURCE=.\d_dehacked.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\d_main.cpp
# End Source File
# Begin Source File

SOURCE=.\d_net.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\d_netinfo.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\d_protocol.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\decallib.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\dobject.cpp
# End Source File
# Begin Source File

SOURCE=.\doomdef.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\doomstat.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\dsectoreffect.cpp
# End Source File
# Begin Source File

SOURCE=.\dthinker.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\f_finale.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\farchive.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\files.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_game.cpp
# End Source File
# Begin Source File

SOURCE=.\g_level.cpp
# End Source File
# Begin Source File

SOURCE=.\gameconfigfile.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\gi.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\hu_scores.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\info.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\infodefaults.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_alloc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_argv.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_bbox.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_cheat.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_fixed.cpp

!IF  "$(CFG)" == "CDoom - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\m_misc.cpp
# End Source File
# Begin Source File

SOURCE=.\m_options.cpp
# End Source File
# Begin Source File

SOURCE=.\m_png.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\m_random.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mus2midi.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\nodebuild.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\nodebuild_events.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\nodebuild_extract.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\nodebuild_gl.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\nodebuild_utility.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_acs.cpp
# End Source File
# Begin Source File

SOURCE=.\p_buildmap.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_ceiling.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_conversation.cpp
# End Source File
# Begin Source File

SOURCE=.\p_doors.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_effect.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\p_floor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_interaction.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_lights.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_lnspec.cpp
# End Source File
# Begin Source File

SOURCE=.\p_map.cpp
# End Source File
# Begin Source File

SOURCE=.\p_maputl.cpp
# End Source File
# Begin Source File

SOURCE=.\p_mobj.cpp
# End Source File
# Begin Source File

SOURCE=.\p_pillar.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_plats.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_pspr.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_saveg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_sectors.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_setup.cpp
# End Source File
# Begin Source File

SOURCE=.\p_sight.cpp
# End Source File
# Begin Source File

SOURCE=.\p_spec.cpp
# End Source File
# Begin Source File

SOURCE=.\p_switch.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_teleport.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_terrain.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_things.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_tick.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_trace.cpp
# End Source File
# Begin Source File

SOURCE=.\p_user.cpp
# End Source File
# Begin Source File

SOURCE=.\p_writemap.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\p_xlat.cpp
# End Source File
# Begin Source File

SOURCE=.\po_man.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\precompiled.cpp
# ADD CPP /Yc"doomall.h"
# End Source File
# Begin Source File

SOURCE=.\r_data.cpp
# End Source File
# Begin Source File

SOURCE=.\r_draw.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\r_drawt.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\r_sky.cpp
# End Source File
# Begin Source File

SOURCE=.\r_things.cpp
# End Source File
# Begin Source File

SOURCE=.\s_advsound.cpp
# End Source File
# Begin Source File

SOURCE=.\s_environment.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\s_playlist.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\s_sndseq.cpp
# End Source File
# Begin Source File

SOURCE=.\s_sound.cpp
# End Source File
# Begin Source File

SOURCE=.\sc_man.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\st_stuff.cpp
# End Source File
# Begin Source File

SOURCE=.\stats.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\stringtable.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\tables.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\tempfiles.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\v_collection.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\v_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\v_font.cpp
# End Source File
# Begin Source File

SOURCE=.\v_palette.cpp
# End Source File
# Begin Source File

SOURCE=.\v_pfx.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\v_text.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\v_video.cpp
# End Source File
# Begin Source File

SOURCE=.\vectors.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\w_wad.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\wi_stuff.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\actor.h
# End Source File
# Begin Source File

SOURCE=.\am_map.h
# End Source File
# Begin Source File

SOURCE=.\announcer.h
# End Source File
# Begin Source File

SOURCE=.\autosegs.h
# End Source File
# Begin Source File

SOURCE=.\b_bot.h
# End Source File
# Begin Source File

SOURCE=.\basicinlines.h
# End Source File
# Begin Source File

SOURCE=.\c_bind.h
# End Source File
# Begin Source File

SOURCE=.\c_console.h
# End Source File
# Begin Source File

SOURCE=.\c_cvars.h
# End Source File
# Begin Source File

SOURCE=.\c_dispatch.h
# End Source File
# Begin Source File

SOURCE=.\cmdlib.h
# End Source File
# Begin Source File

SOURCE=.\colormatcher.h
# End Source File
# Begin Source File

SOURCE=.\configfile.h
# End Source File
# Begin Source File

SOURCE=.\d_dehacked.h
# End Source File
# Begin Source File

SOURCE=.\d_dehackedactions.h
# End Source File
# Begin Source File

SOURCE=.\d_event.h
# End Source File
# Begin Source File

SOURCE=.\d_gui.h
# End Source File
# Begin Source File

SOURCE=.\d_main.h
# End Source File
# Begin Source File

SOURCE=.\d_net.h
# End Source File
# Begin Source File

SOURCE=.\d_netinf.h
# End Source File
# Begin Source File

SOURCE=.\D_player.h
# End Source File
# Begin Source File

SOURCE=.\d_protocol.h
# End Source File
# Begin Source File

SOURCE=.\d_textur.h
# End Source File
# Begin Source File

SOURCE=.\d_ticcmd.h
# End Source File
# Begin Source File

SOURCE=.\decallib.h
# End Source File
# Begin Source File

SOURCE=.\dobject.h
# End Source File
# Begin Source File

SOURCE=.\Doomall.h
# End Source File
# Begin Source File

SOURCE=.\doomdata.h
# End Source File
# Begin Source File

SOURCE=.\Doomdef.h
# End Source File
# Begin Source File

SOURCE=.\doomerrors.h
# End Source File
# Begin Source File

SOURCE=.\doomstat.h
# End Source File
# Begin Source File

SOURCE=.\doomtype.h
# End Source File
# Begin Source File

SOURCE=.\dscript.h
# End Source File
# Begin Source File

SOURCE=.\dsectoreffect.h
# End Source File
# Begin Source File

SOURCE=.\dthinker.h
# End Source File
# Begin Source File

SOURCE=.\f_finale.h
# End Source File
# Begin Source File

SOURCE=.\farchive.h
# End Source File
# Begin Source File

SOURCE=.\files.h
# End Source File
# Begin Source File

SOURCE=.\g_game.h
# End Source File
# Begin Source File

SOURCE=.\g_level.h
# End Source File
# Begin Source File

SOURCE=.\gameconfigfile.h
# End Source File
# Begin Source File

SOURCE=.\gccinlines.h
# End Source File
# Begin Source File

SOURCE=.\gi.h
# End Source File
# Begin Source File

SOURCE=.\gstrings.h
# End Source File
# Begin Source File

SOURCE=.\hu_stuff.h
# End Source File
# Begin Source File

SOURCE=.\i_cd.h
# End Source File
# Begin Source File

SOURCE=.\i_movie.h
# End Source File
# Begin Source File

SOURCE=.\i_video.h
# End Source File
# Begin Source File

SOURCE=.\info.h
# End Source File
# Begin Source File

SOURCE=.\infomacros.h
# End Source File
# Begin Source File

SOURCE=.\lists.h
# End Source File
# Begin Source File

SOURCE=.\m_alloc.h
# End Source File
# Begin Source File

SOURCE=.\m_argv.h
# End Source File
# Begin Source File

SOURCE=.\m_bbox.h
# End Source File
# Begin Source File

SOURCE=.\m_cheat.h
# End Source File
# Begin Source File

SOURCE=.\m_crc32.h
# End Source File
# Begin Source File

SOURCE=.\m_fixed.h
# End Source File
# Begin Source File

SOURCE=.\m_menu.h
# End Source File
# Begin Source File

SOURCE=.\m_misc.h
# End Source File
# Begin Source File

SOURCE=.\m_png.h
# End Source File
# Begin Source File

SOURCE=.\m_random.h
# End Source File
# Begin Source File

SOURCE=.\m_swap.h
# End Source File
# Begin Source File

SOURCE=.\mscinlines.h
# End Source File
# Begin Source File

SOURCE=.\mus2midi.h
# End Source File
# Begin Source File

SOURCE=.\nodebuild.h
# End Source File
# Begin Source File

SOURCE=.\p_acs.h
# End Source File
# Begin Source File

SOURCE=.\p_ActorIterator.h
# End Source File
# Begin Source File

SOURCE=.\p_conversation.h
# End Source File
# Begin Source File

SOURCE=.\p_effect.h
# End Source File
# Begin Source File

SOURCE=.\p_enemy.h
# End Source File
# Begin Source File

SOURCE=.\p_inter.h
# End Source File
# Begin Source File

SOURCE=.\p_lnspec.h
# End Source File
# Begin Source File

SOURCE=.\p_local.h
# End Source File
# Begin Source File

SOURCE=.\P_mobj.h
# End Source File
# Begin Source File

SOURCE=.\p_mobjflags.h
# End Source File
# Begin Source File

SOURCE=.\p_pspr.h
# End Source File
# Begin Source File

SOURCE=.\p_saveg.h
# End Source File
# Begin Source File

SOURCE=.\p_setup.h
# End Source File
# Begin Source File

SOURCE=.\p_spec.h
# End Source File
# Begin Source File

SOURCE=.\p_terrain.h
# End Source File
# Begin Source File

SOURCE=.\p_tick.h
# End Source File
# Begin Source File

SOURCE=.\p_trace.h
# End Source File
# Begin Source File

SOURCE=.\r_bsp.h
# End Source File
# Begin Source File

SOURCE=.\r_data.h
# End Source File
# Begin Source File

SOURCE=.\r_defs.h
# End Source File
# Begin Source File

SOURCE=.\r_draw.h
# End Source File
# Begin Source File

SOURCE=.\r_local.h
# End Source File
# Begin Source File

SOURCE=.\r_main.h
# End Source File
# Begin Source File

SOURCE=.\r_plane.h
# End Source File
# Begin Source File

SOURCE=.\r_segs.h
# End Source File
# Begin Source File

SOURCE=.\r_sky.h
# End Source File
# Begin Source File

SOURCE=.\r_state.h
# End Source File
# Begin Source File

SOURCE=.\r_things.h
# End Source File
# Begin Source File

SOURCE=.\res_colormap.h
# End Source File
# Begin Source File

SOURCE=.\s_playlist.h
# End Source File
# Begin Source File

SOURCE=.\s_sndseq.h
# End Source File
# Begin Source File

SOURCE=.\s_sound.h
# End Source File
# Begin Source File

SOURCE=.\sc_man.h
# End Source File
# Begin Source File

SOURCE=.\st_stuff.h
# End Source File
# Begin Source File

SOURCE=.\statnums.h
# End Source File
# Begin Source File

SOURCE=.\stats.h
# End Source File
# Begin Source File

SOURCE=.\stringenums.h
# End Source File
# Begin Source File

SOURCE=.\stringlist.h
# End Source File
# Begin Source File

SOURCE=.\stringtable.h
# End Source File
# Begin Source File

SOURCE=.\tables.h
# End Source File
# Begin Source File

SOURCE=.\tarray.h
# End Source File
# Begin Source File

SOURCE=.\tempfiles.h
# End Source File
# Begin Source File

SOURCE=.\templates.h
# End Source File
# Begin Source File

SOURCE=.\v_collection.h
# End Source File
# Begin Source File

SOURCE=.\v_font.h
# End Source File
# Begin Source File

SOURCE=.\v_palette.h
# End Source File
# Begin Source File

SOURCE=.\v_pfx.h
# End Source File
# Begin Source File

SOURCE=.\v_text.h
# End Source File
# Begin Source File

SOURCE=.\v_video.h
# End Source File
# Begin Source File

SOURCE=.\vectors.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\w_wad.h
# End Source File
# Begin Source File

SOURCE=.\weightedlist.h
# End Source File
# Begin Source File

SOURCE=.\wi_stuff.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\win32\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\win32\icon1.ico
# End Source File
# End Group
# Begin Group "GL"

# PROP Default_Filter ""
# Begin Group "GL Header"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\gl\gfxreader.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_basic.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_clipper.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_data.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_functions.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_glow.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_intern.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_lights.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_portal.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_renderstruct.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_struct.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_texture.h
# End Source File
# Begin Source File

SOURCE=.\gl\gl_video.h
# End Source File
# Begin Source File

SOURCE=.\gl\gltexture.h
# End Source File
# Begin Source File

SOURCE=.\gl\models.h
# End Source File
# Begin Source File

SOURCE=.\gl\tab_anorms.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\gl\a_dynlight.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\am_dukemap.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gfxfuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gfxreader.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_bsp.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_clipper.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_colormap.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_data.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_decal.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_drawinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_fakeflat.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_Flats.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_fontchar.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_framebuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_geometric.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_glow.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_Light.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_main.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_portal.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_scene.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_shader.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_sky.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_skydome.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_tessel.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_texture.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_video.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_Walls.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gl_Weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\gltexture.cpp
# End Source File
# Begin Source File

SOURCE=.\gl\models.cpp
# End Source File
# End Group
# Begin Group "G_Shared"

# PROP Default_Filter ""
# Begin Group "G_Shared Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_shared\a_action.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_artifacts.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_keys.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_lightning.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_pickups.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_sharedglobal.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_weaponpiece.h
# End Source File
# Begin Source File

SOURCE=.\g_shared\sbar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\g_shared\a_action.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_artifacts.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_bridge.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_camera.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_debris.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_decals.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_flashfader.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_fountain.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_hatetarget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_keys.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_lightning.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_movingcamera.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_pickups.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_quake.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_secrettrigger.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_sectoraction.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_sharedmisc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_skies.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_soundenvironment.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_spark.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_splashes.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_waterzone.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_weaponpiece.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\a_weapons.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_shared\hudmessages.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "G_Doom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_doom\a_arachnotron.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_archvile.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_bossbrain.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_bruiser.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_cacodemon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_cyberdemon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_demon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomarmor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomartifacts.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomdecorations.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomglobal.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomhealth.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomimp.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomkeys.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doommisc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_doomweaps.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_fatso.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_keen.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_lostsoul.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_painelemental.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_possessed.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_revenant.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_scriptedmarine.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_doom\a_spidermaster.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "G_Heretic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_heretic\a_beast.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_chicken.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_clink.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_dsparil.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticambience.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticarmor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticartifacts.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticdecorations.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticglobal.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticimp.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_heretickeys.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticmisc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_hereticweaps.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_ironlich.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_knight.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_mummy.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_snake.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_heretic\a_wizard.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "G_Hexen"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_hexen\a_bats.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_bishop.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_blastradius.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_boostarmor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_centaur.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericboss.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericflame.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericholy.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericmace.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_clericstaff.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_demons.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_dragon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_ettin.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fighteraxe.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fighterboss.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fighterhammer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fighterplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fighterquietus.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_firedemon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_flame.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_flechette.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_fog.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_healingradius.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_heresiarch.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_hexenarmor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_hexendecorations.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_hexenglobal.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_hexenkeys.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_hexenspecialdecs.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_iceguy.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_korax.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_mageboss.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_magecone.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_magelightning.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_mageplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_magestaff.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_magewand.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_mana.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_pig.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_puzzleitems.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_scriptprojectiles.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_serpent.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_speedboots.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_spike.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_summon.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_teleportother.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_weaponpieces.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_hexen\a_wraith.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "G_Strife"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_strife\a_acolyte.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_alienspectres.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_beggars.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_coin.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_crusader.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_entityboss.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_inquisitor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_loremaster.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_macil.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_merchants.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_oracle.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_peasant.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_programmer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_questitems.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_ratbuddy.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_reaver.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_rebels.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_sentinel.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_spectral.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_stalker.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeammo.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifearmor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifebishop.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeglobal.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeitems.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifekeys.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeplayer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifestuff.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeweapons.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_strifeweaps.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_templar.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_thingstoblowup.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_strife\a_zombie.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "G_Raven"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\g_raven\a_artiegg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\a_artitele.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\a_minotaur.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\a_ravenambient.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\a_ravenartifacts.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\a_ravenhealth.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\g_raven\ravenshared.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "FraggleScript"

# PROP Default_Filter ""
# Begin Group "FS Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fragglescript\T_func.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_oper.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_parse.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_prepro.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_script.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_spec.h
# End Source File
# Begin Source File

SOURCE=.\fragglescript\T_vari.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\fragglescript\t_fspic.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_func.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_load.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_oper.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_parse.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_prepro.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_Saveg.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_script.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_spec.cpp
# End Source File
# Begin Source File

SOURCE=.\fragglescript\t_vari.cpp
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Group "Sound Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sound\altsound.h
# End Source File
# Begin Source File

SOURCE=.\sound\fmodsound.h
# End Source File
# Begin Source File

SOURCE=.\sound\i_music.h
# End Source File
# Begin Source File

SOURCE=.\sound\i_musicinterns.h
# End Source File
# Begin Source File

SOURCE=.\sound\i_sound.h
# End Source File
# Begin Source File

SOURCE=.\sound\mus2midi.h
# End Source File
# Begin Source File

SOURCE=.\sound\sample_flac.h
# End Source File
# End Group
# Begin Group "OPL Synth"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\oplsynth\deftypes.h
# End Source File
# Begin Source File

SOURCE=.\oplsynth\fmopl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\oplsynth\fmopl.h
# End Source File
# Begin Source File

SOURCE=.\oplsynth\mlkernel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\oplsynth\mlopl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\oplsynth\mlopl_io.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\oplsynth\muslib.h
# End Source File
# Begin Source File

SOURCE=.\oplsynth\opl_mus_player.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\oplsynth\opl_mus_player.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\sound\altsound.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\altsoundmixer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\fmodsound.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\i_Music.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\i_sound.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_cd.cpp

!IF  "$(CFG)" == "CDoom - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CDoom - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CDoom - Win32 Optimized Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sound\music_flac.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_midi_midiout.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_midi_stream.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_midi_timidity.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_mod.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_mus_midiout.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_mus_opl.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_spc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\music_stream.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound\sample_flac.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "ZLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\gzio.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\afxres.h
# End Source File
# Begin Source File

SOURCE=.\win32\eaxedit.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\hardware.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\hardware.h
# End Source File
# Begin Source File

SOURCE=.\win32\helperthread.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\helperthread.h
# End Source File
# Begin Source File

SOURCE=.\win32\i_cd.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_crash.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\win32\i_input.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_main.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_main.h
# End Source File
# Begin Source File

SOURCE=.\win32\i_movie.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_net.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_net.h
# End Source File
# Begin Source File

SOURCE=.\win32\i_system.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\i_system.h
# End Source File
# Begin Source File

SOURCE=.\win32\resource.h
# End Source File
# Begin Source File

SOURCE=.\win32\win32iface.h
# End Source File
# Begin Source File

SOURCE=.\win32\win32video.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\zdoom.rc
# End Source File
# End Group
# Begin Group "To Do"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\to_do\gl_viewpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\to_do\gl_viewpoint.h
# End Source File
# Begin Source File

SOURCE=.\to_do\p_glnodes.cpp
# End Source File
# Begin Source File

SOURCE=.\to_do\p_map.h
# End Source File
# Begin Source File

SOURCE=.\to_do\r_main.cpp
# End Source File
# Begin Source File

SOURCE=.\to_do\res_fontex.cpp
# End Source File
# End Group
# Begin Group "Private"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\private\d_multiwad.cpp
# End Source File
# Begin Source File

SOURCE=.\private\d_multiwad.h
# End Source File
# Begin Source File

SOURCE=.\private\i_musvol.cpp
# End Source File
# Begin Source File

SOURCE=.\private\p_spec2.cpp
# End Source File
# Begin Source File

SOURCE=.\private\p_spec2.h
# End Source File
# Begin Source File

SOURCE=.\private\SBar_common.cpp
# End Source File
# Begin Source File

SOURCE=.\private\SBar_Doom.cpp
# End Source File
# Begin Source File

SOURCE=.\private\SBar_Heretic.cpp
# End Source File
# Begin Source File

SOURCE=.\private\SBar_Hexen.cpp
# End Source File
# Begin Source File

SOURCE=.\private\SBar_HUD.cpp
# End Source File
# Begin Source File

SOURCE=.\private\SBar_Strife.Cpp
# End Source File
# End Group
# Begin Group "PCH Loaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_pch\a_acolyte.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_action.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_alienspectres.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_arachnotron.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_archvile.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_artiegg.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_artifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_artitele.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_bats.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_beast.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_beggars.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_bishop.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_blastradius.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_boostarmor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_bossbrain.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_bridge.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_bruiser.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_cacodemon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_centaur.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_chicken.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericboss.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericflame.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericholy.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericmace.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clericstaff.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_clink.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_coin.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_crusader.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_cyberdemon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_debris.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_decals.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_demon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_demons.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomarmor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomartifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomdecorations.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomhealth.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomimp.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomkeys.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doommisc.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_doomweaps.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_dragon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_dsparil.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_entityboss.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ettin.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fatso.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fighteraxe.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fighterboss.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fighterhammer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fighterplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fighterquietus.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_firedemon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_flame.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_flashfader.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_flechette.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fog.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_fountain.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hatetarget.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_healingradius.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_heresiarch.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticambience.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticarmor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticartifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticdecorations.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticimp.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_heretickeys.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticmisc.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hereticweaps.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hexenarmor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hexendecorations.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hexenkeys.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_hexenspecialdecs.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_iceguy.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_inquisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ironlich.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_keen.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_keys.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_knight.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_korax.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_loremaster.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_lostsoul.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_macil.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_mageboss.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_magecone.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_magelightning.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_mageplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_magestaff.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_magewand.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_mana.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_merchants.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_minotaur.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_movingcamera.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_mummy.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_oracle.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_painelemental.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_peasant.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_pickups.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_pig.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_possessed.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_programmer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_puzzleitems.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_quake.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_questitems.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ratbuddy.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ravenambient.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ravenartifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_ravenhealth.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_reaver.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_rebels.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_revenant.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_scriptedmarine.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_scriptprojectiles.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_secrettrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_sectoraction.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_sentinel.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_serpent.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_sharedmisc.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_skies.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_snake.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_soundenvironment.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_spark.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_spectral.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_speedboots.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_spidermaster.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_spike.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_splashes.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_stalker.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifeammo.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifearmor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifebishop.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifeitems.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifekeys.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifeplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifestuff.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_strifeweapons.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_summon.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_teleportother.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_templar.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_thingstoblowup.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_waterzone.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_weaponpiece.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_weaponpieces.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_wraith.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\a_zombie.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\altsound.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\altsoundmixer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\b_bot.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\b_func.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\b_game.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\b_move.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\b_think.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\bbannouncer.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\c_cmds.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\c_cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\c_dispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\c_expr.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\cmdlib.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\colormatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\configfile.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\d_dehacked.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\d_net.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\d_netinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\d_protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\decallib.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\decorations.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\doomstat.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\dthinker.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\f_finale.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\farchive.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\files.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\fmodsound.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\gameconfigfile.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\gi.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\hu_scores.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\hudmessages.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\i_Music.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\i_sound.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\info.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\infodefaults.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_alloc.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_argv.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_bbox.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_cheat.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_png.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\m_random.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_cd.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_flac.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_midi_midiout.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_midi_stream.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_midi_timidity.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_mod.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_mus_midiout.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_mus_opl.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_spc.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\music_stream.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\nodebuild.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\nodebuild_events.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\nodebuild_extract.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\nodebuild_gl.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\nodebuild_utility.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_buildmap.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_ceiling.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_doors.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_effect.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_floor.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_interaction.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_lights.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_pillar.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_plats.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_pspr.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_saveg.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_sectors.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_switch.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_teleport.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_things.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_tick.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\p_writemap.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\po_man.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\r_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\r_drawt.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\s_environment.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\s_playlist.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\sample_flac.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\sc_man.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\stats.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\stringtable.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\tempfiles.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\thingdef.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\thingdef_codeptr.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\v_collection.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\v_pfx.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\v_text.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\vectors.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\w_wad.cpp
# End Source File
# Begin Source File

SOURCE=.\_pch\wi_stuff.cpp
# End Source File
# End Group
# End Target
# End Project
