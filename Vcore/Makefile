
C++ =			g++

CFLAGS =		-DAW_NEW_IOSTREAMS \
				-DCC_GNU_ \
				-DOSMac_ \
				-DOSMacOSX_ \
				-DOSMac_MachO_ \
				-DREQUIRE_IOSTREAM \
				-fno-gnu-keywords \
				-fpascal-strings \
				-O3 \
				$(ARCH_FLAGS) \
				-D_LANGUAGE_C_PLUS_PLUS \
				-isysroot /Developer/SDKs/MacOSX10.6.sdk \
				-shared

C++FLAGS =	   $(CFLAGS)

INCLUDES =	   -I.

INCLUDE_FILES_ALL = 	Mesh/BoundingBox.h \
			Mesh/BoundingBox.cpp \
			Mesh/Mesh.h \
			Mesh/Mesh.cpp \
			Mesh/Face.h \
			Mesh/Face.cpp \
			Mesh/Vertex.h \
			Mesh/Vertex.cpp \
			Mesh/Vec3.h \
			Mesh/Vec3.cpp \
			Vcore.h \
			Vcore.cpp \
			Plane.h \
			Plane.cpp \
			GrassStalk.h \
			GrassStalk.cpp

INCLUDE_FILES_CPP = 	Mesh/BoundingBox.cpp \
			Mesh/Mesh.cpp \
			Mesh/Face.cpp \
			Mesh/Vertex.cpp \
			Mesh/Vec3.cpp \
			Vcore.cpp \
			Plane.cpp \
			GrassStalk.cpp

INCLUDE_FILES_OBJECTS = 	Plane.o \
				GrassStalk.o \
				BoundingBox.o \
				Mesh.o \
				Face.o \
				Vertex.o \
				Vec3.o 

LD =	ld

all: libVcore.o

Vcore.o: $(INCLUDE_FILES_ALL)
	$(C++) -c $(INCLUDE_FILES_CPP) $(C++FLAGS) $(INCLUDES)

libVcore.o: Vcore.o $(INCLUDE_FILES_OBJECTS)
	ld -r -o libVcore.o Vcore.o $(INCLUDE_FILES_OBJECTS)

clean:
	rm -f *.a *.o *.so *.bundle
