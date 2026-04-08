# Visual C++ 4.x makefile for PTC 

LIBDIR  = ..\library\win32\vc4.x

!IF "$(CFG)"=="release" || "$(CFG)"==""
!MESSAGE Building PTC: Visual C++ 4.x release.lib
CPPOPTS = -c -MT -WX -W3 -O2 -Ox -D "WIN32" -D "NDEBUG" -D "_WINDOWS" -D "_MT" -nologo
LIBNAME = $(LIBDIR)\release.lib
!ENDIF 

!IF "$(CFG)"=="debug"
!MESSAGE Building PTC: Visual C++ 4.x debug.lib
CPPOPTS = -c -MTd -WX -W3 -GX -D "WIN32" -D "_DEBUG" -D "_WINDOWS" -D "_MT" -nologo
LIBNAME = $(LIBDIR)\debug.lib             
!ENDIF 


CPPCMD  = cl $(CPPCMD)
ASMCMD  = nasm -o $@ -f win32 
LIBCMD  = link -lib -out:$(LIBNAME) @vc4x.cmd

OBJS    = ptc.obj near.obj file.obj format.obj dpmi.obj               \
          color.obj palette.obj clipper.obj manager.obj block.obj     \
          keyboard.obj                                                \
          raster.obj surface.obj idummy.obj isoft.obj                 \
          ivga.obj ivesa.obj                                          \
          iwin32.obj igdi.obj idirectx.obj                            \
          image.obj tga.obj                                           \
          cpp_copy.obj cpp_8.obj cpp_16.obj cpp_32.obj                \
          x86_copy.obj x86_8.obj x86_16.obj x86_32.obj x86_fake.obj   \
          mmx_copy.obj mmx_8.obj mmx_16.obj mmx_32.obj mmx_fake.obj   \
          tables.obj



all : $(LIBDIR) $(LIBNAME)

$(LIBDIR) :
            if not exist $(LIBDIR) mkdir $(LIBDIR)

$(LIBNAME) : $(OBJS)
             $(LIBCMD)

ptc.obj : ptc.cpp ptc.h
          $(CPP) ptc.cpp $(CPPOPTS)

near.obj : near.cpp near.h
           $(CPP) near.cpp $(CPPOPTS)

file.obj : file.cpp file.h
           $(CPP) file.cpp $(CPPOPTS)

format.obj : format.cpp format.h
             $(CPP) format.cpp $(CPPOPTS)

dpmi.obj : dpmi.cpp dpmi.h
           $(CPP) dpmi.cpp $(CPPOPTS)

color.obj : color.cpp color.h
            $(CPP) color.cpp $(CPPOPTS)

palette.obj : palette.cpp palette.h
              $(CPP) palette.cpp $(CPPOPTS)

clipper.obj : clipper.cpp clipper.h
              $(CPP) clipper.cpp $(CPPOPTS)

keyboard.obj : keyboard.cpp keyboard.h
               $(CPP) keyboard.cpp $(CPPOPTS)

iface.obj : iface.cpp iface.h
            $(CPP) iface.cpp $(CPPOPTS)

idummy.obj : idummy.cpp idummy.h
             $(CPP) idummy.cpp $(CPPOPTS)

isoft.obj : isoft.cpp isoft.h
            $(CPP) isoft.cpp $(CPPOPTS)

ivga.obj : ivga.cpp ivga.h
           $(CPP) ivga.cpp $(CPPOPTS)

ivesa.obj : ivesa.cpp ivesa.h
            $(CPP) ivesa.cpp $(CPPOPTS)

iwin32.obj : iwin32.cpp iwin32.h
             $(CPP) iwin32.cpp $(CPPOPTS)

igdi.obj : igdi.cpp igdi.h
           $(CPP) igdi.cpp $(CPPOPTS)

idirectx.obj : idirectx.cpp idirectx.h
               $(CPP) idirectx.cpp $(CPPOPTS)


surface.obj : surface.cpp surface.h
              $(CPP) surface.cpp $(CPPOPTS)

manager.obj : manager.cpp manager.h
              $(CPP) manager.cpp $(CPPOPTS)

block.obj : block.cpp block.h
            $(CPP) block.cpp $(CPPOPTS)

raster.obj : raster.cpp raster.h
              $(CPP) raster.cpp $(CPPOPTS)

image.obj : image.cpp image.h
            $(CPP) image.cpp $(CPPOPTS)

tga.obj : tga.cpp tga.h
          $(CPP) tga.cpp $(CPPOPTS)

cpp_8.obj : cpp_8.cpp cpp_8.h
            $(CPP) cpp_8.cpp $(CPPOPTS)

cpp_16.obj : cpp_16.cpp cpp_16.h
             $(CPP) cpp_16.cpp $(CPPOPTS)

cpp_32.obj : cpp_32.cpp cpp_32.h
             $(CPP) cpp_32.cpp $(CPPOPTS)

cpp_copy.obj : cpp_copy.cpp cpp_copy.h
               $(CPP) cpp_copy.cpp $(CPPOPTS)

x86_8.obj : x86_8.asm x86_8.h
            $(ASMCMD) x86_8.asm

x86_16.obj : x86_16.asm x86_16.h
             $(ASMCMD) x86_16.asm

x86_32.obj : x86_32.asm x86_32.h
             $(ASMCMD) x86_32.asm

x86_fake.obj : x86_fake.asm x86_fake.h
               $(ASMCMD) x86_fake.asm

x86_copy.obj : x86_copy.asm x86_copy.h
               $(ASMCMD) x86_copy.asm

mmx_8.obj : mmx_8.asm mmx_8.h
            $(ASMCMD) mmx_8.asm

mmx_16.obj : mmx_16.asm mmx_16.h
             $(ASMCMD) mmx_16.asm

mmx_32.obj : mmx_32.asm mmx_32.h
             $(ASMCMD) mmx_32.asm

mmx_fake.obj : mmx_fake.asm mmx_fake.h
               $(ASMCMD) mmx_fake.asm

mmx_copy.obj : mmx_copy.asm mmx_copy.h
               $(ASMCMD) mmx_copy.asm

tables.obj : tables.asm
             $(ASMCMD) tables.asm
