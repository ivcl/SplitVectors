#ifndef __SV_OUTLINE_H
#define __SV_OUTLINE_H


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

namespace __svl_lib{

class svOutline
{
public:
	svOutline(svVector3 l, svVector3 r,int w){lbbox = l;rbbox = r;width=w;SELECTED=GL_FALSE;SELECTED_CONTOUR=GL_FALSE;}
	void SetOutline(svVector3 l, svVector3 r,int w){lbbox = l;rbbox = r;width=w;SELECTED=GL_FALSE;SELECTED_CONTOUR=GL_FALSE;}
	~svOutline(){}
	int SELECTED;
	int SELECTED_CONTOUR;
	int NEAR;

	void DrawAxis(svVector3 &l, svVector3 &r);
	void Draw3dAxis(svVector3 &l, svVector3 &r);
	void DrawXYZ();
	void DrawContour(float scalar);
	void SetAxisWidgetPos(svVector3 l, svVector3 r);
	void SetContourWidgetPos(svVector3 l);
	void SetContourDen(svScalar num){ContourPointDen = num;}


	void move_ctrlpoint(int x, int y);
	void Find_Nearest(int x, int y);
	bool Find_Near(int x, int y);
	GLboolean invert_matrix(const GLfloat *m, GLfloat *out);
	int width;
	svVector3 GetContourPos(){svVector3 p; p[0]=AxisWidgetPos[6][0];p[1]=AxisWidgetPos[6][1];p[2]=AxisWidgetPos[6][2];return p;}

	GLfloat tb_transform[4][4];
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
private:
	svVector3 lbbox;
	svVector3 rbbox;
  	svVector3 AxisWidgetPos[10];
  	svVector3 AxisWidgetCol[10];
  	svScalar ContourPointDen;
	int ni;
	GLdouble  wz[10];
	GLdouble wx[10];
	GLdouble wy[10];
};



}
#endif
