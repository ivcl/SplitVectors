#smake
#  @@ Makefile for flow vis lib
#  @@ Jian Chen (jichen@umbc.edu)
# please replace "/usr/include/vtk" by your own vtk directory

MACHINE= $(shell uname -s)
VTKPath = /usr/include/vtk

ifeq ($(MACHINE),Darwin)
	ifeq ($(wildcard /usr/include/vtk), /usr/include/vtk)
	OPENGL_INC= -FOpenGL -I../src/LU -I../src/jcUtil -I../src  -I/usr/include/vtk 
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa 
	LLDLIBS = -L../lib -L/usr/lib64/vtk -lvtkCommon -lvtkHybrid -lem-1.0 -lsvl-1.0 -lLU -ljcUtil-1.0  -lm
	else
	OPENGL_INC= -FOpenGL -I../src/LU -I../src/jcUtil -I../src
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa 
	LLDLIBS = -L../lib -lem-1.0 -lsvl-1.0 -lLU -ljcUtil-1.0  -lm
	endif
else
	ifeq ($(wildcard /usr/include/vtk), /usr/include/vtk)
	OPENGL_INC= -I/usr/include -I/usr/include/vtk -I../ann_1.1.1/include
	OPENGL_LIB= -L/usr/lib64   -L/usr/lib64/vtk -L../ann_1.1.1/lib
	LLDLIBS = -L../lib -L/usr/lib64/vtk -lglui -lANN -lvtkCommon -lvtkHybrid -lem-1.0 -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm
	else
	OPENGL_INC= -I/usr/include -I../ann_1.1.1/include
	OPENGL_LIB= -L/usr/lib64 -L../ann_1.1.1/lib
	LLDLIBS =-L../lib -lem-1.0 -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lANN -lm
	endif
endif

ifeq ($(wildcard /usr/include/vtk), /usr/include/vtk)
	CXX = g++ -D_VTK
else
	CXX=g++ 
endif

COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS)

TARGETS = qdotVis3DGlyph
#qdotVisBScoloring

SRCS = qdotVis3DGlyph.cpp  File.cpp  Sliders.cpp 
#qdotVisBScoloring  
OBJECTS = qdotVis3DGlyph.o File.o  Sliders.o 
#qdotVisBScoloring.o

default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

install: qdotVis3DGlyph
	cp qdotVis3DGlyph ../exe

.cpp.o:
	$(CXX) -O3 -fopenmp -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

qdotVis3DGlyph: qdotVis3DGlyph.o Sliders.o File.o 
	$(CXX) -O3 -fopenmp $(COMPILER_FLAGS) $(LIBS) qdotVis3DGlyph.o Sliders.o File.o -o qdotVis3DGlyph -lglui -lem-1.0 -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lANN -lm

clean:
	rm -f $(OBJS) $(TARGETS) *~
	rm ../exe/qdot*
