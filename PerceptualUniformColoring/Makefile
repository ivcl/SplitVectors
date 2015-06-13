
MACHINE= $(shell uname -s)


ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa 
	LLDLIBS = -L./lib -lsvl-1.0 -lLU -ljcUtil-1.0  -lm
else
	OPENGL_INC= -I/usr/include
	OPENGL_LIB= -L/usr/lib64
	LLDLIBS = -L./lib -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

endif

CXX=g++ 
COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS)

TARGETS = main


SRCS = main.cpp

OBJECTS = main.o

default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

main: main.o
	$(CXX) -O3 -fopenmp $(COMPILER_FLAGS) $(LIBS) main.o -o main -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

clean:
	rm -f $(OBJS) $(TARGETS) *~
