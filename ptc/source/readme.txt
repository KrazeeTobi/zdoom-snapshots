prometheus truecolor source
Copyright (c) Glenn Fiedler 1998

Build instructions
-use the batch/script files to compile the source (wc11.bat for watcom C++
11.0, vc4x.bat for Visual C++ 4.x etc, linux for Linux GCC 2.x)
-these scripts are setup to easily let you build the debug/release versions
of PTC libraries, the libraries are outputted to /library/[os]/[compiler]
-by default the release library is build (for speed) however, it is
recommended that you use the debug library during development
-see the comments inside the batch/script files for more information

Source dependencies
-requires nasm to assemble the *.asm files (www.cryogen.com/Nasm)
-needs svgalib for linux (available from sunsite)
-does NOT require the DirectX SDK to develop DX apps under Win32

Watcom C++ 10.5/10.6
-needs watcom c++ 11.0 wlib and wmake (wc10x cannot handle COFF objs)
-does not support Watcom C++ 10.0 (cannot get nasm objs working with wc10)

Watcom C++ 11.0
-may run out of space on the command line to run the wc11.bat on examples if
the /binw compiler tools are used under win32. solution: use /binnt tools

Visual C++ 4.x
-to compile you need to setup the command line tools by running
"vcvars32.bat". In many cases you will need to expand the size of the
dos box environment. solution: setup a shortcut to run "command /e:2048" to
allocate 2k for the environment
-the library is configured to use MULTITHREADED DEBUG, and MULTITHREADED
RELEASE runtime libraries in the debug.lib and release.lib respectively.
You will need to setup your project to match these settings, or you will get
link errors (Project->Settings ...)

Visual C++ 5.x
-same setup as per visual c++ 4.x
-for convenience a project file (vc5x.dsp) has been included to build the
PTC source inside Developer Studio 

DJGPP 2.x
-nothing special here, only that DJGPP programs will not run correctly
under windows NT dos box because of a djgpp limitation

Cygnus GNU-Win32
-only tested with cygnus b18. you will need to recompile with cygnus
gnu-win32 b19 most likely in order to use PTC, should work, but it has not
been tested!
-cygnus b18 does not support resources so the PTC icon is not setup
(windows.rc, windows.ico). Apparently b19 can support resources, so it
shouldnt be too hard to get the icon working with b19

Linux GCC
-tested with GCC 2.8.1 and svgalib 1.2.13
-the executable must run as root in order to work (svgalib)
-displays some warnings (iostream stuff) when building modelist.cpp


problems building the source?
email ptc@gaffer.org