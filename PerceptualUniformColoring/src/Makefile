#!smake
#  @@ Makefile for flow vis lib
#  @@ Jian Chen (jichen@umbc.edu)
#

MACHINE= $(shell uname -s)
VTKPath = /usr/include/vtk
#VTKPath = /usr/include/vtk

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I./LU -I./jcUtil -I/usr/include/vtk 
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa 
	LLDLIBS = -L./jcUtil -L./LU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

else
	OPENGL_INC= -I/usr/include -I./LU -I./jcUtil 
	OPENGL_LIB= -L/usr/lib64 
	LLDLIBS = -L./jcUtil -L./LU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

endif

CXX=g++ 

COMPILER_FLAGS= -g

INCPATH= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS) 

TARGETS = libsvl-1.0.a

SRCS =svVectorField.cpp \
svUtil.cpp \
svParticle.cpp \
ivview.cpp \
ivTrackball.cpp \
CIEColor.cpp \
Readdata.cpp

OBJECTS = svVectorField.o \
svUtil.o \
svParticle.o \
ivview.o \
ivTrackball.o \
CIEColor.o \
Readdata.o

default: $(TARGETS)

EMclustering-install:
	$(MAKE) -C EMclustering install

jcUtil-install:
	$(MAKE) -C jcUtil install

LU-install:
	$(MAKE) -C LU install

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c $(COMPILER_FLAGS) $(INCLUDE) -o $@ $< 

$(TARGETS): jcUtil-install LU-install $(OBJECTS)
	ar -ruv libsvl-1.0.a *.o

install: $(TARGETS)
	cp ./libsvl-1.0.a ../lib

svVectorField.o: svVectorField.cpp svException.h  svVectorField.h  svType.h svVectorMatrix.h svParticle.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svVectorField.o svVectorField.cpp

svUtil.o: svUtil.cpp svUtil.h svVectorField.h svType.h svVectorMatrix.h svParticle.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svUtil.o svUtil.cpp

svParticle.o: svParticle.cpp svParticle.h svType.h svVectorMatrix.h \
svVectorField.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svParticle.o svParticle.cpp

ivview.o: ivview.cpp ivview.h svType.h svVectorMatrix.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ivview.o ivview.cpp

ivTrackball.o: ivTrackball.cpp ivTrackball.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ivTrackball.o ivTrackball.cpp

CIEColor.o: CIEColor.cpp CIEColor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CIEColor.o CIEColor.cpp

Readdata.o: Readdata.cpp Readdata.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Readdata.o Readdata.cpp
clean:
	$(MAKE) -C jcUtil clean
	$(MAKE) -C LU clean
	rm -f $(OBJS) $(TARGETS) *~
