#ifndef __SV_STICKY_PANE_H
#define __SV_STICKY_PANE_H


#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svUtil.h"
#include "svType.h"
#include "svVectorField.h"
#include "svVectorMatrix.h"
#include "svArray.h"

namespace __svl_lib{

class svStickyPane
{
public:
	svStickyPane(){dataPos=NULL;dataDir=NULL;SELECTED=GL_FALSE;radius = 1.;r=0.1;
			SECTION1=50;SECTION2=20;sizePos=svGetPerpendicularVector(arrowDir)*(radius+1);
			panePos = new svVector3[SECTION2*SECTION1];paneNorm = new svVector3[SECTION2*SECTION1];
			ni = -1;Frequency = 1;inPos=NULL;anglex=NULL;angley=NULL;anglez=NULL;dirIndex=NULL;ONE=false;OTHER=false;}//selectedIndex=NULL;
	~svStickyPane();

	void SetData(svVector3 *pos, svVector3 *dir, int datasize);
	void SetArea();//not used
	void SetPane();
	void SetSize();//not used
	void SetConnection();//not used
	void SetSelectedIndex(int *in, int num){for(int i=0;i<8;i++)selectIndex[i]=in[i];}

	void move_ctrlpoint(int x, int y,int w,int h);
	void Find_Widget(int x, int y);
		
	GLfloat tb_transform[4][4];

	void RenderStickyPane();
	void RenderInformation(int w, int h);
	
	void ProcessInformation();

	int GetSelectedIndex(int *c){for(int i=0;i<8;i++)c[i]=selectIndex[i];c[8]=arrowIndex;}
	int GetSelectedArrow(){return arrowIndex;}

	void Set2DLocation();
	void SetROI(svVector3 l, svVector3 r){ROIl = l;ROIr = r;}

	GLboolean invert_matrix(const GLfloat *m, GLfloat *out);
	void cleanup();
	int SELECTED;
	int width;

	int SECTION1;
	int SECTION2;
	int Frequency;
	bool ONE;
	bool OTHER;

    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

private:
	svVector3Array *dataPos;
	svVector3Array *dataDir;

	svVector3Array *inPos;
	svScalarArray *anglex;
	svScalarArray *angley;
	svScalarArray *anglez;

	//svIntArray *selectedIndex;
	svIntArray *dirIndex;
	int arrowIndex;

	svVector3 arrowPos;
	svVector3 arrowDir;

	svVector3 sizePos;

	svVector3 *panePos;
	svVector3 *paneNorm;

	int selectIndex[8];

	svFloat radius;
	svFloat r;
	GLdouble wx[2];
	GLdouble wy[2];
	GLdouble wz[2];

	int ni;

	svVector3 ROIl;
	svVector3 ROIr;
};



}
#endif
