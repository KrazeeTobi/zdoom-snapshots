# Microsoft Developer Studio Project File - Name="source" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=source - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "source.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "source.mak" CFG="source - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "source - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "source - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "source - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\library\win32\vc5.x\release"
# PROP Intermediate_Dir "..\library\win32\vc5.x\release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\library\win32\vc5.x\release.lib"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\library\win32\vc5.x\debug"
# PROP Intermediate_Dir "..\library\win32\vc5.x\debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\library\win32\vc5.x\debug.lib"

!ENDIF 

# Begin Target

# Name "source - Win32 Release"
# Name "source - Win32 Debug"
# Begin Group "Source Code"

# PROP Default_Filter ""
# Begin Group "cpp"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\block.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\clipper.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\color.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cpp_16.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cpp_32.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cpp_8.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cpp_copy.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dpmi.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\file.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\format.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\idirectx.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\idummy.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\igdi.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\image.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\isoft.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ivesa.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ivga.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iwin32.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\keyboard.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\manager.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\matrox.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\near.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\palette.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ptc.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raster.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\surface.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tga.cpp

!IF  "$(CFG)" == "source - Win32 Release"

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# ADD CPP /G5 /MTd
# SUBTRACT CPP /Z<none> /X /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "asm"

# PROP Default_Filter "asm"
# Begin Source File

SOURCE=.\mmx_16.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\mmx_16.asm
InputName=mmx_16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\mmx_16.asm
InputName=mmx_16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmx_32.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\mmx_32.asm
InputName=mmx_32

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\mmx_32.asm
InputName=mmx_32

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmx_8.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\mmx_8.asm
InputName=mmx_8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\mmx_8.asm
InputName=mmx_8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmx_copy.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\mmx_copy.asm
InputName=mmx_copy

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\mmx_copy.asm
InputName=mmx_copy

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmx_fake.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\mmx_fake.asm
InputName=mmx_fake

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\mmx_fake.asm
InputName=mmx_fake

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tables.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\tables.asm
InputName=tables

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\tables.asm
InputName=tables

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x86_16.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\x86_16.asm
InputName=x86_16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\x86_16.asm
InputName=x86_16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x86_32.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\x86_32.asm
InputName=x86_32

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\x86_32.asm
InputName=x86_32

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x86_8.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\x86_8.asm
InputName=x86_8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\x86_8.asm
InputName=x86_8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x86_copy.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\x86_copy.asm
InputName=x86_copy

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\x86_copy.asm
InputName=x86_copy

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x86_fake.asm

!IF  "$(CFG)" == "source - Win32 Release"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\release
InputPath=.\x86_fake.asm
InputName=x86_fake

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

# Begin Custom Build - Assembling...
OutDir=.\..\library\win32\vc5.x\debug
InputPath=.\x86_fake.asm
InputName=x86_fake

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm $(InputPath) -f coff -o $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
