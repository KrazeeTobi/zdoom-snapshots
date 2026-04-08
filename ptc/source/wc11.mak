# Watcom C++ 11.0 makefile for PTC 


!IF "$(PLATFORM)"=="win32"

LIBPATH  = ..\library\win32\wc11.0

!IF "$(LIBRARY)"=="debug"
!MESSAGE Building PTC: Watcom C++ 11.0 win32 debug.lib
CPPOPTS = -zq -bm -bt=nt # -we -wx 
LIBNAME = $(LIBPATH)\debug.lib             
!ENDIF 

!IF "$(LIBRARY)"!="debug"
!MESSAGE Building PTC: Watcom C++ 11.0 win32 release.lib
CPPOPTS = -zq -bm -bt=nt -oneatxhl+ -5r -fp5 -fpi87 # -we -wx 
LIBNAME = $(LIBPATH)\release.lib
!ENDIF 

!ENDIF

!IF "$(PLATFORM)"!="win32"

LIBPATH  = ..\library\dos\wc11.0

!IF "$(LIBRARY)"=="debug"
!MESSAGE Building PTC: Watcom C++ 11.0 dos debug.lib
CPPOPTS = -zq -bt=dos # -we -wx 
LIBNAME = $(LIBPATH)\debug.lib             
!ENDIF 

!IF "$(LIBRARY)"!="debug"
!MESSAGE Building PTC: Watcom C++ 11.0 dos release.lib
CPPOPTS = -zq -bt=dos -oneatxhl+ -5r -fp5 -fpi87 # -we -wx 
LIBNAME = $(LIBPATH)\release.lib
!ENDIF

!ENDIF 


CPP  = wpp386
ASM  = nasm
LIB  = wlib

OBJS = ptc.obj near.obj file.obj format.obj dpmi.obj               \
       color.obj palette.obj clipper.obj manager.obj block.obj     \
       keyboard.obj                                                \
       raster.obj surface.obj idummy.obj isoft.obj                 \
       ivga.obj ivesa.obj                                          \
       iwin32.obj igdi.obj idirectx.obj                            \
       image.obj tga.obj matrox.obj                                \
       cpp_copy.obj cpp_8.obj cpp_16.obj cpp_32.obj                \
       x86_copy.obj x86_8.obj x86_16.obj x86_32.obj x86_fake.obj   \
       mmx_copy.obj mmx_8.obj mmx_16.obj mmx_32.obj mmx_fake.obj   \
       tables.obj


all: $(LIBPATH) $(LIBNAME)

$(LIBPATH):
            if not exist $(LIBPATH) mkdir $(LIBPATH)

$(LIBNAME): $(OBJS)
            $(LIB) -b -c -n $(LIBNAME) @wc11.cmd

.cpp.obj: .AUTODEPEND
          $(CPP) $(CPPOPTS) $[@ 

.asm.obj: .AUTODEPEND
          $(ASM) -o $@ -f coff $[@ 
