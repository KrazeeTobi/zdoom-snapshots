LIBDIR = ../library/linux/gcc

ifeq ($(LIBRARY),debug)

    ##################################################
    # debug build
    CFLAGS = -D__LINUX__ -g #-O2 -Wall
    CXXFLAGS = -D__LINUX__ -g #-O2 -Wall

    ASM = nasm
    ASMFLAGS = -f elf

    ARFLAGS = rcv

    LIBNAME = $(LIBDIR)/debug.a

else

    ##################################################
    # release build

    CFLAGS = -D__LINUX__ -O2 #-Wall
    CXXFLAGS = -D__LINUX__ -O2 #-Wall

    ASM = nasm
    ASMFLAGS = -f elf

    ARFLAGS = rcv

    LIBNAME = $(LIBDIR)/release.a

endif


##################################################
# objects to build
OBJS_CPP  = ptc.o surface.o palette.o clipper.o keyboard.o idummy.o \
			isoft.o ivga.o ivesa.o iwin32.o igdi.o idirectx.o \
			isvgalib.o raster.o near.o file.o dpmi.o format.o \
			color.o manager.o block.o image.o tga.o cpp_copy.o \
			cpp_8.o cpp_16.o cpp_32.o matrox.o

OBJS_ASM  = x86_copy.o x86_8.o x86_16.o x86_32.o x86_fake.o mmx_copy.o \
			mmx_8.o mmx_16.o mmx_32.o mmx_fake.o tables.o

OBJS = $(LIBNAME)($(OBJS_CPP) $(OBJS_ASM))

SRCS_CPP = block.cpp clipper.cpp color.cpp cpp_16.cpp cpp_32.cpp \
			cpp_8.cpp cpp_copy.cpp dpmi.cpp file.cpp format.cpp \
			idirectx.cpp idummy.cpp igdi.cpp image.cpp isoft.cpp \
			isvgalib.cpp ivesa.cpp ivga.cpp iwin32.cpp keyboard.cpp \
			manager.cpp matrox.cpp near.cpp palette.cpp ptc.cpp \
			raster.cpp surface.cpp tga.cpp

##################################################
# targets 

all: $(LIBNAME)

$(LIBNAME): $(OBJS)

clean:
	rm -f .depend
	rm -f $(OBJS_CPP) $(OBJS_ASM)
	rm -f $(LIBNAME)

depend:
	rm -f .depend
	$(CXX) -M $(CXXFLAGS) $(SRCS_CPP) > .depend

##################################################
# rules 
%.o: %.cpp
	$(COMPILE.cc) $< $(OUTPUT_OPTION)

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $*.o $*.asm

##################################################
# dependancies 
ifeq (.depend,$(wildcard .depend))
include .depend
endif

