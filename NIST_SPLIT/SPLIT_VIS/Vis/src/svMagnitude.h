#ifndef __SV_MAGNITUDE_H
#define __SV_MAGNITUDE_H
#endif
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svType.h"
#include "svVectorField.h"
#include "svVectorMatrix.h"
#include "svArray.h"
//#include "trackball.h"

namespace __svl_lib{


struct POINT {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};


class svMagnitude
{
public:
	svMagnitude();
	~svMagnitude(){cleanup();}

	void SetQuadricMag(svScalar *density, int datasize, int w, int h);
	void SetLinearMag(svScalar *density, int datasize, int w, int h);
	void SetMag(svScalar *density, int datasize,int w, int h);
//	void SetQuadric();


	void RenderMagnitude(int w, int h);
	void RenderQuadric(int w, int h);
	void RenderLinearMagnitude(int w, int h);

	void Find_Nearest(int x, int y);
	bool Find_Near(int x, int y);
	void move_ctrlpoint(int x, int y, int w, int h);

	void SetLocation(int w, int h);
	void SetEncodeMode(int e){encode_mode=e;}
	
	void SetLinearLocation(int w, int h);

	int SELECTED;
	int width;
	int NEAR;

	int GetLevel(){return level;}
	svScalar *GetMag(svScalar *den,int datasize,int w, int h);
	svScalar *GetSplitVectorMag(svScalar *den,int datasize, int w, int h); //size is adjusted based on the exponet part
	svScalar *GetLinearMag(svScalar *den, int datasize,int w, int h);	
	svScalar *GetQuadricMagA(svScalar *den, int datasize, int w, int h);
	svScalar *GetQuadricMagB(svScalar *den, int datasize, int w, int h);
	int SCALE;
	int LINEARSCALE;
	int LEVEL1;
	int LEVEL2;

        GLboolean invert_matrix(const GLfloat *m, GLfloat *out);

	void SetMagScalar(float s){magscale = s;}

	void cleanup();
	void cleanquadric();

	bool LinearMap;
private: 
	int level;
	int llevel, rlevel;
	svScalar middle;
	svScalar a1, b1, a2, b2;
	svScalar maxmag[2];
	svScalar * MagWidget;
	svScalar lmag, rmag; //automatically updated with the region of insterest;
	svVector3 * WidgetH;
	svVector3 * WidgetV;
	struct POINT CtrlPoints[5];
	int W;
	int H;
	int encode_mode;//0:linear 1:splitvector 2:log
	svBool Linear;
	svBool super;
	svVector3Array *superquadricpos;
	
	int ni,nj;
	GLdouble wz[5];
	GLdouble wx[5];
	GLdouble wy[5];

	float magscale;
};


}

