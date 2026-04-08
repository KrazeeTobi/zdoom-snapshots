# DJGPP 2.x makefile for prometheus truecolor

LIBDIR  = ../library/dos/djgpp

ifeq ($(LIBRARY),debug)

    CPPOPTS = # -Wall -pedantic
    LIBNAME = $(LIBDIR)/debug.lib

else

    CPPOPTS = -O3 # -Wall -pedantic
    LIBNAME = $(LIBDIR)/release.lib

endif


CPPCMD  = gcc $(CPPOPTS) -c
ASMCMD  = nasm $(ASMOPTS) -f coff
LIBCMD  = ar rs $(LIBNAME)

OBJS    = ptc.o surface.o palette.o clipper.o               \
          keyboard.o                                        \
          idummy.o isoft.o ivga.o ivesa.o                   \
          iwin32.o igdi.o idirectx.o                        \
          raster.o near.o file.o dpmi.o format.o            \
          color.o manager.o block.o image.o tga.o           \
          cpp_copy.o cpp_8.o cpp_16.o cpp_32.o              \
          x86_copy.o x86_8.o x86_16.o x86_32.o x86_fake.o   \
          mmx_copy.o mmx_8.o mmx_16.o mmx_32.o mmx_fake.o   \
          tables.o matrox.o



all: $(LIBDIR) $(LIBNAME)

$(LIBDIR):
	if not exist $(LIBDIR) mkdir $(LIBDIR)

$(LIBNAME): $(OBJS)
	$(LIBCMD) ptc.o near.o file.o format.o
	$(LIBCMD) dpmi.o color.o palette.o
	$(LIBCMD) clipper.o manager.o block.o
	$(LIBCMD) raster.o surface.o idummy.o
	$(LIBCMD) isoft.o ivga.o ivesa.o
	$(LIBCMD) iwin32.o igdi.o idirectx.o
	$(LIBCMD) image.o tga.o cpp_copy.o cpp_8.o
	$(LIBCMD) cpp_16.o cpp_32.o x86_copy.o
	$(LIBCMD) x86_8.o x86_16.o x86_32.o
	$(LIBCMD) x86_fake.o mmx_copy.o mmx_8.o
	$(LIBCMD) mmx_16.o mmx_32.o mmx_fake.o
	$(LIBCMD) tables.o keyboard.o matrox.o

ptc.o : ptc.cpp
	$(CPPCMD) $<

near.o : near.cpp
	$(CPPCMD) $<

file.o : file.cpp
	$(CPPCMD) $<

format.o : format.cpp
	$(CPPCMD) $<

dpmi.o : dpmi.cpp
	$(CPPCMD) $<

color.o : color.cpp
	$(CPPCMD) $<

palette.o : palette.cpp
	$(CPPCMD) $<

clipper.o : clipper.cpp
	$(CPPCMD) $<

keyboard.o : keyboard.cpp
	$(CPPCMD) $<

iface.o : iface.cpp
	$(CPPCMD) $<

idummy.o : idummy.cpp
	$(CPPCMD) $<

isoft.o : isoft.cpp
	$(CPPCMD) $<

ivga.o : ivga.cpp
	$(CPPCMD) $<

ivesa.o : ivesa.cpp
	$(CPPCMD) $<

iwin32.o : iwin32.cpp
	$(CPPCMD) $<

igdi.o : igdi.cpp
	$(CPPCMD) $<

idirectx.o : idirectx.cpp
	$(CPPCMD) $<

surface.o : surface.cpp
	$(CPPCMD) $<

manager.o : manager.cpp
	$(CPPCMD) $<

block.o : block.cpp
	$(CPPCMD) $<

raster.o : raster.cpp
	$(CPPCMD) $<

matrox.o : matrox.cpp
	$(CPPCMD) $<

image.o : image.cpp
	$(CPPCMD) $<

tga.o : tga.cpp
	$(CPPCMD) $<

cpp_8.o : cpp_8.cpp
	$(CPPCMD) $<

cpp_16.o : cpp_16.cpp
	$(CPPCMD) $<

cpp_32.o : cpp_32.cpp
	$(CPPCMD) $<

cpp_copy.o : cpp_copy.cpp
	$(CPPCMD) $<

x86_8.o : x86_8.asm
	$(ASMCMD) $<

x86_16.o : x86_16.asm
	$(ASMCMD) $<

x86_32.o : x86_32.asm
	$(ASMCMD) $<

x86_fake.o : x86_fake.asm
	$(ASMCMD) $<

x86_copy.o : x86_copy.asm
	$(ASMCMD) $<

mmx_8.o : mmx_8.asm
	$(ASMCMD) $<

mmx_16.o : mmx_16.asm
	$(ASMCMD) $<

mmx_32.o : mmx_32.asm
	$(ASMCMD) $<

mmx_fake.o : mmx_fake.asm
	$(ASMCMD) $<

mmx_copy.o : mmx_copy.asm
	$(ASMCMD) $<

tables.o : tables.asm
	$(ASMCMD) $<
