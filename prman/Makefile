

RATDIR = /Applications/Pixar/RenderManStudio-3.0.3-maya2012/devkit/include

RMANTREE = /Applications/Pixar/RenderManStudio-3.0.3-maya2012/rmantree

MAYAINCLUDE = /Applications/Autodesk/maya2012/devkit/include 

# These OPTS were stolen from watching the Maya stuff compile.
OPTS = \
	-DAW_NEW_IOSTREAMS \
	-DCC_GNU_ \
	-DOSMac_ \
	-DOSMacOSX_ \
	-DOSMac_MachO_ \
	-DREQUIRE_IOSTREAM \
	-fno-gnu-keywords \
	-fpascal-strings \
	-O3 \
	-arch x86_64 \
	-D_LANGUAGE_C_PLUS_PLUS \
	-isysroot /Developer/SDKs/MacOSX10.6.sdk


INCLUDES = \
	-I$(RATDIR) 
#	-I$(MAYAINCLUDE) \


C++ = g++

LD = g++ -bundle -undefined dynamic_lookup


default: VrIslands.so

#install: $(DSO)
#	install -u root -m 644 $(DSO) $(SLI) $(RATINSTALL)/lib/dsos

VrIslands.o: VrIslands.cpp ../Vcore/Vcore.h
	$(C++) $(INCLUDES) $(OPTS) -o VrIslands.o -c VrIslands.cpp

VrIslands.so: VrIslands.o ../Vcore/Vcore.o 
	$(LD) -o VrIslands.so VrIslands.o ../Vcore/Vcore.o 

clean:
	rm -rf *.so *.o ii_files so_locations


