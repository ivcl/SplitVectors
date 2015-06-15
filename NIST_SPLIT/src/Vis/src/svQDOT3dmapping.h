
#ifndef __QDOT_GLYPH_H
#define __QDOT_GLYPH_H

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <algorithm>
#include <vector>
#include <time.h>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

//#include "QDOTPrimitive.h"
using namespace __svl_lib;
using namespace std;

class svQDOT3dmapping{
public:
  svQDOT3dmapping(){glyphArrowPos=NULL;glyphArrowNorm=NULL;glyphFrequency=1;NUM_SEG=10;minExp = 9e+9;
	maxExp = -9e+9;ni=-1;taskarrow[0]=-1;taskarrow[1]=-1;taskarrow_task4[0]=-1;taskarrow_task4[1]=-1;questionnum=0;
	dataPos=NULL;dataEnd=NULL;dataDen=NULL;dataDir=NULL;dataPoint=NULL;dataEnd2=NULL;
	dataVel=NULL;dataExp=NULL;dataCoe=NULL;dataCol=NULL;}

 ~svQDOT3dmapping();
  void cleanup();

  void SetData(svVector3Array *pos, svVector3Array *dir, svScalarArray *den, svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,int num,
		svVector3 l, svVector3 r);
  void SetEndPos();
  void SetColors(svVector3Array *c);

  void RenderCylinderLengthLength();
  void RenderCylinderLengthColor();
  void RenderCylinderLengthLightness();
  //void RenderCylinderColorColor();

  void RenderLines();
  void GenerateLines();

  int image_width;
  int image_height;

  void SetEye(svVector3 e){eye = e;}


private:

  svVector3Array *dataPos;
  svVector3Array *dataEnd;
  svScalarArray *dataDen;
  svVector3Array *dataDir;
  svVector3Array *dataPoint;
  svVector3Array *dataEnd2;
  svScalarArray *dataVel;
  svScalarArray *dataExp;
  svScalarArray *dataCoe;  
  svVector3Array *dataCol;
  
  int dataSize;
  int zlayer;

  svVector3Array  *glyphArrowPos;
  svVector3Array  *glyphArrowNorm;

  int correctflag;

  svVector3 lbbox;
  svVector3 rbbox;
 
  float minExp;
  float maxExp;

  int glyphFrequency;
  int NUM_SEG;


  float minl_distribute;
  float minr_splitvector;

  float arrow_radius;
  float arrow_length;

  svVector3 eye;


  int listIndex;

private:
}; // end class QDOTglyph


#endif //__QDOT_GLYPH_H
