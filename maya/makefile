
# NOTE: MAYA_LOCATION on Mac OS X points to Maya.app/Contents
MAYA_LOCATION = /Applications/Autodesk/maya2012/Maya.app/Contents

# Change location if you have a non standard install.
DEVKIT_LOCATION = $(MAYA_LOCATION)/../../devkit

# Make sure this is in your path.
C++ = 		g++

# By default, we will try to build a universal binary to match the same
# architectures as the installed Maya binary. If you only want one specific
# architecture (e.g i386 or x86_64) then set it below.
PREFERRED_ARCHITECTURE =

# Determine the architectures to build.
MAYABIN = ${MAYA_LOCATION}/bin/maya
MAYA_ARCHES = $(shell lipo -info $(MAYABIN) | sed 's/^.*://')

ifneq ($(PREFERRED_ARCHITECTURE),)
	MAYA_ARCHES = $(filter $(PREFERRED_ARCHITECTURE),$(MAYA_ARCHES))

	ifeq ($(MAYA_ARCHES),)
		$(error $(MAYABIN) does not support the '$(PREFERRED_ARCHITECTURE)' architecture.)
	endif
endif

ARCH_FLAGS = 	$(patsubst %,-arch %,$(MAYA_ARCHES))

CFLAGS = 	-DAW_NEW_IOSTREAMS -DCC_GNU_ -DOSMac_ -DOSMacOSX_ \
		-DOSMac_MachO_ -DREQUIRE_IOSTREAM -fno-gnu-keywords -fpascal-strings -O3 \
		$(ARCH_FLAGS) -D_LANGUAGE_C_PLUS_PLUS -isysroot /Developer/SDKs/MacOSX10.6.sdk \
		-include $(MAYA_LOCATION)/../../devkit/include/maya/OpenMayaMac.h \
		-shared

C++FLAGS = 	$(CFLAGS)

INCLUDES = 	-I. -I$(MAYA_LOCATION)/../../devkit/include

LDFLAGS =	-framework Carbon -framework OpenGL -framework GLUT -lOpenMayaUI

LD = 		$(MAYA_LOCATION)/../../devkit/bin/mayald MAYA_ARCHES="$(MAYA_ARCHES)" MAYA_LOCATION="$(MAYA_LOCATION)"


all: VmCityislands.bundle

VmIslandNode.o: VmIslandNode.cpp
	$(C++) -c VmIslandNode.cpp $(C++FLAGS) $(INCLUDES)

vmPluginMain.o: vmPluginMain.cpp
	$(C++) -c vmPluginMain.cpp $(C++FLAGS) $(INCLUDES)

VmCityislands.bundle: VmIslandNode.o vmPluginMain.o
	$(LD) -dynamic -bundle -o VmCityislands.bundle VmIslandNode.o vmPluginMain.o ../Vcore/libVcore.o $(LDFLAGS)


clean:
	rm -f *.o *.so *.bundle

